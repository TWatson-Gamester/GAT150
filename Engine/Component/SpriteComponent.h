#pragma once
#include "GraphicComponent.h"
#include <memory>

namespace gn {


	class Texture;
	class SpriteComponent : public GraphicComponent {
	public:
		
		std::unique_ptr<Object> Clone() const { return std::make_unique<SpriteComponent>(*this); }
		
		void Update() override;
		void Draw(Renderer* renderer) override;

		bool Write(const rapidjson::Value& value) const override;
		bool Read(const rapidjson::Value& value) override;

	public:
		std::shared_ptr<Texture> texture;
		SDL_Rect rect;
	};
}