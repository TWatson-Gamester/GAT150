#pragma once
#include "GraphicComponent.h"
#include "Math/Color.h"

namespace gn {
		class Texture;
		class Font;

		class TextComponent : public GraphicComponent
		{
		public:
			virtual void Update() override;
			virtual void Draw(Renderer* renderer) override;

			void SetText(const std::string& text);

			virtual bool Write(const rapidjson::Value& value) const override;
			virtual bool Read(const rapidjson::Value& value) override;


		public:
			std::string fontName;
			int fontSize;
			Color color = Color::white;
			std::string text;

			std::shared_ptr<Font> font;
			std::shared_ptr<Texture> texture;
		};

}