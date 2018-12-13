#include "MenuWindowMissionPause.h"
#include "SoundManagerMenuWindow.h"



namespace{

	const int iSELECT_NUM_YES_INDEX = 0;

}



clsMENU_WINDOW_MISSION_PAUSE::clsMENU_WINDOW_MISSION_PAUSE(
	const HWND hWnd,
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	std::vector<unsigned int>* const pInformationVec )
	:clsMENU_WINDOW_MISSION_BASE( hWnd, pPtrGroup, pParentWindow, pInformationVec, "MenuWindowAssembleBackTitle" )
{
	//‚±‚Ì‘‹‚ÌƒTƒCƒY.
	const D3DXVECTOR2 vTHIS_WINDOW_SIZE = { 600.0f, 340.0f };
	Open( vTHIS_WINDOW_SIZE );
}

clsMENU_WINDOW_MISSION_PAUSE::~clsMENU_WINDOW_MISSION_PAUSE()
{
}




void clsMENU_WINDOW_MISSION_PAUSE::UpdateProduct()
{
}

void clsMENU_WINDOW_MISSION_PAUSE::RenderProduct()
{

}
