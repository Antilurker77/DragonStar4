// ================================================================
//
// camera.cpp
//
// ================================================================

#include "camera.hpp"

#include <iostream>
#include "settings.hpp"

Camera::Camera(sf::Vector2f size) {
	camera.setSize(size);
	camera.setCenter(sf::Vector2f(size.x / 2, size.y / 2));
	updatePos = camera.getCenter();
	goalPos = updatePos;
}

Camera::Camera(sf::Vector2f pos, sf::Vector2f size) {
	camera.setSize(size);
	camera.setCenter(pos);
	updatePos = pos;
	goalPos = pos;
}

sf::View Camera::GetCamera() {
	return camera;
}

void Camera::SetPosition(sf::Vector2f pos) {
	camera.setCenter(pos);
	currentPos = pos;
	updatePos = pos;
	goalPos = pos;
	velocity = { 0.f, 0.f };
	velocityPerUpdate = { 0.f, 0.f };
}

void Camera::Move(sf::Vector2f pos, float unitsPerSecond) {
	goalPos = pos;
	
	float distance = sqrt(powf(pos.x - currentPos.x, 2.f) + powf(pos.y - currentPos.y, 2.f));
	velocity.x = (unitsPerSecond / distance) * (pos.x - currentPos.x);
	velocity.y = (unitsPerSecond / distance) * (pos.y - currentPos.y);
}

void Camera::SetSize(sf::Vector2f size) {
	camera.setSize(size);
}

void Camera::SetZoom(float cameraZoom) {
	camera.zoom(1.f / zoom);
	camera.zoom(cameraZoom);
	zoom = cameraZoom;
}

void Camera::GetInput(sf::Event event) {
	moveUp = false;
	moveDown = false;
	moveLeft = false;
	moveRight = false;
	
	
	// Repeats while key is held down.
	if (sf::Keyboard::isKeyPressed(settings.CameraKeybinds.CameraUp)) {
		//velocity.y = -sensitivty * zoom;
		moveUp = true;
	}

	if (sf::Keyboard::isKeyPressed(settings.CameraKeybinds.CameraRight)) {
		//velocity.x = sensitivty * zoom;
		moveRight = true;
	}

	if (sf::Keyboard::isKeyPressed(settings.CameraKeybinds.CameraDown)) {
		//velocity.y = sensitivty * zoom;
		moveDown = true;
	}

	if (sf::Keyboard::isKeyPressed(settings.CameraKeybinds.CameraLeft)) {
		//velocity.x = -sensitivty * zoom;
		moveLeft = true;
	}

	if (sf::Keyboard::isKeyPressed(settings.CameraKeybinds.ResetZoom)) {
		camera.zoom(1.f / zoom);
		zoom = 1.f;
	}

	if (event.type == sf::Event::MouseWheelScrolled) {
		//std::cout << "mouse wheel scroll: " << event.mouseWheelScroll.delta << "\n"; // this line stops the camera from infinitely zooming and I have no idea why SFML plz
		// up -- zoom in
		//if (event.mouseWheelScroll.delta == 1) {
		//	if (zoom == 0.5f) {
		//		camera.zoom(1.f / zoom);
		//		camera.zoom(1.f / 3.f);
		//		zoom = 1.f / 3.f;
		//	}
		//	else if (zoom == 1.f) {
		//		camera.zoom(0.5f);
		//		zoom = 0.5f;
		//	}
		//}
		//// down -- zoom out
		//if (event.mouseWheelScroll.delta == -1) {
		//	if (zoom == 1.f / 3.f) {
		//		camera.zoom(1.f / zoom);
		//		camera.zoom(0.5f);
		//		zoom = 0.5f;
		//	}
		//	else if (zoom == 0.5f) {
		//		camera.zoom(1.f / zoom);
		//		zoom = 1.f;
		//	}
		//}
	}
}

void Camera::Update(float secondsPerUpdate) {
	camera.setCenter(updatePos);
	currentPos = camera.getCenter();

	if (moveUp) {
		velocity.y = -sensitivty * zoom;
		goalPos.y = currentPos.y + std::roundf(velocity.y * secondsPerUpdate);
	}
	if (moveDown) {
		velocity.y = sensitivty * zoom;
		goalPos.y = currentPos.y + std::roundf(velocity.y * secondsPerUpdate);
	}
	if (moveLeft) {
		velocity.x = -sensitivty * zoom;
		goalPos.x = currentPos.x + std::roundf(velocity.x * secondsPerUpdate);
	}
	if (moveRight) {
		velocity.x = sensitivty * zoom;
		goalPos.x = currentPos.x + std::roundf(velocity.x * secondsPerUpdate);
	}

	if (moveUp && moveDown) {
		velocity.y = 0.f;
		goalPos.y = currentPos.y;
	}

	if (moveLeft && moveRight) {
		velocity.x = 0.f;
		goalPos.x = currentPos.x;
	}

	if (currentPos != goalPos) {
		updatePos = currentPos;
		velocityPerUpdate = { std::roundf(velocity.x * secondsPerUpdate), std::roundf(velocity.y * secondsPerUpdate) };

		if (abs(goalPos.x - currentPos.x) < abs(velocityPerUpdate.x)) {
			updatePos.x = goalPos.x;
		}
		else {
			updatePos.x += velocityPerUpdate.x;
		}

		if (abs(goalPos.y - currentPos.y) < abs(velocityPerUpdate.y)) {
			updatePos.y = goalPos.y;
		}
		else {
			updatePos.y += velocityPerUpdate.y;
		}
	}
	else {
		velocity = { 0.f, 0.f };
		velocityPerUpdate = { 0.f, 0.f };
		updatePos = goalPos;
	}
}

void Camera::Interpolate(float intlop) {
	sf::Vector2f interlopPos = { currentPos.x + velocityPerUpdate.x * intlop, currentPos.y + velocityPerUpdate.y * intlop };
	
	if (abs(updatePos.x - currentPos.x) < abs(velocityPerUpdate.x * intlop)) {
		interlopPos.x = updatePos.x;
	}

	if (abs(updatePos.y - currentPos.y) < abs(velocityPerUpdate.y * intlop)) {
		interlopPos.y = updatePos.y;
	}

	camera.setCenter(interlopPos);
}