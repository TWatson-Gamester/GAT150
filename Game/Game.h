#pragma once
#include "Engine.h"

#include <memory>

class Game {

	enum class eState {
		Reset,
		Title,
		StartGame,
		StartLevel,
		Level,
		PlayerDead,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();
	bool IsQuit() { return quit; }
	void Update();
	void Draw();

	void OnAddScore(const gn::Event& event);

	void Reset();
	void Title();
	void StartGame();
	void StartLevel();
	void Level();
	void PlayerDead();
	void GameOver();

public:
	std::unique_ptr<gn::Engine> engine;
	std::unique_ptr<gn::Scene> scene;

private:
	bool quit = false;
	eState state = eState::Reset;
	int score = 0; 
	float stateTimer = 0;
	float spawnTimer = 0;
};