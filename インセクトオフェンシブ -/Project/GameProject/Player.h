#pragma once
#include "CharaBase.h"
#include "TaskManager.h"

class Player : public CharaBase
{
public:
	// コンストラクタ
	Player(const CVector3D& pos);
	// デストラクタ
	~Player();


	//プレイヤーの位置を取得
	CVector3D GetPlyPos();
	//矩形を取得
	SRect3D GetRect();

	//無敵にする
	void SetInv(bool _inv);
	//無敵状態かを取得
	bool GetInv();

	//攻撃フラグをセット
	void SetAtk(bool _atk);
	//攻撃フラグ取得
	bool GetAtk();

	//プレイヤーの当たり判定
	bool CheckCollisionPlayer(float _enemyShotX, float _enemyShotY,
		float _enemyShotZ, SRect3D _enemyRect);	
	

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



	//ダメージ処理
	void Damage(int damage);

	//毛虫の判定
	void HitCheckCatp(void);

	//ハチの判定
	void HitCheckBee(void);


	//無敵時間
	void InvTime();



	//無敵フラグ
	bool m_invincible;	
	//無敵時間用タイマー
	int m_invTimer;	

	// 横方向の移動速度
	float move_Speed_X = 7.0f;	
	//攻撃判定の有無
	bool m_atk;

	EState m_state;
	int m_stateStep;

	// アニメーションの種類
	enum class EAnimType
	{
		Idle,	// 待機
		Move,	// 移動
		Death,	// 死亡
		Attack,	// 攻撃
		Jump,	// 跳躍

		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// プレイヤーの画像

	float m_moveSpeedY;	// Y軸（高さ）の移動速度
};
