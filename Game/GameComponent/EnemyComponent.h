#pragma once
#include "Engine.h"

class EnemyComponent : public gn::Component {
public:

	std::unique_ptr<Object> Clone() const { return std::make_unique<EnemyComponent>(*this); }


	bool Write(const rapidjson::Value& value) const override;

	bool Read(const rapidjson::Value& value) override;

	void Update() override;

public:
	float speed{ 0 };

};