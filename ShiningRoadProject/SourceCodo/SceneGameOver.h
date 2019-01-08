#ifndef SCENE_GAME_OVER_H_
#define SCENE_GAME_OVER_H_


class clsMENU_WINDOW_GAME_OVER_CONTINUE;
#include "SceneBase.h"
#include "CameraGameOver.h"
#include "Sprite2DCenter.h"

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

#ifdef _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:

	//音の引数.
	enum enBGM : int
	{
		enBGM_AMACHA0 = 0,
		enBGM_HART2,
		enBGM_NOVA1,
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
	//メニューから帰ってくる情報との照合用.
	std::vector<unsigned int> m_vecuiInformationDataArray;


	//黒背景.
	std::unique_ptr< clsSprite2D > m_upBlackBack;


};
#endif//#ifndef SCENE_GAME_OVER_H_