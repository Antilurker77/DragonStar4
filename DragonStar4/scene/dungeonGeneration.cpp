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
#include "../core/tileMath.hpp"
#include "../data/gameData.hpp"
#include "../data/roomData.hpp"
#include "../ui/messageLog.hpp"

typedef sf::Vector2<size_t> ConnectionPoint;

struct Room {
	RoomData* RoomData = nullptr;
	std::vector<ConnectionPoint> ConnectionPoints;
	size_t XPos = 0;
	size_t YPos = 0;
};

enum class Direction : size_t {
	Undefined = 0,
	North,
	South,
	East,
	West
};

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
	std::vector<Room> rooms;
	size_t xBound = 32;
	size_t yBound = 32;
	const size_t corridorIndex = 270;

	floor = std::vector<std::vector<size_t>>(xBound, std::vector<size_t>(yBound));
	std::vector<sf::Vector2<size_t>> corridors{};

	// Fill floor with stone walls.
	for (size_t x = 0; x < floor.size(); x++) {
		for (size_t y = 0; y < floor[x].size(); y++) {
			floor[x][y] = 3;
		}
	}

	// Add a random number of rooms.
	size_t numRooms = Random::RandomSizeT(6, 8, pcg);
	rooms.reserve(numRooms);

	for (size_t i = 0; i < numRooms; i++) {
		RoomData* choosenRoom = gameData.GetRandomRoom(pcg, currentFloor);
		size_t roomWidth = choosenRoom->Tiles.size();
		size_t roomHeight = choosenRoom->Tiles[0].size();

		bool searching = true;
		size_t attempts = 0;

		while (searching && attempts < 10000) {
			attempts++;
			size_t xPos = Random::RandomSizeT(2, xBound - roomWidth - 3, pcg);
			size_t yPos = Random::RandomSizeT(2, yBound - roomHeight - 3, pcg);

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
				std::vector<sf::Vector2<size_t>> connectionPoints;
				for (size_t x = 0; x < roomWidth; x++) {
					for (size_t y = 0; y < roomHeight; y++) {
						switch (choosenRoom->Tiles[x][y]) {
						case '.':
							floor[x + xPos][y + yPos] = 1;
							break;
						case '+':
							connectionPoints.push_back(ConnectionPoint(x + xPos, y + yPos));
							break;
						default:
							break;
						}
					}
				}
				searching = false;
				Room room{choosenRoom, connectionPoints, xPos, yPos};
				rooms.push_back(room);
			}
		}
	}

	// Connect the rooms.
	calcAllowCorridor();
	for (Room& r : rooms) {
		// Find the closest room.
		int64_t lowestDistance = std::numeric_limits<int64_t>::max();
		size_t closestIndex = 0;
		for (size_t i = 0; i < rooms.size(); i++) {
			Room& check = rooms[i];
			if (r.XPos != check.XPos || r.YPos != check.YPos) {
				int64_t checkDistance = TileMath::Distance(sf::Vector2i(r.XPos, r.YPos), sf::Vector2i(check.XPos, check.YPos));
				if (checkDistance < lowestDistance) {
					lowestDistance = checkDistance;
					closestIndex = i;
				}
			}
		}

		// Pick a starting point.
		size_t startIndex = 0;
		bool searchingForIndex = true;
		while (searchingForIndex) {
			startIndex = Random::RandomSizeT(0, r.ConnectionPoints.size() - 1, pcg);
			ConnectionPoint& test = r.ConnectionPoints[startIndex];
			if (test.x >= 2 && test.x <= xBound - 3 && test.y >= 2 && test.y <= yBound - 3) {
				searchingForIndex = false;
				std::cout << "Found Start: " << test.x << ", " << test.y << "\n";
				allowCorridor[test.x][test.y] = 'y';
				floor[test.x][test.y] = corridorIndex;
			}
		}

		// Pick an ending point.
		size_t endIndex = 0;
		searchingForIndex = true;
		while (searchingForIndex) {
			endIndex = Random::RandomSizeT(0, rooms[closestIndex].ConnectionPoints.size() - 1, pcg);
			ConnectionPoint& test = rooms[closestIndex].ConnectionPoints[endIndex];
			if (test.x >= 2 && test.x <= xBound - 3 && test.y >= 2 && test.y <= yBound - 3) {
				searchingForIndex = false;
				std::cout << "Found End: " << test.x << ", " << test.y << "\n";
				allowCorridor[test.x][test.y] = 'y';
				floor[test.x][test.y] = corridorIndex;
			}
		}

		// Create the corridor.
		bool connecting = true;
		ConnectionPoint currentPoint = r.ConnectionPoints[startIndex];
		ConnectionPoint& end = rooms[closestIndex].ConnectionPoints[endIndex];
		while (connecting) {
			std::cout << "Current Point: " << currentPoint.x << ", " << currentPoint.y << "\n";

			auto edgeNeighbors = TileMath::EdgeNeighbors({ static_cast<int>(currentPoint.x), static_cast<int>(currentPoint.y) });
			std::vector<sf::Vector2i> goodNeighbors{};
			for (auto& t : edgeNeighbors) {
				if (doesAllowCorridor(t)) {
					goodNeighbors.push_back(t);
					std::cout << "Good Neighbor Found: " << t.x << ", " << t.y << "\n";
				}
			}

			std::vector<int64_t> costs{};
			std::vector<int64_t> weights{};
			int64_t highestCost = 0;
			int64_t lowestCost = std::numeric_limits<int64_t>::max();
			int64_t sum = 0;
			ConnectionPoint chosenPoint;
			bool skipRoll = false;
			for (auto& t : goodNeighbors) {
				if (t == sf::Vector2i(static_cast<int>(end.x), static_cast<int>(end.y))) {
					chosenPoint = end;
					skipRoll = true;
				}
				auto path = CorridorPathfind(t, { static_cast<int>(end.x), static_cast<int>(end.y)});
				if (!path.empty()) {
					int64_t cost = GetRawPathCost(path);
					costs.push_back(cost);
					std::cout << "Path Cost: " << cost << "\n";
					if (cost > highestCost) {
						highestCost = cost;
					}
					if (cost < lowestCost) {
						lowestCost = cost;
					}
				}
				else {
					std::cout << "Empty path.\n";
					costs.push_back(0);
				}
			}
			for (auto c : costs) {
				if (c == lowestCost) {
					weights.push_back(c);
					sum += c;
				}
				else {
					weights.push_back(0);
				}
			}

			if (goodNeighbors.empty()) {
				std::cout << "Bug? No good neighbors.\n";
			}

			if (!skipRoll) {
				int64_t roll = Random::RandomInt64(1, sum, pcg);
				for (size_t i = 0; i < weights.size(); i++) {
					roll -= weights[i];
					if (roll <= 0) {
						chosenPoint = { static_cast<size_t>(goodNeighbors[i].x), static_cast<size_t>(goodNeighbors[i].y) };
						break;
					}
				}
			}

			if (chosenPoint.x == 0 && chosenPoint.y == 0) {
				std::cout << "Bug. Chosen Point is 0, 0.\n";
				connecting = false;
				break;
			}

			int64_t continueChance = 10000;
			bool proceeding = true;
			floor[currentPoint.x][currentPoint.y] = corridorIndex;
			// Going right.
			if (chosenPoint.x > currentPoint.x) {
				std::cout << "Going east.\n";
				while (proceeding) {
					int64_t continueRoll = Random::RandomInt64(1, 10000, pcg);
					if (continueRoll <= continueChance) {
						floor[chosenPoint.x][chosenPoint.y] = corridorIndex;
						std::cout << "Corridor at: " << chosenPoint.x << ", " << chosenPoint.y << "\n";
						if (doesAllowCorridor({ static_cast<int>(chosenPoint.x) + 1, static_cast<int>(chosenPoint.y) })) {
							continueChance = continueChance * 50 / 100;
							chosenPoint.x++;
						}
						else {
							proceeding = false;
						}
					}
					else {
						proceeding = false;
					}
				}
				currentPoint = chosenPoint;
			}
			// Going left.
			else if (chosenPoint.x < currentPoint.x) {
				std::cout << "Going west.\n";
				while (proceeding) {
					int64_t continueRoll = Random::RandomInt64(1, 10000, pcg);
					if (continueRoll <= continueChance) {
						floor[chosenPoint.x][chosenPoint.y] = corridorIndex;
						std::cout << "Corridor at: " << chosenPoint.x << ", " << chosenPoint.y << "\n";
						if (doesAllowCorridor({ static_cast<int>(chosenPoint.x) - 1, static_cast<int>(chosenPoint.y) })) {
							chosenPoint.x--;
							continueChance = continueChance * 50 / 100;
						}
						else {
							proceeding = false;
						}
					}
					else {
						proceeding = false;
					}
				}
				currentPoint = chosenPoint;
			}
			// Going down.
			else if (chosenPoint.y > currentPoint.y) {
				std::cout << "Going south.\n";
				while (proceeding) {
					int64_t continueRoll = Random::RandomInt64(1, 10000, pcg);
					if (continueRoll <= continueChance) {
						floor[chosenPoint.x][chosenPoint.y] = corridorIndex;
						std::cout << "Corridor at: " << chosenPoint.x << ", " << chosenPoint.y << "\n";
						if (doesAllowCorridor({ static_cast<int>(chosenPoint.x), static_cast<int>(chosenPoint.y) + 1 })) {
							chosenPoint.y++;
							continueChance = continueChance * 50 / 100;
						}
						else {
							proceeding = false;
						}
					}
					else {
						proceeding = false;
					}
				}
				currentPoint = chosenPoint;
			}
			// Going up.
			else if (chosenPoint.y < currentPoint.y) {
				std::cout << "Going north.\n";
				while (proceeding) {
					int64_t continueRoll = Random::RandomInt64(1, 10000, pcg);
					if (continueRoll <= continueChance) {
						floor[chosenPoint.x][chosenPoint.y] = corridorIndex;
						std::cout << "Corridor at: " << chosenPoint.x << ", " << chosenPoint.y << "\n";
						if (doesAllowCorridor({ static_cast<int>(chosenPoint.x), static_cast<int>(chosenPoint.y) - 1 })) {
							chosenPoint.y--;
							continueChance = continueChance * 50 / 100;
						}
						else {
							proceeding = false;
						}
					}
					else {
						proceeding = false;
					}
				}
				currentPoint = chosenPoint;
			}
			currentPoint = chosenPoint;

			if (currentPoint == end) {
				connecting = false;
				std::cout << "Corridor created.\n\n";
			}
		}
	}

	removeDeadEnds();
	for (size_t i = 0; i < floor.size(); i++) {
		for (size_t j = 0; j < floor[i].size(); j++) {
			if (floor[i][j] == corridorIndex) {
				floor[i][j] = 1;
			}
		}
	}
}

void DungeonScene::spawnMonsters() {
	actors.resize(1);

	actors.push_back(std::make_shared<Monster>(1, sf::Vector2i(4, 5)));
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

void DungeonScene::calcAllowCorridor() {
	size_t xBound = floor.size();
	size_t yBound = floor[0].size();
	allowCorridor = std::vector<std::vector<char>>(xBound, std::vector<char>(yBound));

	for (size_t x = 0; x < floor.size(); x++) {
		for (size_t y = 0; y < floor[x].size(); y++) {
			if (floor[x][y] != 3) {
				allowCorridor[x][y] = 'n';
			}
			else if (x == 0 || x == floor.size() - 1) {
				allowCorridor[x][y] = 'n';
			}
			else if (y == 0 || y == floor[x].size() - 1) {
				allowCorridor[x][y] = 'n';
			}
			else {
				allowCorridor[x][y] = 'y';
				auto neighbors = TileMath::Neighbors({ static_cast<int>(x), static_cast<int>(y) });
				for (auto& c : neighbors) {
					if (c.x >= 0 && c.y >= 0 && c.x < floor.size() && c.y < floor[x].size()) {
						if (floor[c.x][c.y] != 3) {
							allowCorridor[x][y] = 'n';
							break;
						}
					}
				}
			}
		}
	}
}

bool DungeonScene::doesAllowCorridor(sf::Vector2i tile) {
	if (tile.x < 0 || tile.y < 0 || tile.x > floor.size() || tile.y > floor[tile.x].size()) {
		return false;
	}
	return allowCorridor[tile.x][tile.y] == 'y';
}

void DungeonScene::removeDeadEnds() {
	bool removing = true;
	while (removing) {
		size_t removed = 0;
		for (size_t x = 0; x < floor.size(); x++) {
			for (size_t y = 0; y < floor[x].size(); y++) {
				if (floor[x][y] == 270) {
					size_t connectionCount = 0;
					auto neighbors = TileMath::EdgeNeighbors({ static_cast<int>(x), static_cast<int>(y) });
					for (auto& c : neighbors) {
						if (floor[c.x][c.y] == 270 || floor[c.x][c.y] == 1) {
							connectionCount++;
						}
					}
					if (connectionCount <= 1) {
						floor[x][y] = 3;
						removed++;
					}
				}
			}
		}
		if (removed == 0) {
			removing = false;
		}
	}
}