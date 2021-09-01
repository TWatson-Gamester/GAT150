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

	engine->Get<gn::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1));

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

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	//update Score
	auto scoreActor = scene->FindActor("Score");
	if (scoreActor) {
		scoreActor->GetComponent<gn::TextComponent>()->SetText("Score: " + std::to_string(score));
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	engine->Get<gn::Renderer>() -> BeginFrame();
	

	//Systems Draw
	engine->Draw(engine->Get<gn::Renderer>());
	scene->Draw(engine->Get<gn::Renderer>());

	engine->Get<gn::Renderer>()->EndFrame();
}

void Game::OnAddScore(const gn::Event& event){
	score += std::get<int>(event.data);
}

void Game::Reset(){
	scene->RemoveAllActors();

	rapidjson::Document document;
	bool success = gn::json::Load("title.txt", document);
	assert(success);

	scene->Read(document);

	state = eState::Title;
}

void Game::Title(){
	if (engine->Get<gn::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == gn::InputSystem::eKeyState::Pressed) {
		auto title = scene->FindActor("Title");
		assert(title);
		title->active = false;
		state = eState::StartGame;
	}
}

void Game::StartGame(){

	rapidjson::Document document;
	bool success = gn::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);

	gn::TileMap tilemap;
	tilemap.scene = scene.get();
	success = gn::json::Load("map.txt", document);
	assert(success);

	tilemap.Read(document);
	tilemap.Create();

	for (size_t i = 0; i < 10; i++) {
		auto actor = gn::ObjectFactory::Instance().Create<gn::Actor>("Coin");
		actor->transform.position = gn::Vector2{ gn::RandomRange(0,800), gn::RandomRange(400,500) };
		scene->AddActor(std::move(actor));
	}

	state = eState::StartLevel;
	stateTimer = 0;
}

void Game::StartLevel(){
	stateTimer += engine->time.deltaTime;
	if (stateTimer >= 1) {
		auto player = gn::ObjectFactory::Instance().Create<gn::Actor>("Player");
		player->transform.position = gn::Vector2{ 400, 350 };
		scene->AddActor(std::move(player));

		spawnTimer = 2;
		state = eState::Level;
	}
}

void Game::Level(){
	spawnTimer -= engine->time.deltaTime;
	if (spawnTimer <= 0) {
		auto actor = gn::ObjectFactory::Instance().Create<gn::Actor>("Coin");
		actor->transform.position = gn::Vector2{ gn::RandomRange(0,800), gn::RandomRange(400,500) };
		scene->AddActor(std::move(actor));
		spawnTimer = 2;
	}
}

void Game::PlayerDead(){

}

void Game::GameOver(){

}
