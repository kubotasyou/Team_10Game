#define _USE_MATH_DEFINES
#include "PlayerBullet.h"
#include "SafeDelete.h"
#include <cmath>

//�R���X�g���N�^
PlayerBullet::PlayerBullet(Vector3D pos): _position(pos), _size(Vector3D(32, 32,32))
{
	_velocity = Vector3D(0, 0,1);
}

PlayerBullet::PlayerBullet(Vector3D pos, float angle) : _position(pos), _size(Vector3D(32, 32,32))
{
	float velocity = 4.0f;

	auto x = cos((M_PI / 180) * angle) * velocity;
	auto y = sin((M_PI / 180) * angle) * velocity;
	auto z = tan((M_PI / 180) * angle) * velocity;
	_velocity = Vector3D(x, y,z);
}

//	�f�X�g���N�^
PlayerBullet::~PlayerBullet()
{
	//	�摜���������
	//safe_delete(grp);
}

