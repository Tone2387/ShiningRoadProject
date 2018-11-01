
//これを有効にするとすべてのキーで判定を行う.
#define IS_ALL_KEY

#include "KeyInput.h"
#include <Windows.h>


using namespace std;


namespace{

#ifdef IS_ALL_KEY

	const unsigned short enKEY_size = 256;

#else//#ifdef IS_ALL_KEY

	//使うキー.
	enum enKEY : unsigned char
	{
		enKEY_UP = 0,
		enKEY_DOWN,
		enKEY_RIGHT,
		enKEY_LEFT,

		enKEY_Z,
		enKEY_SPACE,

		enKEY_ESCAPE,

		enKEY_size
	};

#endif//#ifdef IS_ALL_KEY

}


clsKEY_INPUT::clsKEY_INPUT()
	:m_isStartFrame( true )
{
	m_bPushs.resize( enKEY_size, false );
//	m_bPushsOld = m_bPushs;//どうせUpdateでやるので封印.

	m_iKeyCodes.resize( m_bPushs.size() );

#ifdef IS_ALL_KEY
	for( unsigned int i=0; i<m_bPushs.size(); i++ ){
		m_iKeyCodes[i] = i;
	}
#else//#ifdef IS_ALL_KEY
	m_iKeyCodes[ enKEY_UP ]		= VK_UP;
	m_iKeyCodes[ enKEY_DOWN ]	= VK_DOWN;
	m_iKeyCodes[ enKEY_RIGHT ]	= VK_RIGHT;
	m_iKeyCodes[ enKEY_LEFT ]	= VK_LEFT;
	m_iKeyCodes[ enKEY_Z ]		= 'Z';
	m_iKeyCodes[ enKEY_SPACE ]	= VK_SPACE;
	m_iKeyCodes[ enKEY_ESCAPE ] = VK_ESCAPE;
#endif//#ifdef IS_ALL_KEY

}

clsKEY_INPUT::~clsKEY_INPUT()
{
	m_isStartFrame = false;
}



void clsKEY_INPUT::Update()
{
	m_bPushsOld = m_bPushs;

	for( unsigned int i=0; i<m_bPushs.size(); i++ )
	{
		if( GetAsyncKeyState( m_iKeyCodes[i] ) & 0x8000 ){
			m_bPushs[i] = true;
		}
		else{
			m_bPushs[i] = false;
		}
	}

	//起動時のoldはnowにあわせる.
	if( m_isStartFrame ){
		m_isStartFrame = false;
		m_bPushsOld = m_bPushs;
	}
}

bool clsKEY_INPUT::isEnter( unsigned char ucKey )
{
	int tmpIndex = GetKeyIndex( ucKey );

	if( m_bPushsOld[ tmpIndex ] ){
		return false;
	}

	if( m_bPushs[ tmpIndex ] ){
		return true;
	}
	
	return false;
}

bool clsKEY_INPUT::isStay( unsigned char ucKey )
{
	int tmpIndex = GetKeyIndex( ucKey );

	if( m_bPushs[ tmpIndex ] ){
		return true;
	}

	return false;
}

bool clsKEY_INPUT::isExit( unsigned char ucKey )
{
	int tmpIndex = GetKeyIndex( ucKey );

	if( m_bPushs[ tmpIndex ] ){
		return false;
	}
	
	if( m_bPushsOld[ tmpIndex ] ){
		return true;
	}

	return false;
}


int clsKEY_INPUT::GetKeyIndex( unsigned char ucKey )
{
	for( unsigned int i=0; i<m_bPushs.size(); i++ )
	{
		if( ucKey == m_iKeyCodes[i] ){
			return i;
		}
	}

	return -1;
}
