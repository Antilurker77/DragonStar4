// ================================================================
//
// game.cpp
//
// ================================================================

#include "game.hpp"

#include "random.hpp"
#include "settings.hpp"

// Time per update.
const sf::Time timePerUpdate = sf::seconds(1.f / 20.f);

// Game title.
const std::string gameTitle = "Dragon Star v0.4.0";

Game::Game() :
	window(sf::VideoMode(settings.ScreenWidth, settings.ScreenHeight), gameTitle),
	camera(sf::Vector2f(0.f, 0.f), sf::Vector2f(settings.ScreenWidthF, settings.ScreenHeightF)),
	uiView(sf::Vector2f(settings.ScreenWidthF / 2.f, settings.ScreenHeightF / 2.f), sf::Vector2f(settings.ScreenWidthF, settings.ScreenHeightF))
{
	window.setVerticalSyncEnabled(settings.EnableVSync);

	gameState = GameState::MainMenu;

	dungeonScene.SetCamera(&camera);
}

void Game::Run() {
	sf::Clock clock;
	sf::Time timeSinceUpdate = sf::Time::Zero;
	sf::Time x2 = sf::Time::Zero;
	float timeRatio = 0.f;
	float secondsPerUpdate = timePerUpdate.asSeconds();

	while (window.isOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceUpdate += elapsedTime;
		x2 += elapsedTime;
		timeRatio = 0.f;

		while (timeSinceUpdate > timePerUpdate) {
			timeSinceUpdate -= timePerUpdate;
			processInput();
			update(secondsPerUpdate);
			x2 = sf::Time::Zero;
		}

		timeRatio = x2.asSeconds() / timePerUpdate.asSeconds();
		draw(timeRatio);
		//fpsWindow.Update(elapsedTime);
	}
}

void Game::processInput() {
	switch (gameState) {
	case GameState::MainMenu:
		mainMenuScene.ReadInput(window);
		break;
	case GameState::CharacterCreation:
		characterCreationScene.ReadInput(window);
		break;
	case GameState::Dungeon:
		dungeonScene.ReadInput(window);
		break;
	default:
		break;
	}
}

void Game::update(float secondsPerUpdate) {
	switch (gameState) {
	case GameState::MainMenu:
		gameState = mainMenuScene.Update(secondsPerUpdate);
		break;
	case GameState::CharacterCreation:
		gameState = characterCreationScene.Update(secondsPerUpdate);

		// Transition from Character Creation to Dungeon
		if (gameState == GameState::Dungeon) {
			dungeonScene.Generate(Random::RandomSeed());
		}

		break;
	case GameState::Dungeon:
		gameState = dungeonScene.Update(secondsPerUpdate);
		break;
	default:
		break;
	}

	camera.Update(secondsPerUpdate);
}

void Game::draw(float timeRatio) {
	window.clear();

	camera.Interpolate(timeRatio);
	window.setView(camera.GetCamera());

	switch (gameState) {
	case GameState::Dungeon:
		dungeonScene.DrawWorld(window, timeRatio);
		break;
	default:
		break;
	}

	// UI
	window.setView(uiView);

	switch (gameState) {
	case GameState::MainMenu:
		mainMenuScene.Draw(window, timeRatio);
		break;
	case GameState::CharacterCreation:
		characterCreationScene.Draw(window, timeRatio);
		break;
	case GameState::Dungeon:
		dungeonScene.DrawUI(window, timeRatio);
		break;
	default:
		break;
	}

	window.display();
}