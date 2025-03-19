#pragma once
#include "Task.h"

class Task;	//クラスの前宣言
			//前宣言だけでポインタは扱える
class ObjectBase;

//タスクを管理するクラス
class TaskManager
{
public:	//シングルトン(気になる人は調べてみて)
	//TaskManagerのインスタンスを取得
	static TaskManager* Instance();
	//TaskManagerのインスタンスを破棄
	static void ClearInstance();
	
	//タスクリストにタスクを追加
	void Add(Task* task, bool sort = false);
	//タスクリストからタスクを取り除く
	void Remove(Task* task, bool sort = false);

	//全てのタスクを削除
	void DeleteAll();
	//削除フラグが立っているタスクを削除
	void DeleteKilledTask();

	//指定したidを持ち、指定した番号のタスクを返す
	void* GetTask(CAST _id, DWORD _number = 0);

	//リストに登録されているタスクを更新
	void Update();
	//リストに登録されているタスクを描画
	void Render();

private:
	//コンストラクタ
	TaskManager();
	//デストラクタ
	~TaskManager();

	//TaskManagerのインスタンス
	static TaskManager* ms_instance;
	//生成済みのタスクリスト
	std::list<Task*> m_taskList;	//生成済みのタスクのリスト
	//生成済みのオブジェクトのリスト
	std::list<Task*> m_objectList;

};