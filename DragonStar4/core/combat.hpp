// ================================================================
//
// combat.hpp
// 
// Set of functions for handling combat actions and their results.
//
// ================================================================

#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Actor;
struct AbilityData;
enum class Attribute;
enum class Category : uint32_t;
enum class Element : uint8_t;

struct CombatOptions {
	std::string EventName = "Unknown Event";

	Category Categories{};
	Element Elements{};

	size_t AbilityID = 0;
	size_t AuraID = 0;

	int64_t BaseHitChance = 8000;

	bool CanDodge = false;
	bool CanBlock = false;
	bool CanCrit = false;
	bool CanDoubleStrike = false;
};

struct CombatResult {
	int64_t Amount = 0;

	bool DidHit = false;
	bool DidCrit = false;
	bool DidDoubleStrike = false;
	bool DidKill = false;
	bool DidBlock = false;
};

namespace Combat {
	// One actor deals damage to another.
	CombatResult Damage(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t base, Attribute attribute);
}