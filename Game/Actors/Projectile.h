#pragma once

#include "Object/Actor.h"
#include "Graphics/Texture.h"
#include "Math/MathUtils.h"

class Projectile : public gn::Actor {
public:
	Projectile(const gn::Transform& transform, std::shared_ptr<gn::Texture> texture, float speed) : gn::Actor{ transform, texture }, speed{ speed } {};
	void Update(float dt) override;

private:
	float lifetime{ 1 };
	float speed = 300;
};