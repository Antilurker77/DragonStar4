// ================================================================
//
// dungeonGeneration.cpp
//
// This is a seperate file to keep the size of dungeonScene more
// managable.
// 
// ================================================================

#include "dungeonScene.hpp"

#include <iomanip>
#include <sstream>
#include "../core/random.hpp"
#include "../data/gameData.hpp"
#include "../data/roomData.hpp"
#include "../ui/messageLog.hpp"

void DungeonScene::generateFloor() {
	floor.clear();
	pcg64 pcg(floorSeeds[currentFloor - 1]);

	std::stringstream ss;
	ss << "#808080 Floor Seed: " << std::setfill('0') << std::setw(16) << std::hex << floorSeeds[currentFloor - 1] << std::dec;
	messageLog.AddMessage(ss.str());

	// Select an dungeon generation type to use. More will be added
	// in the future.
	//genTest();
	genPrefab(pcg);

	// Spawns monsters.
	spawnMonsters();

	// Clean up walls to use the proper graphic.
	formatWalls();

	// Finally, build the vertex array for the floor.
	buildFloorVertexArray();
}

void DungeonScene::genTest() {
	size_t xBound = 8;
	size_t yBound = 8;

	floor = std::vector<std::vector<size_t>>(xBound, std::vector<size_t>(yBound));

	for (size_t i = 0; i < floor.size(); i++) {
		for (size_t j = 0; j < floor[i].size(); j++) {
			if (i == 0 || i == xBound - 1 || j == 0 || j == yBound - 1) {
				floor[i][j] = 3;
			}
			else {
				floor[i][j] = 1;
			}
		}
	}
}

void DungeonScene::genPrefab(pcg64& pcg) {
	size_t xBound = 32;
	size_t yBound = 32;

	floor = std::vector<std::vector<size_t>>(xBound, std::vector<size_t>(yBound));

	// Fill floor with stone walls.
	for (size_t x = 0; x < floor.size(); x++) {
		for (size_t y = 0; y < floor[x].size(); y++) {
			floor[x][y] = 3;
		}
	}

	// Add a random number of rooms.
	size_t numRooms = Random::RandomSizeT(4, 6, pcg);

	for (size_t i = 0; i < numRooms; i++) {
		RoomData* choosenRoom = gameData.GetRandomRoom(pcg, currentFloor);
		size_t roomWidth = choosenRoom->Tiles.size();
		size_t roomHeight = choosenRoom->Tiles[0].size();

		bool searching = true;
		size_t attempts = 0;

		while (searching && attempts < 10000) {
			attempts++;
			size_t xPos = Random::RandomSizeT(0, xBound - roomWidth - 1, pcg);
			size_t yPos = Random::RandomSizeT(0, yBound - roomHeight - 1, pcg);

			// Check to see if a room is already placed there.
			bool failed = false;
			for (size_t x = xPos; x < xPos + roomWidth; x++) {
				for (size_t y = yPos; y < yPos + roomHeight; y++) {
					if (floor[x][y] != 3) {
						failed = true;
						break;
					}
				}
			}

			if (failed) {
				continue;
			}
			// Area is empty, add room to the floor.
			else {
				for (size_t x = 0; x < roomWidth; x++) {
					for (size_t y = 0; y < roomHeight; y++) {
						switch (choosenRoom->Tiles[x][y]) {
						case '.':
							floor[x + xPos][y + yPos] = 1;
							break;
						default:
							break;
						}
					}
				}
				searching = false;
			}
		}
	}

	// Connect the rooms.
}

void DungeonScene::spawnMonsters() {
	actors.resize(1);

	actors.push_back(std::make_shared<Monster>(1, sf::Vector2i(4, 4)));
}

void DungeonScene::formatWalls() {
	for (size_t i = 0; i < floor.size(); i++) {
		for (size_t j = 0; j < floor[i].size() - 1; j++) {
			if (floor[i][j] == 3) {
				if (floor[i][j + 1] == 1) {
					floor[i][j] = 2;
				}
			}
		}
	}
}