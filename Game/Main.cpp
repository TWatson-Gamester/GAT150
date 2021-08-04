#include "engine.h"
#include <SDL_Image.h>
#include <iostream>
#include <SDL.h>

int main(int, char**)
{

	gn::Engine engine;
	engine.Startup();

	engine.Get<gn::Renderer>()->Create("THE WINDOW WORKS", 800, 600);

	std::cout << gn::GetFilePath() << std::endl;
	gn::SetFilePath("../Resources");
	std::cout << gn::GetFilePath() << std::endl;

	std::shared_ptr<gn::Texture> texture = engine.Get<gn::ResourceSystem>()->Get<gn::Texture>("sf2.png", engine.Get<gn::Renderer>());
	
	// wait for keyboard enter to exit
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		}

		engine.Get<gn::Renderer>() -> BeginFrame();

		gn::Vector2 position{ 300,400 };
		engine.Get<gn::Renderer>()->Draw(texture, position);

		engine.Get<gn::Renderer>() -> EndFrame();

	//	for (size_t i = 0; i < 50; i++) {
	//		SDL_Rect src{ 32, 64, 32, 64 };
	//		SDL_Rect dest{gn::RandomRangeInt(0,screen.x), gn::RandomRangeInt(0,screen.y), 16, 24};
	//		SDL_RenderCopy(renderer, texture, &src, &dest);

		//}


	}



	SDL_Quit();

	return 0;
}
