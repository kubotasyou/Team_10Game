#pragma once

#include "DirectXManager.h"
#include "Input.h"
#include "GameObject.h"
#include "Model.h"
#include "CollisionPrimitive.h"
#include "Bullet.h"
#include "SceneManager.h"
#include "CountDownTimer.h"

class Player
{
private:
	// DirectX::���ȗ�
	using float3 = DirectX::XMFLOAT3;

public:
	Player(Input * input, Model* model);
	~Player();

	void Initialize();

	void Update();

	void Draw();

	//�_���[�W����
	void Damage(int damage);

	//���݈ʒu���擾
	float3 GetPosition() { return position; }
	//���݂̊p�x
	float3 GetRotation() { return rotation; }
	//���@�̓����蔻����擾
	Sphere GetSphere() { return sphere; }
	//���݂̗͎̑擾
	int GetHp() { return hp; }

	//�e�̃��X�g���擾������
	std::vector<Bullet*> GetBulletList() { return bulletList; }

private:

	//���@�̈ړ�
	void Move();

	//�J�����̈ړ�
	void CameraMove();

	//�e����
	void Shot();

	//�_���[�W�̓_��
	void Blinking();

	//�͈͂̐���(�N�����v�������l�E�ŏ��l�E�ő�l)
	float Clamp(float value, float min, float max);


private:
	Input* input = nullptr;
	GameObject* player = nullptr;
	GameObject* camera = nullptr;//�J��������p
	GameObject* objtest = nullptr;
	Model* sphereModel = nullptr;

	float3 position;//���@�̈ʒu
	float3 velocity;//���@�̈ړ���
	float3 pointervelocity;//�Ə��̈ړ���
	float3 rotation;//���@�̊p�x
	float3 cameraPosition;//�J�����̈ʒu
	float3 cameraVelocity;//�J�����̈ړ���
	float3 pointerPosition;//�Ə��̈ʒu
	float speed = 0.09f;//�ړ����x
	int hp = 3;        //�̗�

	//�e�̃��X�g
	std::vector<Bullet*> bulletList;

	//���@�̓����蔻��
	Sphere sphere;

	//�_���[�W�t���O
	bool damageFlag = false;

	//�_���[�W�p�^�C�}�[
	CountDownTimer* downTimer;

	//�e�̊Ԋu�p�^�C�}�[
	CountDownTimer* bulletTimer;

	//���G����
	float noDamageTime = 3.0f;

	//�_�ŗp�J�E���g
	int count = 0;

	//�e�̃C���^�[�o��
	float bulletTime = 1.5f;

	//���[�e�[�V�����̑���
	float rotSpeed = 0;
};

