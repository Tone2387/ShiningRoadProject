#include "CameraMission.h"

clsCAMERA_MISSION::clsCAMERA_MISSION()
{
}

clsCAMERA_MISSION::~clsCAMERA_MISSION()
{
}

void clsCAMERA_MISSION::Create()
{
	
}

void clsCAMERA_MISSION::Update(const D3DXVECTOR3 vCamPos, const D3DXVECTOR3 vLookPos)
{
	m_vPos = vCamPos;
	m_vLook = vLookPos;
}



#ifdef Tahara
void clsCAMERA_MISSION::Update()
{
}
#endif//#ifdef Tahara