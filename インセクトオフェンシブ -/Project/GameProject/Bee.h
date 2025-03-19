#pragma once
#include "EnemyBase.h"
#include "Player.h"

class Bee : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">毛虫の種類</param>
	/// <param name="pos">初期位置</param>
	Bee(int type, const CVector3D& pos);
	// デストラクタ
	~Bee();

	// 死亡処理
	virtual void Death() override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	CAST m_id;

private:
	// 状態
	enum class EState
	{
		Idle,	// 待機状態
		Death,	// 死亡
	};
	void ChangeState(EState state);

	bool UpdateMove();


	// 待機時の更新処理
	void StateIdle();
	// 死亡時の更新処理
	void StateDeath();

	EState m_state;
	int m_stateStep;

	// アニメーションの種類
	enum class EAnimType
	{
		Idle,	// 待機
		Death,	// 死亡

		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// 画像
	int m_type;			// スライムの種類


};


