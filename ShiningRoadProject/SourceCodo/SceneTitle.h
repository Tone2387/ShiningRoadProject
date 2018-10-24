#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_


#include "SceneBase.h"
#include "CameraTitle.h"
#include "Sprite2DCenter.h"

#include "AssembleModel.h"


class clsMENU_WINDOW_TITLE_START_OR_END;

//================================//
//========== タイトルクラス ==========//
//================================//
class clsSCENE_TITLE : public clsSCENE_BASE
{
public:
	clsSCENE_TITLE( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_TITLE();


private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//「 UIの 」Render.

	//メニューの動き.
	void MenuUpdate( enSCENE &enNextScene );

	//タイトルでズンとただずむロボット君.
	clsASSEMBLE_MODEL*	m_pRoboModel;

	//背景.
	std::unique_ptr< clsCharaStatic > m_upBack;

	std::unique_ptr< clsSprite2D > m_upLogo;

	std::unique_ptr< clsSprite2D > m_upFlash;



	std::unique_ptr< clsMENU_WINDOW_TITLE_START_OR_END > m_upMenuBox;

	//メニューから受け取った情報を照合する.
	enum enINFORMATION : char
	{
		enINFORMATION_GAME_END = 0,
		enINFORMATION_NEXT_SCENE,
		enINFORMATION_CLOSE_MENU,

		enINFORMATION_size
	};
	unsigned int m_uiInformationDataArray[ enINFORMATION_size ];


	//テスト用エフェクト.
	::Effekseer::Handle m_ehHibana;

	//音の引数.
	enum enBGM : int
	{
		enBGM_MAOU1 = 0,
		enBGM_MAOU3,
	};

	enum enSE : int
	{
		enSE_CURSOL_MOVE = 0,
		enSE_ENTER,
		enSE_EXIT,

		enSE_BOMBER,

		enSE_WIN_APP,
		enSE_WIN_DIS_APP
	};


#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

};
#endif//#ifndef SCENE_TITLE_H_