// ================================================================
//
// gameData.hpp
// 
// Global that holds all game data.
//
// ================================================================

#pragma once

#include <vector>
#include "abilityData.hpp"
#include "itemData.hpp"
#include "monsterData.hpp"
#include "raceData.hpp"
#include "roomData.hpp"
#include "tileData.hpp"
#include "../pcg/pcg_random.hpp"

class GameData {
public:
	GameData();

	// Loads data from the database into vectors of structs.
	void LoadData();

	// Returns a monster with the specified ID.
	AbilityData* GetAbility(size_t id);

	// Returns a monster with the specified ID.
	ItemData* GetItem(size_t id);

	// Returns a monster with the specified ID.
	MonsterData* GetMonster(size_t id);

	// Returns a race with the specified ID.
	RaceData* GetRace(size_t id);

	// Returns a random room based on the current floor.
	RoomData* GetRandomRoom(pcg64& pcg, size_t currentFloor);

	// Returns a tile with the specified ID.
	TileData* GetTile(size_t id);

private:
	// Resizes the ability vector to the proper size.
	static int resizeAbilities(void* notUsed, int argc, char** data, char** colname);
	
	// Loads the ability data from the database.
	static int loadAbilities(void* notUsed, int argc, char** data, char** colname);

	// Loads the ability effect data from the database.
	static int loadAbilityEffects(void* notUsed, int argc, char** data, char** colname);

	// Resizes the item vector to the proper size.
	static int resizeItems(void* notUsed, int argc, char** data, char** colname);

	// Loads the item data from the database.
	static int loadItems(void* notUsed, int argc, char** data, char** colname);

	// Resizes the monster vector to the proper size.
	static int resizeMonsters(void* notUsed, int argc, char** data, char** colname);

	// Loads the monster data from the database.
	static int loadMonsters(void* notUsed, int argc, char** data, char** colname);
	
	// Loads the monster ability data from the database.
	static int loadMonsterAbilities(void* notUsed, int argc, char** data, char** colname);

	// Loads the monster AI data from the database.
	static int loadMonsterAI(void* notUsed, int argc, char** data, char** colname);

	// Resizes the race vector to the proper size.
	static int resizeRaces(void* notUsed, int argc, char** data, char** colname);

	// Loads the race data from the database.
	static int loadRaces(void* notUsed, int argc, char** data, char** colname);
	
	// Resizes the room vector to the proper size.
	static int resizeRooms(void* notUsed, int argc, char** data, char** colname);

	// Loads the room data from the database.
	static int loadRooms(void* notUsed, int argc, char** data, char** colname);

	// Resizes the tile vector to the proper size.
	static int resizeTiles(void* notUsed, int argc, char** data, char** colname);

	// Loads the tile data from the database.
	static int loadTiles(void* notUsed, int argc, char** data, char** colname);

	// ================================
	static std::vector<AbilityData> abilities;
	static std::vector<ItemData> items;
	static std::vector<MonsterData> monsters;
	static std::vector<RaceData> races;
	static std::vector<RoomData> rooms;
	static std::vector<TileData> tiles;
};

extern GameData gameData;