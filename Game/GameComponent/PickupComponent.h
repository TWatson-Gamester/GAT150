#pragma once
#include "Engine.h"

class PickupComponent : public gn::Component {
	// Inherited via Component
	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual void Update() override;
};