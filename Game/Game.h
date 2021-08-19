#pragma once
#include "Engine.h"

#include <memory>

class Game {

public:
	void Initialize();
	void Shutdown();
	bool IsQuit() { return quit; }
	void Update();
	void Draw();

public:
	std::unique_ptr<gn::Engine> engine;
	std::unique_ptr<gn::Scene> scene;

private:
	bool quit = false;

};