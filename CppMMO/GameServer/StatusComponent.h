#pragma once
#include "Component.h"
class StatusComponent : public Component
{
public:
	virtual void Update() override;

	virtual void Notify(const EventData& message) override;

	virtual void OnNotify(const EventData& message) override;

	const Protocol::StatInfo& GetStatInfo() const { return _statInfo; }
	void SetStatInfo(const Protocol::StatInfo& statInfo) { _statInfo = statInfo; }

private:
	Protocol::StatInfo _statInfo;
};