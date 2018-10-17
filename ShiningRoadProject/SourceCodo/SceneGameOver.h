#pragma once

#include "SceneBase.h"
#include "CameraGameOver.h"
#include "MissionModel.h"
#include "Sprite2DCenter.h"
class clsWINDOW_BOX;

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
		enMESSAGE_INDEX_CONTINUE,
		enMESSAGE_INDEX_YES,
		enMESSAGE_INDEX_NO,
		enMESSAGE_INDEX_GAME_OVER,

		enMESSAGE_INDEX_size
	};
	//メッセボックス.
	std::unique_ptr< clsWINDOW_BOX > m_upBox;
	MESSAGE_TEXT_STATE m_TextStateArray[ enMESSAGE_INDEX_size ];
	float m_fTextAlphaWhite;
	float m_fTextAlphaRed;

	bool m_bCanBoxOpen;
	bool m_isBoxOpened;

	//黒背景.
	std::unique_ptr< clsSprite2D > m_upBlackBack;



//	std::unique_ptr< clsMISSION_MODEL > m_upMissModel;
//	std::unique_ptr< clsSPRITE2D_CENTER > m_upMissionFailed;


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
	};


#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG


};
