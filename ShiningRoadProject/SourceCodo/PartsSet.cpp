#include "PartsSet.h"


clsPARTS_SET::clsPARTS_SET() :
	m_pLeg( nullptr ),
	m_pCore( nullptr ),
	m_pHead( nullptr ),
//	m_pArms( nullptr ),
	m_pArmL( nullptr ),
	m_pArmR( nullptr ),
	m_pWeapon( nullptr )
{
}

clsPARTS_SET::~clsPARTS_SET()
{
	SAFE_DELETE( m_pWeapon );
//	SAFE_DELETE( m_pArms );
	SAFE_DELETE( m_pArmR );
	SAFE_DELETE( m_pArmL );
	SAFE_DELETE( m_pHead );
	SAFE_DELETE( m_pCore );
	SAFE_DELETE( m_pLeg );
}







