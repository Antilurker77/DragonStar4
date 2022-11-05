// ================================================================
//
// statMod.hpp
// 
// Struct that modifies a stat.
//
// ================================================================

#pragma once

#include <cstdint>
#include <vector>

enum class Category : uint32_t;
enum class Element : uint8_t;
enum class StatModType;

struct StatMod {
	StatModType StatModType{};
	int64_t Value = 0;
	Category Categories{};
	Element Elements{};
	size_t AbilityID = 0;
	size_t AuraID = 0;
};
