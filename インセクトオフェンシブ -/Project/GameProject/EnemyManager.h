#pragma once

class EnemyBase;

// 敵管理クラス
class EnemyManager
{
public:
	// インスタンス取得
	static EnemyManager* Instance();
	// インスタンス破棄
	static void ClearInstance();

	// 敵をリストに追加
	void Add(EnemyBase* enemy);
	// 敵をリストから取り除く
	void Remove(EnemyBase* enemy);

	//文字描画
	void Print();


private:
	// コンストラクタ
	EnemyManager();
	// デストラクタ
	~EnemyManager();

	// インスタンス
	static EnemyManager* sp_instance;
	// 生成済みのエネミーのリスト
	std::list<EnemyBase*> m_enemyList;
};
