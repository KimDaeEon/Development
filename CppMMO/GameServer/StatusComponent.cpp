#include "pch.h"
#include "StatusComponent.h"

void StatusComponent::Update()
{
	std::cout << "StatusComponent is updating!" << std::endl;
	NotifyObservers(100);  // int�� �޽��� ����
}

void StatusComponent::Notify(const EventData& message)
{
	NotifyObservers(message);
}

void StatusComponent::OnNotify(const EventData& message)
{
	if (std::holds_alternative<int>(message))
	{
		std::cout << "StatusComponent received message: " << std::get<int>(message) << std::endl;
	}
}
