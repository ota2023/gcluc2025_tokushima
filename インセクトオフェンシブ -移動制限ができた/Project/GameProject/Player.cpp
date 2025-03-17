#include "Player.h"

#define CHIP_WIDTH 640		// 1コマのサイズ
#define CHIP_HEIGHT 360 
#define CENTER_POS CVector2D(CHIP_WIDTH/2, CHIP_HEIGHT/2)	// 中心座標
#define MOVE_SPEED_X 5.0f	// 横方向の移動速度
#define MOVE_SPEED_Z 3.0f	// 奥方向の移動速度
#define JUMP_SPEED 15.0f	// ジャンプ速度
#define GRAVITY -1.0f		// 重力
#define LEFT_UPPER_LIMIT 0.0f	//左移動制限(割合)
#define RIGHT_UPPER_LIMIT 3.0f	//右移動制限(割合)
#define UPPER_LIMIT 5.0f	//上移動制限(割合)
#define LOWER_LIMIT 3.0f	//下移動制限(割合)


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
};


// コンストラクタ
Player::Player(const CVector3D& pos)
	: CharaBase(pos)
	, m_state(EState::Idle)
	, m_stateStep(0)
	, mp_image(nullptr)
{
	m_hp = 100;

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
	// 左キーを押している間
	if (HOLD(CInput::eLeft))
	{
		// 左方向へ移動
		m_pos.x = max(m_pos.x - MOVE_SPEED_X, LEFT_UPPER_LIMIT);
		mp_image->SetFlipH(true);	//画像の向き
		isMove = true;
	}
	// 右キーを押している間
	else if (HOLD(CInput::eRight))
	{
		// 右方向へ移動
		m_pos.x = min(m_pos.x + MOVE_SPEED_X, SCREEN_WIDTH - CHIP_WIDTH / RIGHT_UPPER_LIMIT);
		mp_image->SetFlipH(false);	//画像の向き
		isMove = true;
	}
	// 上キーを押している間
	if (HOLD(CInput::eUp))
	{
		// 奥方向へ移動
		m_pos.z = max(m_pos.z - MOVE_SPEED_Z, -SCREEN_HEIGHT / UPPER_LIMIT);
		isMove = true;
	}
	// 下キーを押している間
	else if (HOLD(CInput::eDown))
	{
		// 手前方向へ移動
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
	// [X]キーで攻撃状態へ移行
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
			m_isGrounded = false;
			m_stateStep++;
			break;
		// ステップ1：ジャンプ終了
		case 1:
			// 接地したら、待機状態へ移行
			if (m_isGrounded)
			{
				ChangeState(EState::Idle);
			}
			break;
	}

	// 移動処理
	bool isMove = UpdateMove();
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

// 攻撃中の更新処理
void Player::StateAttack()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：攻撃アニメーションに切り替え
		case 0:
			mp_image->ChangeAnimation((int)EAnimType::Attack, false);
			m_stateStep++;
			break;
		// ステップ1：アニメーション終了待ち
		case 1:
			// 攻撃アニメーションが終了したら、待機状態へ移行
			if (mp_image->CheckAnimationEnd())
			{
				ChangeState(EState::Idle);
			}
			break;
	}
}

// 死亡時の更新処理
void Player::StateDeath()
{
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

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// 描画処理
void Player::Render()
{
	mp_image->Draw();
	RenderShadow();

}
