#pragma once

#include <Windows.h>
#include <string>

#include "Sprite.h"

//�f�o�b�O�m�F�p�e�L�X�g�\��

class DebugText
{
public:
	
	static const int maxCharCount = 256;//�ő啶����
	static const int fontWidth = 9;     //�摜�̂P�������̉���
	static const int fontHeight = 18;   //�摜�̂P�������̏c��
	static const int fontLineCount = 14;//�摜�̈�s������̕�����

	DebugText();

	~DebugText();

	void Initialize(UINT texNumber);

	void Draw(const std::string& text, float x, float y, float size);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private:

	//�X�v���C�g�̃f�[�^�z��
	Sprite* spriteDatas[maxCharCount] = {};

	//�����摜�̔ԍ�
	int spriteIndex = 0;
};

