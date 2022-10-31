// ================================================================
//
// characterCreationScene.cpp
//
// ================================================================

#include "characterCreationScene.hpp"

#include "../core/assetManager.hpp"
#include "../core/gameState.hpp"
#include "../core/settings.hpp"

CharacterCreationScene::CharacterCreationScene() {
	SetPositions();
}

void CharacterCreationScene::ReadInput(sf::RenderWindow& window) {
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

GameState CharacterCreationScene::Update(float secondsPerUpdate) {
	return GameState::Dungeon;
}

void CharacterCreationScene::Draw(sf::RenderTarget& window, float timeRatio) {

}

void CharacterCreationScene::SetPositions() {

}