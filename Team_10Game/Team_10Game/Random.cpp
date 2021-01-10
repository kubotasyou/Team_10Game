#include "Random.h"

float Random::SetRandom(int min, int max)
{
	//�����V�[�h
	std::mt19937 mt{ std::random_device{}() };
	//��l���z����
	std::uniform_int_distribution<int> rnd(min, max);
	//�l������
	float result = static_cast<float>(rnd(mt));

	return result;
}
