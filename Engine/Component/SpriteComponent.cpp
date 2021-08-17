#include "SpriteComponent.h"
#include "Object/Actor.h"
#include "Graphics\Renderer.h"

namespace gn {
	void SpriteComponent::Update() {

	}

	void SpriteComponent::Draw(Renderer* renderer) {
		renderer->Draw(texture, owner->transform);
	}
}