#pragma once

#include <random>
#include <ctime>

class Random
{
	std::mt19937 rng;
public:
	Random(uint_least32_t seed = time(nullptr));

	int32_t next(int32_t minval = std::numeric_limits<int32_t>::min(), int32_t maxval = std::numeric_limits<int32_t>::max());
	float_t next_float(float_t minval = std::numeric_limits<float_t>::min(), float_t maxval = std::numeric_limits<float_t>::max());
	double_t next_double(double_t minval = std::numeric_limits<double_t>::min(), double_t maxval = std::numeric_limits<double_t>::max());
};