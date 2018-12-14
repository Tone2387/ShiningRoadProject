#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_

class clsASSEMBLE_MODEL;
class clsMENU_WINDOW_TITLE_START_OR_END;

class clsStage;

#include "SceneBase.h"



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

	//テキストの明滅.
	void TextAlphaUpdate();


	//メニューの動き.
	void MenuUpdate( enSCENE &enNextScene );


#ifdef _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:

	//音の引数.
	enum enBGM : int
	{
		enBGM_MAOU1 = 0,
		enBGM_MAOU3,
		enBGM_AMACHA0,
		enBGM_NOVA2,
	};

	enum enSE : int
	{
		enSE_CURSOL_MOVE = 0,
		enSE_ENTER,
		enSE_EXIT,

		enSE_BOMBER,

		enSE_WIN_APP,
		enSE_WIN_DISAPP
	};

	//タイトルでズンとただずむロボット君.
	std::unique_ptr< clsASSEMBLE_MODEL >	m_upRoboModel;

	//背景.
	std::unique_ptr< clsStage > m_upBack;

	std::unique_ptr< clsSprite2D > m_upLogo;

	std::unique_ptr< clsSprite2D > m_upFlash;

	//push buttonの透明度.
	float	m_fTextAlpha;
	int		m_iTextAlphaStopFrame;
	enum class encTEXT_ALPHA_MODE : UCHAR
	{
		PLUS = 0,
		NEXT_MINUS,//マイナスの準備.
		MINUS,
		NEXT_PLUS
	}	m_encTextAlphaMode;


	std::unique_ptr< clsMENU_WINDOW_TITLE_START_OR_END > m_upMenuBox;

	//メニューから受け取った情報を照合する.
	std::vector<unsigned int> m_vecuiInformationDataArray;


	//テスト用エフェクト.
	::Effekseer::Handle m_ehHibana;

};
#endif//#ifndef SCENE_TITLE_H_