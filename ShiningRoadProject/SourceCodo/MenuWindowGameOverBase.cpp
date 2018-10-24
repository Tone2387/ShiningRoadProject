#include "MenuWindowGameOverBase.h"
#include "SoundManagerGameOver.h"


clsMENU_WINDOW_GAME_OVER_BASE::clsMENU_WINDOW_GAME_OVER_BASE(		
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	unsigned int* const pInformationArray )
	:clsMENU_WINDOW_BASE( pPtrGroup, pParentWindow, pInformationArray, enSE_WIN_DISAPP )
	,m_INFORMATION__INDEX_GAME_OVER( 0 )
	,m_INFORMATION__INDEX_CONTINUE( 1 )
	,m_INFORMATION__INDEX_ASSEMBLE( 2 )
{
	assert( m_wpSound );
	m_wpSound->PlaySE( enSE_WIN_APP );

}


clsMENU_WINDOW_GAME_OVER_BASE::~clsMENU_WINDOW_GAME_OVER_BASE()
{
}
