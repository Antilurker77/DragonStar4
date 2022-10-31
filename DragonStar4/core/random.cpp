// ================================================================
//
// random.cpp
//
// ================================================================

#include "random.hpp"

uint64_t Random::RandomSeed() {
	pcg_extras::seed_seq_from<std::random_device> seed_source;
	pcg64 rng(seed_source);
	std::uniform_int_distribution<uint64_t> dist(0);
	return dist(rng);
}

uint64_t Random::RandomSeed(pcg64& rng) {
	std::uniform_int_distribution<uint64_t> dist(0);
	return dist(rng);
}

int64_t Random::RandomInt64(int64_t min, int64_t max) {
	pcg_extras::seed_seq_from<std::random_device> seed_source;
	pcg64 rng(seed_source);
	std::uniform_int_distribution<int64_t> dist(min, max);
	return dist(rng);
}

int64_t Random::RandomInt64(int64_t min, int64_t max, pcg64& rng) {
	std::uniform_int_distribution<int64_t> dist(min, max);
	return dist(rng);
}

size_t Random::RandomSizeT(size_t min, size_t max) {
	pcg_extras::seed_seq_from<std::random_device> seed_source;
	pcg64 rng(seed_source);
	std::uniform_int_distribution<size_t> dist(min, max);
	return dist(rng);
}

size_t Random::RandomSizeT(size_t min, size_t max, pcg64& rng) {
	std::uniform_int_distribution<size_t> dist(min, max);
	return dist(rng);
}

float Random::RandomFloat(float min, float max) {
	pcg_extras::seed_seq_from<std::random_device> seed_source;
	pcg64 rng(seed_source);
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}

float Random::RandomFloat(float min, float max, pcg64& rng) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}