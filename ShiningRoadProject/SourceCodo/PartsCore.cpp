#include "PartsCore.h"


clsPARTS_CORE::clsPARTS_CORE() :
	m_iHp( 0 )
{
}

clsPARTS_CORE::~clsPARTS_CORE()
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




