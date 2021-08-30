#pragma once
#include "Engine.h"
#include "Framework/EventSystem.h"

class PickupComponent : public gn::Component {

	virtual ~PickupComponent();

	// Inherited via Component
	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual void Update() override;
};