// ================================================================
//
// item.cpp
//
// ================================================================

#include "item.hpp"

#include "gameData.hpp"
#include "id/itemType.hpp"

Item::Item() {

}

Item::Item(size_t id) {
	Initialize(id);
}

void Item::Initialize(size_t id) {
	itemData = gameData.GetItem(id);

	if (itemData != nullptr) {
		currentName = itemData->Name;
		currentRarity = itemData->ItemRarity;
	}
}

bool Item::IsNull() {
	return itemData == nullptr;
}

std::string Item::GetName() {
	return currentName;
}

int64_t Item::GetWeaponDamage() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Weapon) {
		return itemData->WeaponDamage;
	}
	return 0;
}

int64_t Item::GetWeaponMultiplier() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Weapon) {
		return itemData->WeaponMultiplier;
	}
	return 0;
}

int64_t Item::GetWeaponSpeed() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Weapon) {
		return itemData->WeaponSpeed;
	}
	return 0;
}

int64_t Item::GetWeaponHitChance() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Weapon) {
		return itemData->WeaponHitChance;
	}
	return 0;
}

int64_t Item::GetArmor() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Armor) {
		return itemData->Armor;
	}
	return 0;
}

int64_t Item::GetSpellDefense() {
	if (itemData != nullptr && itemData->ItemType == ItemType::Armor) {
		return itemData->SpellDefense;
	}
	return 0;
}