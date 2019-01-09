#include "Common.h"
#include "BlendState.h"
#include "Singleton.h"

clsCommon::clsCommon()
	:m_psinBlend( nullptr )
	,m_wpDevice( nullptr )
	,m_wpContext( nullptr )
{

	//ブレンドステート取得.
	m_psinBlend = &clsSINGLETON<clsBLEND_STATE>::GetInstance();

//	if( FAILED( CreateBlendState() ) ){
//		assert( !"Can't Create Blend State" );
//	}

};
clsCommon::~clsCommon()
{
	m_psinBlend = nullptr;

	//ここでは開放しない.
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
};

