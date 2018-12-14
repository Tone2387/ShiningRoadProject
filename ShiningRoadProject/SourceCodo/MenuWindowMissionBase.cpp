#include "MenuWindowMissionBase.h"


namespace{
	const float fBACK_ALPHA = 0.75f;
}


clsMENU_WINDOW_MISSION_BASE::clsMENU_WINDOW_MISSION_BASE(		
	const HWND						hWnd,
	clsPOINTER_GROUP* const			pPtrGroup,
	clsMENU_WINDOW_BASE* const		pParentWindow,
	std::vector<unsigned int>* const pInformationVec,
	const D3DXVECTOR2&				vSize,
	const char*						sWindowName )
	:clsMENU_WINDOW_BASE( hWnd, pPtrGroup, pParentWindow, pInformationVec, vSize, sWindowName )
{
	SetAlphaBack( fBACK_ALPHA );
}

clsMENU_WINDOW_MISSION_BASE::~clsMENU_WINDOW_MISSION_BASE()
{
}


//シーンクラスと情報のやり取りをするデータ作成関数( のラップ関数 ).
clsMENU_WINDOW_BASE::INFORMATION_MENU_DATA_ARRAY clsMENU_WINDOW_MISSION_BASE::CreateInformation( 
	INFORMATION_MENU_DATA_ARRAY* const InformationDataArray,
	const int iInformationSize )
{
	const char sInformationDataPath[] = "Data\\FileData\\Tahara\\MissionMenuInformation.csv";

	return clsMENU_WINDOW_BASE::CreateInformationProduct(
		InformationDataArray,
		iInformationSize,
		sInformationDataPath );
}



