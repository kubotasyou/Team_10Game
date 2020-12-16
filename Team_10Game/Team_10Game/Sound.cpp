#include "Sound.h"

#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

using namespace std;

Sound::Sound()
{
	Initialize();
}

Sound::~Sound()
{
	//マップ内のすべて(forとか)の音声データのバッファひとつづつをdeleteする
	delete soundList[0].pBuffer;


	//XAudio2の解放
	audio->Release();
	masterVoice->DestroyVoice();
}

void Sound::LoadSound(const std::string& filename)
{
	//ファイル入力ストリームのインスタンス
	std::ifstream file;

	const std::string fileN = filename;
	const std::string soundName = fileN + ".wav";
	const std::string directoryName = "Resources/Sound/";

	// .wavファイルをバイナリモードで開く
	file.open(directoryName + soundName, std::ios_base::binary);
	//file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	if (file.fail()) { assert(0 && "ファイルオープンに失敗"); }
	
	//データ格納用構造体
	WaveData wavData;

	//RIFFヘッダーの読み込み
	/*RIFFHeader riffHeader;*/
	file.read((char*)&wavData.riffHeader, sizeof(wavData.riffHeader));
	// ファイルがRIFFかチェック
	if (strncmp(wavData.riffHeader.chunk.ID, "RIFF", 4) != 0) { assert(0 && "ファイルがRIFFではない"); }

	//Formatチャンクの読み込み
	/*FormatChunk format;*/
	file.read((char*)&wavData.format, sizeof(wavData.format));

	//Dataチャンクの読み込み
	/*Chunk data;*/
	file.read((char*)&wavData.data, sizeof(wavData.data));


	//Dataチャンクの波形データ読み込み
	/*char* */wavData.pBuffer = new char[wavData.data.size];
	file.read(wavData.pBuffer, wavData.data.size);

	//リストに名前と、データを保存
	soundList.emplace(filename, wavData);

	//Waveファイルを閉じる
	file.close();
}

void Sound::Initialize()
{
	HRESULT result;

	//XAudio2初期化
	result = XAudio2Create(&audio);
	if FAILED(result) assert(0 && "XAudio2が作れませんでした。");

	//マスターボイス
	result = audio->CreateMasteringVoice(&masterVoice);
	if FAILED(result) assert(0 && "MasteringVoiceがつくれませんでした");
}

void Sound::Play(const std::string& filename, float volume)
{
	HRESULT result;

	//名前に対応したデータ構造体を持ってくる
	WaveData& test = soundList[filename];

	WAVEFORMATEX waveFormatex{};

	//波形フォーマットの設定
	memcpy(&waveFormatex, &test.format.format, sizeof(test.format.format));
	waveFormatex.wBitsPerSample = test.format.format.nBlockAlign * 8 / test.format.format.nChannels;

	IXAudio2SourceVoice* source;

	//波形フォーマットを元に SourceVoiceの生成
	result = audio->CreateSourceVoice(
		&source,
		&waveFormatex
	);

	//サウンドの設定構造体
	XAUDIO2_BUFFER audioBuff{};

	//再生する波形データを設定
	audioBuff.pAudioData = (BYTE*)test.pBuffer;
	//pContextにバッファのアドレスを渡す。
	audioBuff.pContext = test.pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = test.data.size;

	//始まる場所を指定できる。
	audioBuff.PlayBegin = 128 * 5;

	//ソースボイスにデータを送信
	result = source->SubmitSourceBuffer(&audioBuff);

	//音量調節(通常は1.0)
	result = source->SetVolume(volume);

	//XAUDIO2_VOICE_STATE state;
	//source->GetState(&state);

	////再生中なら1が入る。
	//UINT tester = state.BuffersQueued;

	////再生が終わったら1or0が返るみたいな
	//if (state.BuffersQueued <= 0)
	//{
	//	//再生が終わっている
	//	isPlay = true;
	//	UINT tester = state.BuffersQueued;

	//}
	//else
	//{
	//	//再生している
	//	isPlay = false;
	//}

	//波形データの再生
	result = source->Start();
	result = source->Stop();

	sourceData.emplace_back(source);
}

void Sound::PlayLoop(const std::string & filename, float volume)
{
	HRESULT result;

	//名前に対応したデータ構造体を持ってくる
	WaveData& test = soundList[filename];

	WAVEFORMATEX waveFormatex{};

	//波形フォーマットの設定
	memcpy(&waveFormatex, &test.format.format, sizeof(test.format.format));
	waveFormatex.wBitsPerSample = test.format.format.nBlockAlign * 8 / test.format.format.nChannels;

	IXAudio2SourceVoice* source;

	//波形フォーマットを元に SourceVoiceの生成
	result = audio->CreateSourceVoice(
		&source,
		&waveFormatex
	);

	//サウンドの設定構造体
	XAUDIO2_BUFFER audioBuff{};

	//再生する波形データを設定
	audioBuff.pAudioData = (BYTE*)test.pBuffer;
	//pContextにバッファのアドレスを渡す。
	audioBuff.pContext = test.pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = test.data.size;
	audioBuff.LoopCount = XAUDIO2_LOOP_INFINITE;

	//ソースボイスにデータを送信
	result = source->SubmitSourceBuffer(&audioBuff);

	//音量調節(通常は1.0)
	result = source->SetVolume(volume);

	//波形データの再生
	result = source->Start();

	sourceData.emplace_back(source);
}
