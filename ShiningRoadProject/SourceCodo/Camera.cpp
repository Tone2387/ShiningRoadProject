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


