#pragma once
#include "CharaBase.h"

// 敵のベースクラス
class EnemyBase : public CharaBase
{
public:
	// コンストラクタ
	EnemyBase();
	EnemyBase(const CVector3D& pos,CAST _id);
	// デストラクタ
	virtual ~EnemyBase();

private:
};
