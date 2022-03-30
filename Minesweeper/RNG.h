#pragma once
#include <algorithm>
#include <iterator>
#include <random>
#include <vector>

class RNG
{
private:
	std::random_device m_rd{};
	std::mt19937 m_mt{};

public:
	RNG() : m_rd{}, m_mt { m_rd() } {}

	template <class INT_TYPE>
	INT_TYPE GetInt(INT_TYPE min, INT_TYPE max) { return std::uniform_int_distribution<INT_TYPE>{ min, max }(m_mt); }

	template <class T>
	std::vector<T> SampleVector(std::vector<T>& vp, std::size_t numElements)
	{
		std::vector<T> sample{};
		sample.reserve(numElements);
		std::sample(vp.begin(), vp.end(), std::back_inserter(sample), numElements, m_mt);

		return sample;
	}
};