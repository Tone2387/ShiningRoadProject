#include "MenuWindowGameOverBase.h"
#include "SoundManagerGameOver.h"

namespace{
	const float fBACK_ALPHA = 0.75f;
}

clsMENU_WINDOW_GAME_OVER_BASE::clsMENU_WINDOW_GAME_OVER_BASE(		
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	std::vector<unsigned int>* const pInformationVec )
	:clsMENU_WINDOW_BASE( pPtrGroup, pParentWindow, pInformationVec, enSE_WIN_DISAPP )
	,m_INFORMATION__INDEX_GAME_OVER( 0 )
	,m_INFORMATION__INDEX_CONTINUE( 1 )
	,m_INFORMATION__INDEX_ASSEMBLE( 2 )
{
	assert( m_wpSound );
	m_wpSound->PlaySE( enSE_WIN_APP );

	SetAlphaBack( fBACK_ALPHA );
}


clsMENU_WINDOW_GAME_OVER_BASE::~clsMENU_WINDOW_GAME_OVER_BASE()
{
}
