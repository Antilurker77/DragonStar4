// ================================================================
//
// tileData.hpp
// 
// Struct that holds data for a tile.
//
// ================================================================

#pragma once

#include <string>

struct TileData {
	size_t ID = 0;
	std::string Name = "Unknown Tile";
	uint8_t SpriteNumber = 0;
	int64_t MoveMod = 10000;
	bool IsPassible = false;
	bool IsOpeque = false;
};