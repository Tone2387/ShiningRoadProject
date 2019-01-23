#ifndef SCENE_TAKEOFF_H_
#define SCENE_TAKEOFF_H_

class clsStage;
class clsROBO_TAKEOFF;

#include "SceneMovieBase.h"

//#include "Stage.h"


//================================//
//========== エンディングクラス ==========//
//================================//
class clsSCENE_MOVIE_TAKEOFF : public clsSCENE_MOVIE_BASE
{
public:
	clsSCENE_MOVIE_TAKEOFF( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_MOVIE_TAKEOFF();


private:
	void CreateProduct() final;
	//継承元のものを使う.
//	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//「 UIの 」Render.

	void InitMovieProduct() final;
	void UpdateMovieProduct( int iOtherDataIndex ) final;

	//指定した行のファイルデータをカメラに与える.
	void SetCamPosFromFile( const int iFileRow );
	//指定した行のファイルデータをギガポンたちに与える.
	void SetGigaponPosFromFile( const int iFileRow );

	void SetOtherDataFromFile( const int iFileRow );

	//カメラのカット割りの段階.
	enum enCUT : int
	{
		enCUT_START = 0,//エリア全体を写す.
		enCUT_RED_1,	//ステージが赤くなる.
		enCUT_RED_2,		
		enCUT_RED_3,	//( 天井を写す ).
		enCUT_LIA_OPEN,	//天井が開く.
		enCUT_ENEMY_APP,//天井から敵登場.
		enCUT_ROAD,		//廊下を自機が通り過ぎる.
		enCUT_PLAYER_UP,//通路を行く自機のアップ( 正面 ).
		enCUT_PLAYER_ROAD,//通路を行く自機( 背面 ).
		enCUT_PLAYER_APP,//広場に現れる自機.
		enCUT_ENCOUNT,	//そして出会う二機のギガポン.
		enCUT_ENEMY_LANDING,//敵着地.
		enCUT_VS,		//自機の顔のアップ.
		enCUT_END,		//敵の顔のアップ.

		enCUT_size
	};

#ifdef _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:


	std::unique_ptr< clsStage >	m_upStage;
	std::unique_ptr< clsROBO_TAKEOFF >	m_upPlayer;
	std::unique_ptr< clsROBO_TAKEOFF >	m_upEnemy;



	//音の引数.
	enum enBGM : int
	{
//		enBGM_MAFIA0 = 0,
	};

	enum enSE : int
	{
		enSE_SIREN = 0,		//Siren.
		enSE_DOOR_OPEN,		//DoorOpen.
		enSE_DOOR_CLOSE,	//DoorClose.
		enSE_PASS,			//通り過ぎる.
		enSE_FIGHT_LEADY,	//FightLeady.
		enSE_BOOSTER,		//Booster.
		enSE_LANDING,		//Landing.
		enSE_ENVIRONMENTAL,	//環境音.
	};

};



#endif//#ifndef SCENE_TAKEOFF_H_