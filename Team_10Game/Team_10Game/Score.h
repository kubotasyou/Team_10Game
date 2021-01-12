#pragma once

#include "DirectXManager.h"
#include "DebugText.h"

//�X�R�A�\���N���X(2D�e�N�X�`��)
class Score
{
public:
	Score(DirectXManager * dxManager);
	~Score();

	void Initialize();

	void Update();

	void Draw(float x, float y, float scale, DirectX::XMFLOAT4 color = { 1,1,1,1 });

	//���݂̃X�R�A���擾(���ꂢ��Ȃ�����)
	int GetScore() { return currentScore; }
	//���݂̃X�R�A�𑝂₷�B
	void AddScore(int score);

private:
	DirectXManager * dxManager;
	DebugText scoreText;

	int currentScore;//���݂̃X�R�A
	int scoreValue;  //���Z�����X�R�A
};

