#include "PhysicsComponent.h"
#include "Engine.h"

namespace gn {
	void PhysicsComponent::Update() {
		velocity += acceleration * owner->scene->engine->time.deltaTime;
		owner->transform.position += velocity * owner->scene->engine->time.deltaTime;
	}
}