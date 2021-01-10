#include "Player.h"

#include "SafeDelete.h"

#include <DirectXMath.h>

Player::Player(DirectXManager * dxManager, Input * input)
{
	this->dxManager = dxManager;
	this->input = input;

	//プレイヤー生成
	sphereModel = new Model(this->dxManager->GetDevice());
<<<<<<< HEAD
	sphereModel->CreateModel("sphere2");
	player = GameObject::Create();
	player->SetModel(sphereModel);
	player->SetPosition(XMFLOAT3(0, 5, 0));

	//足元の球の生成
	collider = GameObject::Create();
	collider->SetModel(sphereModel);
	//足元となる位置に設置!
	collider->SetPosition(XMFLOAT3(player->GetPosition().x, player->GetPosition().y - player->GetRadius(), player->GetPosition().z));
	//サイズは小さめ
	collider->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	//色は赤色
	collider->SetColor({ 1,0,0,1 });
	//足元の球の半径を変更
	collider->SetRadius(colliderRadius);

	sound = new Sound();
	sound->LoadSE("Alarm01");

	timer = new CountDownTimer();
	timer->SetTime(1.5f);
=======
	sphereModel->CreateModel("sphere2");   //モデル生成
	player = GameObject::Create();         //オブジェクト生成
	player->SetModel(sphereModel);         //モデルセット
	player->SetPosition(XMFLOAT3(0, 5, 0));//位置初期化

>>>>>>> Masataka
}

Player::~Player()
{
	safe_delete(player);
	safe_delete(sphereModel);
	//std::vector<Bullet*>::iterator it = bl.begin();
	//for (; it != bl.end(); ++it) {
	//	delete *it;
	//}
}

void Player::Initialize()
{
	velocity = { 0,0,0 };

	//たくさん作っておく
	//for (int i = 0; i < 10; i++)
	//{
	//	bl.emplace_back(new Bullet(dxManager, player->GetPosition()));
	//}

}

void Player::Update()
{
	player->Update();

	Move();

	//ボタンを押したら使用される
	if (input->GetKeyTrigger(KeyCode::Z))
	{
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
		Shot();

	}
	//リストの中を全検索して、フラグがtrueなのを探す。
	for (auto test : bl)
	{
		//tureのフラグを見つけたら更新する
		test->Update();
	}

}
void Player::Shot()
{
	bl.emplace_back(new Bullet(player->GetPosition(), sphereModel));
	for (int i = 0; i < bl.size(); i++)
	{
		////リスト内のフラグがfalseのものを探す
		if (!bl[i]->GetUsedFlag())
		{
			//falseのフラグを見つけたら、trueに変える
			bl[i]->ChangeUsed(true);
			break;
		}
	}

}

void Player::BulletDel()
{
}


void Player::Draw()
{
	player->Draw();

	//リストの中を全検索して、フラグがtrueなのを探す。
	for (auto test : bl)
	{
		//trueのフラグを見つけたら更新する
		test->Draw();
	}
}

void Player::Move()
{
<<<<<<< HEAD
	if (input->GetJoyPadRelease(JoyPad::B))
	{
		sound->PlaySE("Alarm01", 0.2f);
	}
=======
	//これ必須
	velocity = { 0,0,0 };

	position = player->GetPosition();

	velocity.x += input->GetStick("Vertices") * speed;
	velocity.y += input->GetStick("Horizontal") * speed;

	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);

	player->SetPosition(position);
>>>>>>> Masataka
}
