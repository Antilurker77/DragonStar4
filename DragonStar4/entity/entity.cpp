// ================================================================
//
// entity.cpp
//
// ================================================================

#include "entity.hpp"

#include "../core/assetManager.hpp"
#include "../core/random.hpp"
#include "../core/settings.hpp"

Entity::Entity() {

}

Entity::Entity(sf::Vector2f spawnPos, std::string filepath) {
	sprites.resize(1);
	sprites[0].setTexture(*assetManager.LoadTexture("gfx/" + settings.Tileset + "/" + filepath));
	sprites[0].setPosition(spawnPos);

	destination = spawnPos;
	updatePos = spawnPos;
}

Entity::Entity(sf::Vector2f spawnPos, std::vector<std::string> filepaths) {
	sprites.resize(filepaths.size());

	for (size_t i = 0; i < filepaths.size(); i++) {
		sprites[i].setTexture(*assetManager.LoadTexture("gfx/" + settings.Tileset + "/" + filepaths[i]));
		sprites[i].setPosition(spawnPos);
	}

	destination = spawnPos;
	updatePos = spawnPos;
}

void Entity::Update(float secondsPerUpdate) {
	if (!sprites.empty() && sprites[0].getPosition() != destination) {
		for (size_t i = 0; i < sprites.size(); i++) {
			sprites[i].setPosition(updatePos);
		}

		updatePos = sprites[0].getPosition();
		velocityPerUpdate = { velocity.x * secondsPerUpdate, velocity.y * secondsPerUpdate };

		// Make sure the entity doesn't overshoot it's destination.
		if (abs(destination.x - sprites[0].getPosition().x) < abs(velocityPerUpdate.x)) {
			updatePos.x = destination.x;
		}
		else {
			updatePos.x += velocityPerUpdate.x;
		}

		// Make sure the entity doesn't overshoot it's destination.
		if (abs(destination.y - sprites[0].getPosition().y) < abs(velocityPerUpdate.y)) {
			updatePos.y = destination.y;
		}
		else {
			updatePos.y += velocityPerUpdate.y;
		}
	}
	else {
		velocity = { 0.f, 0.f };
		velocityPerUpdate = { 0.f, 0.f };
		updatePos = destination;
	}

	UpdateExtend(secondsPerUpdate);
}

void Entity::UpdateExtend(float secondsPerUpdate) {

}

void Entity::Draw(sf::RenderTarget& window, float timeRatio) {
	if (!sprites.empty()) {
		if (IsMoving()) {
			sf::Vector2f oldPos = sprites[0].getPosition();
			sf::Vector2f interlopPos = { oldPos.x + velocityPerUpdate.x * timeRatio, oldPos.y + velocityPerUpdate.y * timeRatio };

			// Don't overshoot target destination.
			if (abs(destination.x - oldPos.x) < abs(velocityPerUpdate.x * timeRatio)) {
				interlopPos.x = destination.x;
			}
			if (abs(destination.y - oldPos.y) < abs(velocityPerUpdate.y * timeRatio)) {
				interlopPos.y = destination.y;
			}

			for (size_t i = 0; i < sprites.size(); i++) {
				sprites[i].setPosition(interlopPos);
				if (IsOnScreen(window)) {
					window.draw(sprites[i]);
				}
				sprites[i].setPosition(oldPos);
			}

			count++;
		}
		else {
			for (size_t i = 0; i < sprites.size(); i++) {
				if (IsOnScreen(window)) {
					window.draw(sprites[i]);
				}
			}
			count = 0;
		}
	}

	DrawExtend(window, timeRatio);
}

void Entity::DrawExtend(sf::RenderTarget& window, float timeRatio) {

}

sf::Vector2f Entity::GetPosition() {
	if (!sprites.empty()) {
		return sprites[0].getPosition();
	}
	return { 0.f, 0.f };
}

void Entity::SetSpriteCount(size_t count) {
	sprites.clear();
	sprites.resize(count);
}

void Entity::SetTexture(std::string filepath, size_t index) {
	if (index < sprites.size()) {
		sprites[index].setTexture(*assetManager.LoadTexture("gfx/" + settings.Tileset + "/" + filepath));
	}
}

void Entity::SetColor(sf::Color color) {
	for (size_t i = 0; i < sprites.size(); i++) {
		sprites[i].setColor(color);
	}
}

void Entity::Move(sf::Vector2f target, float unitsPerSecond) {
	if (!sprites.empty()) {
		if (unitsPerSecond == 0.f) {
			destination = target;
			for (size_t i = 0; i < sprites.size(); i++) {
				sprites[i].setPosition(target);
			}
		}
		else {
			destination = target;
			speed = unitsPerSecond;

			float distance = sqrt(powf(target.x - sprites[0].getPosition().x, 2.f) + powf(target.y - sprites[0].getPosition().y, 2.f));
			velocity.x = (unitsPerSecond / distance) * (target.x - sprites[0].getPosition().x);
			velocity.y = (unitsPerSecond / distance) * (target.y - sprites[0].getPosition().y);
		}
	}
}

bool Entity::IsMoving() {
	if (!sprites.empty()) {
		return (sprites[0].getPosition() != destination);
	}

	return false;
}

bool Entity::IsOnScreen(sf::RenderTarget& window) {
	if (!sprites.empty()) {
		sf::View view = window.getView();
		sf::FloatRect screen(view.getCenter().x - view.getSize().x / 2.f, view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);
		sf::FloatRect rect = sprites[0].getGlobalBounds();
		return screen.intersects(rect);
	}
	return false;
}