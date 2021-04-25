#include "Random.h"


Random::Random(std::uint_least32_t seed)
	: randomNumberGenerator(seed),
	byteDistribution(0, 256)
{
}

std::int32_t Random::Next()
{
	return this->Next(0, std::numeric_limits<std::int32_t>::max());
}

std::int32_t Random::Next(std::int32_t maxValue)
{
	return this->Next(0, maxValue);
}

std::int32_t Random::Next(std::int32_t minValue, std::int32_t maxValue)
{
	if (minValue < 0 || maxValue < minValue)
	{
		throw
			std::invalid_argument("minValue and maxValue must be non-negative. maxValue must be greater than minvalue");
	}
	std::uniform_int_distribution<std::int32_t> distribution(minValue, maxValue);
	return distribution(this->randomNumberGenerator);
}

std::double_t Random::NextDouble()
{
	return this->NextDouble(0.0, 1.0);
}

std::double_t Random::NextDouble(std::double_t minValue, std::double_t maxValue)
{
	if(maxValue < minValue)
		std::swap(minValue, maxValue);
	//if (minValue < 0.0 || maxValue < minValue)
	//{
	//	throw
	//		std::invalid_argument("minValue and maxValue must be non-negative. maxValue must be greater than minvalue");
	//}
	std::uniform_real_distribution<std::double_t> distribution(minValue, maxValue);
	return distribution(this->randomNumberGenerator);
}

std::float_t Random::NextFloat()
{
	return this->NextFloat(0.0, 1.0);
}

std::float_t Random::NextFloat(std::float_t minValue, std::float_t maxValue)
{
	if (maxValue < minValue)
		std::swap(minValue, maxValue);
	//if (minValue < 0.0 || maxValue < minValue)
	//{
	//	throw
	//		std::invalid_argument("minValue and maxValue must be non-negative. maxValue must be greater than minvalue");
	//}
	std::uniform_real_distribution<std::float_t> distribution(minValue, maxValue);
	return distribution(this->randomNumberGenerator);
}

void Random::NextBytes(std::vector<std::uint8_t>& buffer)
{
	for (auto& i : buffer)
	{
		i = static_cast<std::uint8_t>(byteDistribution(this->randomNumberGenerator));
	}
}