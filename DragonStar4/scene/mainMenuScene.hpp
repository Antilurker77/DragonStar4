// ================================================================
//
// mainMenuScene.hpp
// 
// Main menu UI.
//
// ================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include "../ui/button.hpp"

enum class GameState;

class MainMenuScene {
public:
	MainMenuScene();

	// Reads input.
	void ReadInput(sf::RenderWindow& window);

	// Updates the scene and returns the game state.
	GameState Update(float secondsPerUpdate);

	// Draws the scene.
	void Draw(sf::RenderTarget& window, float timeRatio);

	// Sets the positions of the UI elements. Should be called whenever the resolution
	// is changed.
	void SetPositions();

private:
	// Input
	bool leftClick = false;
	bool rightClick = false;
	sf::Vector2i mousePos;

	// UI
	sf::Text titleText;
	sf::Text versionText;

	Button newGameButton;
};