#pragma once
#include "Engine.h"

#include <memory>

class Game {

public:
	enum class eState {
		TitleScreen,
		StartGame,
		StartLevel,
		Game,
		GameOver,
		GameWin
	};

public:
	void Initialize();
	void Shutdown();
	bool IsQuit() { return quit; }
	void Update();
	void Draw();

private:
	void UpdateTitle(float dt);
	void UpdateLevelStart(float dt);
	void OnAddPoints(const gn::Event& event);
	void OnPlayerDead(const gn::Event& event);

public:
	std::unique_ptr<gn::Engine> engine;
	std::unique_ptr<gn::Scene> scene;

private:
	bool quit = false;
	eState state = eState::TitleScreen;
	float stateTimer = 0.0f;
	size_t HighScore = 0;
	size_t lifeCounter = 0;
	size_t score = 0;
	size_t lives = 0;
	bool waitTimer = true;

	gn::AudioChannel musicChannel;
	std::shared_ptr<gn::Texture> particleTexture;
	std::shared_ptr<gn::Texture> textTexture;
};