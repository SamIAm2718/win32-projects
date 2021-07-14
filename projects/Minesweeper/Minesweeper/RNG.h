#pragma once
#include <algorithm>
#include <ctime>
#include <iterator>
#include <random>
#include <vector>

class RNG
{
private:
	std::mt19937 m_mt{};

public:
	RNG() : m_mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) } {}

	template <class INT_TYPE>
	INT_TYPE GetInt(INT_TYPE min, INT_TYPE max) { return std::uniform_int<INT_TYPE>{ min, max }(m_mt); }

	template <class T>
	std::vector<T> SampleVector(std::vector<T>& vp, std::size_t numElements)
	{
		std::vector<T> sample{};
		sample.reserve(numElements);
		std::sample(vp.begin(), vp.end(), std::back_inserter(sample), numElements, m_mt);

		return sample;
	}
};