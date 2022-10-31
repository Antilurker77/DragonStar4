// ================================================================
//
// roomData.hpp
// 
// Struct that holds data for a dungeon room.
//
// ================================================================

#pragma once

#include <string>
#include <vector>

struct RoomData {
	size_t ID = 0;
	std::string Name = "Unknown Room";
	size_t MinDepth = 0;
	size_t MaxDepth = 0;
	int64_t Weight = 0;
	std::vector<std::vector<char>> Tiles{};
};