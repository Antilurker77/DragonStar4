// ================================================================
//
// monster.hpp
//
// Class for monster actors.
//
// ================================================================

#pragma once

#include <string>
#include <utility>
#include "actor.hpp"

struct MonsterData;

typedef std::pair<size_t, sf::Vector2i> AIResult;

enum class AIState {
	Undefined = 0,
	Sleeping,
	Idle,
	Chasing,
	Alert
};

class Monster : public Actor {
public:
	Monster(size_t monsterID, sf::Vector2i spawnTile);

	CommandPtr CalcAI(DungeonScene* dungeonScene);
	void MoveToTile(sf::Vector2i tile, float unitsPerSecond = 0.f);
	std::string GetName();
	int64_t GetWeaponDamage();
private:
	int64_t getBaseStat(StatModType statModType);

	AIResult getAIResult(DungeonScene* dungeonScene);
	// ================================
	MonsterData* monsterData = nullptr;

	AIState aiState{};
	int chaseTurnsRemaining = 0;
};