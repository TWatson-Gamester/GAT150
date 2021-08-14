#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Projectile.h"
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

	musicChannel = engine->Get<gn::AudioSystem>()->PlayAudio("Epic", 1, 1, true);


	/*std::shared_ptr<gn::Texture> texture = engine->Get<gn::ResourceSystem>()->Get<gn::Texture>("sf2.png", engine->Get<gn::Renderer>());

	for (size_t i = 0; i < 20; i++) {
		gn::Transform transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600)}, gn::RandomRange(0,360), 1.0f };
		std::unique_ptr<gn::Actor> actor = std::make_unique<gn::Actor>(transform, texture);
		scene->AddActor(std::move(actor));
	}*/

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
		musicChannel.Stop();
		if (engine->Get<gn::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == gn::InputSystem::eKeyState::Release) {
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		//musicChannel = engine->Get<gn::AudioSystem>()->PlayAudio("Epic", .5f, 1, true);
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
		if (stateTimer >= 10) {
			/*scene->AddActor(std::make_unique<Enemy>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 3.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("EnemyShape.txt"), 50.0f));
			scene->AddActor(std::make_unique<Asteroid>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 2.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("LargeAsteroid.txt"), 10.0f, "Large"));
			scene->AddActor(std::make_unique<Asteroid>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 2.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("LargeAsteroid.txt"), 10.0f, "Large"));*/
			stateTimer = 0;
		}
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

	}

	/*engine->time.timeScale = 10;*/

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
			//Create Transform
			gn::Transform titleText;
			gn::Transform continueText;
			
			//Create Texture
			std::shared_ptr<gn::Texture> titleTextTexture;
			std::shared_ptr<gn::Texture> continueTexture;

			//Set Transform Position
			titleText.position = { 400, 300 };
			continueText.position = { 400, 325 };

			//Set Texture to Renderer
			titleTextTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
			continueTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
			
			//Create Texture Text
			titleTextTexture->Create(font->CreateSurface("This Isn't Asteroids", gn::Color{ 0, 0, 1 }));
			continueTexture->Create(font->CreateSurface("Press Space to Start", gn::Color{ 0,1,0 }));

			//Add to ResourceSystem
			engine->Get<gn::ResourceSystem>()->Add("title", titleTextTexture);
			engine->Get<gn::ResourceSystem>()->Add("continue", continueTexture);

			//Draw Text
			engine->Get<gn::Renderer>()->Draw(titleTextTexture, titleText);
			engine->Get<gn::Renderer>()->Draw(continueTexture, continueText);

		}
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
	{
		//Create Transform
		gn::Transform gameOverText;

		//Create Texture
		std::shared_ptr<gn::Texture> gameOverTexture;

		//Set Transform Position
		gameOverText.position = { 400, 300 };

		//Set Texture to Renderer
		gameOverTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());

		//Create Texture Text
		gameOverTexture->Create(font->CreateSurface("GAME OVER", gn::Color{ 1, 0, 0 }));

		//Add to ResourceSystem
		engine->Get<gn::ResourceSystem>()->Add("gameWin", gameOverTexture);

		//Draw Text
		engine->Get<gn::Renderer>()->Draw(gameOverTexture, gameOverText);

		if (score > HighScore) {
			gn::Transform highscoreText;
			std::shared_ptr<gn::Texture> highscoreTexture;
			highscoreText.position = { 400, 325 };
			highscoreTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
			highscoreTexture->Create(font->CreateSurface("HIGHSCORE!!!", gn::Color{ 0,1,0 }));
			engine->Get<gn::ResourceSystem>()->Add("highscore", highscoreTexture);
			engine->Get<gn::Renderer>()->Draw(highscoreTexture, highscoreText);
		}
		break;
	}
	case Game::eState::GameWin:
	{
		//Create Transform
		gn::Transform gameWinText;

		//Create Texture
		std::shared_ptr<gn::Texture> gameWinTextTexture;

		//Set Transform Position
		gameWinText.position = { 400, 300 };

		//Set Texture to Renderer
		gameWinTextTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());

		//Create Texture Text
		gameWinTextTexture->Create(font->CreateSurface("YOU WIN!!!", gn::Color{ 1, 0, 0 }));

		//Add to ResourceSystem
		engine->Get<gn::ResourceSystem>()->Add("gameWin", gameWinTextTexture);

		//Draw Text
		engine->Get<gn::Renderer>()->Draw(gameWinTextTexture, gameWinText);

		if (score > HighScore) {
			gn::Transform highscoreText;
			std::shared_ptr<gn::Texture> highscoreTexture;
			highscoreText.position = { 400, 325 };
			highscoreTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
			highscoreTexture->Create(font->CreateSurface("HIGHSCORE!!!", gn::Color{ 0,1,0 }));
			engine->Get<gn::ResourceSystem>()->Add("highscore", highscoreTexture);
			engine->Get<gn::Renderer>()->Draw(highscoreTexture, highscoreText);
		}
	}
		break;
	default:
		break;
	}

	//Create Transform
	gn::Transform highScoreInt;
	gn::Transform highScoreText;
	gn::Transform scoreInt;
	gn::Transform scoreText;
	gn::Transform livesInt;
	gn::Transform livesText;

	//Create Texture
	std::shared_ptr<gn::Texture> highScoreIntTexture;
	std::shared_ptr<gn::Texture> highScoreTexture;
	std::shared_ptr<gn::Texture> scoreIntTexture;
	std::shared_ptr<gn::Texture> scoreTexture;
	std::shared_ptr<gn::Texture> livesIntTexture;
	std::shared_ptr<gn::Texture> livesTexture;

	//Set Transform Position
	highScoreInt.position = { 145, 20 };
	highScoreText.position = { 60, 20 };
	scoreInt.position = { 145, 40 };
	scoreText.position = { 85, 40 };
	livesInt.position = { 790, 20 };
	livesText.position = { 740, 20 };

	//Set Texture to Renderer
	highScoreIntTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
	highScoreTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
	scoreIntTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
	scoreTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
	livesIntTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());
	livesTexture = std::make_shared<gn::Texture>(engine->Get<gn::Renderer>());

	//Create Texture Text
	highScoreIntTexture->Create(font->CreateSurface(std::to_string(HighScore).c_str(), gn::Color{ 1, 1, 1 }));
	highScoreTexture->Create(font->CreateSurface("High Score:", gn::Color{ 1, 1, 1 }));
	scoreIntTexture->Create(font->CreateSurface(std::to_string(score).c_str(), gn::Color{ 1, 1, 1 }));
	scoreTexture->Create(font->CreateSurface("Score:", gn::Color{ 1, 1, 1 }));
	livesIntTexture->Create(font->CreateSurface(std::to_string(lives).c_str(), gn::Color{ 1, 1, 1 }));
	livesTexture->Create(font->CreateSurface("Lives:", gn::Color{ 1, 1, 1 }));

	//Add to ResourceSystem
	engine->Get<gn::ResourceSystem>()->Add("highScoreInt", highScoreIntTexture);
	engine->Get<gn::ResourceSystem>()->Add("highScoreText", highScoreTexture);
	engine->Get<gn::ResourceSystem>()->Add("scoreInt", scoreIntTexture);
	engine->Get<gn::ResourceSystem>()->Add("scoreText", scoreTexture);
	engine->Get<gn::ResourceSystem>()->Add("livesInt", livesIntTexture);
	engine->Get<gn::ResourceSystem>()->Add("livesText", livesTexture);

	//Draw Text
	engine->Get<gn::Renderer>()->Draw(highScoreIntTexture, highScoreInt);
	engine->Get<gn::Renderer>()->Draw(highScoreTexture, highScoreText);
	engine->Get<gn::Renderer>()->Draw(scoreIntTexture, scoreInt);
	engine->Get<gn::Renderer>()->Draw(scoreTexture, scoreText);
	engine->Get<gn::Renderer>()->Draw(livesIntTexture, livesInt);
	engine->Get<gn::Renderer>()->Draw(livesTexture, livesText);

	//Systems Draw
	engine->Draw(engine->Get<gn::Renderer>());
	scene->Draw(engine->Get<gn::Renderer>());

	engine->Get<gn::Renderer>()->EndFrame();
}

void Game::UpdateLevelStart(float dt){

	std::unique_ptr<Player> player = std::make_unique<Player>(gn::Transform({ 400,300 }, 0.0f, 0.5f), engine->Get<gn::ResourceSystem>()->Get<gn::Texture>("Player.png", engine->Get<gn::Renderer>()), 350.0f);
	scene->AddActor(std::move(player));
	//for (size_t i = 0; i < 2; i++) {
	//	scene->AddActor(std::make_unique<Enemy>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 3.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("EnemyShape.txt"), 50.0f));
	//}
	//for (size_t i = 0; i < 10; i++) {
	//	scene->AddActor(std::make_unique<Asteroid>(gn::Transform{ {gn::RandomRange(0,800),gn::RandomRange(0,600) }, gn::RandomRange(0, 800), 2.0f }, engine->Get<gn::ResourceSystem>()->Get<gn::Shape>("LargeAsteroid.txt"), 10.0f, "Large"));
	//}

	stateTimer = 0;
	waitTimer = true;	
	state = eState::Game;
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
