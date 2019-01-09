#include "RoboTakeoff.h"

clsROBO_TAKEOFF::clsROBO_TAKEOFF()
{
}

clsROBO_TAKEOFF::~clsROBO_TAKEOFF()
{
}


void clsROBO_TAKEOFF::IgnitionCoreBoost( const bool isIgnition )
{
//	clsRoboCommand* pRoboCom;
//	pRoboCom = m_pInput->LSInput(fPush, fAngle);
//
//	if (pRoboCom)
//	{
//		pRoboCom->Trigger(this, fPush, fAngle);
//	}

	//スティックが前方向に倒されている状態になる.
	if( isIgnition ) { SetMoveDir( 0.0f ); }
}

