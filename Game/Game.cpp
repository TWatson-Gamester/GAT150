#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "Audio/AudioSystem.h"

void Game::Initialize(){
	engine = std::make_unique<gn::Engine>(); //Makes the new Engine
	engine->Startup();

	engine->Get<gn::Renderer>()->Create("Coin_Collector_8999", 800, 600);

	//Regster Classes
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);

	scene = std::make_unique<gn::Scene>(); //Makes the Scene
	scene->engine = engine.get();

	gn::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	gn::SetFilePath("../Resources");

	engine->Get<gn::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1));
	engine->Get<gn::EventSystem>()->Subscribe("player_hit", std::bind(&Game::OnPlayerHit, this, std::placeholders::_1));

}

void Game::Shutdown(){
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update(){

	engine->Update();
	stateTimer += scene->engine->time.deltaTime;

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
	auto livesActor = scene->FindActor("Lives");
	if (livesActor) {
		livesActor->GetComponent<gn::TextComponent>()->SetText("Lives: " + std::to_string(lives));

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

void Game::OnPlayerHit(const gn::Event& event){
 	lives--;
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

	auto actor = gn::ObjectFactory::Instance().Create<gn::Actor>("Coin");
	actor->transform.position = gn::Vector2{ gn::RandomRange(150,650), gn::RandomRange(200,400) };
	scene->AddActor(std::move(actor));

	lives = 3;
	score = 0;

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
		actor->transform.position = gn::Vector2{ gn::RandomRange(150,650), gn::RandomRange(200,400) };
		scene->AddActor(std::move(actor));
		spawnTimer = 2;
	}
	if (lives <= 0) {
		state = eState::PlayerDead;
		stateTimer = 0;
	}
}

void Game::PlayerDead(){
	auto player = scene->FindActor("Player");
	if (player) {
		player->destroy = true;
		player->active = false;
		player = scene->FindActor("Title");
		player->active = true;
		player->GetComponent<gn::TextComponent>()->SetText("GAME OVER");
		
	}
	if (stateTimer >= 3) {
		stateTimer = 0;
		state = eState::GameOver;
	}
}

void Game::GameOver(){
	scene->RemoveAllActors();
	state = eState::Reset;
}
