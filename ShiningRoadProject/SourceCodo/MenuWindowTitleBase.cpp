#include "MenuWindowTitleBase.h"
#include "SoundManagerTitle.h"

namespace{
	const float fBACK_ALPHA = 0.75f;
}

clsMENU_WINDOW_TITLE_BASE::clsMENU_WINDOW_TITLE_BASE(		
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	unsigned int* const pInformationArray )
	:clsMENU_WINDOW_BASE( pPtrGroup, pParentWindow, pInformationArray, enSE_WIN_DIS_APP )
	,m_INFORMATION__INDEX_GAME_END( 0 )
	,m_INFORMATION__INDEX_NEXT_SCENE( 1 )
	,m_INFORMATION__INDEX_CLOSE_MENU( 2 )
{
	assert( m_wpSound );
	m_wpSound->PlaySE( enSE_WIN_APP );

	SetAlphaBack( fBACK_ALPHA );
}

clsMENU_WINDOW_TITLE_BASE::~clsMENU_WINDOW_TITLE_BASE()
{
}
