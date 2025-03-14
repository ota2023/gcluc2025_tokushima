#pragma once
#include "EnemyBase.h"

class Slime : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">スライムの種類</param>
	/// <param name="pos">初期位置</param>
	Slime(int type, const CVector3D& pos);
	// デストラクタ
	~Slime();

	// 死亡処理
	virtual void Death() override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;



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