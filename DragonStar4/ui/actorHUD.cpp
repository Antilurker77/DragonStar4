// ================================================================
//
// actorHUD.cpp
//
// ================================================================

#include "actorHUD.hpp"

#include "../core/tileMath.hpp"
#include "../entity/actor.hpp"

ActorHUD::ActorHUD() {
	barBackground.setSize(sf::Vector2f(20.f, 4.f));
	barBackground.setFillColor(sf::Color(150, 50, 50, 255));
	barBackground.setOutlineThickness(1.f);
	barBackground.setOutlineColor(sf::Color(255, 255, 255, 255));

	barLife.setSize(sf::Vector2f(20.f, 4.f));
	barLife.setFillColor(sf::Color(50, 200, 50, 255));
}

void ActorHUD::Update(Actor* actor) {
	sf::Vector2f pos = actor->GetPosition();

	pos.x += 6.f;
	pos.y += 34.f;

	if (actor->IsPlayer()) {
		pos.y += 4.f;
	}

	barBackground.setPosition(pos);
	barLife.setPosition(pos);
}

void ActorHUD::Draw(sf::RenderTarget& window) {
	window.draw(barBackground);
	window.draw(barLife);
}

void ActorHUD::UpdateLifeBar(Actor* actor) {
	float ratio = static_cast<float>(actor->GetCurrentHP()) / static_cast<float>(actor->GetMaxHP());
	ratio *= 20.f;

	// Always display a little sliver of life bar if the actor is alive.
	if (ratio < 1.f) {
		ratio = 1.f;
	}

	ratio = roundf(ratio);

	barLife.setSize(sf::Vector2f(ratio, 4.f));
}