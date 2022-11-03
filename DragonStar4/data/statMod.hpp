// ================================================================
//
// statMod.hpp
// 
// Struct that modifies a stat.
//
// ================================================================

#pragma once

#include <vector>

enum class Category;
enum class Element : uint8_t;
enum class StatModType;

struct StatMod {
	StatModType StatModType{};
	int64_t Value = 0;
	std::vector<Category> Categories{};
	Element Elements{};
	size_t AbilityID = 0;
	size_t AuraID = 0;
};
