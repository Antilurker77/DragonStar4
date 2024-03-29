// ================================================================
//
// dungeonScene.hpp
// 
// Scene object that handles most of the main gameplay.
//
// ================================================================

#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "../command/command.hpp"
#include "../entity/actor.hpp"
#include "../entity/monster.hpp"
#include "../pcg/pcg_random.hpp"

class Actor;
class Camera;
struct TileData;

enum class GameState;

class DungeonScene {
public:
	DungeonScene();

	// Reads input.
	void ReadInput(sf::RenderWindow& window);

	// Updates the scene and returns the game state.
	GameState Update(float secondsPerUpdate);

	// Draws the scene.
	void DrawWorld(sf::RenderTarget& window, float timeRatio);

	// Draws the UI for this scene.
	void DrawUI(sf::RenderTarget& window, float timeRatio);

	// Sets the camera reference.
	void SetCamera(Camera* c);

	// Sets the positions of the UI elements. Should be called whenever the resolution
	// is changed.
	void SetPositions();

	// Sets up the dungeon seeds and generates the current floor of the dungeon.
	void Generate(uint64_t seed);

	// Returns the most optimal path between two points. If the vector is empty, then
	// no possible path exists.
	std::vector<sf::Vector2i> Pathfind(sf::Vector2i start, sf::Vector2i end);

	// Returns the most optimal path between two points for drawing a corridor. Cannot
	// move diagonally. If the vector is empty, then no possible path exists.
	std::vector<sf::Vector2i> CorridorPathfind(sf::Vector2i start, sf::Vector2i end);

	// Returns the cost of a given path.
	int64_t GetPathCost(std::vector<sf::Vector2i>& path);

	// Returns the raw cost of a given path (ignores a tile's move mod).
	int64_t GetRawPathCost(std::vector<sf::Vector2i>& path);

	// Returns the cost of a straight line between two points.
	int64_t GetLineCost(sf::Vector2i start, sf::Vector2i end, bool negativeOffset);

	// Returns the raw cost of a straight line between two points (ignores a tile's move mod).
	int64_t GetRawLineCost(sf::Vector2i start, sf::Vector2i end, bool negativeOffset);

	// Returns true if two tiles are within line of sight of each other.
	bool InLineOfSight(sf::Vector2i start, sf::Vector2i end);

	// Gets the tile data of the specified tile.
	TileData* GetTileData(sf::Vector2i tile);

	// Returns true if the tile is passible.
	bool IsPassible(sf::Vector2i tile);

	// Returns true if the tile is occupied by an actor.
	bool IsTileOccupiedByActor(sf::Vector2i tile);

	// Returns the actor occupying the tile if there is one.
	Actor* GetActorAtTile(sf::Vector2i tile);

	// Returns all actors in a given area.
	std::vector<Actor*> GetActorsInArea(std::vector<sf::Vector2i> area);

	// Returns the player actor.
	Actor* GetPlayer();

private:
	// Updates the world elements.
	void updateWorld(float secondsPerUpdate);

	// Updates the UI.
	GameState updateUI(float secondsPerUpdate);
	
	// Generates the vertex array for the dungeon floor tiles.
	void buildFloorVertexArray();

	// ================================
	// dungeonGeneration.cpp
	
	// Generates the current floor.
	void generateFloor();
	
	// Erases all existing monsters and spawns monsters for the current floor.
	void spawnMonsters();

	// Generates a dungeon which consists of a small test room.
	void genTest();

	// Generates a dungeon by placing prefab rooms and linking them together.
	void genPrefab(pcg64& pcg);

	// Sets walls to the proper tile if they are above a floor.
	void formatWalls();

	// Calculates to see which tiles corridors are allowed to override.
	void calcAllowCorridor();

	// Returns true if a corridor can be placed at the given tile.
	bool doesAllowCorridor(sf::Vector2i tile);

	// Removes dead-ends in corridors.
	void removeDeadEnds();

	// Sets the spawn location for the player.
	void placePlayer(pcg64& pcg, size_t xBound, size_t yBound);

	// Sets the spawn location for stairs.
	void spawnStairs(pcg64& pcg, size_t xBound, size_t yBound);

	// ================================

	// Graphics
	sf::Texture* dungeonTileTexture = nullptr;
	sf::VertexArray floorVertexArray;
	
	// Input
	bool leftClick = false;
	bool rightClick = false;
	sf::Vector2i mousePos;

	// UI
	Camera* camera = nullptr;

	// Data
	uint64_t masterSeed = 0;

	size_t currentFloor = 1;
	std::vector<std::vector<size_t>> floor;
	std::vector<std::vector<char>> allowCorridor;
	std::vector<uint64_t> floorSeeds;
	sf::Vector2i playerSpawnPosition;

	std::vector<ActorPtr> actors;
	Actor* activeActor = nullptr;

	CommandPtr command = nullptr;
};
