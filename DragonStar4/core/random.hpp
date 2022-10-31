// ================================================================
//
// random.hpp
// 
// Functions used to generate random values.
//
// ================================================================

#pragma once

#include <random>
#include "../pcg/pcg_random.hpp"

namespace Random {
	// Returns a random seed.
	uint64_t RandomSeed();

	// Returns a random seed.
	uint64_t RandomSeed(pcg64& rng);

	// Returns a random 64-bit int. Arguments are inclusive.
	int64_t RandomInt64(int64_t min, int64_t max);

	// Returns a random 64-bit int. Arguments are inclusive.
	int64_t RandomInt64(int64_t min, int64_t max, pcg64& rng);

	// Returns a random 64-bit int. Arguments are inclusive.
	size_t RandomSizeT(size_t min, size_t max);

	// Returns a random 64-bit int. Arguments are inclusive.
	size_t RandomSizeT(size_t min, size_t max, pcg64& rng);

	// Returns a random float value. Min is inclusive, max is exlusive.
	float RandomFloat(float min, float max);

	// Returns a random float value. Min is inclusive, max is exlusive.
	float RandomFloat(float min, float max, pcg64& rng);
}