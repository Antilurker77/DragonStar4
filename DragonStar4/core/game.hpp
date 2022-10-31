// ================================================================
//
// game.hpp
// 
// Handles the core game loop.
//
// ================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "camera.hpp"
#include "gameState.hpp"
#include "../scene/characterCreationScene.hpp"
#include "../scene/dungeonScene.hpp"
#include "../scene/mainMenuScene.hpp"

class Game {
public:
	Game();

	// Runs the game.
	void Run();

private:
	// Reads player input.
	void processInput();

	// Updates game logic.
	void update(float secondsPerUpdate);

	// Draws the game to the window.
	void draw(float timeRatio);

	// ================================
	sf::RenderWindow window;

	sf::View uiView;
	Camera camera;

	GameState gameState{};

	MainMenuScene mainMenuScene;
	CharacterCreationScene characterCreationScene;
	DungeonScene dungeonScene;
};
