#pragma once
#include "Object/Actor.h"
#include "Math/MathUtils.h"
#include "Graphics/Texture.h"

class Asteroid : public gn::Actor {
public:
	Asteroid(const gn::Transform& transform, std::shared_ptr<gn::Texture> texture, float speed, std::string tag) : gn::Actor{ transform, texture }, speed{ speed }, tag{tag} {};
	void Update(float dt) override;

	void OnCollision(Actor* actor) override;

private:
	std::string tag = "";
	float speed = 10;
};