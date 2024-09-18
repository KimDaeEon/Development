#pragma once
#include "Component.h"
class SkillComponent : public Component
{
public:
	SkillComponent() = default;

	virtual void Update() override;

	virtual void Notify(const EventData& message) override;

	virtual void OnNotify(const EventData& message) override;
};