#include "MenuWindowMissionBase.h"


namespace{
	const float fBACK_ALPHA = 0.75f;
}


clsMENU_WINDOW_MISSION_BASE::clsMENU_WINDOW_MISSION_BASE(		
	const HWND hWnd,
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	std::vector<unsigned int>* const pInformationVec,
	const char* sWindowName )
	:clsMENU_WINDOW_BASE( hWnd, pPtrGroup, pParentWindow, pInformationVec, sWindowName )
{
	SetAlphaBack( fBACK_ALPHA );
}

clsMENU_WINDOW_MISSION_BASE::~clsMENU_WINDOW_MISSION_BASE()
{
}




