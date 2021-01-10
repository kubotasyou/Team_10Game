#include "Random.h"

float Random::SetRandom(int min, int max)
{
	//初期シード
	std::mt19937 mt{ std::random_device{}() };
	//一様分布乱数
	std::uniform_int_distribution<int> rnd(min, max);
	//値を決定
	float result = static_cast<float>(rnd(mt));

	return result;
}
