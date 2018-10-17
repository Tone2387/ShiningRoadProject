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


	enum enMESSAGE_INDEX
	{
		enMESSAGE_INDEX_MISSION_FAILD = 0,
		enMESSAGE_INDEX_CONTINUE,
		enMESSAGE_INDEX_YES,
		enMESSAGE_INDEX_NO,
		enMESSAGE_INDEX_GAME_OVER
	};
	//メッセボックス.
	std::unique_ptr< clsWINDOW_BOX > m_upBox;

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
