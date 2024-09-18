#include "pch.h"
#include "SkillComponent.h"

void SkillComponent::Update()
{
	std::cout << "SkillComponent is updating!" << std::endl;
	NotifyObservers(100);  // int형 메시지 전달
}

void SkillComponent::Notify(const EventData& message)
{
	NotifyObservers(message);
}

void SkillComponent::OnNotify(const EventData& message)
{
	if (std::holds_alternative<int>(message))
	{
		std::cout << "SkillComponent received message: " << std::get<int>(message) << std::endl;
	}

}
