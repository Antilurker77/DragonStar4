// ================================================================
//
// tileMath.cpp
//
// ================================================================

#include "tileMath.hpp"

int64_t TileMath::Distance(sf::Vector2i start, sf::Vector2i end) {
	int64_t result = 0;

	int64_t xdist = abs(start.x - end.x) * 100ll;
	int64_t ydist = abs(start.y - end.y) * 100ll;

	if (xdist == ydist) {
		result = xdist * 14142 / 10000;
	}
	else {
		result = abs(xdist - ydist) + (std::min(xdist, ydist) * 14142) / 10000;
	}

	return result;
}

std::vector<sf::Vector2i> TileMath::Line(sf::Vector2i start, sf::Vector2i end, bool negativeOffset) {
	std::vector<sf::Vector2i> result;

	// https://www.redblobgames.com/grids/line-drawing.html
	int diagonalDistance = std::max(abs(end.x - start.x), abs(end.y - start.y));
	result.reserve(static_cast<size_t>(diagonalDistance) + 1);
	for (int i = 0; i <= diagonalDistance; i++) {
		double t = 0.0;
		double offset = 0.00001;
		if (negativeOffset) {
			offset *= -1.0;
		}
		if (diagonalDistance != 0) {
			t = static_cast<double>(i) / static_cast<double>(diagonalDistance);
		}
		double startX = static_cast<double>(start.x);
		double startY = static_cast<double>(start.y);
		double endX = static_cast<double>(end.x);
		double endY = static_cast<double>(end.y);

		int pointX = static_cast<int>(std::round(startX + t * (endX - startX) + offset));
		int pointY = static_cast<int>(std::round(startY + t * (endY - startY) + offset));

		result.push_back(sf::Vector2i(pointX, pointY));
	}

	return result;
}

std::vector<sf::Vector2i> TileMath::CircleArea(sf::Vector2i center, int64_t radius) {
	std::vector<sf::Vector2i> result{};
	int tileDistance = static_cast<int>(radius) / 100;

	for (int x = center.x - tileDistance; x <= center.x + tileDistance; x++) {
		for (int y = center.y - tileDistance; y <= center.y + tileDistance; y++) {
			int64_t distanceCheck = TileMath::Distance(center, { x, y });
			if (distanceCheck <= radius) {
				result.push_back({ x, y });
			}
		}
	}

	return result;
}

std::vector<sf::Vector2i> TileMath::Neighbors(sf::Vector2i tile) {
	std::vector<sf::Vector2i> result{
		{ tile.x + 1, tile.y },
		{ tile.x + 1, tile.y - 1 },
		{ tile.x + 1, tile.y + 1 },
		{ tile.x - 1, tile.y },
		{ tile.x - 1, tile.y - 1 },
		{ tile.x - 1, tile.y + 1 },
		{ tile.x, tile.y - 1 },
		{ tile.x, tile.y + 1 }
	};
	return result;
}

std::vector<sf::Vector2i> TileMath::EdgeNeighbors(sf::Vector2i tile) {
	std::vector<sf::Vector2i> result{
		{ tile.x + 1, tile.y },
		{ tile.x - 1, tile.y },
		{ tile.x, tile.y - 1 },
		{ tile.x, tile.y + 1 }
	};
	return result;
}

sf::Vector2f TileMath::TileToPixel(sf::Vector2i tile) {
	return sf::Vector2f(tile.x * 32.f, tile.y * 32.f);
}