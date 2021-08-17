#pragma once
#include "GraphicComponent.h"
#include <memory>

namespace gn {
	class Texture;
	class SpriteComponent : public GraphicComponent {
	public:
		void Update() override;
		void Draw(Renderer* renderer) override;

	public:
		std::shared_ptr<Texture> texture;
	};
}