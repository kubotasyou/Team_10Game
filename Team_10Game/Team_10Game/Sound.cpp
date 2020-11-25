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
	//XAudio2�̉��
	audio->Release();
	masterVoice->DestroyVoice();
}

void Sound::Initialize()
{
	HRESULT result;

	//�G���W���C���X�^���X
	result = XAudio2Create(&audio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) assert(0 &&"XAudio2�����܂���ł����B");

	//�}�X�^�[�{�C�X
	result = audio->CreateMasteringVoice(&masterVoice);
	if FAILED(result) assert(0 && "MasteringVoice������܂���ł���");
}

void Sound::Play(const char * filename)
{
	HRESULT result;

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0 && "�t�@�C���I�[�v���Ɏ��s");
	}

	//RIFF�w�b�_�[�̓ǂݍ���
	RIFFHeader riffHeader;
	file.read((char*)&riffHeader, sizeof(riffHeader));
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riffHeader.chunk.ID, "RIFF", 4) != 0)
	{
		assert(0 && "�t�@�C����RIFF�ł͂Ȃ�");
	}

	//Format�`�����N�̓ǂݍ���
	FormatChunk format;
	file.read((char*)&format, sizeof(format));

	//Data�`�����N�̓ǂݍ���
	Chunk data;
	file.read((char*)&data, sizeof(data));


	//Data�`�����N�̔g�`�f�[�^�ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Wave�t�@�C�������
	file.close();



	WAVEFORMATEX waveFormatex{};

	//�g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&waveFormatex, &format.format, sizeof(format.format));
	waveFormatex.wBitsPerSample = format.format.nBlockAlign * 8 / format.format.nChannels;

	//�g�`�t�H�[�}�b�g������ SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = audio->CreateSourceVoice(
		&pSourceVoice,
		&waveFormatex,
		0,
		2.0f,
		&voiceCallback
	);

	//�Đ�����g�`�f�[�^��ݒ�
	XAUDIO2_BUFFER audioBuff{};

	audioBuff.pAudioData = (BYTE*)pBuffer;
	//pContext�Ƀo�b�t�@�̃A�h���X��n���B
	audioBuff.pContext = pBuffer;
	audioBuff.Flags = XAUDIO2_END_OF_STREAM;
	audioBuff.AudioBytes = data.size;
	//audioBuff.LoopCount = XAUDIO2_LOOP_INFINITE;

	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&audioBuff);
	result = pSourceVoice->Start();


}
