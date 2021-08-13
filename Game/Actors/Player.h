#pragma once
#include "Object/Actor.h"

class Player : public gn::Actor {
public:
	Player(const gn::Transform& transform, std::shared_ptr<gn::Texture> texture, float speed);
	void Update(float dt) override;
	void OnCollision(Actor* actor) override;
	void Initialize() override;

private:
	gn::Vector2 velocity = 0;
	float fireTimer{ 0 };
	float fireRate{ .5f };
	float speed = 600;
};