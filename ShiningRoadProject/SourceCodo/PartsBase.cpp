#include "PartsBase.h"


clsPARTS_BASE::clsPARTS_BASE()
{	
}

clsPARTS_BASE::~clsPARTS_BASE()
{
}



//作成時.
void clsPARTS_BASE::Create()
{
	CreateProduct();
}

//毎フレーム.
void clsPARTS_BASE::Update()
{
	UpdateProduct();
	ModelUpdate( m_Trans );
}

//描画.
void clsPARTS_BASE::Render(
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3& vLight, 
	const D3DXVECTOR3& vEye,
	const D3DXVECTOR4 &vColor,
	const bool isAlpha )
{
	ModelRender( mView, mProj, vLight, vEye, vColor, isAlpha );
}



//アニメーション変更.//変更できるならtrue, 変更できないならfalseが返る.
bool clsPARTS_BASE::PartsAnimChange( const int iIndex )
{
	return SetAnimChange( iIndex );
}
