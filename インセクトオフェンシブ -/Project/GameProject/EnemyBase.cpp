#include "EnemyBase.h"
#include "EnemyManager.h"

// コンストラクタ
EnemyBase::EnemyBase()
	//: EnemyBase(CVector3D::zero)
{
}

EnemyBase::EnemyBase(const CVector3D& pos, CAST _id)
	: CharaBase(pos,_id)
{
	// 敵の管理クラスのリストに自身を追加
	EnemyManager::Instance()->Add(this);
}

// デストラクタ
EnemyBase::~EnemyBase()
{
	// 敵の管理クラスのリストから自身を取り除く
	EnemyManager::Instance()->Remove(this);
}
