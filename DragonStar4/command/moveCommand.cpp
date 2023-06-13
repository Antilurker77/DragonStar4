// ================================================================
//
// moveCommand.cpp
//
// ================================================================

#include "moveCommand.hpp"

#include "../core/camera.hpp"
#include "../core/settings.hpp"
#include "../entity/actor.hpp"

MoveCommand::MoveCommand(sf::Vector2i tile, int64_t moveCostMod, Camera* c) {
	targetTile = tile;
	moveMod = moveCostMod;
	camera = c;
}

void MoveCommand::Execute(Actor* actor) {
	if (camera != nullptr) {
		sf::Vector2i start = actor->GetTileLocation();
		camera->SetPositionTile(start);
	}
	
	actor->Walk(targetTile, moveMod);

	if (camera != nullptr) {
		sf::Vector2f finish = { targetTile.x * settings.TileSizeF + 20.f, targetTile.y * settings.TileSizeF + 20.f };
		camera->Move(finish, 256.f);
	}
}