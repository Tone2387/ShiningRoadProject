#include "SceneTakeoff.h"
#include "Stage.h"
#include "Robo.h"
#include "File.h"
#include "RoboStatusEnemy.h"

using namespace std;

//これをつけていると手動じゃないとカメラの動きが進まない.
//#define CAMERA_FREE_MOVE_


namespace
{

	const string sFILE_PATH_CAMERA = "Data\\FileData\\Tahara\\TakeoffCamPos.csv";
	const string sFILE_PATH_GIGAPON= "Data\\FileData\\Tahara\\TakeoffGigaponPos.csv";


}



clsSCENE_TAKEOFF::clsSCENE_TAKEOFF( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_enCut( enCUT_START )
	,m_iCountCameraCutChange( 0 )
	,m_fMovieFrameNextArray()
{
	clsFILE File;
	assert( File.Open( sFILE_PATH_CAMERA ) );
	assert( File.GetSizeRow() >= enCUT_size );

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


	unique_ptr< clsROBO_STATUS_ENEMY > upEnemyState;
	const int iMovieEnemyRow = 0;
	upEnemyState = make_unique< clsROBO_STATUS_ENEMY >( iMovieEnemyRow );

	m_upEnemy = make_unique< clsRobo >();
	m_upEnemy->RoboInit( m_wpPtrGroup, upEnemyState.get() );
	m_upEnemy->SetRotation( { 0.0f, static_cast<float>( D3DX_PI ), 0.0f } );

	SetGigaponPosFromFile( m_iCountCameraCutChange );
}


void clsSCENE_TAKEOFF::UpdateProduct( enSCENE &enNextScene )
{
#ifndef CAMERA_FREE_MOVE_
	m_fMovieFrame ++;
#endif//CAMERA_FREE_MOVE_

	if( isPressEnter() ){
		m_fMovieFrame = m_fMovieFrameNextArray[ m_iCountCameraCutChange ];
	}

	if( m_fMovieFrame >= m_fMovieFrameNextArray[ m_iCountCameraCutChange ] ){
		NextCut( &m_enCut );
		SetCamPosFromFile(	 ++m_iCountCameraCutChange );
		SetGigaponPosFromFile( m_iCountCameraCutChange );
		m_fMovieFrame = 0;
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
	assert( File.Open( sFILE_PATH_CAMERA ) );
	assert( File.GetSizeRow() > iFileRow );

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

//指定した行のファイルデータをギガポンたちに与える.
void clsSCENE_TAKEOFF::SetGigaponPosFromFile( const int iFileRow )
{
	clsFILE File;
	assert( File.Open( sFILE_PATH_GIGAPON ) );
	assert( File.GetSizeRow() > iFileRow );

	D3DXVECTOR3 vPos;
	int iColIndex = 0;

	vPos.x = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.y = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.z = File.GetDataFloat( iFileRow, iColIndex++ ), 
	m_upPlayer->SetPosition( vPos );

	vPos.x = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.y = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.z = File.GetDataFloat( iFileRow, iColIndex++ ), 
	m_upEnemy->SetPosition( vPos );

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
