#include "Slime.h"

#define CHIP_SIZE 256		// 1コマのサイズ
#define CENTER_POS CVector2D(128.0f, 184.0f)	// 中心座標

// スライムのアニメーションデータ
TexAnimData Slime::ANIM_DATA[(int)EAnimType::Num] =
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
};

// コンストラクタ
Slime::Slime(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;
	m_rect.TopX = CHIP_SIZE / 4; m_rect.TopY = CHIP_SIZE / 4;
	m_rect.TopZ = -10; m_rect.Width = CHIP_SIZE / 2;
	m_rect.Height = CHIP_SIZE / 2; m_rect.Depth = 20;

	// スライムの画像を読み込み
	std::string imagePath;
	if (m_type == 0) imagePath = "slime_a.png";
	else if (m_type == 1) imagePath = "slime_b.png";
	else if (m_type == 2) imagePath = "slime_c.png";
	mp_image = CImage::CreateImage
	(
		imagePath.c_str(),	// 画像ファイルのパス
		ANIM_DATA,			// アニメーションのデータ
		CHIP_SIZE, CHIP_SIZE		// 1コマの幅と高さ
	);
	mp_image->ChangeAnimation(0);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetFlipH(true);
}

// デストラクタ
Slime::~Slime()
{
	// 画像データを削除
	delete mp_image;
}

// 死亡処理
void Slime::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::Death);
}

// 現在の状態を切り替え
void Slime::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// 待機時の更新処理
void Slime::StateIdle()
{
	// 待機アニメーションを再生
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

// 死亡時の更新処理
void Slime::StateDeath()
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

void Slime::Damage(int damage)
{
	m_hp = max(m_hp - damage, 0);

	if (m_hp <= 0) {
		//死亡処理
		Death();
	}
}

bool Slime::CheckCollisionEnemy(float _playerShotX, float _playerShotY, float _playerShotZ, SRect3D _playerShotRect)
{
	if (IsHitBox3D(_playerShotX, _playerShotY, _playerShotZ, m_pos.x, m_pos.y,
		m_pos.z, _playerShotRect, m_rect));
	{
		printf("ダメージ！\n");
		Damage(1);	//1は暫定値
		return true;
	}
	return false;
}

void Slime::HitCheck(void)
{

}

// 更新処理
void Slime::Update()
{
	if (PUSH(CInput::eButton5))
	{
		ChangeState(EState::Death);
	}
	// 状態に合わせて、更新処理を切り替える
	switch (m_state)
	{
	case EState::Idle:	StateIdle();	break;
	case EState::Death:	StateDeath();	break;
	}

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// 描画処理
void Slime::Render()
{
	mp_image->Draw();
}
