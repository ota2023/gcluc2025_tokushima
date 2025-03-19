#include "CharaBase.h"

// コンストラクタ
CharaBase::CharaBase()
	: CharaBase(CVector3D::zero)
{
}

CharaBase::CharaBase(const CVector3D& pos)
	: ObjectBase(pos)
	, m_maxHp(100)
	, m_hp(m_maxHp)
	, m_isDeath(false)
{
}

// デストラクタ
CharaBase::~CharaBase()
{
}

// ダメージを受ける
void CharaBase::TakeDamage(int damage)
{
	if (m_hp == 0) return;	// HP0なのでスルー

	// 現在HPよりダメージ量が少ない
	if (m_hp > damage)
	{
		m_hp -= damage;	// そのままHPを減らす
	}
	// ダメージ量の方が大きい
	else
	{
		// HPを0にして、死亡処理を実行
		m_hp = 0;
		Death();
	}
}

// 死亡処理
void CharaBase::Death()
{
	m_isDeath = true;
}
