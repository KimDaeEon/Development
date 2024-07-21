#pragma once
#include <random>

class Utils
{
public:
	template<typename T>
	static T GetRandom(T min, T max)
	{
		// 시드값 생성을 위한 random_device 생성
		std::random_device randomDevice;

		std::mt19937 generator(randomDevice());

		if constexpr (std::is_integral_v<T>)
		{
			std::uniform_int_distribution<T> distribution(min, max);
			return distribution(generator);
		}
		else
		{
			std::uniform_real_distribution<T> distribution(min, max);
			return distribution(generator);
		}

	}
};

