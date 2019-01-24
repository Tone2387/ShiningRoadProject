#ifndef SCENE_CLEAR_H_
#define SCENE_CLEAR_H_

class clsStage;
class clsROBO_TAKEOFF;

#include "SceneMovieBase.h"
//#include "CameraTakeoff.h"

class clsSCENE_MOVIE_CLEAR : public clsSCENE_MOVIE_BASE
{
public:
	clsSCENE_MOVIE_CLEAR( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_MOVIE_CLEAR();

private:
	void CreateProduct() final;
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
		enCUT_START = 0,//敵よろめく.
		enCUT_END,		//敵大爆発.

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

	float m_fBomberEffectOffsetFrame;

	//音の引数.
//	enum enBGM : int
//	{
////		enBGM_MAFIA0 = 0,
//	};

	enum enSE : int
	{
		enSE_KNEE = 0,	//膝附.( ニー ).
		enSE_BOMBER,	//爆発.
		enSE_ARM_DOWN,	//武器を下す.
	};

};



#endif//#ifndef SCENE_CLEAR_H_