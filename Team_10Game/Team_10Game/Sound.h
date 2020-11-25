#pragma once
#include <Windows.h>
#include <wrl.h>

//xaudioのインクルード
#include <xaudio2.h>

using namespace Microsoft::WRL;

class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	////ボイス処理パスの開始
	//void OnVoiceProcessingPassStart(UINT32 byteRequired) {};
	////ボイス処理パスの終了時
	//void OnVoiceProcessingPassEnd() {};
	////バッファストリームの再生が終了したとき
	//void OnStreamEnd() {};
	////バッファの使用開始時
	//void OnBufferStart(void* pBufferContext) {};
	////バッファの末尾に到達したとき
	//void OnBufferEnd(void* pBufferContext)
	//{
	//	//バッファを解放する
	//	delete[] pBufferContext;
	//}
	////再生がループ位置に達したとき
	//void OnLoopEnd(void* pBufferContext) {};
	////ボイスrの実行エラー時
	//void OnvoiceError(void* pBufferContext, HRESULT error) {};

	//ボイス処理パスの開始
	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
	//ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};

	//バッファストリームの再生が終了したとき
	STDMETHOD_(void, OnStreamEnd)(THIS) {};
	//バッファの使用開始時
	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
	//バッファの末尾に到達したとき
	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext)
	{
		//バッファを解放する
		delete[] pBufferContext;
	}

	//再生がループ位置に達したとき
	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContext) {};

	//ボイスrの実行エラー時
	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContext, HRESULT error) {};
};

class Sound
{
public:
	//各チャンクIDを読み取るための構造体

	//チャンクヘッダ
	struct Chunk
	{
		char ID[4];  //各チャンクのID
		int size;//チャンクサイズ
	};

	//RIFFチャンク
	struct RIFFHeader
	{
		Chunk chunk;//RIFF
		char type[4];//Wave
	};

	//FMTチャンク
	struct FormatChunk
	{
		Chunk chunk;      //fmt
		WAVEFORMAT format;//波形フォーマット
	};

public:
	Sound();
	~Sound();

	void Play(const char* filename);

private:
	void Initialize();

private:
	IXAudio2 *audio;
	IXAudio2MasteringVoice* masterVoice;
	//インスタンス
	XAudio2VoiceCallback voiceCallback;
};

