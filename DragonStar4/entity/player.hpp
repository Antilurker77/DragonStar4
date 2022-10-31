// ================================================================
//
// player.hpp
//
// Class for the player characters.
//
// ================================================================

#pragma once

struct RaceData;

#include <array>
#include <string>
#include "actor.hpp"
#include "../data/item.hpp"

class Player : public Actor {
public:
	Player(std::string n, size_t raceID);

	void MoveToTile(sf::Vector2i tile, float unitsPerSecond = 0.f);
	bool IsPlayer();
	std::string GetName();
	int64_t GetWeaponDamage();
private:
	int64_t getBaseStat(StatModType statModType);

	// ================================
	std::string name = "Unknown";
	RaceData* race = nullptr;

	int64_t level = 1;
	int64_t currentEXP = 0;
	int64_t gold = 0;
	int64_t dust = 0;

	std::array<Item, 18> equipment;
};
