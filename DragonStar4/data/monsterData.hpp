// ================================================================
//
// monsterData.hpp
// 
// Struct that holds data for a monster.
//
// ================================================================

#pragma once

#include <string>
#include <vector>

enum class AICondition;
enum class AITarget;
enum class EquipType;

struct MonsterTactic {
	int Rating = 0;
	int64_t Weight = 0;
	AITarget Target{};
	std::vector<int64_t> TargetArguments{};
	AICondition Condition{};
	std::vector<int64_t> ConditionArguments{};
	size_t AbilityID = 0;
};

struct MonsterData {
	size_t ID = 0;
	std::string Name = "Unknown Monster";
	std::string Title = "";
	std::string Graphic = "target_dummy.png";
	bool IsUnique = false;
	int64_t Level = 1;
	int64_t BaseHP = 10;
	int64_t BaseMP = 10;
	int64_t BaseSP = 100;
	int64_t BaseAttackPower = 0;
	int64_t BaseSpellPower = 0;
	int64_t BaseHealingPower = 0;
	int64_t BaseArmor = 0;
	int64_t BaseSpellDefense = 0;
	int64_t BaseAccuracy = 0;
	int64_t BaseEvasion = 0;
	int64_t HitChance = 8000;
	int64_t AttackSpeed = 200;
	int64_t WeaponDamage = 0;
	int64_t WeaponMultiplier = 10000;
	EquipType WeaponType{};
	int64_t EXP = 0;
	int64_t Gold = 0;
	int64_t LootPoints = 0;
	std::vector<size_t> Abilities{};
	std::vector<MonsterTactic> Tactics{};
};
