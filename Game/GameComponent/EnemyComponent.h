#pragma once
#include "Engine.h"

class EnemyComponent : public gn::Component {
public:

	bool Write(const rapidjson::Value& value) const override;

	bool Read(const rapidjson::Value& value) override;

	void Update() override;

public:
	float speed{ 0 };

};