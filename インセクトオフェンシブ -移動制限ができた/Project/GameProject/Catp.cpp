#include "Catp.h"

#define CHIP_SIZE 479		// 1コマのサイズ
#define CENTER_POS CVector2D(239.5f, 380.0f)	// 中心座標
#define MOVE_SPEED_X 1.5f //　横方向の移動速度

// スライムのアニメーションデータ
/*TexAnimData Slime::ANIM_DATA[(int)EAnimType::Num] =
{
	{
		new TexAnim[4]
		{
			{ 0, 6}, { 1, 6},
			{ 2, 6}, { 3, 6},
		},
		4
	},
	{
		new TexAnim[5]
		{
			{5, 6}, {6, 6}, {7, 6},
			{8, 6}, {9, 6},
		},
		5
	},
};*/

// コンストラクタ
Catp::Catp(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;

	// 毛虫の画像を読み込み
	std::string imagePath = "毛虫＿本番データ.png";
	//if (m_type == 0) imagePath = "slime_a.png";
	//else if (m_type == 1) imagePath = "slime_b.png";
	//else if (m_type == 2) imagePath = "slime_c.png";
	mp_image = CImage::CreateImage
	(
		imagePath.c_str(),	// 画像ファイルのパス
		nullptr,			// アニメーションのデータ
		CHIP_SIZE, CHIP_SIZE		// 1コマの幅と高さ
	);
	mp_image->ChangeAnimation(0);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetFlipH(false);
}

// デストラクタ
Catp::~Catp()
{
	// 画像データを削除
	delete mp_image;
}

// 死亡処理
void Catp::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::Death);
}

// 現在の状態を切り替え
void Catp::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// 移動処理の更新
bool Catp::UpdateMove()
{

	m_pos.x -= MOVE_SPEED_X;
	mp_image->SetFlipH(false);

	bool isMove = true;

	return isMove;
}

// 待機時の更新処理
void Catp::StateIdle()
{

	// 待機アニメーションを再生
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

// 死亡時の更新処理
void Catp::StateDeath()
{

	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：死亡アニメーションを再生
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Death, false);
		m_stateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// アニメーションが終了したら、削除
		if (mp_image->CheckAnimationEnd())
		{
			// 削除フラグを立てる
			Kill();
		}
		break;
	}
}

// 更新処理
void Catp::Update()

{
	/*if (PUSH(CInput::eButton5))
	{
		ChangeState(EState::Death);
	}*/

	// 状態に合わせて、更新処理を切り替える
	switch (m_state)
	{
	case EState::Idle:	StateIdle();	break;
	case EState::Death:	StateDeath();	break;
	}

	if (UpdateMove())
	{
	}

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// 描画処理
void Catp::Render()
{
	mp_image->Draw();
}