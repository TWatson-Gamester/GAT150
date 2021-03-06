#include "Font.h"
#include <cassert>

namespace gn {

	Font::~Font(){

		if (font)
		{
			delete font;
		}
	}

	bool Font::Load(const std::string& name, void* data){

		font = font = TTF_OpenFont(name.c_str(), *(static_cast<int*>(data)));
			if (font == nullptr)
			{
				std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
				return false;
			}

		return true;
	}

	SDL_Surface* Font::CreateSurface(const std::string& text, const gn::Color& color){

		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), SDL_Color(color)); //, use TTF fontand color for the parameters>
			if (surface == nullptr)
			{
				std::cout << "TTF_RenderText_Solid Error: " << SDL_GetError() << std::endl;
			}
		return surface;
	}
}