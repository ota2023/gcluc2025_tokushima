#include "BeeLow.h"



#define CHIP_WIDTH 479		// 1コマのサイズ
#define CHIP_HEIGHT 376
#define CENTER_POS CVector2D(CHIP_WIDTH/2, CHIP_HEIGHT/2)	// 中心座標
#define MOVE_SPEED_X 3.0f //　横方向の移動速度

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
BeeLow::BeeLow(int type, const CVector3D& pos)
	: EnemyBase(pos, CAST::BEE)
	, mp_image(nullptr)
	, m_type(type)
	//, m_id(CAST::ENEMY)
{
	m_hp = 200;
	m_rect.TopX = CHIP_WIDTH / 4; m_rect.TopY = CHIP_HEIGHT / 4;
	m_rect.TopZ = -10; m_rect.Width = CHIP_WIDTH / 2.5;
	m_rect.Height = CHIP_HEIGHT / 2; m_rect.Depth = 20;

	// 毛虫の画像を読み込み
	std::string imagePath = "ハチ＿02＿本番データ.png";
	//if (m_type == 0) imagePath = "slime_a.png";
	//else if (m_type == 1) imagePath = "slime_b.png";
	//else if (m_type == 2) imagePath = "slime_c.png";
	mp_image = CImage::CreateImage
	(
		imagePath.c_str(),	// 画像ファイルのパス
		nullptr,			// アニメーションのデータ
		CHIP_WIDTH, CHIP_HEIGHT		// 1コマの幅と高さ
	);
	mp_image->ChangeAnimation(0);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetSize(CHIP_WIDTH * 0.7, CHIP_HEIGHT * 0.7);
	mp_image->SetFlipH(false);
}

// デストラクタ
BeeLow::~BeeLow()
{
	// 画像データを削除
	delete mp_image;
}

// 死亡処理
void BeeLow::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::Death);
}

void BeeLow::Damage(int damage)
{
	m_hp = max(m_hp - damage, 0);

	if (m_hp <= 0) {
		//死亡処理
		Death();
	}
}

bool BeeLow::CheckCollisionEnemy(float _playerShotX, float _playerShotY, float _playerShotZ, SRect3D _playerShotRect)
{
	if (IsHitBox3D(_playerShotX, _playerShotY, _playerShotZ, m_pos.x, m_pos.y,
		m_pos.z, _playerShotRect, m_rect))
	{
		printf("ハチにダメージ！\n");
		//Damage(1);	//1は暫定値
		return true;
	}
	return false;
}

void BeeLow::HitCheck(void)
{
	Player* ptask = (Player*)TaskManager::
		Instance()->GetTask(CAST::PLAYER);
	if (!ptask)return;

	if (ptask->CheckCollisionPlayer(
		m_pos.x, m_pos.y, m_pos.z, m_rect));

}

// 現在の状態を切り替え
void BeeLow::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// 移動処理の更新
bool BeeLow::UpdateMove()
{
	m_pos.x -= MOVE_SPEED_X;

	Player* ptask = (Player*)TaskManager::
		Instance()->GetTask(CAST::PLAYER);
	m_pos.z = ptask->GetPlyPos().z;


	mp_image->SetFlipH(false);

	bool isMove = true;

	return isMove;
}

// 待機時の更新処理
void BeeLow::StateIdle()
{

	// 待機アニメーションを再生
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

// 死亡時の更新処理
void BeeLow::StateDeath()
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
void BeeLow::Update()

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

	HitCheck();

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// 描画処理
void BeeLow::Render()
{
	RenderShadow();

	mp_image->Draw();
	mp_shadowImg->SetSize(CHIP_WIDTH * 0.5, CHIP_HEIGHT * 0.5);
	mp_shadowImg->SetCenter(CHIP_WIDTH * 0.5, CHIP_HEIGHT * 0.3);

}