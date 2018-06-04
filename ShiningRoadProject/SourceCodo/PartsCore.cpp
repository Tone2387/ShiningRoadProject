#include "PartsCore.h"


clsPARTS_CORE::clsPARTS_CORE() :
	m_iHp( 0 )
{
}

clsPARTS_CORE::~clsPARTS_CORE()
{
}

//çÏê¨éû.
void clsPARTS_CORE::CreateProduct()
{
}

//ñàÉtÉåÅ[ÉÄ.
void clsPARTS_CORE::UpdateProduct()
{
}

//ï`âÊ.
void clsPARTS_CORE::RenderProduct(
	const D3DXMATRIX& const mView, 
	const D3DXMATRIX& const mProj, 
	const D3DXVECTOR3& const vLight, 
	const D3DXVECTOR3& const vEye,
	const D3DXVECTOR4 &vColor,
	const bool isAlpha )
{
}

void clsPARTS_CORE::SetHp( const int iHp )
{
	m_iHp = iHp;
}
int clsPARTS_CORE::GetHp() const
{
	return m_iHp;
}
void clsPARTS_CORE::AddHp( const int iHp )
{
	m_iHp += iHp;
}

void clsPARTS_CORE::SetActTime( const int iTime )
{
	m_iActTime = iTime;
}
int clsPARTS_CORE::GetActTime() const
{
	return m_iActTime;
}
void clsPARTS_CORE::AddActTime( const int iTime )
{
	m_iActTime += iTime;
}




