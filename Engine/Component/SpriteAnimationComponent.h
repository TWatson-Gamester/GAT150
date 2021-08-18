#pragma once
#include "Component/SpriteComponent.h"
#include <SDL_rect.h>

namespace gn {
	class Renderer;

	class SpriteAnimationComponent : public SpriteComponent {
	
	public:
		void Update() override;
		void Draw(Renderer* renderer) override;

	public:
		int frame = 0;
		int fps = 0;
		int numFramesX = 0;
		int numFramesY = 0;
		float frameTimer = 0;
		float frameTime = 0;
		SDL_Rect rect;

	};
}