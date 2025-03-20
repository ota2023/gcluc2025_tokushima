#pragma once

class TaskManager;

//タスクの優先度の種類
enum class ETaskPrio
{
	Field,	//フィールド
	Shadow,	//影
	Object,	//オブジェクト（プレイヤーとか敵とか）

};

enum class CAST {
	PLAYER,ENEMY,NONE
};

class Task
{
	friend TaskManager;

public :
	Task();

	//コンストラクタ
	Task(int prio, int sortOrder,CAST _id);
	//デストラクタ
	virtual ~Task();

	//有効フラグ設定
	void SetEnable(bool enable);
	//有効フラグを取得
	bool IsEnable() const;

	//表示フラグを設定
	void SetShow(bool show);
	//j表示フラグを取得
	bool IsShow() const;

	//タスクを削除
	void Kill();
	//削除フラグが立っているか
	bool IsKill() const;

	//優先度を設定
	void SetPriority(int prio);
	//優先度を取得
	int GetPriority() const;

	//優先度内の順番を設定
	void SetSortOeder(int order);
	//優先度内の順番を取得
	int GetSortOeder() const;

	//グループ識別子(id)を取得
	CAST GetId() const;

	//更新
	virtual void Update();
	//描画
	virtual void Render();


private:
	bool m_isEnable;	//有効フラグ
	bool m_isShow;		//表示フラグ
	bool m_isKill;		//削除フラグ
	int m_prio;			//タスクの優先度
	int m_sortOrder;	//同じ優先度内での順番
	CAST m_id = CAST::NONE;	//グループ識別子


};