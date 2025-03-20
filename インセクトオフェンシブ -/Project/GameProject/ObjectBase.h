#pragma once
#include "task.h"
#include "TaskManager.h"
#include "CollisionCheck.h"

// 全てのオブジェクトのベースクラス
class ObjectBase : public Task
{
public:
	// コンストラクタ
	ObjectBase();
	ObjectBase(const CVector3D& pos,CAST _id);
	// デストラクタ
	virtual ~ObjectBase();

	// 座標を取得
	const CVector3D& GetPos() const;
	// 座標を設定
	void SetPos(const CVector3D& pos);


	/// <summary>
	/// 3次元座標からスクリーン座標（2D）に変換
	/// </summary>
	/// <param name="grounded">trueならば、高さは考慮せず地面の位置を返す</param>
	/// <returns>スクリーン座標</returns>
	CVector2D CalcScreenPos(bool grounded = false) const;



	// 影描画
	virtual void RenderShadow();

protected:
	CVector3D m_pos;		// オブジェクトの座標
	bool m_isGrounded;		// 地面に接地してあるかどうか
	CImage* mp_shadowImg;	// 影の画像
	SRect3D m_rect = SRect3D{ 0, 0, 0, 0, 0, 0 };
	int m_timer = 0;//タイマー

};