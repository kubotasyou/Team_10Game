#pragma once

#include "GameObject.h"
#include "Model.h"
#include "CountDownTimer.h"
#include "Sound.h"
#include "DirectXManager.h"

class PlayerTest
{
public:
	PlayerTest(const DirectXManager &dxManager);
	~PlayerTest();

	void Initialize();

	void Update();

	void Draw();
};

