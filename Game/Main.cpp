#include "engine.h"
#include <SDL_Image.h>
#include <iostream>
#include <SDL.h>

int main(int, char**)
{

	gn::Timer timer;

	gn::Engine engine;
	engine.Startup();

	engine.Get<gn::Renderer>()->Create("THE WINDOW WORKS", 800, 600);

	gn::Scene scene;
	scene.engine = &engine;

	gn::SetFilePath("../Resources");

	std::shared_ptr<gn::Texture> texture = engine.Get<gn::ResourceSystem>()->Get<gn::Texture>("sf2.png", engine.Get<gn::Renderer>());

	for (size_t i = 0; i < 20; i++) {
		gn::Transform transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600)}, gn::RandomRange(0,360), 1.0f };
		std::unique_ptr<gn::Actor> actor = std::make_unique<gn::Actor>(transform, texture);
		scene.AddActor(std::move(actor));
	}
	
	// wait for keyboard enter to exit
	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 3.0f;

	while (!quit) {
		SDL_PollEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
		}

		engine.Update();

		quit = (engine.Get<gn::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == gn::InputSystem::eKeyState::Pressed);

		scene.Update(engine.time.deltaTime);

		if (engine.time.time >= quitTime) quit = true;
		engine.time.timeScale = 50000;

		engine.Get<gn::Renderer>() -> BeginFrame();

		scene.Draw(engine.Get<gn::Renderer>());

		engine.Get<gn::Renderer>() -> EndFrame();

	}



	SDL_Quit();

	return 0;
}
