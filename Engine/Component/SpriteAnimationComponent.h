#pragma once
#include "Component/SpriteComponent.h"
#include <SDL_rect.h>
#include <map>

namespace gn {
	class Renderer;

	class SpriteAnimationComponent : public SpriteComponent {
	
	private:
		struct Sequence {
			int fps{ 0 };
			int startFrame{ 0 };
			int endFrame{ 0 };
		};

	public:

		std::unique_ptr<Object> Clone() const { return std::make_unique<SpriteAnimationComponent>(*this); }


		void Update() override;
		void Draw(Renderer* renderer) override;

		void StartSequence(const std::string& name);

		bool Write(const rapidjson::Value& value) const override;
		bool Read(const rapidjson::Value& value) override;

	public:
		int frame = 0;
		int startFrame = 0;
		int endFrame = 0;
		int fps = 0;
		int numFramesX = 0;
		int numFramesY = 0;
		float frameTimer = 0;
		float frameTime = 0;

		std::map<std::string, Sequence> sequences;
		std::string sequenceName;

	};
}