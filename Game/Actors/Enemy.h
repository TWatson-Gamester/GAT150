#pragma once
#include "Object/Actor.h"
#include "Math/MathUtils.h"
#include "Graphics/Texture.h"

class Enemy : public gn::Actor {
public:
	Enemy(const gn::Transform& transform, std::shared_ptr<gn::Texture> texture, float speed) : gn::Actor{ transform, texture }, speed{ speed } {};
	void Update(float dt) override;

	void OnCollision(Actor* actor) override;

private:
	float fireTimer{ 0 };
	float fireRate{ 2.0f };
	float speed = 300;
};