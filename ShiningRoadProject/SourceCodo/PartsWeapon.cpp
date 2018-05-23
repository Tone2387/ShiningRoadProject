#include "PartsWeapon.h"


clsPARTS_WEAPON::clsPARTS_WEAPON()
{
}

clsPARTS_WEAPON::~clsPARTS_WEAPON()
{
}


void clsPARTS_WEAPON::SetAtk( const int iAtk )
{
	m_iAtk = iAtk;
}
int clsPARTS_WEAPON::GetAtk() const
{
	return m_iAtk;
}


void clsPARTS_WEAPON::SetLoadTime( const int iLoad )
{
	m_iLoadTime = iLoad;
}
int clsPARTS_WEAPON::GetLoadTime() const
{
	return m_iLoadTime;
}




