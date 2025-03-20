#include "Task.h"
#include "TaskManager.h"

Task::Task()
{
}

//コンストラクタ
Task::Task(int prio, int sortOrder,CAST _id)
	:m_isEnable(true)
	,m_isShow(true)
	,m_isKill(false)
	,m_prio(prio)
	,m_sortOrder(sortOrder)
	,m_id(_id)
{
	//TaskManagerのリストに自身を追加
	TaskManager::Instance()->Add(this);

}

//デストラクタ
Task::~Task()
{
	//TaskManagerのリストから自身を取り除く
	TaskManager::Instance()->Remove(this);
}

//有効フラグを設定
void Task::SetEnable(bool enable)
{
	m_isEnable = enable;
}

//有効フラグを取得
bool Task::IsEnable() const
{
	return m_isEnable;
}

void Task::SetShow(bool show)
{
	m_isShow = show;
}

bool Task::IsShow() const
{
	return m_isShow;
}

void Task::Kill()
{
	m_isKill = true;
}

bool Task::IsKill() const
{
	return m_isKill;
}

void Task::SetPriority(int prio)
{
	m_prio = prio;
	//一度リストから取り除いて、彩度追加しなおすことで並び替える
	TaskManager::Instance()->Remove(this, true);
	TaskManager::Instance()->Add(this, true);
}

int Task::GetPriority() const
{
	return m_prio;
}

void Task::SetSortOeder(int order)
{
	m_sortOrder = order;
	//一度リストから取り除いて、彩度追加しなおすことで並び替える
	TaskManager::Instance()->Remove(this, true);
	TaskManager::Instance()->Add(this, true);
}

int Task::GetSortOeder() const
{
	return m_sortOrder;
}

CAST Task::GetId() const
{
	return m_id;
}

//更新
void Task::Update()
{

}

//描画
void Task::Render()
{

}
