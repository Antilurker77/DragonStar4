// ================================================================
//
// ability.hpp
// 
// Class for an individual instance of an ability.
//
// ================================================================

#pragma once

#include <string>
#include <vector>
#include <SFML/System.hpp>
#include "../core/combat.hpp"

class Actor;
struct AbilityData;

class Ability {
public:
	Ability();
	Ability(size_t id);

	// Executes the ability.
	void Execute(Actor* user, std::vector<Actor*> targets);

	// Returns the ability's ID.
	size_t GetAbilityID();

	// Returns the maxinum range of the ability.
	int64_t GetMaxRange(Actor* user = nullptr);

	// Returns the tiles the ability will effect.
	std::vector<sf::Vector2i> GetAreaOfEffect(sf::Vector2i userTile, sf::Vector2i targetTile);

	// Returns true if the ability is usable.
	bool IsUsable(Actor* user);

	// Builds the CombatOptions struct based on ability data.
	CombatOptions GetCombatOptions();

private:

	// ================================
	AbilityData* abilityData = nullptr;

	int64_t currentCooldown = 0;
	int64_t currentCharges = 0;
};
