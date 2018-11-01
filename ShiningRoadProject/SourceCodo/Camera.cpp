#include "Camera.h"

namespace{
	const D3DXVECTOR3 vVEC3_ZERO = { 0.0f, 0.0f, 0.0f };
}

clsCAMERA_BASE::clsCAMERA_BASE() :
	m_vPos( vVEC3_ZERO ),
	m_vLook( vVEC3_ZERO ),
	m_vRot( vVEC3_ZERO )
{
	 D3DXMatrixIdentity( &m_mRot );
}

clsCAMERA_BASE::~clsCAMERA_BASE()
{
}


void clsCAMERA_BASE::SetPos( const D3DXVECTOR3& vPos, bool isWithLook )
{
	if( isWithLook ){
		D3DXVECTOR3 tmpVec;//ÉJÉÅÉâÇÃà⁄ìÆó .
		tmpVec = vPos - m_vPos;
		m_vLook = vPos + tmpVec;
	}
	m_vPos = vPos;
}

void clsCAMERA_BASE::AddPos( const D3DXVECTOR3& vVec, bool isWithLook )
{
	if( isWithLook ){
		m_vLook += vVec;
	}
	m_vPos += vVec;
}

D3DXVECTOR3 clsCAMERA_BASE::GetPos()
{
	return m_vPos;
}


void clsCAMERA_BASE::SetLookPos( const D3DXVECTOR3& vPos )
{
	m_vLook = vPos;
}

void clsCAMERA_BASE::AddLookPos( const D3DXVECTOR3& vVec )
{
	m_vLook += vVec;
}

D3DXVECTOR3 clsCAMERA_BASE::GetLookPos()
{
	return m_vLook;
}

D3DXVECTOR3 clsCAMERA_BASE::GetRot()
{
	return m_vRot;
};
void clsCAMERA_BASE::SetRot( const D3DXVECTOR3 &vRot )
{
	m_vRot = vRot;
}
