#pragma once
#include "Engine.h"
#include "Framework/EventSystem.h"
#include <list>

class PlayerComponent : public gn::Component {
public:

	virtual ~PlayerComponent();

	std::unique_ptr<Object> Clone() const { return std::make_unique<PlayerComponent>(*this); }

	void Create() override;
	virtual void OnCollisionEnter(const gn::Event& event);
	virtual void OnCollisionExit(const gn::Event& event);

	bool Write(const rapidjson::Value& value) const override;

	bool Read(const rapidjson::Value& value) override;

	void Update() override;

public:
	float speed{ 0 };
	float jump = 0;

private:
	std::list<gn::Actor*> contacts;

};