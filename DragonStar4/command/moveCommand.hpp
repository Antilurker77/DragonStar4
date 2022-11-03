// ================================================================
//
// moveCommand.hpp
//
// Command class that makes an actor move to a different tile.
//
// ================================================================

#pragma once

#include <SFML/System.hpp>
#include "command.hpp"

class Camera;

class MoveCommand : public Command {
public:
	MoveCommand(sf::Vector2i tile, int64_t moveCostMod, Camera* c = nullptr);
	void Execute(Actor* actor);
private:
	sf::Vector2i targetTile;
	int64_t moveMod = 0;
	Camera* camera = nullptr;
};