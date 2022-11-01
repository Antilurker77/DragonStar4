// ================================================================
//
// element.hpp
// 
// List of all elements (damage types).
//
// ================================================================

#pragma once

/*
enum class Element {
	Undefined = 0,
	Physical,
	Arcane,
	Fire,
	Ice,
	Lightning,
	Poison,
	Light,
	Dark
};
*/

enum class Element : uint8_t {
	Undefined = 0,
	Physical = 1 << 0,
	Arcane = 1 << 1,
	Fire = 1 << 2,
	Ice = 1 << 3,
	Lightning = 1 << 4,
	Poison = 1 << 5,
	Light = 1 << 6,
	Dark = 1 << 7
};

inline Element operator & (Element lhs, Element rhs) {
	using T = std::underlying_type_t<Element>;
	return static_cast<Element>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

inline Element operator | (Element lhs, Element rhs) {
	using T = std::underlying_type_t<Element>;
	return static_cast<Element>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline Element operator |= (Element lhs, Element rhs) {
	return lhs | rhs;
}
