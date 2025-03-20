#pragma once
#include "ObjectBase.h"

// キャラクターのベースクラス
class CharaBase : public ObjectBase
{
public:
	// コンストラクタ
	CharaBase();
	CharaBase(const CVector3D& pos,CAST _id);
	// デストラクタ
	virtual ~CharaBase();

	// ダメージを受ける
	void TakeDamage(int damage);
	// 死亡処理
	virtual void Death();

protected:
	int m_maxHp;		// 最大HP
	int m_hp;			// 現在HP
	bool m_isDeath;		// 死亡フラグ
};
