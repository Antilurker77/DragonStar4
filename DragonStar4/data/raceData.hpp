// ================================================================
//
// raceData.hpp
// 
// Struct that holds data for a player race.
//
// ================================================================

#pragma once

#include <string>
#include <vector>
#include "statMod.hpp"

struct RaceData {
	size_t ID = 0;
	std::string Name = "Unknown Race";
	int64_t BaseHP = 100;
	int64_t BaseMP = 100;
	int64_t BaseSP = 100;
	int64_t BaseEXPPerLevel = 100;
	std::string FolderName = "human";
	std::vector<StatMod> StatMods{};
};