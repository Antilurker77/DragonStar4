// ================================================================
//
// camera.hpp
// 
// Manages camera view, camera movement, and camera input.
//
// ================================================================

#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

class Camera {
public:
	// Default location is left corner at 0, 0.
	Camera(sf::Vector2f size);

	// Sets size and center.
	Camera(sf::Vector2f pos, sf::Vector2f size);

	// Returns the view.
	sf::View GetCamera();

	// Moves the camera to the designated position.
	void SetPosition(sf::Vector2f pos);

	// Moves the camera centered on the designated tile.
	void SetPositionTile(sf::Vector2i tilePos);

	// Moves the camera to the designated position at a give units per second.
	void Move(sf::Vector2f pos, float unitsPerSecond);

	// Resizes the camera.
	void SetSize(sf::Vector2f size);

	// Sets the camera's zoom.
	void SetZoom(float cameraZoom);

	// Makes camera movement smooth.
	void Interpolate(float intlop);

	// Checks for input from player.
	void GetInput(sf::Event event);

	// Adjusts camera speed based on how long it takes inbetween frames.
	void Update(float secondsPerUpdate);

private:
	sf::View camera;
	float sensitivty = 400.f;
	float zoom = 1.f;
	sf::Vector2f velocity = { 0.f, 0.f };
	sf::Vector2f velocityPerUpdate = { 0.f, 0.f };

	sf::Vector2f currentPos;
	sf::Vector2f updatePos;
	sf::Vector2f goalPos;

	bool moveUp = false;
	bool moveDown = false;
	bool moveLeft = false;
	bool moveRight = false;
};

