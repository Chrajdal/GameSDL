#pragma once

#include <cstdint>
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>

class IRandom
{
public:
	virtual ~IRandom() = default;

	virtual std::int32_t Next() = 0;

	virtual std::int32_t Next(std::int32_t maxValue) = 0;

	virtual std::int32_t Next(std::int32_t minValue, std::int32_t maxValue) = 0;

	virtual std::double_t NextDouble() = 0;

	virtual std::double_t NextDouble(std::double_t minValue, std::double_t maxValue) = 0;

	virtual std::float_t NextFloat() = 0;

	virtual std::float_t NextFloat(std::float_t minValue, std::float_t maxValue) = 0;

	virtual void NextBytes(std::vector<std::uint8_t>& buffer) = 0;
};

class Random : public IRandom
{
	std::mt19937 randomNumberGenerator;
	std::uniform_int_distribution<std::int32_t> byteDistribution;
public:
	Random(std::uint_least32_t seed);
	Random() : Random(std::_Random_device()) {};
	std::int32_t Next() override;
	std::int32_t Next(std::int32_t maxValue) override;
	std::int32_t Next(std::int32_t minValue, std::int32_t maxValue) override;
	std::double_t NextDouble() override;
	std::double_t NextDouble(std::double_t minValue, std::double_t maxValue) override;
	std::float_t NextFloat() override;
	std::float_t NextFloat(std::float_t minValue, std::float_t maxValue) override;
	void NextBytes(std::vector<std::uint8_t>& buffer) override;
};