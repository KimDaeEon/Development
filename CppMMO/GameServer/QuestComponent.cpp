#include "pch.h"
#include "QuestComponent.h"

void QuestComponent::Update()
{
	std::cout << "QuestComponent is updating!" << std::endl;
}

void QuestComponent::Notify(const EventData& message)
{
	NotifyObservers(message);
}

void QuestComponent::OnNotify(const EventData& message)
{
	if (std::holds_alternative<int>(message))
	{
		std::cout << "QuestComponent received message: " << std::get<int>(message) << std::endl;
	}
	else if (std::holds_alternative<std::string>(message))
	{
		std::cout << "QuestComponent received message: " << std::get<std::string>(message) << std::endl;
	}
	else if (std::holds_alternative<ComponentEventExample>(message))
	{
		std::cout << "QuestComponent received message: " << std::get<ComponentEventExample>(message).action << std::endl;
	}
}
