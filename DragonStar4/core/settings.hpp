// ================================================================
//
// settings.hpp
// 
// Global struct that stores setting information.
//
// ================================================================

#pragma once

#include <string>
#include <SFML/Window.hpp>

struct CameraKeys {
	sf::Keyboard::Key CameraUp = sf::Keyboard::Up;
	sf::Keyboard::Key CameraDown = sf::Keyboard::Down;
	sf::Keyboard::Key CameraLeft = sf::Keyboard::Left;
	sf::Keyboard::Key CameraRight = sf::Keyboard::Right;
	sf::Keyboard::Key ResetZoom = sf::Keyboard::F;
};

struct MoveKeys {
	//sf::Keyboard::Key MoveN = sf::Keyboard::K;
	//sf::Keyboard::Key MoveS = sf::Keyboard::J;
	//sf::Keyboard::Key MoveE = sf::Keyboard::L;
	//sf::Keyboard::Key MoveW = sf::Keyboard::H;
	//sf::Keyboard::Key MoveNW = sf::Keyboard::Y;
	//sf::Keyboard::Key MoveNE = sf::Keyboard::U;
	//sf::Keyboard::Key MoveSW = sf::Keyboard::B;
	//sf::Keyboard::Key MoveSE = sf::Keyboard::N;
	//sf::Keyboard::Key Wait = sf::Keyboard::Period;

	sf::Keyboard::Key MoveN = sf::Keyboard::W;
	sf::Keyboard::Key MoveS = sf::Keyboard::S;
	sf::Keyboard::Key MoveE = sf::Keyboard::D;
	sf::Keyboard::Key MoveW = sf::Keyboard::A;
	sf::Keyboard::Key MoveNW = sf::Keyboard::Q;
	sf::Keyboard::Key MoveNE = sf::Keyboard::E;
	sf::Keyboard::Key MoveSW = sf::Keyboard::Z;
	sf::Keyboard::Key MoveSE = sf::Keyboard::C;
	sf::Keyboard::Key Wait = sf::Keyboard::X;

	sf::Keyboard::Key MoveNAlt = sf::Keyboard::Numpad8;
	sf::Keyboard::Key MoveSAlt = sf::Keyboard::Numpad2;
	sf::Keyboard::Key MoveEAlt = sf::Keyboard::Numpad6;
	sf::Keyboard::Key MoveWAlt = sf::Keyboard::Numpad4;
	sf::Keyboard::Key MoveNWAlt = sf::Keyboard::Numpad7;
	sf::Keyboard::Key MoveNEAlt = sf::Keyboard::Numpad9;
	sf::Keyboard::Key MoveSWAlt = sf::Keyboard::Numpad1;
	sf::Keyboard::Key MoveSEAlt = sf::Keyboard::Numpad3;
	sf::Keyboard::Key WaitAlt = sf::Keyboard::Numpad5;
};

struct Settings {
	uint32_t ScreenWidth = 1280;
	uint32_t ScreenHeight = 720;
	float ScreenWidthF = static_cast<float>(ScreenWidth);
	float ScreenHeightF = static_cast<float>(ScreenHeight);

	bool EnableVSync = true;
	bool EnableFullscreen = false;
	bool DrawFPS = true;

	std::string Font = "gfx/font/B612Mono-Regular.ttf";
	std::string Tileset = "standard";

	int TileSize = 40;
	float TileSizeF = static_cast<float>(TileSize);

	CameraKeys CameraKeybinds;
	MoveKeys MoveKeybinds;
};

extern Settings settings;
