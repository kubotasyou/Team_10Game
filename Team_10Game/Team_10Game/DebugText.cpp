#include "DebugText.h"

DebugText::DebugText()
{
}

DebugText::~DebugText()
{
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		delete spriteDatas[i];
	}
}

void DebugText::Initialize(UINT texNumber)
{
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		//スプライトを生成する
		spriteDatas[i] = Sprite::Create(texNumber, { 0,0 });
	}
}

void DebugText::Draw(const std::string & text, float x, float y, float size)
{
	for (int i = 0; i < text.size(); i++)
	{
		//最大文字数を超えないように
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		const unsigned char& character = text[i];

		int fontIndex = character - 32;

		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		//座標計算
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * size * i, y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetScale({ fontWidth * size, fontHeight * size });

		//文字を１つ進める
		spriteIndex++;
	}
}

void DebugText::DrawAll(ID3D12GraphicsCommandList * cmdList)
{
	//全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		//スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}
