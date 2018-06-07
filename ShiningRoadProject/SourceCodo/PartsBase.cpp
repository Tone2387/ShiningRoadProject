#include "PartsBase.h"


clsPARTS_BASE::clsPARTS_BASE()
{	
}

clsPARTS_BASE::~clsPARTS_BASE()
{
}



//çÏê¨éû.
void clsPARTS_BASE::Create()
{
	CreateProduct();
}

//ñàÉtÉåÅ[ÉÄ.
void clsPARTS_BASE::Update()
{
	UpdateProduct();
	ModelUpdate( m_Trans );
}

//ï`âÊ.
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



