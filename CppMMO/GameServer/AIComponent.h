#pragma once
#include "Component.h"
class AIComponent : public Component
{
public:
	void Update() override;

	void Notify(const EventData& message) override;

	void OnNotify(const EventData& message) override;
};