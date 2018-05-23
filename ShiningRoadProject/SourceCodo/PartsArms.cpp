#include "PartsArms.h"

clsPARTS_ARMS::clsPARTS_ARMS() :
	m_pL( nullptr ),
	m_pR( nullptr )
{
}

clsPARTS_ARMS::~clsPARTS_ARMS()
{
	SAFE_DELETE( m_pR );
	SAFE_DELETE( m_pL );
}