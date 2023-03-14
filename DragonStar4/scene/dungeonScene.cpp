// ================================================================
//
// dungeonScene.cpp
//
// ================================================================

#include "dungeonScene.hpp"

#include <iostream>
#include <unordered_map>
#include "../command/abilityCommand.hpp"
#include "../command/moveCommand.hpp"
#include "../command/waitCommand.hpp"
#include "../core/assetManager.hpp"
#include "../core/camera.hpp"
#include "../core/gameState.hpp"
#include "../core/random.hpp"
#include "../core/settings.hpp"
#include "../core/tileMath.hpp"
#include "../data/gameData.hpp"
#include "../entity/player.hpp"
#include "../ui/messageLog.hpp"

DungeonScene::DungeonScene() {
	dungeonTileTexture = assetManager.LoadTexture("gfx/" + settings.Tileset + "/dungeon/dungeon.png");

	actors.push_back(std::make_shared<Player>("Test Char", 0));
	actors[0]->MoveToTile(sf::Vector2i(1, 1), 0.f);
}

void DungeonScene::ReadInput(sf::RenderWindow& window) {
	sf::Event ev;
	sf::Mouse mouse;

	command = nullptr;
	leftClick = false;
	rightClick = false;
	mousePos = mouse.getPosition(window);
	sf::Vector2i destination = actors[0]->GetTileLocation();

	while (window.pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::MouseButtonReleased:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				leftClick = true;
			}
			else if (ev.mouseButton.button == sf::Mouse::Right) {
				rightClick = true;
			}
			break;
		case sf::Event::KeyReleased:
			if (ev.key.code == settings.MoveKeybinds.MoveN || ev.key.code == settings.MoveKeybinds.MoveNAlt) {
				destination.y -= 1;
			}
			else if (ev.key.code == settings.MoveKeybinds.MoveW || ev.key.code == settings.MoveKeybinds.MoveWAlt) {
				destination.x -= 1;
			}
			else if (ev.key.code == settings.MoveKeybinds.MoveS || ev.key.code == settings.MoveKeybinds.MoveSAlt) {
				destination.y += 1;
			}
			else if (ev.key.code == settings.MoveKeybinds.MoveE || ev.key.code == settings.MoveKeybinds.MoveEAlt) {
				destination.x += 1;
			}
			else if (ev.key.code == settings.MoveKeybinds.Wait || ev.key.code == settings.MoveKeybinds.WaitAlt) {
				command = std::make_shared<WaitCommand>(100);
			}

			//debug
			else if (ev.key.code == sf::Keyboard::R) {
				floorSeeds[0] = Random::RandomSeed();
				generateFloor();
			}

			if (actors[0]->GetTileLocation() != destination) {
				Actor* actorAtTile = GetActorAtTile(destination);

				// Occupied Square: Bump Attack
				if (actorAtTile != nullptr && actorAtTile->IsAlive()) {
					std::vector<Actor*> targets = { actorAtTile };
					command = std::make_shared<AbilityCommand>(0, targets);
				}
				// Unoccupied Square: Movement
				else {
					TileData* td = GetTileData(destination);
					if (td->IsPassible) {
						command = std::make_shared<MoveCommand>(destination, td->MoveMod, camera);
					}
				}
			}
			break;
		default:
			break;
		}
	}

	if (camera != nullptr) {
		camera->GetInput(ev);
	}
}

GameState DungeonScene::Update(float secondsPerUpdate) {
	GameState gs = GameState::Dungeon;
	
	updateWorld(secondsPerUpdate);
	gs = updateUI(secondsPerUpdate);

	return gs;
}

void DungeonScene::DrawWorld(sf::RenderTarget& window, float timeRatio) {
	window.draw(floorVertexArray, dungeonTileTexture);

	// todo: sort by y for layering
	for (auto& a : actors) {
		if (a->IsAlive()) {
			a->Draw(window, timeRatio);
		}
	}
}

void DungeonScene::DrawUI(sf::RenderTarget& window, float timeRatio) {
	messageLog.Draw(window, timeRatio);
}

void DungeonScene::SetCamera(Camera* c) {
	camera = c;
}

void DungeonScene::SetPositions() {

}

void DungeonScene::Generate(uint64_t seed) {
	masterSeed = seed;

	pcg64 pcg(seed);
	floorSeeds.clear();
	floorSeeds.resize(100);
	std::generate(floorSeeds.begin(), floorSeeds.end(), [&pcg]() {return Random::RandomSeed(pcg); });

	generateFloor();
}

std::vector<sf::Vector2i> DungeonScene::Pathfind(sf::Vector2i start, sf::Vector2i end) {
	std::vector<sf::Vector2i> path{};

	std::vector<std::pair<sf::Vector2i, int64_t>> frontier{};
	std::unordered_map<sf::Vector2i, sf::Vector2i> cameFrom{};
	std::unordered_map<sf::Vector2i, int64_t> costSoFar{};

	frontier.push_back({ start, 0 });
	cameFrom[start] = start;
	costSoFar[start] = 0;

	while (!frontier.empty()) {
		// Sort frontier by cost for optimization.
		std::sort(frontier.begin(), frontier.end(), [](const std::pair<sf::Vector2i, int64_t>& left, const std::pair<sf::Vector2i, int64_t>& right) {return left.second < right.second; });

		sf::Vector2i current = frontier.front().first;
		if (current == end) {
			break;
		}

		// Get unoccupied tiles the actor can travel to.
		std::vector<sf::Vector2i> neighboors = TileMath::Neighboors(current);
		std::vector<sf::Vector2i> walkableNeighboors{};
		for (auto& t : neighboors) {
			if (IsPassible(t) && GetActorAtTile(t) == nullptr) {
				walkableNeighboors.push_back(t);
			}
		}

		// The algorithm.
		for (auto& next : walkableNeighboors) {
			int64_t cost = costSoFar[current] + std::min(GetLineCost(current, next, false), GetLineCost(current, next, true));
			if (costSoFar.find(next) == costSoFar.end() || cost < costSoFar[next]) {
				costSoFar[next] = cost;
				int64_t priority = cost + std::min(GetLineCost(next, end, false), GetLineCost(next, end, true));
				cameFrom[next] = current;
				frontier.push_back({ next, priority });
			}
		}

		frontier.erase(frontier.begin());
	}

	// Create the vector and return the path.
	sf::Vector2i current = end;
	path.push_back(current);

	while (current != start) {
		auto i = cameFrom.find(current);
		if (i != cameFrom.end()) { // Prevents the algorithm from adding the end to the path if it's impossible to reach.
			current = i->second;
		}
		else {
			return std::vector<sf::Vector2i>{};
		}
		path.push_back(current);
	}

	// Path needs to be reversed so that it goes start -> end.
	std::reverse(path.begin(), path.end());
	//path.erase(path.begin()); 	// Also removes starting position from path.

	return path;
}

std::vector<sf::Vector2i> DungeonScene::CorridorPathfind(sf::Vector2i start, sf::Vector2i end) {
	std::vector<sf::Vector2i> path{};

	std::vector<std::pair<sf::Vector2i, int64_t>> frontier{};
	std::unordered_map<sf::Vector2i, sf::Vector2i> cameFrom{};
	std::unordered_map<sf::Vector2i, int64_t> costSoFar{};

	frontier.push_back({ start, 0 });
	cameFrom[start] = start;
	costSoFar[start] = 0;

	while (!frontier.empty()) {
		// Sort frontier by cost for optimization.
		std::sort(frontier.begin(), frontier.end(), [](const std::pair<sf::Vector2i, int64_t>& left, const std::pair<sf::Vector2i, int64_t>& right) {return left.second < right.second; });

		sf::Vector2i current = frontier.front().first;
		if (current == end) {
			break;
		}

		// For corridors, don't path at the edge of an existing room.
		std::vector<sf::Vector2i> neighboors = TileMath::EdgeNeighbors(current);
		std::vector<sf::Vector2i> walkableNeighboors{};
		for (auto& t : neighboors) {
			if (doesAllowCorridor(t)) {
				walkableNeighboors.push_back(t);
			}
		}

		// The algorithm.
		for (auto& next : walkableNeighboors) {
			int64_t cost = costSoFar[current] + std::min(GetRawLineCost(current, next, false), GetRawLineCost(current, next, true));
			if (costSoFar.find(next) == costSoFar.end() || cost < costSoFar[next]) {
				costSoFar[next] = cost;
				int64_t priority = cost + std::min(GetRawLineCost(next, end, false), GetRawLineCost(next, end, true));
				cameFrom[next] = current;
				frontier.push_back({ next, priority });
			}
		}

		frontier.erase(frontier.begin());
	}

	// Create the vector and return the path.
	sf::Vector2i current = end;
	path.push_back(current);

	while (current != start) {
		auto i = cameFrom.find(current);
		if (i != cameFrom.end()) { // Prevents the algorithm from adding the end to the path if it's impossible to reach.
			current = i->second;
		}
		else {
			return std::vector<sf::Vector2i>{};
		}
		path.push_back(current);
	}

	// Path needs to be reversed so that it goes start -> end.
	std::reverse(path.begin(), path.end());

	return path;
}

int64_t DungeonScene::GetPathCost(std::vector<sf::Vector2i>& path) {
	int64_t result = 0;

	for (size_t i = 1; i < path.size(); i++) {
		if (path[i].x != path[i - 1].x && path[i].y != path[i - 1].y) {
			result += GetTileData(path[i])->MoveMod * 14142 / 10000;
		}
		else {
			result += GetTileData(path[i])->MoveMod;
		}
	}

	return result;
}

int64_t DungeonScene::GetRawPathCost(std::vector<sf::Vector2i>& path) {
	int64_t result = 0;

	for (size_t i = 1; i < path.size(); i++) {
		if (path[i].x != path[i - 1].x && path[i].y != path[i - 1].y) {
			result += 14142l;
		}
		else {
			result += 10000l;
		}
	}

	return result;
}

int64_t DungeonScene::GetLineCost(sf::Vector2i start, sf::Vector2i end, bool negativeOffset) {
	int64_t result = 0;
	std::vector<sf::Vector2i> line = TileMath::Line(start, end, negativeOffset);

	for (size_t i = 1; i < line.size(); i++) {
		if (line[i].x != line[i - 1].x && line[i].y != line[i - 1].y) {
			result += GetTileData(line[i])->MoveMod * 14142 / 10000;
		}
		else {
			result += GetTileData(line[i])->MoveMod;
		}
	}

	return result;
}

int64_t DungeonScene::GetRawLineCost(sf::Vector2i start, sf::Vector2i end, bool negativeOffset) {
	int64_t result = 0;

	result = std::abs(start.x - end.x) + std::abs(start.y - end.y);
	result *= 10000;

	return result;
}

bool DungeonScene::InLineOfSight(sf::Vector2i start, sf::Vector2i end) {
	bool result = true;

	// Try negative line first.
	std::vector<sf::Vector2i> line = TileMath::Line(start, end, false);

	for (auto& t : line) {
		TileData* data = GetTileData(t);
		if (data == nullptr || data->IsOpeque) {
			result = false;
			break;
		}
	}

	// Try positive line.
	if (result == false) {
		result = true;
		line = TileMath::Line(start, end, true);

		for (auto& t : line) {
			TileData* data = GetTileData(t);
			if (data == nullptr || data->IsOpeque) {
				return false;
				break;
			}
		}
	}

	return result;
}

TileData* DungeonScene::GetTileData(sf::Vector2i tile) {
	size_t id = 0;
	if (tile.x > 0 && tile.y > 0) {
		if (tile.x < floor.size() && tile.y < floor[0].size()) {
			id = floor[tile.x][tile.y];
		}
	}
	return gameData.GetTile(id);
}

bool DungeonScene::IsPassible(sf::Vector2i tile) {
	TileData* data = GetTileData(tile);
	if (data != nullptr) {
		return data->IsPassible;
	}
	return false;
}

bool DungeonScene::IsTileOccupiedByActor(sf::Vector2i tile) {
	for (auto& a : actors) {
		if (a->GetTileLocation() == tile && a->IsAlive()) {
			return true;
		}
	}
	return false;
}

Actor* DungeonScene::GetActorAtTile(sf::Vector2i tile) {
	for (auto& a : actors) {
		if (a->GetTileLocation() == tile && a->IsAlive()) {
			return a.get();
		}
	}

	return nullptr;
}

std::vector<Actor*> DungeonScene::GetActorsInArea(std::vector<sf::Vector2i> area) {
	std::vector<Actor*> result{};

	for (auto& a : actors) {
		if (std::find(area.begin(), area.end(), a->GetTileLocation()) != area.end()) {
			result.push_back(a.get());
		}
	}

	return result;
}

Actor* DungeonScene::GetPlayer() {
	if (!actors.empty()) {
		return actors[0].get();
	}
	return nullptr;
}

void DungeonScene::updateWorld(float secondsPerUpdate) {
	for (auto& a : actors) {
		a->Update(secondsPerUpdate);
	}

	// Find the next actor ready to take their turn.
	while (activeActor == nullptr) {
		for (auto& a : actors) {
			if (a->IsReadyToAct() && a->IsAlive()) {
				activeActor = a.get();
				break;
			}
		}

		// No actors ready, so decrement exhaustion.
		if (activeActor == nullptr) {
			for (auto& a : actors) {
				if (a->IsAlive()) {
					a->DecrementExhaustion();
				}
			}
		}
		// An actor is ready.
		else {
			if (!activeActor->IsPlayer()) {
				command = activeActor->CalcAI(this);
			}
		}
	}

	// Actor is ready and command is chosen.
	if (activeActor != nullptr && command != nullptr) {
		command->Execute(activeActor);

		command = nullptr;
		activeActor = nullptr;
	}
}

GameState DungeonScene::updateUI(float secondsPerUpdate) {
	GameState gs = GameState::Dungeon;

	messageLog.Update(secondsPerUpdate);

	return gs;
}

void DungeonScene::buildFloorVertexArray() {
	floorVertexArray.setPrimitiveType(sf::Quads);
	floorVertexArray.resize(floor.size() * floor[0].size() * 4);

	const float tileSize = 32.f;

	size_t i = 0;

	for (size_t x = 0; x < floor.size(); x++) {
		for (size_t y = 0; y < floor[x].size(); y++) {
			sf::Vertex* quad = &floorVertexArray[i * 4];
			uint8_t spriteNumber = gameData.GetTile(floor[x][y])->SpriteNumber;

			float texX = spriteNumber % 4 * tileSize;
			float texY = spriteNumber / 4 * tileSize;
			float posX = x * tileSize;
			float posY = y * tileSize;

			// quad cords
			// 0  1
			// 3  2
			quad[0].position = sf::Vector2f(posX, posY);
			quad[1].position = sf::Vector2f(posX + tileSize, posY);
			quad[2].position = sf::Vector2f(posX + tileSize, posY + tileSize);
			quad[3].position = sf::Vector2f(posX, posY + tileSize);

			// quad texture cords
			// 0  1
			// 3  2
			quad[0].texCoords = sf::Vector2f(texX, texY);
			quad[1].texCoords = sf::Vector2f(texX + tileSize, texY);
			quad[2].texCoords = sf::Vector2f(texX + tileSize, texY + tileSize);
			quad[3].texCoords = sf::Vector2f(texX, texY + tileSize);

			i++;
		}
	}
}