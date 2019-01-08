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
	//指定した行のファイルデータをギガポンたちに与える.
	void SetGigaponPosFromFile( const int iFileRow );

	void SetOtherDataFromFile( const int iFileRow );

	//カメラのカット割りの段階.
	enum enCUT : int
	{
//		enCUT_START = 0,//通路を通る主人公.
//		enCUT_GO_YOU,	//広場に出た主人公.
//		enCUT_LOOK_UP,	//天井を見上げる.
//		enCUT_ENEMY_APP,//天井から敵登場.
//		enCUT_ENEMY_LANDING,//敵着地.

		enCUT_START = 0,//エリア全体を写す.
		enCUT_RED_1,	//ステージが赤くなる.
		enCUT_RED_2,		
		enCUT_RED_3,	//( 天井を写す ).
		enCUT_LIA_OPEN,	//天井が開く.
		enCUT_ENEMY_APP,//天井から敵登場.
		enCUT_PLAYER_UP,//通路を行く自機のアップ( 正面 ).
		enCUT_PLAYER_ROAD,//通路を行く自機( 背面 ).
		enCUT_PLAYER_APP,//広場に現れる自機.
		enCUT_ENCOUNT,	//そして出会う二機のギガポン.
		enCUT_ENEMY_LANDING,//敵着地.
		enCUT_VS,		//自機の顔のアップ.
		enCUT_END,		//敵の顔のアップ.

		enCUT_size
	};
	//カット変数を更新.
	void AddCut( enCUT* const penCut );

	//ムービーっぽく動かすための為の関数.
	void InitMovie();
	void UpdateMovie();

	//フレームが満たしていなくても次のカットへ飛ぶ.
	void NextCut();

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
	float m_fMovieFrame;
	//全カットのフレーム数.
	float m_fMovieFrameNextArray[ enCUT_size ];


	std::unique_ptr< clsStage >	m_upStage;
	std::unique_ptr< clsRobo >	m_upPlayer;
	std::unique_ptr< clsRobo >	m_upEnemy;

	//カメラの移動速度など.
	std::vector< float > m_vecfOtherData;

	//一凱桐乃各カット一回きりのフラグトリガー.
	bool m_isTrigger;

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