#pragma once
// QuestComponent Å¬·¡½º
#include "Component.h"
class QuestComponent : public Component
{
public:
	virtual void Update() override;

	virtual void Notify(const EventData& message) override;

	virtual void OnNotify(const EventData& message) override;
};