#include "PartsHead.h"


clsPARTS_HEAD::clsPARTS_HEAD()
{
}

clsPARTS_HEAD::~clsPARTS_HEAD()
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
