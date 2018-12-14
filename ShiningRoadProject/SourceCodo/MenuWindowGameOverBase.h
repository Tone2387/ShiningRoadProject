#ifndef MENU_WINDOW_GAME_OVER_BASE_H_
#define MENU_WINDOW_GAME_OVER_BASE_H_


#include "MenuWindowBase.h"
#include "SceneGameOverInformation.h"

class clsMENU_WINDOW_GAME_OVER_BASE : public clsMENU_WINDOW_BASE
{
public:
	clsMENU_WINDOW_GAME_OVER_BASE(		
		const HWND hWnd,
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec,
		const char* sWindowName );

	virtual ~clsMENU_WINDOW_GAME_OVER_BASE();


	//シーンクラスと情報のやり取りをするデータ作成関数( のラップ関数 ).
	static INFORMATION_MENU_DATA_ARRAY CreateInformation(
		INFORMATION_MENU_DATA_ARRAY* const InformationDataArray,
		const int iInformationSize );

private:

};

#endif//#ifndef MENU_WINDOW_GAME_OVER_BASE_H_