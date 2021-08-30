#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"

void Game::Initialize(){
	engine = std::make_unique<gn::Engine>(); //Makes the new Engine
	engine->Startup();

	engine->Get<gn::Renderer>()->Create("Not Asteroids", 800, 600);

	//Regster Classes
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);

	scene = std::make_unique<gn::Scene>(); //Makes the Scene
	scene->engine = engine.get();

	gn::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	gn::SetFilePath("../Resources");

	rapidjson::Document document;
	bool success = gn::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);
	for (size_t i = 0; i < 10; i++) {
		auto actor = gn::ObjectFactory::Instance().Create<gn::Actor>("Coin");
		actor->transform.position = gn::Vector2{ gn::RandomRange(0,800), gn::RandomRange(400,500) };
		scene->AddActor(std::move(actor));
	}
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

