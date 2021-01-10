#pragma once
#include <list>
#include "Vector3D.h"

//	�e�̃N���X
class PlayerBullet
{
public:
	//	�R���X�g���N�^
	PlayerBullet(Vector3D pos);

	PlayerBullet(Vector3D pos, float angle);

	//	�f�X�g���N�^
	~PlayerBullet();

	//	�J�n
	void Start();

	//	�`��
	void Render();

	//	�X�V
	void Update();

	//	���W�̎擾
	Vector3D Position() {
		return _position;
	}

	//	�T�C�Y�̎擾
	Vector3D Size() {
		return _size;
	}

private:
	const int AnimationSpeed = 4;	//	�A�j���[�V�����̑��x
	const int HorizonSheet = 5;		//	���摜�̖���
	const int VerticalSheet = 4;	//	�c�摜�̖���
	Vector3D _position;				//	���W
	Vector3D _size;					//	�摜�T�C�Y
	Vector3D _offset;				//	�I�t�Z�b�g���W
	Vector3D _velocity;				//	���x
	int grp;						//	�摜ID
	int _animFrameCount;					//	�A�j���[�V�����̃t���[���J�E���g
};