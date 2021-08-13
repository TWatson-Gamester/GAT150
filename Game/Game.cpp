#include "Game.h"
#include <vector>

void Game::Initialize(){
	engine = std::make_unique<gn::Engine>(); //Makes the new Engine
	engine->Startup();

	engine->Get<gn::Renderer>()->Create("THE WINDOW WORKS", 800, 600);

	scene = std::make_unique<gn::Scene>(); //Makes the Scene
	scene->engine = engine.get();

	gn::SetFilePath("../Resources");

	//Added from old main
	engine->Get<gn::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<gn::AudioSystem>()->AddAudio("Epic", "audio/Epic.mp3");
	//Game Code
	engine->Get<gn::AudioSystem>()->AddAudio("Player_Shot", "audio/Player_Shot.wav");
	engine->Get<gn::AudioSystem>()->AddAudio("Enemy_Shot", "audio/Enemy_Shot.wav");
	//Game Code
	//musicChannel = engine->Get<gn::AudioSystem>()->PlayAudio("Epic", 1, 1, true);


	/*std::shared_ptr<gn::Texture> texture = engine->Get<gn::ResourceSystem>()->Get<gn::Texture>("sf2.png", engine->Get<gn::Renderer>());

	for (size_t i = 0; i < 20; i++) {
		gn::Transform transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600)}, gn::RandomRange(0,360), 1.0f };
		std::unique_ptr<gn::Actor> actor = std::make_unique<gn::Actor>(transform, texture);
		scene->AddActor(std::move(actor));
	}*/


	//// create font texture
	textTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
	//// set font texture with font surface
	//textTexture->Create(font->CreateSurface("Hello World", gn::Color{ 1, 1, 1 }));
	//// add font texture to resource system
	//engine->Get<gn::ResourceSystem>()->Add("textTexture", textTexture);

	//Game Code
	engine->Get<gn::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<gn::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
}

void Game::Shutdown(){
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update(){

	stateTimer += engine->time.deltaTime;
	
	engine->Update();

	switch (state)
	{
	case Game::eState::TitleScreen:
		if (engine->Get<gn::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == gn::InputSystem::eKeyState::Pressed) {
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		score = 0;
		lives = 3;
		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
	{
		UpdateLevelStart(engine->time.deltaTime);
	}
		break;
	case Game::eState::Game:
		//if (scene->GetActors<Enemy>().size() == 0 && scene->GetActors<Asteroid>().size() == 0) {
		//	state = eState::GameOver;
		//}
		//if (stateTimer >= 10) {
		//	scene->AddActor(std::make_unique<Enemy>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 3.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("EnemyShape.txt"), 50.0f));
		//	scene->AddActor(std::make_unique<Asteroid>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 2.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("LargeAsteroid.txt"), 10.0f, "Large"));
		//	scene->AddActor(std::make_unique<Asteroid>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 2.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("LargeAsteroid.txt"), 10.0f, "Large"));
		//	stateTimer = 0;
		//}
		break;
	case Game::eState::GameOver:
		if (waitTimer) {
			stateTimer = 0;
			waitTimer = false;
		}
		scene->RemoveAllActors();
		if (stateTimer >= 5) {
			if (score > HighScore) HighScore = score;
			state = eState::TitleScreen;
			stateTimer = 0;
		}
		break;
	case Game::eState::GameWin:
		if (waitTimer) {
			stateTimer = 0;
			waitTimer = false;
		}
		scene->RemoveAllActors();
		if (stateTimer >= 5) {
			if (score > HighScore) HighScore = score;
			state = eState::TitleScreen;
			stateTimer = 0;
		}
		break;
	default:
		break;
	}

	//Main Code

	if (engine->Get<gn::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == gn::InputSystem::eKeyState::Pressed) {
		quit = true;
	}

	if (engine->Get<gn::InputSystem>()->GetButtonState((int)gn::InputSystem::eMouseButton::Left) == gn::InputSystem::eKeyState::Pressed) {

		gn::Vector2 position = engine->Get<gn::InputSystem>()->GetMousePosition();
		//Create Particles
		scene->engine->Get<gn::ParticleSystem>()->Create(position, 50, 10.0f, engine->Get<gn::ResourceSystem>()->Get<gn::Texture>("particle01.png", engine->Get<gn::Renderer>()), 100.0f);
		engine->Get<gn::AudioSystem>()->PlayAudio("explosion", 1, gn::RandomRange(.2f, 2.0f));
		musicChannel.SetPitch(musicChannel.GetPitch() + .1f);

	}

	//if (engine.time.time >= quitTime) quit = true;
	engine->time.timeScale = 10;

	//Main Code End

	scene->Update(engine->time.deltaTime);
}

void Game::Draw(){
	int size = 20;
	std::shared_ptr<gn::Font> font = engine->Get<gn::ResourceSystem>()->Get<gn::Font>("Fonts/Arcade.ttf", &size);
	engine->Get<gn::Renderer>() -> BeginFrame();
	switch (state)
	{
	case Game::eState::TitleScreen:
		{
			gn::Transform titleText;
			std::shared_ptr<gn::Texture> titleTextTexture;
			titleText.position = { 400, 300 };

			titleTextTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
			titleTextTexture->Create(font->CreateSurface("This Isn't Asteroids", gn::Color{ 0, 0, 1 }));
			engine->Get<gn::ResourceSystem>()->Add("title", titleTextTexture);


			engine->Get<gn::Renderer>()->Draw(titleTextTexture, titleText);

		}



		// add font texture to resource system

		//graphics.SetColor(gn::Color::blue);
		//graphics.DrawString(340, 300 + static_cast<int>(std::sin(stateTimer * 2) * 50), "This Isn't Asteroids");
		//graphics.SetColor(gn::Color::green);
		//graphics.DrawString(340, 400, "Press Space to Start");
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
		//graphics.SetColor(gn::Color::red);
		//graphics.DrawString(370, 300 + static_cast<int>(std::sin(stateTimer * 2) * 50), "GAME OVER");
		//if (score > HighScore) {
 	//		graphics.DrawString(350, 20, "HIGHSCORE!!!");
		//}
		break;
	case Game::eState::GameWin:
		//graphics.SetColor(gn::Color::red);
		//graphics.DrawString(370, 300 + static_cast<int>(std::sin(stateTimer * 2) * 50), "YOU WIN!");
		//if (score > HighScore) {
		//	graphics.DrawString(350, 20, "HIGHSCORE!!!");
		//}
		break;
	default:
		break;
	}

	//graphics.SetColor(gn::Color::white);
	//graphics.DrawString(100, 20, std::to_string(HighScore).c_str());
	//graphics.DrawString(10, 20, "High Score: ");
	//graphics.DrawString(60,40, std::to_string(score).c_str());
	//graphics.DrawString(10,40, "Score: ");
	//graphics.DrawString(780,20, std::to_string(lives).c_str());
	//graphics.DrawString(730,20, "Lives: ");

	//Main Code

		// Draw the font texture in the update loop


	engine->Draw(engine->Get<gn::Renderer>());
	scene->Draw(engine->Get<gn::Renderer>());

	engine->Get<gn::Renderer>()->EndFrame();
	//Main Code End

}

void Game::UpdateTitle(float dt){
	//if (Core::Input::IsPressed(VK_SPACE)) {
	//	state = eState::StartGame;
	//}
}

void Game::UpdateLevelStart(float dt){

	//scene->AddActor(std::make_unique<Player>(gn::Transform{ {400,300},0.0f, 5.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("PlayerShape.txt"), 300.0f));
	//for (size_t i = 0; i < 2; i++) {
	//	scene->AddActor(std::make_unique<Enemy>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 3.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("EnemyShape.txt"), 50.0f));
	//}
	//for (size_t i = 0; i < 10; i++) {
	//	scene->AddActor(std::make_unique<Asteroid>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 2.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("LargeAsteroid.txt"), 10.0f, "Large"));
	//}
	//stateTimer = 0;
	//waitTimer = true;	
	//state = eState::Game;
}

void Game::OnAddPoints(const gn::Event& event){
	score += std::get<int>(event.data);
	if (score >= 2000 && lifeCounter == 0) {
		lives++;
		lifeCounter++;
	}
	if (score >= 4000 && lifeCounter == 1) {
		lives++;
		lifeCounter++;
	}
	if (score >= 6000 && lifeCounter == 2) {
		lives++;
		lifeCounter++;
	}
}

void Game::OnPlayerDead(const gn::Event& event){
	lives--;
 	std::cout << std::get<std::string>(event.data) << std::endl;
	if (lives <= 0) {
		state = eState::GameOver;
	}
}
