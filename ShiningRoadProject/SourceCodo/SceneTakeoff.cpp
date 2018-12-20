#include "SceneTakeoff.h"
#include "Stage.h"
#include "Robo.h"
#include "File.h"

using namespace std;

//これをつけていると手動じゃないとカメラの動きが進まない.
#define CAMERA_FREE_MOVE_


namespace
{

	const string sFILE_PATH = "Data\\FileData\\Tahara\\TakeoffCamPos.csv";


}



clsSCENE_TAKEOFF::clsSCENE_TAKEOFF( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_enCut( enCUT_START )
	,m_iCountCameraCutChange( 0 )
	,m_fMovieFrameNextArray()
{
	clsFILE File;
	assert( File.Open( sFILE_PATH ) );
	assert( File.GetSizeRow() > enCUT_size );

	//カットのフレーム数のindex.
	const float fCOL_INDEX = File.GetSizeCol() - 1;


	for( int i=0; i<enCUT_size; i++ ){
		m_fMovieFrameNextArray[i] = File.GetDataInt( i, fCOL_INDEX );
	}
}

clsSCENE_TAKEOFF::~clsSCENE_TAKEOFF()
{
}




void clsSCENE_TAKEOFF::CreateProduct()
{
//	m_wpSound->PlayBGM( enBGM_AMACHA0 );
	
//	m_wpFont->Create( sFONT_TEXT_PATH_GAME_OVER );

	SetCamPosFromFile( m_iCountCameraCutChange );

	m_upStage = make_unique< clsStage >( m_wpPtrGroup );

	m_upPlayer= make_unique< clsRobo >();
	m_upPlayer->RoboInit( m_wpPtrGroup, m_wpRoboStatus );



	m_upEnemy = make_unique< clsRobo >();
	m_upEnemy->RoboInit( m_wpPtrGroup, m_wpRoboStatus );
}


void clsSCENE_TAKEOFF::UpdateProduct( enSCENE &enNextScene )
{
#ifndef CAMERA_FREE_MOVE_
	g_fMovieFrame ++;
#endif//CAMERA_FREE_MOVE_

	if( isPressEnter() ){
		m_fMovieFrame = m_fMovieFrameNextArray[ m_iCountCameraCutChange ];
	}

	if( m_fMovieFrame >= m_fMovieFrameNextArray[ m_iCountCameraCutChange ] ){
		NextCut( &m_enCut );
		SetCamPosFromFile( ++m_iCountCameraCutChange );
	}

	if( m_enCut == enCUT_END ){
		enNextScene = enSCENE::MISSION;
	}
}

void clsSCENE_TAKEOFF::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	if( m_upStage ){	m_upStage-> Render( m_mView, m_mProj, m_vLight, vCamPos ); }
	if( m_upPlayer ){	m_upPlayer->Render( m_mView, m_mProj, m_vLight, vCamPos ); }
	if( m_upEnemy ){	m_upEnemy-> Render( m_mView, m_mProj, m_vLight, vCamPos ); }
}

void clsSCENE_TAKEOFF::RenderUi()
{

}


//指定した行のファイルデータをカメラに与える.
void clsSCENE_TAKEOFF::SetCamPosFromFile( const int iFileRow )
{
	clsFILE File;
	assert( File.Open( sFILE_PATH ) );

	D3DXVECTOR3 vPos;
	int iColIndex = 0;

	vPos.x = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.y = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.z = File.GetDataFloat( iFileRow, iColIndex++ ), 
	m_wpCamera->SetPos( vPos );

	vPos.x = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.y = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.z = File.GetDataFloat( iFileRow, iColIndex++ ), 
	m_wpCamera->SetLookPos( vPos );
}

void clsSCENE_TAKEOFF::NextCut( enCUT* const penCut )
{
	*penCut = static_cast<enCUT>( static_cast<int>( *penCut ) + 1 );
}


//============================ デバッグテキスト ===========================//
#ifdef _DEBUG
void clsSCENE_TAKEOFF::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.

//	sprintf_s( strDbgTxt, 
//		"CameraPos : x[%f], y[%f], z[%f]",
//		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"CamLokPos : x[%f], y[%f], z[%f]",
//		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

//	sprintf_s( strDbgTxt, 
//		"MisModelPos : x[%f], y[%f], z[%f]",
//		m_upMissModel->GetPos().x, m_upMissModel->GetPos().y, m_upMissModel->GetPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"MisModelBoneNum : L[%f], C[%f], H[%f], A[%f]",
//		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_LEG ), 
//		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_CORE ), 
//		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_HEAD ), 
//		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_ARMS ) );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );


}
#endif //#ifdef _DEBUG
