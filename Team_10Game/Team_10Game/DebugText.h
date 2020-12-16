#pragma once

#include <Windows.h>
#include <string>

#include "Sprite.h"

//デバッグ確認用テキスト表示

class DebugText
{
public:
	
	static const int maxCharCount = 256;//最大文字数
	static const int fontWidth = 9;     //画像の１文字分の横幅
	static const int fontHeight = 18;   //画像の１文字分の縦幅
	static const int fontLineCount = 14;//画像の一行あたりの文字数

	DebugText();

	~DebugText();

	void Initialize(UINT texNumber);

	void Draw(const std::string& text, float x, float y, float size);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private:

	//スプライトのデータ配列
	Sprite* spriteDatas[maxCharCount] = {};

	//文字画像の番号
	int spriteIndex = 0;
};

