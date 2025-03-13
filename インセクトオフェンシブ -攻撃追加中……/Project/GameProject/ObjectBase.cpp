#include "ObjectBase.h"
#define FIELD_CENTER_Z (SCREEN_HEIGHT * 0.75f)

// コンストラクタ
ObjectBase::ObjectBase()
	: ObjectBase(CVector3D::zero)
{
}

ObjectBase::ObjectBase(const CVector3D& pos)
	: Task((int)ETaskPrio::Object,0)
	, m_pos(pos)
	, m_isGrounded(true)
	, mp_shadowImg(nullptr)
{
	// 影の画像を読み込み
	mp_shadowImg = CImage::CreateImage("shadow.png");
	mp_shadowImg->SetCenter(mp_shadowImg->GetSize() * 0.5f);
}

// デストラクタ
ObjectBase::~ObjectBase()
{
	// 影の画像データを削除
	if (mp_shadowImg != nullptr)
	{
		delete mp_shadowImg;
		mp_shadowImg = nullptr;
	}
}

// 座標を取得
const CVector3D& ObjectBase::GetPos() const
{
	return m_pos;
}

// 座標を設定
void ObjectBase::SetPos(const CVector3D& pos)
{
	m_pos = pos;
}

// 3次元座標から2次元座標を計算
CVector2D ObjectBase::CalcScreenPos(bool grounded) const
{
	CVector2D ret;

	// X座標はそのまま設定
	ret.x = m_pos.x;
	// 通常座標を求める場合
	if (!grounded)
	{
		// Y座標は、3次元座標のY（高さ）とZ（奥行）を合わせる
		float posZ = FIELD_CENTER_Z + m_pos.z;
		ret.y = -m_pos.y + posZ;
	}
	// 高さを考慮しない地面の位置を求める場合
	else
	{
		// Y座標は、3次元座標のZ（奥行）のみ反映する
		ret.y = FIELD_CENTER_Z + m_pos.z;
	}

	return ret;
}

// 影描画
void ObjectBase::RenderShadow()
{
	// 影を地面の位置で描画
	mp_shadowImg->SetPos(CalcScreenPos(true));
	mp_shadowImg->Draw();
}
