#pragma once

namespace gn {
	void SeedRandom(unsigned int seed);

	float Random(); // 0 - 1
	float RandomRange(int min, int max); // min - max

	int RandomInt();
	int RandomRangeInt(int min, int max);

}
