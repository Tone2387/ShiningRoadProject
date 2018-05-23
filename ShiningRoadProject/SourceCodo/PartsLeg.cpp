#include "PartsLeg.h"



clsPARTS_LEG::clsPARTS_LEG()
{
}

clsPARTS_LEG::~clsPARTS_LEG()
{
}


void clsPARTS_LEG::SetWalkSpd( const float fSpd )
{
	m_fWalkSpd = fSpd;
}
float clsPARTS_LEG::GetWalkSpd() const
{
	return m_fWalkSpd;
}

void clsPARTS_LEG::SetBoostSpd( const float fSpd )
{
	m_fBoostSpd = fSpd;
}
float clsPARTS_LEG::GetBoostSpd() const
{
	return m_fBoostSpd;
}
