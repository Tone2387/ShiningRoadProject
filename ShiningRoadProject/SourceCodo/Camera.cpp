#include "Camera.h"

const D3DXVECTOR3 vVEC3_ZERO = { 0.0f, 0.0f, 0.0f };

clsCAMERA_BASE::clsCAMERA_BASE() :
	m_vPos( vVEC3_ZERO ),
	m_vLook( vVEC3_ZERO ),
	m_vRot( vVEC3_ZERO )
{
	 D3DXMatrixIdentity( &m_mRot );

	 //テスト用でとりあえずおいている.
	 m_vPos.z -= 10.0f;
}

clsCAMERA_BASE::~clsCAMERA_BASE()
{
}


void clsCAMERA_BASE::SetPos( const D3DXVECTOR3& vPos, bool isWithLook )
{
	if( isWithLook ){
		D3DXVECTOR3 tmpVec;//カメラの移動量.
		tmpVec = vPos - m_vPos;
		m_vLook = vPos + tmpVec;
	}
	m_vPos = vPos;
}

void clsCAMERA_BASE::AddPos( const D3DXVECTOR3& vPos, bool isWithLook )
{
	if( isWithLook ){
		m_vLook += vPos;
	}
	m_vPos += vPos;
}


D3DXVECTOR3 clsCAMERA_BASE::GetPos()
{
	return m_vPos;
}

D3DXVECTOR3 clsCAMERA_BASE::GetLookPos()
{
	return m_vLook;
}