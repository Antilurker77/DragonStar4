// ================================================================
//
// player.cpp
//
// ================================================================

#include "player.hpp"

#include "../core/settings.hpp"
#include "../core/tileMath.hpp"
#include "../data/gameData.hpp"
#include "../data/id/statModType.hpp"

Player::Player(std::string n, size_t raceID) {
	name = n;

	// todo: races
	race = gameData.GetRace(1);

	if (race != nullptr) {
		currentHP = race->BaseHP;
	}

	abilities.push_back(Ability(1)); // Player always has basic attack.

	// Graphics
	sprites.resize(1);
	SetTexture("player/human_f/human_f_test.png", 0);

	// debug test
	equipment[0] = Item(2); // Test Sword
}

void Player::MoveToTile(sf::Vector2i tile, float unitsPerSecond) {
	tileLocation = tile;
	sf::Vector2f goal = TileMath::TileToPixel(tile);
	goal.y -= 4.f;
	Move(goal, unitsPerSecond);
}

bool Player::IsPlayer() {
	return true;
}

std::string Player::GetName() {
	return name;
}

int64_t Player::GetWeaponDamage() {
	if (!equipment[0].IsNull()) {
		return equipment[0].GetWeaponDamage();
	}
	return 0;
}

int64_t Player::getBaseStat(StatModType statModType) {
	int64_t result = 0;

	if (race != nullptr) {
		switch (statModType) {
		case StatModType::HP:
			result = race->BaseHP;
			result = static_cast<int64_t>(std::round(result + (level - 1.0) * (result / 10.0) + std::pow(level - 1.0, 2.0) * (result / 100.0)));
			break;
		case StatModType::MP:
			result = race->BaseMP;
			result = static_cast<int64_t>(std::round(result + (level - 1.0) * (result / 10.0)));
			break;
		case StatModType::SP:
			result = race->BaseSP;
			break;
		case StatModType::AttackPower:
			result = 0;
			break;
		case StatModType::SpellPower:
			result = 0;
			break;
		case StatModType::HealingPower:
			result = 0;
			break;
		case StatModType::Accuracy:
			result = 1;
			break;
		case StatModType::Evasion:
			result = 0;
			break;
		default:
			break;
		}
	}
	return result;
}