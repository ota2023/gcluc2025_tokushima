#include "EnemyManager.h"
#include "EnemyBase.h"
#include "Player.h"

EnemyManager* EnemyManager::sp_instance = nullptr;

// インスタンス取得
EnemyManager* EnemyManager::Instance()
{
	// インスタンスがなければ、生成
	if (sp_instance == nullptr)
	{
		sp_instance = new EnemyManager();
	}
	return sp_instance;
}

// インスタンス破棄
void EnemyManager::ClearInstance()
{
	if (sp_instance != nullptr)
	{
		delete sp_instance;
	}
}

// 敵をリストに追加
void EnemyManager::Add(EnemyBase* enemy)
{
	m_enemyList.push_back(enemy);
}

// 敵をリストから取り除く
void EnemyManager::Remove(EnemyBase* enemy)
{
	m_enemyList.remove(enemy);
}

void EnemyManager::Print()
{
	printf("ﾆﾌﾞ\n");

}





// コンストラクタ
EnemyManager::EnemyManager()
{
}

// デストラクタ
EnemyManager::~EnemyManager()
{
	sp_instance = nullptr;
}
