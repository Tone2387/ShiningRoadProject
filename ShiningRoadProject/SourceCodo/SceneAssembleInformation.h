#ifndef SCENE_ASSEMBLE_INFORMATION_H_
#define SCENE_ASSEMBLE_INFORMATION_H_
#include "DxInput.h"


//音の引数.
enum enBGM : int
{
	enBGM_RENGOKU0 = 0,
	enBGM_MAOU0,
	enBGM_MAOU2,
	enBGM_HART1,
	enBGM_NOVA1,
	enBGM_NOVA2,
};

enum enSE : int
{
	enSE_CURSOL_MOVE = 0,
	enSE_ENTER,
	enSE_EXIT,
	enSE_MISSION_START,
	enSE_WIN_APP,
	enSE_WIN_DISAPP,
};


//メニューから受け取った情報を照合する.
enum enINFORMATION_INDEX : char
{
	enINFORMATION_INDEX_GO_TITLE = 0,
	enINFORMATION_INDEX_GO_MISSION,
	enINFORMATION_INDEX_CLOSE_WINDOW,

	enINFORMATION_INDEX_COLOR_1_R_INCREMENT,
	enINFORMATION_INDEX_COLOR_1_R_DECREMENT,
	enINFORMATION_INDEX_COLOR_1_G_INCREMENT,
	enINFORMATION_INDEX_COLOR_1_G_DECREMENT,
	enINFORMATION_INDEX_COLOR_1_B_INCREMENT,
	enINFORMATION_INDEX_COLOR_1_B_DECREMENT,
	
	enINFORMATION_INDEX_COLOR_2_R_INCREMENT,
	enINFORMATION_INDEX_COLOR_2_R_DECREMENT,
	enINFORMATION_INDEX_COLOR_2_G_INCREMENT,
	enINFORMATION_INDEX_COLOR_2_G_DECREMENT,
	enINFORMATION_INDEX_COLOR_2_B_INCREMENT,
	enINFORMATION_INDEX_COLOR_2_B_DECREMENT,

	enINFORMATION_INDEX_size
};

//Xインプットの配置を専用コントローラにするとどうなるか.
enum enGIGANT_WEAPONS_CONTROLLER
{
	enGIGANT_WEAPONS_CONTROLLER_X		= enPKey_06,//L1.
	enGIGANT_WEAPONS_CONTROLLER_Y		= enPKey_07,//R1.
	enGIGANT_WEAPONS_CONTROLLER_BACK	= enPKey_04,//L2.
	enGIGANT_WEAPONS_CONTROLLER_START	= enPKey_05,//R2.
};



#endif//#ifndef SCENE_ASSEMBLE_INFORMATION_H_