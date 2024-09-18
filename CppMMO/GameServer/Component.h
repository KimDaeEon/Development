#pragma once
#include "ComponentEvent.h"

// �ʿ��� Event ���� ������ ComponentEvent.h�� �߰��ϰ� �Ʒ��� ���
using EventData = std::variant<int, float, std::string, ComponentEventExample>;

// IComponent �������̽�
class Component
{
public:
	virtual ~Component() = default;
	virtual void Update() = 0;

	// Observer ���� ����
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

// Component�� ������ Pack�� Ǯ� �����ڸ� ȣ���ϱ� ���� ��
template <typename... Ts>
std::tuple<Ts...> CreateComponents()
{
    return std::tuple<Ts...>(Ts()...);  // �� ������Ʈ�� �⺻ �����ڸ� ȣ��
}

// Composite Ŭ���� ���ø� ����
template <typename... Components>
class Composite
{
    // �ߺ��� Component�� �ִ��� Ȯ��
    static_assert(!HasDuplicates<Components...>::value, "Duplicate components are not allowed.");
    static_assert((std::is_base_of_v<Component, Components> && ...), "All components must inherit from Component");

public:
    // �Ϻ��� ������ ����� ������
    template <typename... Args>
    Composite(Args&&... args)
        : components_(std::forward<Args>(args)...)
    {
    }

    // GetComponent<T>: Ư�� Ÿ���� ������Ʈ�� ��ȯ (������ Ÿ�� ����)
    template <typename T>
    T& GetComponent()
    {
        static_assert(HasComponent<T>(), "The requested component does not exist in this Composite.");
        return std::get<T>(components_);
    }

private:
    // ������Ʈ�� ������ Ʃ��
    std::tuple<Components...> components_;

    // Ư�� Ÿ���� ������Ʈ�� �����ϴ��� Ȯ���ϴ� ��Ÿ �Լ�
    template <typename T>
    static constexpr bool HasComponent()
    {
        return (std::is_same_v<T, Components> || ...);
    }
};
