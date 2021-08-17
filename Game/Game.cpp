#include "Game.h"
#include <vector>

void Game::Initialize(){
	engine = std::make_unique<gn::Engine>(); //Makes the new Engine
	engine->Startup();

	engine->Get<gn::Renderer>()->Create("Not Asteroids", 800, 600);

	scene = std::make_unique<gn::Scene>(); //Makes the Scene
	scene->engine = engine.get();

	gn::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	gn::SetFilePath("../Resources");

	std::unique_ptr<gn::Actor> actor = std::make_unique<gn::Actor>(gn::Transform{ gn::Vector2{400,300}, 0, 5 });
	{
		std::unique_ptr<gn::SpriteComponent> component = std::make_unique<gn::SpriteComponent>();
		component->texture = engine->Get<gn::ResourceSystem>()->Get<gn::Texture>("sf2.png", engine->Get<gn::Renderer>());

		actor->AddComponent(std::move(component));
	}
	{
		std::unique_ptr<gn::PhysicsComponent> component = std::make_unique<gn::PhysicsComponent>();
		component->ApplyForce(gn::Vector2::right * 200);
		actor->AddComponent(std::move(component));
	}
		scene->AddActor(std::move(actor));

}

void Game::Shutdown(){
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update(){

	engine->Update();

	if (engine->Get<gn::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == gn::InputSystem::eKeyState::Pressed) {
		quit = true;
	}

	//Main Code End

	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	engine->Get<gn::Renderer>() -> BeginFrame();
	

	//Systems Draw
	engine->Draw(engine->Get<gn::Renderer>());
	scene->Draw(engine->Get<gn::Renderer>());

	engine->Get<gn::Renderer>()->EndFrame();
}

