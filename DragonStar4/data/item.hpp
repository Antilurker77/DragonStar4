// ================================================================
//
// item.hpp
// 
// Class for an individual instance of an item.
//
// ================================================================

#pragma once

enum class EquipType;
enum class ItemRarity;
enum class ItemType;
struct ItemData;

#include <string>
#include <vector>
#include "statMod.hpp"

class Item {
public:
	Item();
	Item(size_t id);

	// Initializes the item based on the given item ID.
	void Initialize(size_t id);

	// Returns true if the item has no item data.
	bool IsNull();

	// Returns the name of the item.
	std::string GetName();

	// Returns the damage dealt by the weapon. Non-weapons
	// return 0.
	int64_t GetWeaponDamage();

	// Returns the attack power multiplier of the weapon.
	// Non-weapons return 0.
	int64_t GetWeaponMultiplier();

	// Returns the attack speed of the weapon. Non-weapons
	// return 0.
	int64_t GetWeaponSpeed();

	// Returns the base hit chance of the weapon. Non-weapons
	// return 0.
	int64_t GetWeaponHitChance();

	// Returns the armor value of the armor. Non-armor returns 0.
	int64_t GetArmor();

	// Returns the spell defense value of the armor. Non-armor returns 0.
	int64_t GetSpellDefense();

private:

	// ================================
	ItemData* itemData = nullptr;

	std::string currentName = "Unknown Item";
	ItemRarity currentRarity{};
	std::vector<StatMod> statMods{};
};
