#include "PartsHead.h"


clsPARTS_HEAD::clsPARTS_HEAD()
{
}

clsPARTS_HEAD::~clsPARTS_HEAD()
{
}

//作成時.
void clsPARTS_HEAD::CreateProduct()
{
}

//毎フレーム.
void clsPARTS_HEAD::UpdateProduct()
{
}

//描画.
void clsPARTS_HEAD::RenderProduct(
	const D3DXMATRIX& const mView, 
	const D3DXMATRIX& const mProj, 
	const D3DXVECTOR3& const vLight, 
	const D3DXVECTOR3& const vEye,
	const D3DXVECTOR4 &vColor,
	const bool isAlpha )
{
}

void clsPARTS_HEAD::SetHp( const int iHp )
{
	m_iHp = iHp;
}
int clsPARTS_HEAD::GetHp() const
{
	return m_iHp;
}
void clsPARTS_HEAD::AddHp( const int iHp )
{
	m_iHp += iHp;
}
