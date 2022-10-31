// ================================================================
//
// entity.hpp
// 
// Base class for any object that needs to move. If an entity is
// using multiple sprites, they will be stacked on top of each
// other.
//
// ================================================================

#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Entity {
public:
	Entity();
	Entity(sf::Vector2f spawnPos, std::string filepath);
	Entity(sf::Vector2f spawnPos, std::vector<std::string> filepaths);

	// Updates the entity's position (if moving).
	void Update(float secondsPerUpdate);

	// Hacky way to update UI elements for actors.
	virtual void UpdateExtend(float secondsPerUpdate);

	// Draws all the entity's sprites to the screen.
	void Draw(sf::RenderTarget& window, float timeRatio);

	// Hacky way to draw UI elements for actors.
	virtual void DrawExtend(sf::RenderTarget& window, float timeRatio);

	// Returns the position of this entity.
	sf::Vector2f GetPosition();

	// Sets the number of sprites in this entity.
	void SetSpriteCount(size_t count);

	// Sets the texture of the specified sprite.
	void SetTexture(std::string filepath, size_t index = 0);

	// Sets the color of the sprites.
	void SetColor(sf::Color color);

	// Moves the sprite to the specified location. If the velocity is set to 0, the move will be instant.
	void Move(sf::Vector2f target, float unitsPerSecond = 0.f);

	// Returns true if the entity is currently moving.
	bool IsMoving();

	// Returns true if the entity is on screen.
	bool IsOnScreen(sf::RenderTarget& window);

protected:
	// ================================
	std::vector<sf::Sprite> sprites;
	sf::Vector2f velocity = { 0.f, 0.f };
	sf::Vector2f velocityPerUpdate = { 0.f, 0.f };
	sf::Vector2f updatePos = { 0.f, 0.f };
	sf::Vector2f destination = { 0.f, 0.f };
	float speed = 0.f;

	size_t count = 0;
};
