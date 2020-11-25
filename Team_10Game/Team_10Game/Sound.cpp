#include "Sound.h"

#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")



Sound::Sound()
{
	Initialize();
}

Sound::~Sound()
{
	//XAudio2の解放
	audio->Release();
	masterVoice->DestroyVoice();
}

void Sound::Initialize()
{
	HRESULT result;

	//エンジンインスタンス
	result = XAudio2Create(&audio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) assert(0 &&"XAudio2が作れませんでした。");

	//マスターボイス
	result = audio->CreateMasteringVoice(&masterVoice);
	if FAILED(result) assert(0 && "MasteringVoiceがつくれませんでした");
}

void Sound::Play(const char * filename)
{
	HRESULT result;

	//ファイル入力ストリームのインスタンス
	std::ifstream file;

	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0 && "ファイルオープンに失敗");
	}

	//RIFFヘッダーの読み込み
	RIFFHeader riffHeader;
	file.read((char*)&riffHeader, sizeof(riffHeader));
	// ファイルがRIFFかチェック
	if (strncmp(riffHeader.chunk.ID, "RIFF", 4) != 0)
	{
		assert(0 && "ファイルがRIFFではない");
	}

	//Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));

	//Dataチャンクの読み込み
	Chunk data;
	file.read((char*)&data, sizeof(data));


	//Dataチャンクの波形データ読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();



	WAVEFORMATEX waveFormatex{};

	//波形フォーマットの設定
	memcpy(&waveFormatex, &format.format, sizeof(format.format));
	waveFormatex.wBitsPerSample = format.format.nBlockAlign * 8 / format.format.nChannels;

	//波形フォーマットを元に SourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = audio->CreateSourceVoice(
		&pSourceVoice,
		&waveFormatex,
		0,
		2.0f,
		&voiceCallback
	);

	//再生する波形データを設定
	XAUDIO2_BUFFER audioBuff{};

	audioBuff.pAudioData = (BYTE*)pBuffer;
	//pContextにバッファのアドレスを渡す。
	audioBuff.pContext = pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = data.size;
	//audioBuff.LoopCount = XAUDIO2_LOOP_INFINITE;

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&audioBuff);
	result = pSourceVoice->Start();


}
