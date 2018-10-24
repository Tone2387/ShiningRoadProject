#pragma once

#include "SceneBase.h"
#include "CameraGameOver.h"
#include "MissionModel.h"
#include "Sprite2DCenter.h"
class clsMENU_WINDOW_GAME_OVER_CONTINUE;

//================================//
//========== ゲームオーバークラス ==========//
//================================//
class clsSCENE_GAME_OVER : public clsSCENE_BASE
{
public:
	clsSCENE_GAME_OVER( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_GAME_OVER();


private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//「 UIの 」Render.

	//メニューの動き.
	void MenuUpdate( enSCENE &enNextScene );



	struct MESSAGE_TEXT_STATE
	{
		D3DXVECTOR3 vPos;
		float fScale;

		MESSAGE_TEXT_STATE()
		:vPos( { 0.0f, 0.0f, 0.0f } )
		,fScale( 0.0f )
		{}
	};

	enum enMESSAGE_INDEX : int
	{
		enMESSAGE_INDEX_MISSION_FAILD = 0,
		enMESSAGE_INDEX_GAME_OVER,
		enMESSAGE_INDEX_NEVER_GIVE_UP,

		enMESSAGE_INDEX_size
	};
	MESSAGE_TEXT_STATE m_TextStateArray[ enMESSAGE_INDEX_size ];
	enMESSAGE_INDEX m_enTextRenderIndex;







	//メッセボックス.
	std::unique_ptr< clsMENU_WINDOW_GAME_OVER_CONTINUE > m_upMenu;

	//メニューから受け取った情報を照合するindex.
	enum enINFORMATION : char
	{
		enINFORMATION_GAME_OVER = 0,
		enINFORMATION_CONTINUE,
		enINFORMATION_ASSEMBLE,

		enINFORMATION_size
	};
	unsigned int m_uiInformationDataArray[ enINFORMATION_size ];




	//黒背景.
	std::unique_ptr< clsSprite2D > m_upBlackBack;


	//音の引数.
	enum enBGM : int
	{
		enBGM_MAOU2 = 0,
		enBGM_MUS0,
		enBGM_POKET0,
		enBGM_POKET1
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


#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG


};
