// ================================================================
//
// category.hpp
// 
// List of all categories for abilities and combat events.
//
// ================================================================

#pragma once

enum class Category : uint32_t {
	Undefined = 0,
	Damaging = 1,
	Healing = 1 << 1,
	SingleTarget = 1 << 2,
	AreaOfEffect = 1 << 3,
	Direct = 1 << 4,
	OverTime = 1 << 5,
	Attack = 1 << 6,
	Skill = 1 << 7,
	Tech = 1 << 8,
	Spell = 1 << 9,
	Unarmed = 1 << 10,
	Sword = 1 << 11,
	Axe = 1 << 12,
	Mace = 1 << 13,
	Dagger = 1 << 14,
	Spear = 1 << 15,
	Bow = 1 << 16,
	Wand = 1 << 17,
	Staff = 1 << 18,
	Passive = 1 << 19
};

inline Category operator & (Category lhs, Category rhs) {
	using T = std::underlying_type_t<Category>;
	return static_cast<Category>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

inline Category operator | (Category lhs, Category rhs) {
	using T = std::underlying_type_t<Category>;
	return static_cast<Category>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline Category operator |= (Category lhs, Category rhs) {
	return lhs | rhs;
}

