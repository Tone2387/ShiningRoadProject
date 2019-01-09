#include "Common.h"
#include "BlendState.h"
#include "Singleton.h"

clsCommon::clsCommon()
	:m_psinBlend( nullptr )
	,m_wpDevice( nullptr )
	,m_wpContext( nullptr )
{

	//�u�����h�X�e�[�g�擾.
	m_psinBlend = &clsSINGLETON<clsBLEND_STATE>::GetInstance();

//	if( FAILED( CreateBlendState() ) ){
//		assert( !"Can't Create Blend State" );
//	}

};
clsCommon::~clsCommon()
{
	m_psinBlend = nullptr;

	//�����ł͊J�����Ȃ�.
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
};

