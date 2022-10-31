// ================================================================
//
// actorHUD.hpp
//
// UI widget that displays life bars for actors.
//
// ================================================================

#pragma once

class Actor;

#include <SFML/Graphics.hpp>

class ActorHUD {
public:
	ActorHUD();

	// Updates the position of the actor HUD.
	void Update(Actor* actor);

	// Draws the actor HUD.
	void Draw(sf::RenderTarget& window);

	// Updates the life bar.
	void UpdateLifeBar(Actor* actor);

private:

	// ================================
	sf::RectangleShape barBackground;
	sf::RectangleShape barLife;
};