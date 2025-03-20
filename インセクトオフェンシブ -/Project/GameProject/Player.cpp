#include "Player.h"
#include "Bee.h"
#include "Catp.h"
#include "PlayerAttack.h"

#define CHIP_WIDTH 640		// 1コマのサイズ
#define CHIP_HEIGHT 360 
#define CENTER_POS CVector2D(CHIP_WIDTH/2, CHIP_HEIGHT/2)	// 中心座標
#define MOVE_SPEED_Z 4.0f	// 奥方向の移動速度
#define JUMP_SPEED 25.0f	// ジャンプ速度
#define GRAVITY -1.0f		// 重力
#define LEFT_UPPER_LIMIT 240.0f	//左移動制限(割合)
#define RIGHT_UPPER_LIMIT 3.0f	//右移動制限(割合)
#define UPPER_LIMIT 5.0f	//上移動制限(割合)
#define LOWER_LIMIT 1.0f	//下移動制限(割合)


// プレイヤーのアニメーションデータの前宣言
TexAnimData Player::ANIM_DATA[(int)EAnimType::Num] =
{
	// 待機アニメーション
	{
		new TexAnim[4]
		{
			{0, 10}, {1, 10}, {2, 10},
			{3, 10},
		},
		4
	},
	// 移動アニメーション
	{
		new TexAnim[3]
		{
			{12, 6}, {13, 6}, {14, 6},

		},
		3
	},
	// 死亡アニメーション
	{
		new TexAnim[3]
		{
			{12, 12},
			{13, 12},
			{14, 12},
		},
		3
	},
	// 攻撃アニメーション
	{
		new TexAnim[4]
		{
			{5, 6},
			{6, 6},
			{7, 6},
			{8, 6},
		},
		4
	},

	// ジャンプアニメーション
	{
		new TexAnim[3]
		{
			{8, 6},
			{9, 6},
			{10,6}
		},
		3
	},
};


// コンストラクタ
Player::Player(const CVector3D& pos)
	: CharaBase(pos,CAST::PLAYER)
	, m_state(EState::Idle)
	, m_stateStep(0)
	, mp_image(nullptr)
	, m_invincible(false)
	, m_invTimer(0)
	, m_atk(false)
{
	m_hp = 100;
	m_rect.TopX = CHIP_WIDTH / 4; m_rect.TopY = CHIP_HEIGHT / 4;
	m_rect.TopZ = -10; m_rect.Width = CHIP_WIDTH / 4;
	m_rect.Height = CHIP_HEIGHT / 3; m_rect.Depth = 20;

	// プレイヤーの画像を読み込み
	mp_image = CImage::CreateImage
	(
		"カマキリ＿アニメーション.png",	// 画像ファイルのパス
		ANIM_DATA,		// アニメーションのデータ
		CHIP_WIDTH, CHIP_HEIGHT	// 1コマの幅と高さ
	);
	mp_image->ChangeAnimation((int)EAnimType::Idle);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetSize(CHIP_WIDTH * 0.7, CHIP_HEIGHT * 0.7);

}

// デストラクタ
Player::~Player()
{
	// 画像データを削除
	delete mp_image;
}

CVector3D Player::GetPlyPos()
{
	return m_pos;
}

SRect3D Player::GetRect()
{
	return m_rect;
}

// 現在の状態を切り替え
void Player::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// 移動処理の更新
bool Player::UpdateMove()
{
	bool isMove = false;
	m_pos.x =max(m_pos.x-1.5f,LEFT_UPPER_LIMIT);

	// 左キーを押している間
	if (HOLD(CInput::eLeft))
	{
		// 左方向へ移動
		//m_pos.x -= move_Speed_X;
		 m_pos.x = max(m_pos.x - move_Speed_X, LEFT_UPPER_LIMIT);
		mp_image->SetFlipH(true);	//画像の向き
		isMove = true;
	}
	// 右キーを押している間
	else if (HOLD(CInput::eRight))
	{
		// 右方向へ移動
		//m_pos.x += move_Speed_X;
		 m_pos.x = min(m_pos.x + move_Speed_X, SCREEN_WIDTH - CHIP_WIDTH / RIGHT_UPPER_LIMIT);
		mp_image->SetFlipH(false);	//画像の向き
		isMove = true;
	}
	// 上キーを押している間
	if (HOLD(CInput::eUp))
	{
		// 奥方向へ移動
		//m_pos.z -= MOVE_SPEED_Z;
		m_pos.z = max(m_pos.z - MOVE_SPEED_Z, -SCREEN_HEIGHT / UPPER_LIMIT);
		isMove = true;
	}
	// 下キーを押している間
	else if (HOLD(CInput::eDown))
	{
		// 手前方向へ移動
		//m_pos.z += MOVE_SPEED_Z;
		m_pos.z = min(m_pos.z + MOVE_SPEED_Z, SCREEN_HEIGHT / 2 - CHIP_HEIGHT / LOWER_LIMIT);
		isMove = true;
	}

	return isMove;
}

// 待機時の更新処理
void Player::StateIdle()
{
	// 移動処理
	bool isMove = UpdateMove();

	// 移動状態に合わせて、アニメーションを切り替え
	EAnimType anim = isMove ? EAnimType::Move : EAnimType::Idle;
	mp_image->ChangeAnimation((int)anim);

	// [SPACE]キーでジャンプ状態へ移行
	if (PUSH(CInput::eButton5))
	{
		ChangeState(EState::Jump);
	}
	// [J]キーで攻撃状態へ移行
	else if (PUSH(CInput::eButton2))
	{
		ChangeState(EState::Attack);
	}
}

// ジャンプ中の更新処理
void Player::StateJump()
{
	// ステップごとに処理を切り替え
	switch(m_stateStep)
	{
		// ステップ0：ジャンプ開始
		case 0:
			// Y軸（高さ）の移動速度にジャンプを速度を設定し、
			// 接地状態を解除する
			m_moveSpeedY = JUMP_SPEED;
			move_Speed_X = move_Speed_X * 2;
			m_isGrounded = false;
			m_stateStep++;
			break;
		// ステップ1：ジャンプ終了
		case 1:
			// 接地したら、待機状態へ移行
			if (m_isGrounded)
			{
				move_Speed_X = move_Speed_X / 2;
				ChangeState(EState::Idle);
			}
			break;
	}

	// 移動処理
	bool isMove = UpdateMove();
	mp_image->ChangeAnimation((int)EAnimType::Jump);
}

// 攻撃中の更新処理
void Player::StateAttack()
{
	m_pos.x = max(m_pos.x - 1.5f, LEFT_UPPER_LIMIT);

	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：攻撃アニメーションに切り替え
		case 0:
			mp_image->ChangeAnimation((int)EAnimType::Attack, false);
			m_stateStep++;
			break;
		case 1:

			//　攻撃判定を生成
			TaskManager::Instance()->Add(new PlayerAttack(
				CVector3D(m_pos.x,
									m_pos.y, m_pos.z)));
			//printf("当たり判定生成 X:%f,Y:%f,X:%f\n",m_pos.x, m_pos.y, m_pos.z);
			SetAtk(true);
			m_stateStep++;
			break;
		case 2:
			// 攻撃アニメーションが終了したら、待機状態へ移行
			if (mp_image->CheckAnimationEnd())
			{
				PlayerAttack* plyatk = (PlayerAttack*)TaskManager::
					Instance()->GetTask(CAST::ATTACK);
				printf("当たり判定 X:%f,Y:%f,X:%f\n", plyatk->GetPos().x, GetPos().y, GetPos().z);

				SetAtk(false);
				ChangeState(EState::Idle);
			}
			break;
	}
}

// 死亡時の更新処理
void Player::StateDeath()
{
}



void Player::Damage(int damage)
{
	m_hp = max(m_hp - damage, 0);

	if (m_hp <= 0) {
		//死亡処理
	}
}

void Player::HitCheckBee(void)
{
	Bee* bee = (Bee*)TaskManager::
		Instance()->GetTask(CAST::BEE);
	bee->CheckCollisionEnemy(m_pos.x, m_pos.y, m_pos.z, m_rect);

}

void Player::SetInv(bool _inv)
{
	m_invincible = _inv;
}

bool Player::GetInv()
{
	return m_invincible;
}

void Player::InvTime()
{
	if (m_invincible) {
		if ((m_invTimer++ % 6) < 3) {
			SetShow(false);
		}
		else {
			SetShow(true);
		}
		if (++m_timer >= 45) {
			m_timer = 0;
			SetShow(true);
			SetInv(false);
		}
	}

}

void Player::SetAtk(bool _atk)
{
	m_atk = _atk;
}

bool Player::GetAtk()
{
	return	m_atk;
}

void Player::HitCheckCatp(void)
{
	Catp* catp = (Catp*)TaskManager::
		Instance()->GetTask(CAST::CATP);
	catp->CheckCollisionEnemy(m_pos.x, m_pos.y, m_pos.z, m_rect);

}

bool Player::CheckCollisionPlayer(float _enemyShotX, float _enemyShotY, 
					float _enemyShotZ, SRect3D _enemyRect)
{
	if (!m_invincible) {
		if (IsHitBox3D(_enemyShotX, _enemyShotY, _enemyShotZ, m_pos.x, m_pos.y,
			m_pos.z, _enemyRect, m_rect))
		{
			printf("プレイヤーにダメージ！\n");
			//Damage(1);	//1は暫定値
			SetInv(true);
			return true;
		}
	}
	return false;
}


// 更新処理
void Player::Update()
{
	// 現在の状態に合わせて、処理を切り替える
	switch (m_state)
	{
	case EState::Idle:		StateIdle();	break;
	case EState::Jump:		StateJump();	break;
	case EState::Attack:	StateAttack();	break;
	case EState::Death:		StateDeath();	break;
	}

	// Y軸（高さ）の移動を座標に反映
	m_pos.y += m_moveSpeedY;
	m_moveSpeedY += GRAVITY;	// Y軸の移動速度に重力を加算
	// 地面より下にいくと
	if (m_pos.y <= 0.0f)
	{
		// 地面の座標へ戻す
		m_pos.y = 0.0f;
		m_moveSpeedY = 0.0f;
		m_isGrounded = true;
	}
	//無敵時間
	InvTime();


	//HitCheckCatp();
	//HitCheckBee();

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();

	//printf("Plyer X:%f,Y:%f,Z:%f\n", m_pos.x, m_pos.y, m_pos.z);
}

// 描画処理
void Player::Render()
{
	RenderShadow();

	mp_image->Draw();

}
