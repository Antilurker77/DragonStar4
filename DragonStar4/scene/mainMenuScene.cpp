// ================================================================
//
// mainMenuScene.cpp
//
// ================================================================

#include "mainMenuScene.hpp"

#include "../core/assetManager.hpp"
#include "../core/gameState.hpp"
#include "../core/settings.hpp"

MainMenuScene::MainMenuScene() {
	titleText.setString("Dragon Star");
	titleText.setCharacterSize(64);
	titleText.setFont(*assetManager.LoadFont(settings.Font));

	versionText.setString("v0.4.0");
	versionText.setCharacterSize(16);
	versionText.setFont(*assetManager.LoadFont(settings.Font));

	newGameButton.SetString("New Game", 32);

	SetPositions();
}

void MainMenuScene::ReadInput(sf::RenderWindow& window) {
	sf::Event ev;
	sf::Mouse mouse;

	leftClick = false;
	rightClick = false;
	mousePos = mouse.getPosition(window);

	while (window.pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::MouseButtonReleased:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				leftClick = true;
			}
			else if (ev.mouseButton.button == sf::Mouse::Right) {
				rightClick = true;
			}
			break;
		default:
			break;
		}
	}

}

GameState MainMenuScene::Update(float secondsPerUpdate) {
	GameState gs = GameState::MainMenu;

	if (newGameButton.Update(secondsPerUpdate, mousePos)) {
		if (leftClick) {
			gs = GameState::CharacterCreation;
		}
	}

	return gs;
}

void MainMenuScene::Draw(sf::RenderTarget& window, float timeRatio) {
	window.draw(titleText);
	window.draw(versionText);
	newGameButton.Draw(window);
}

void MainMenuScene::SetPositions() {
	auto titleTextSize = titleText.getLocalBounds();
	auto versionTextSize = versionText.getLocalBounds();
	sf::Vector2f pos;

	pos.x = std::roundf(settings.ScreenWidthF * 0.5f - titleTextSize.width / 2.f);
	pos.y = std::roundf(settings.ScreenHeightF * 0.3f - titleTextSize.height / 2.f);
	titleText.setPosition(pos);

	pos.x = std::roundf(settings.ScreenWidthF - versionTextSize.width - 4.f);
	pos.y = std::roundf(settings.ScreenHeightF - versionTextSize.height - 4.f);
	versionText.setPosition(pos);

	pos.x = std::roundf(settings.ScreenWidthF * 0.5f);
	pos.y = std::roundf(settings.ScreenHeightF * 0.7f);
	newGameButton.SetPositionCenter(pos.x, pos.y);
}