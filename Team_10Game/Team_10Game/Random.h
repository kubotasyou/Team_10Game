#pragma once
#include <random>

//�����_���Ȓl��Ԃ��N���X
class Random
{
public:
	//�����_���Ȓl���쐬(�ŏ��l�E�ő�l)
	static float SetRandom(int min, int max);
	//�t���[�g�̃����_���Ȓl����
	static float SetRandom(float min, float max);
};

