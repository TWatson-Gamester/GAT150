#pragma once
#include "Engine.h"
#include "Framework/EventSystem.h"

class PickupComponent : public gn::Component {

public:
	virtual ~PickupComponent();

	std::unique_ptr<Object> Clone() const { return std::make_unique<PickupComponent>(*this); }


	// Inherited via Component
	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual void Update() override;
};