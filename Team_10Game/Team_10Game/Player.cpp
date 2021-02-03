#include "Player.h"
#include "SafeDelete.h"

#include <DirectXMath.h>

Player::Player(Input * input, Model* model)
{
	this->input = input;
	this->sphereModel = model;

	//プレイヤー生成
	player = GameObject::Create();
	player->SetModel(sphereModel);
	objtest = GameObject::Create();
	objtest->SetModel(sphereModel);
}

Player::~Player()
{
	//safedelete(input);
	//safedelete(player);
	//safedelete(sphereModel);
	//for (auto test : bulletList)
	//{
	//	safedelete(test);
	//}
	safedelete(objtest);
}

void Player::Initialize()
{
	//一応初期化
	velocity = { 0,0,0 };
	pointervelocity = { 0,0,0 };
	//位置初期化
	position = float3(0, 0, 0);
	rotation = float3(0, 0, 0);
	pointerPosition = float3(0, 0, 15);
	player->SetPosition(position);
	player->SetRotation(rotation);
	player->SetColor({ 1, 0, 0, 0.5f });//後で消す
	//自機：当たり判定初期化
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);//位置
	sphere.radius = 1.0f;//半径

	objtest->SetPosition(pointerPosition);

	//体力初期化
	hp = 5;

	//無敵時間の初期化
	downTimer = new CountDownTimer();
	downTimer->SetTime(noDamageTime);

	//カメラの位置初期化
	cameraPosition = float3(position.x, position.y, -5);
	camera->SetTarget(position);//ターゲットはプレイヤーの位置
	camera->SetEye(cameraPosition);//プレイヤーの真後ろから見てる感じ(zを0にすることはできない)
	bulletTimer = new CountDownTimer();
	bulletTimer->SetTime(bulletTime);
	rotSpeed = 0.4f;
}

void Player::Update()
{
	player->Update();
	objtest->Update();
	bulletTimer->Update();
	Move();
	//CameraMove();
	Blinking();

	//ボタンを押したら使用される
	if (input->GetKeyDown(KeyCode::SPACE))
	{
		Shot();
		//memo : ホントにメモ
		////リストの中を全検索して、フラグがfalseなのを探す。
		//for (auto test : bl)
		//{
		//	if (!test->GetUsedFlag())
		//	{
		//		//falseのフラグを1つ見つけたら、代入して処理を中断。
		//		break;
		//	}
		//	//falseのフラグを見つけたらtureに変える。
		//	test->ChangeUsed(true);
		//}

	}
	//リストの中を全検索して、フラグがtrueなのを探す。
	for (auto test : bulletList)
	{
		//tureのフラグを見つけたら更新する
		test->Update();
	}

	pointerPosition = objtest->GetPosition();
}
void Player::Shot()
{
	if (bulletTimer->IsTime())
	{
		bulletList.emplace_back(new Bullet(player->GetPosition(), sphereModel,pointerPosition));
		for (int i = 0; i < bulletList.size(); i++)
		{
			//リスト内のフラグがfalseのものを探す
			if (!bulletList[i]->GetUsedFlag())
			{
				//falseのフラグを見つけたら、trueに変える
				bulletList[i]->ChangeUseFlag(true);
				break;
			}
		}
		bulletTimer->SetTime(bulletTime);//弾時間初期化
	}
}

void Player::Blinking()
{
	//ダメージを受けていなかったら処理しない
	if (!damageFlag) return;

	//時間を進める
	downTimer->Update();

	//カウントを増やす
	count++;

	//カウントが10で割り切れるとき
	if (count % 10 == 0)
	{
		//色を変える
		player->SetColor({ 1, 0, 0, 1 });
	}
	else
	{
		//割り切れないときは通常色
		player->SetColor({ 1, 1, 1, 0.5f });
	}

	//時間になったら
	if (downTimer->IsTime())
	{
		player->SetColor({ 1, 1, 1, 0.5f });
		//ダメージを受けていない状態にする
		count = 0;
		damageFlag = false;
	}
}

void Player::Damage(int damage)
{
	if (!damageFlag)
	{
		hp -= damage;                    //ダメージ受ける
		downTimer->SetTime(noDamageTime);//無敵時間初期化
		damageFlag = true;               //無敵突入
	}
}

void Player::Draw()
{
	player->Draw();
	//リストの中を全検索して、フラグがtrueなのを探す。
	for (auto test : bulletList)
	{
		//trueのフラグを見つけたら更新する
		test->Draw();
	}
	//pointer->Draw();
	objtest->Draw();
}

void Player::Move()
{
	//これ必須
	velocity = { 0,0,0 };
	pointervelocity = { 0,0,0 };
	position = player->GetPosition();

	velocity.x = input->GetStick("Vertices") * speed;
	velocity.y = input->GetStick("Horizontal") * speed;
	pointervelocity.x = input->GetRightStick("Vertices") * speed;
	pointervelocity.y = input->GetRightStick("Horizontal") * speed;

	//float3に+=のoperatorがない。
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;
	pointerPosition.x += pointervelocity.x;
	pointerPosition.y += pointervelocity.y;
	pointerPosition.z += pointervelocity.z;

	//移動範囲の制限をかける
	position.x = Clamp(position.x, -5.0f, 5.0f);
	position.y = Clamp(position.y, -3.5f, 2.0f);
	position.z = Clamp(position.z, -0.1f, 0.1f);

	//回転制限をかける
	rotation.x = Clamp(rotation.x, -45.0f, 45.0f);
	rotation.y = Clamp(rotation.y, -45.0f, 45.0f);
	rotation.z = Clamp(rotation.z, -45.0f, 45.0f);
	//当たり判定も一緒に動かす
	sphere.center = XMVectorSet(position.x, position.y, position.z, 0);

	if (input->GetStick("Vertices")== 1)
	{
		rotation.z = rotation.z - rotSpeed;
	}
	if (input->GetStick("Vertices") == -1)
	{
		rotation.z = rotation.z + rotSpeed;
	}
	if (input->GetStick("Vertices") == 0)
	{
		if (rotation.z >= 0)
		{
			rotation.z =rotation.z - rotSpeed;
		}
		if (rotation.z <= 0)
		{
			rotation.z = rotation.z + rotSpeed;
		}
	}
	if (input->GetStick("Horizontal") == 1)
	{
		rotation.x = rotation.x + rotSpeed;
	}
	if (input->GetStick("Horizontal") == -1)
	{
		rotation.x = rotation.x - rotSpeed;
	}
	if (input->GetStick("Horizontal") == 0)
	{
		if (rotation.x >= 0)
		{
			rotation.x = rotation.x - rotSpeed;
		}
		if (rotation.x <= 0)
		{
			rotation.x = rotation.x + rotSpeed;
		}
	}

	if (velocity.y == 0)
	{

	}
	//位置更新
	player->SetPosition(position);
	player->SetRotation(rotation);
	objtest->SetPosition(pointerPosition);
}

void Player::CameraMove()
{
	cameraVelocity = { 0,0,0 };

	cameraVelocity.x = input->GetStick("Vertices") *   speed;
	cameraVelocity.y = input->GetStick("Horizontal") * speed;

	//float3に+=のoperatorがない。
	cameraPosition.x += cameraVelocity.x;
	cameraPosition.y += cameraVelocity.y;

	//移動範囲の制限をかける
	cameraPosition.x = Clamp(cameraPosition.x, -15.0f, 15.0f);
	cameraPosition.y = Clamp(cameraPosition.y, -15.0f, 15.0f);

	//カメラを動かす
	camera->SetTarget(position);
	camera->SetEye(cameraPosition);
}

float Player::Clamp(float value, float min, float max)
{
	if (value <= min)
	{
		value = min;
	}
	else if (value >= max)
	{
		value = max;
	}

	return value;

}