#include "MenuWindowTitleBase.h"
#include "SoundManagerTitle.h"

namespace{
	const float fBACK_ALPHA = 0.75f;
}

clsMENU_WINDOW_TITLE_BASE::clsMENU_WINDOW_TITLE_BASE(		
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	std::vector<unsigned int>* const pInformationVec )
	:clsMENU_WINDOW_BASE( pPtrGroup, pParentWindow, pInformationVec, enSE_WIN_DISAPP )
{
	assert( m_wpSound );
	m_wpSound->PlaySE( enSE_WIN_APP );

	SetAlphaBack( fBACK_ALPHA );
}

clsMENU_WINDOW_TITLE_BASE::~clsMENU_WINDOW_TITLE_BASE()
{
}

