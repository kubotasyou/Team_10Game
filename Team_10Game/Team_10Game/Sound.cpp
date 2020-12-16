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
	//�}�b�v���̂��ׂ�(for�Ƃ�)�̉����f�[�^�̃o�b�t�@�ЂƂÂ�delete����
	delete soundList[0].pBuffer;


	//XAudio2�̉��
	audio->Release();
	masterVoice->DestroyVoice();
}

void Sound::LoadSound(const std::string& filename)
{
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	const std::string fileN = filename;
	const std::string soundName = fileN + ".wav";
	const std::string directoryName = "Resources/Sound/";

	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(directoryName + soundName, std::ios_base::binary);
	//file.open(filename, std::ios_base::binary);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) { assert(0 && "�t�@�C���I�[�v���Ɏ��s"); }
	
	//�f�[�^�i�[�p�\����
	WaveData wavData;

	//RIFF�w�b�_�[�̓ǂݍ���
	/*RIFFHeader riffHeader;*/
	file.read((char*)&wavData.riffHeader, sizeof(wavData.riffHeader));
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(wavData.riffHeader.chunk.ID, "RIFF", 4) != 0) { assert(0 && "�t�@�C����RIFF�ł͂Ȃ�"); }

	//Format�`�����N�̓ǂݍ���
	/*FormatChunk format;*/
	file.read((char*)&wavData.format, sizeof(wavData.format));

	//Data�`�����N�̓ǂݍ���
	/*Chunk data;*/
	file.read((char*)&wavData.data, sizeof(wavData.data));


	//Data�`�����N�̔g�`�f�[�^�ǂݍ���
	/*char* */wavData.pBuffer = new char[wavData.data.size];
	file.read(wavData.pBuffer, wavData.data.size);

	//���X�g�ɖ��O�ƁA�f�[�^��ۑ�
	soundList.emplace(filename, wavData);

	//Wave�t�@�C�������
	file.close();
}

void Sound::Initialize()
{
	HRESULT result;

	//XAudio2������
	result = XAudio2Create(&audio);
	if FAILED(result) assert(0 && "XAudio2�����܂���ł����B");

	//�}�X�^�[�{�C�X
	result = audio->CreateMasteringVoice(&masterVoice);
	if FAILED(result) assert(0 && "MasteringVoice������܂���ł���");
}

void Sound::Play(const std::string& filename, float volume)
{
	HRESULT result;

	//���O�ɑΉ������f�[�^�\���̂������Ă���
	WaveData& test = soundList[filename];

	WAVEFORMATEX waveFormatex{};

	//�g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&waveFormatex, &test.format.format, sizeof(test.format.format));
	waveFormatex.wBitsPerSample = test.format.format.nBlockAlign * 8 / test.format.format.nChannels;

	IXAudio2SourceVoice* source;

	//�g�`�t�H�[�}�b�g������ SourceVoice�̐���
	result = audio->CreateSourceVoice(
		&source,
		&waveFormatex
	);

	//�T�E���h�̐ݒ�\����
	XAUDIO2_BUFFER audioBuff{};

	//�Đ�����g�`�f�[�^��ݒ�
	audioBuff.pAudioData = (BYTE*)test.pBuffer;
	//pContext�Ƀo�b�t�@�̃A�h���X��n���B
	audioBuff.pContext = test.pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = test.data.size;

	//�n�܂�ꏊ���w��ł���B
	audioBuff.PlayBegin = 128 * 5;

	//�\�[�X�{�C�X�Ƀf�[�^�𑗐M
	result = source->SubmitSourceBuffer(&audioBuff);

	//���ʒ���(�ʏ��1.0)
	result = source->SetVolume(volume);

	//XAUDIO2_VOICE_STATE state;
	//source->GetState(&state);

	////�Đ����Ȃ�1������B
	//UINT tester = state.BuffersQueued;

	////�Đ����I�������1or0���Ԃ�݂�����
	//if (state.BuffersQueued <= 0)
	//{
	//	//�Đ����I����Ă���
	//	isPlay = true;
	//	UINT tester = state.BuffersQueued;

	//}
	//else
	//{
	//	//�Đ����Ă���
	//	isPlay = false;
	//}

	//�g�`�f�[�^�̍Đ�
	result = source->Start();
	result = source->Stop();

	sourceData.emplace_back(source);
}

void Sound::PlayLoop(const std::string & filename, float volume)
{
	HRESULT result;

	//���O�ɑΉ������f�[�^�\���̂������Ă���
	WaveData& test = soundList[filename];

	WAVEFORMATEX waveFormatex{};

	//�g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&waveFormatex, &test.format.format, sizeof(test.format.format));
	waveFormatex.wBitsPerSample = test.format.format.nBlockAlign * 8 / test.format.format.nChannels;

	IXAudio2SourceVoice* source;

	//�g�`�t�H�[�}�b�g������ SourceVoice�̐���
	result = audio->CreateSourceVoice(
		&source,
		&waveFormatex
	);

	//�T�E���h�̐ݒ�\����
	XAUDIO2_BUFFER audioBuff{};

	//�Đ�����g�`�f�[�^��ݒ�
	audioBuff.pAudioData = (BYTE*)test.pBuffer;
	//pContext�Ƀo�b�t�@�̃A�h���X��n���B
	audioBuff.pContext = test.pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = test.data.size;
	audioBuff.LoopCount = XAUDIO2_LOOP_INFINITE;

	//�\�[�X�{�C�X�Ƀf�[�^�𑗐M
	result = source->SubmitSourceBuffer(&audioBuff);

	//���ʒ���(�ʏ��1.0)
	result = source->SetVolume(volume);

	//�g�`�f�[�^�̍Đ�
	result = source->Start();

	sourceData.emplace_back(source);
}
