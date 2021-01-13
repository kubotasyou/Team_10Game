#pragma once
#include <random>

//ランダムな値を返すクラス
class Random
{
public:
	//ランダムな値を作成(最小値・最大値)
	static float SetRandom(int min, int max);
	//フロートのランダムな値生成
	static float SetRandom(float min, float max);
};

