#pragma once
#include "CharaBase.h"

class Player : public CharaBase
{
public:
	// コンストラクタ
	Player(const CVector3D& pos);
	// デストラクタ
	~Player();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	// 状態
	enum class EState
	{
		Idle,	// 待機状態
		Jump,	// ジャンプ中
		Attack,	// 攻撃中
		Death,	// 死亡
	};
	void ChangeState(EState state);

	// 移動処理の更新
	bool UpdateMove();

	// 待機時の更新処理
	void StateIdle();
	// ジャンプ中の更新処理
	void StateJump();
	// 攻撃中の更新処理
	void StateAttack();
	// 死亡時の更新処理
	void StateDeath();

	EState m_state;
	int m_stateStep;

	// アニメーションの種類
	enum class EAnimType
	{
		Idle,	// 待機
		Move,	// 移動
		Death,	// 死亡
		Attack,	// 攻撃

		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// プレイヤーの画像

	float m_moveSpeedY;	// Y軸（高さ）の移動速度
};
