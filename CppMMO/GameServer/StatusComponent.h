#pragma once
#include "Component.h"
class StatusComponent : public Component
{
public:
	virtual void Update() override;

	virtual void Notify(const EventData& message) override;

	virtual void OnNotify(const EventData& message) override;
};