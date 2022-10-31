// ================================================================
//
// button.cpp
//
// ================================================================

#include "button.hpp"

#include "../core/assetManager.hpp"
#include "../core/settings.hpp"

const float Button::margin = 4.f;

Button::Button() {
	background.setFillColor(sf::Color(0, 0, 0, 255));
	background.setOutlineThickness(1.f);
	background.setOutlineColor(sf::Color(255, 255, 255, 255));

	buttonText.setFont(*assetManager.LoadFont(settings.Font));
	buttonText.setCharacterSize(16);
}

bool Button::Update(float secondsPerUpdate, sf::Vector2i mousePos) {
	if (background.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
		background.setFillColor(sf::Color(255, 255, 255, 255));
		buttonText.setFillColor(sf::Color(0, 0, 0, 255));
		return true;
	}
	else {
		background.setFillColor(sf::Color(0, 0, 0, 255));
		buttonText.setFillColor(sf::Color(255, 255, 255, 255));
	}

	return false;
}

void Button::Draw(sf::RenderTarget& window) {
	window.draw(background);
	window.draw(buttonText);
}

void Button::SetString(std::string s) {
	buttonText.setString(s);

	calcSize();
}

void Button::SetString(std::string s, unsigned int size) {
	buttonText.setCharacterSize(size);
	buttonText.setString(s);

	calcSize();
}

void Button::SetPosition(float x, float y) {
	x = std::roundf(x);
	y = std::roundf(y);

	background.setPosition(x, y);
	buttonText.setPosition(x + margin, y + margin);
}

void Button::SetPositionCenter(float x, float y) {
	x -= background.getSize().x / 2.f;
	y -= background.getSize().y / 2.f;
	
	x = std::roundf(x);
	y = std::roundf(y);

	background.setPosition(x, y);
	buttonText.setPosition(x + margin, y + margin);
}

sf::Vector2f Button::GetSize() {
	return background.getSize();
}

void Button::calcSize() {
	sf::Vector2f size;
	size.x = buttonText.getLocalBounds().width + buttonText.getLocalBounds().left + margin * 2.f;
	size.y = buttonText.getLocalBounds().height + buttonText.getLocalBounds().top + margin * 2.f;

	background.setSize(size);
}