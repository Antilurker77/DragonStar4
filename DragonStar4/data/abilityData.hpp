// ================================================================
//
// abilityData.hpp
// 
// Struct that holds data for an ability.
//
// ================================================================

#pragma once

enum class Category;
enum class Element;
enum class EquipType;

#include <string>
#include <vector>
#include "abilityEffect.hpp"
#include "statMod.hpp"

enum class AreaType {
	Undefined = 0,
	
	// No Arguments
	Self,

	// 0: Range, 1: Radius
	Circle,
	
	// 0: Max Length
	Line,

	// 0: Cone Width, 1: Cone Length
	Cone,

	// 0: Range, Following: List of Tiles [1, 2][3, 4] ECT
	Arbitrary
};

struct AbilityData {
	size_t ID = 0;
	std::string Name = "Unknown Ability";
	std::string Icon = "placeholder";
	std::string Description = "Error.";
	bool IsPassive = false;
	std::vector<Category> Categories{};
	std::vector<Element> Elements{};
	std::vector<EquipType> RequiredEquipTypes{};
	AreaType AreaType{};
	std::vector<int64_t> AreaArguments{};
	bool IsProjectile = false;
	int64_t UseTime = 100;
	int64_t Cooldown = 0;
	int64_t MaxCharges = 1;
	int64_t MPCost = 0;
	int64_t SPCost = 0;
	int64_t HitChance = 10000;
	bool CanDodge = false;
	bool CanBlock = false;
	bool CanCrit = false;
	bool CanDoubleStrike = false;
	std::vector<AbilityEffect> Effects{};
	std::vector<StatMod> StatMods{};
};
