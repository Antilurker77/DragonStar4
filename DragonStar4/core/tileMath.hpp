// ================================================================
//
// tileMath.hpp
// 
// Functions for calculating various tile algorithms.
//
// ================================================================

#pragma once

#include <vector>
#include <SFML/System.hpp>

// Hash function for Vector2i.
namespace std {
	template<>
	struct hash<sf::Vector2i> {
		std::size_t operator()(const sf::Vector2i& v) const {
			return std::hash<int>()(v.x) << 32 | std::hash<int>()(v.y);
		}
	};
}

namespace TileMath {
	// Returns the distance between two tiles.
	int64_t Distance(sf::Vector2i start, sf::Vector2i end);
	
	// Returns all tiles in a line between two points.
	std::vector<sf::Vector2i> Line(sf::Vector2i start, sf::Vector2i end, bool negativeOffset = false);
	
	// Returns all tiles in a circular area.
	std::vector<sf::Vector2i> CircleArea(sf::Vector2i center, int64_t radius);

	// Returns tiles adjacent to the given tile, including diagonals.
	std::vector<sf::Vector2i> Neighbors(sf::Vector2i tile);

	// Returns tiles adjacent to the given tile excluding diagonals.
	std::vector<sf::Vector2i> EdgeNeighbors(sf::Vector2i tile);
	
	// Returns the top left pixel cordinates of a tile.
	sf::Vector2f TileToPixel(sf::Vector2i tile);
}
