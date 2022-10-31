// ================================================================
//
// itemData.hpp
// 
// Struct that holds data for an item.
//
// ================================================================

#pragma once

enum class EquipType;
enum class ItemRarity;
enum class ItemType;

#include <string>
#include <vector>
#include "statMod.hpp"

struct ItemData {
	size_t ID = 0;
	std::string Name = "Unknown Item";
	std::string EquipGraphic = "blank";
	ItemRarity ItemRarity{};
	int64_t BaseValue = 0;
	ItemType ItemType{};
	EquipType EquipType{};
	int64_t WeaponDamage = 0;
	int64_t WeaponMultiplier = 0;
	int64_t WeaponSpeed = 0;
	int64_t WeaponHitChance = 0;
	int64_t Armor = 0;
	int64_t SpellDefense = 0;
	size_t InvokedAbilityID = 0;
	std::vector<StatMod> StatMods{};
};
