// ================================================================
//
// moveCommand.cpp
//
// ================================================================

#include "moveCommand.hpp"

#include "../core/camera.hpp"
#include "../entity/actor.hpp"

MoveCommand::MoveCommand(sf::Vector2i tile, int64_t moveCostMod, Camera* c) {
	targetTile = tile;
	moveMod = moveCostMod;
	camera = c;
}

void MoveCommand::Execute(Actor* actor) {
	if (camera != nullptr) {
		sf::Vector2f start = actor->GetPosition();
		start.x += 16.f;
		start.y += 20.f;
		camera->SetPosition(start);
	}
	
	actor->Walk(targetTile, moveMod);

	if (camera != nullptr) {
		sf::Vector2f finish = { targetTile.x * 32.f + 16.f, targetTile.y * 32.f + 20.f };
		camera->Move(finish, 256.f);
	}
}