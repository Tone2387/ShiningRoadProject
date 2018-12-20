#ifndef SCENE_TAKEOFF_H_
#define SCENE_TAKEOFF_H_

class clsStage;
class clsRobo;

#include "SceneBase.h"
#include "CameraTakeoff.h"

//#include "Stage.h"


//================================//
//========== エンディングクラス ==========//
//================================//
class clsSCENE_TAKEOFF : public clsSCENE_BASE
{
public:
	clsSCENE_TAKEOFF( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_TAKEOFF();


private:
	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//「 UIの 」Render.

	//指定した行のファイルデータをカメラに与える.
	void SetCamPosFromFile( const int iFileRow );

	//カメラのカット割りの段階.
	enum enCUT : int
	{
		enCUT_START = 0,
			
		enCUT_END,//ここに来た瞬間にミッションシーンに飛ぶ.

		enCUT_size
	};
	void NextCut( enCUT* const penCut );


#ifdef _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:

	//カット割り.
	enCUT m_enCut;
	//.
	int m_iCountCameraCutChange;

	//今のカットのフレームカウント.
	float m_fMovieFrame = 0;
	//全カットのフレーム数.
	float m_fMovieFrameNextArray[ enCUT_size ];


	std::unique_ptr< clsStage >	m_upStage;
	std::unique_ptr< clsRobo >	m_upPlayer;
	std::unique_ptr< clsRobo >	m_upEnemy;

	//音の引数.
	enum enBGM : int
	{
//		enBGM_MAFIA0 = 0,
	};

	enum enSE : int
	{
//		enSE_CURSOL_MOVE = 0,
	};

};


#endif//#ifndef SCENE_TAKEOFF_H_