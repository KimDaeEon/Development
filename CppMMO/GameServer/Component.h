#pragma once
#include "ComponentEvent.h"

// 필요한 Event 생길 때마다 ComponentEvent.h에 추가하고 아래에 등록
using EventData = std::variant<int, float, std::string, ComponentEventExample>;

// IComponent 인터페이스
class Component
{
public:
	virtual ~Component() = default;
	virtual void Update() = 0;

	// Observer 패턴 적용
	virtual void Notify(const EventData& message) = 0;
	virtual void OnNotify(const EventData& message) = 0;


    void Subscribe(Component* component)
    {
        observers_.push_back(component);
    }

    void NotifyObservers(const EventData& message)
    {
        for (auto* observer : observers_)
        {
            observer->OnNotify(message);
        }
    }

protected:
	std::vector<Component*> observers_;

};

// Component에 정의한 Pack을 풀어서 생성자를 호출하기 위한 것
template <typename... Ts>
std::tuple<Ts...> CreateComponents()
{
    return std::tuple<Ts...>(Ts()...);  // 각 컴포넌트의 기본 생성자를 호출
}

// Composite 클래스 템플릿 정의
template <typename... Components>
class Composite
{
    // 중복된 Component가 있는지 확인
    static_assert(!HasDuplicates<Components...>::value, "Duplicate components are not allowed.");
    static_assert((std::is_base_of_v<Component, Components> && ...), "All components must inherit from Component");

public:
    // 완벽한 전달을 사용한 생성자
    template <typename... Args>
    Composite(Args&&... args)
        : components_(std::forward<Args>(args)...)
    {
    }

    // GetComponent<T>: 특정 타입의 컴포넌트를 반환 (컴파일 타임 검증)
    template <typename T>
    T& GetComponent()
    {
        static_assert(HasComponent<T>(), "The requested component does not exist in this Composite.");
        return std::get<T>(components_);
    }

private:
    // 컴포넌트를 보관할 튜플
    std::tuple<Components...> components_;

    // 특정 타입의 컴포넌트가 존재하는지 확인하는 메타 함수
    template <typename T>
    static constexpr bool HasComponent()
    {
        return (std::is_same_v<T, Components> || ...);
    }
};
