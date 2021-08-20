#include "Game.h"


int main(int, char**)
{

	Game game;
	game.Initialize();


	// wait for keyboard enter to exit
	bool quit = false;
	SDL_Event event;

	rapidjson::Document document;
	bool success = gn::json::Load("json.txt", document);
	assert(success);

	std::string str;
	gn::json::Get(document, "string", str);
	std::cout << str << std::endl;

	bool b;
	gn::json::Get(document, "boolean", b);
	std::cout << b << std::endl;

	int i1;
	gn::json::Get(document, "integer1", i1);
	std::cout << i1 << std::endl;

	int i2;
	gn::json::Get(document, "integer2", i2);
	std::cout << i2 << std::endl;

	float f;
	gn::json::Get(document, "float", f);
	std::cout << f << std::endl;

	gn::Vector2 v2;
	gn::json::Get(document, "vector2", v2);
	std::cout << v2 << std::endl;

	gn::Color color;
	gn::json::Get(document, "color", color);
	std::cout << color << std::endl;



	while (!game.IsQuit() && !quit) {
		SDL_PollEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
		}

		game.Update();
		game.Draw();

	}

	SDL_Quit();

	return 0;
}
