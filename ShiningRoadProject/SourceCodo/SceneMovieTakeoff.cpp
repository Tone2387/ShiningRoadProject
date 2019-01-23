#include "SceneMovieTakeoff.h"
#include "Stage.h"
#include "RoboTakeoff.h"
#include "File.h"
#include "RoboStatusEnemy.h"
#include "CameraTakeoff.h"

using namespace std;

//これをつけていると手動じゃないとカメラの動きが進まない.
//#define CAMERA_FREE_MOVE_


namespace
{
	const string sFILE_PATH_CUT_FRAME	= "Data\\FileData\\Tahara\\TakeoffCutFrame.csv";
	const string sFILE_PATH_CAMERA		= "Data\\FileData\\Tahara\\TakeoffCamPos.csv";
	const string sFILE_PATH_GIGAPON		= "Data\\FileData\\Tahara\\TakeoffGigaponPos.csv";
	const string sFILE_PATH_OTHER		= "Data\\FileData\\Tahara\\TakeoffOtherData.csv";

	const D3DXVECTOR4 vSTAGE_COLOR_RED = { 1.0f, 0.0f, 0.0f, 1.0f };
	const D3DXVECTOR4 vSTAGE_COLOR_BLUE= { 0.0f, 1.0f, 1.0f, 1.0f };
}



clsSCENE_MOVIE_TAKEOFF::clsSCENE_MOVIE_TAKEOFF( clsPOINTER_GROUP* const ptrGroup ) 
	:clsSCENE_MOVIE_BASE( ptrGroup, enSCENE::MISSION, sFILE_PATH_CUT_FRAME.c_str() )
{
}

clsSCENE_MOVIE_TAKEOFF::~clsSCENE_MOVIE_TAKEOFF()
{
}




void clsSCENE_MOVIE_TAKEOFF::CreateProduct()
{
//	m_wpFont->Create( sFONT_TEXT_PATH_GAME_OVER );



	m_upStage = make_unique< clsStage >( m_wpPtrGroup );


	m_upPlayer= make_unique< clsROBO_TAKEOFF >();
	m_upPlayer->RoboInit( m_wpPtrGroup, m_wpRoboStatus );
	m_upPlayer->ActStart();


	unique_ptr< clsROBO_STATUS_ENEMY > upEnemyState;
	const int iMovieEnemyRow = 0;
	upEnemyState = make_unique< clsROBO_STATUS_ENEMY >( iMovieEnemyRow );

	m_upEnemy = make_unique< clsROBO_TAKEOFF >();
	m_upEnemy->RoboInit( m_wpPtrGroup, upEnemyState.get() );
	m_upEnemy->ActStart();

	//向かい合う.
	m_upPlayer->SetRotation( { 0.0f, static_cast<float>( M_PI_2			), 0.0f } );
	m_upEnemy->SetRotation(  { 0.0f, static_cast<float>( M_PI_2 + M_PI  ), 0.0f } );

	SetCamPosFromFile(	   0 );
	SetGigaponPosFromFile( 0 );
	SetOtherDataFromFile(  0 );

	if( m_wpSound ){
		m_wpSound->PlayBGM( 0 );
	}

	clsSCENE_MOVIE_BASE::CreateProduct();
}


void clsSCENE_MOVIE_TAKEOFF::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	if( m_upStage ){	m_upStage-> Render( m_mView, m_mProj, m_vLight, vCamPos ); }
	if( m_upPlayer ){	m_upPlayer->Render( m_mView, m_mProj, m_vLight, vCamPos ); }
	if( m_upEnemy ){	m_upEnemy-> Render( m_mView, m_mProj, m_vLight, vCamPos ); }
}

void clsSCENE_MOVIE_TAKEOFF::RenderUi()
{

}


void clsSCENE_MOVIE_TAKEOFF::InitMovieProduct()
{
	clsCAMERA_TAKEOFF* wpCam = static_cast<clsCAMERA_TAKEOFF*>( m_wpCamera );

	SetCamPosFromFile( m_iCut );
	SetGigaponPosFromFile( m_iCut );
	SetOtherDataFromFile( m_iCut );

	switch( m_iCut )
	{
	case clsSCENE_MOVIE_TAKEOFF::enCUT_START:
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_RED_1:
		m_upStage->SetColor( vSTAGE_COLOR_BLUE );
		m_upStage->SetColorTarget( vSTAGE_COLOR_RED );
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_RED_2:
		m_upStage->SetColor( vSTAGE_COLOR_BLUE );
		m_upStage->SetColorTarget( vSTAGE_COLOR_RED );
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_RED_3:
		m_upStage->SetColor( vSTAGE_COLOR_BLUE );
		m_upStage->SetColorTarget( vSTAGE_COLOR_RED );
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_LIA_OPEN:
		m_upStage->SetAnimDoor( clsStage::enDOOR_Lia, clsStage::enDOOR_ANIM_OPENING );
		m_upEnemy->Boost();
		m_upPlayer->Boost();
		m_wpSound->PlaySE( enSE_DOOR_OPEN );
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_ENEMY_APP:
		{
			const int iARM_ANIM_INDEX = 0;
			m_upEnemy->m_pMesh->SetPartsAnimNo( enPARTS::ARM_L, iARM_ANIM_INDEX );
			m_upEnemy->m_pMesh->SetPartsAnimNo( enPARTS::ARM_R, iARM_ANIM_INDEX );

			m_wpSound->PlaySE( enSE_BOOSTER, true );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_ROAD:
		m_wpSound->StopSE( enSE_BOOSTER );
		m_wpSound->PlaySE( enSE_ENVIRONMENTAL, true );
		m_wpSound->PlaySE( enSE_PASS );
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_PLAYER_UP:
		m_wpSound->PlaySE( enSE_BOOSTER, true );
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_PLAYER_ROAD:
		m_wpSound->PlaySE( enSE_BOOSTER, true );
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_PLAYER_APP:
		m_upEnemy->Boost();
		m_wpSound->StopSE( enSE_ENVIRONMENTAL );
//		m_wpSound->PlaySE( enSE_PASS );
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_ENCOUNT:
		{
			m_upPlayer->Boost();
			m_upStage->SetAnimDoor( clsStage::enDOOR_DOOR_1, clsStage::enDOOR_ANIM_CLOSING );
			m_wpSound->PlaySE( enSE_DOOR_CLOSE );
			m_wpSound->PlaySE( enSE_BOOSTER, true );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_ENEMY_LANDING:
		m_upEnemy->Boost();
		m_wpSound->PlaySE( enSE_BOOSTER, true );
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_VS:
		{
			const int iINDEX = 1;
			const float fDISTANCE = m_vecfOtherData[ iINDEX ];
			D3DXVECTOR3 vHeadPos = m_upEnemy->m_pMesh->GetBonePosHeadCenter();

			wpCam->SetLookPos( vHeadPos );
			wpCam->SetPos( vHeadPos + D3DXVECTOR3( fDISTANCE, 0.0f, 0.0f ), false );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_END:
		{
			const int iINDEX = 1;
			const float fDISTANCE = m_vecfOtherData[ iINDEX ];
			D3DXVECTOR3 vHeadPos = m_upPlayer->m_pMesh->GetBonePosHeadCenter();

			wpCam->SetLookPos( vHeadPos );
			wpCam->SetPos( vHeadPos + D3DXVECTOR3( fDISTANCE, 0.0f, 0.0f ), false );
			m_wpSound->PlaySE( enSE_BOOSTER, true );
		}
		break;

	}

}


void clsSCENE_MOVIE_TAKEOFF::UpdateMovieProduct( int iOtherDataIndex )
{
	clsCAMERA_TAKEOFF* wpCam = static_cast<clsCAMERA_TAKEOFF*>( m_wpCamera );

	D3DXVECTOR3 vPosPlayerOld = m_upPlayer->GetPosition();
	D3DXVECTOR3 vPosEnemyOld = m_upEnemy->GetPosition();

	m_upPlayer->Update( m_upStage.get() );
//	m_upPlayer->ModelUpdate();
	m_upEnemy->Update( m_upStage.get() );
//	m_upEnemy->ModelUpdate();


	switch( m_iCut )
	{
	case clsSCENE_MOVIE_TAKEOFF::enCUT_START:
		{
			const float fADD = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->Spn( fADD );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_RED_1:
		{
			const float fDISTANCE = m_vecfOtherData[ iOtherDataIndex++ ];
			const float fSE_FRAME = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->CrabWalk( fDISTANCE );

			if( !m_isTrigger &&
				m_fMovieFrame >= fSE_FRAME )
			{
				m_wpSound->PlaySE( enSE_SIREN, true );
				m_isTrigger = true;
			}
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_RED_2:
		{
			const float fADD_Y = m_vecfOtherData[ iOtherDataIndex++ ];
			const float fADD_Z = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->AddPos( { 0.0f, fADD_Y, fADD_Z } );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_RED_3:
		{
			const float fADD_Y = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->AddLookPos( { 0.0f, fADD_Y, 0.0f } );

			const float fNEXT_CUT_CAM_LOOK_POS_Y = m_vecfOtherData[ iOtherDataIndex++ ];
			if( wpCam->GetLookPos().y >= fNEXT_CUT_CAM_LOOK_POS_Y ){
				NextCut();
			}
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_LIA_OPEN:
		{
			const float fUP = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->AddPos( { 0.0f, fUP, 0.0f }, false );
			const float fMOVE = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->AddDistance( fMOVE, true );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_ENEMY_APP:
		{
			const float fSPN = m_vecfOtherData[ iOtherDataIndex++ ];
			const float fDOWN_ENEMY = m_vecfOtherData[ iOtherDataIndex++ ];
			const float fDOWN_CAM = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->Spn( fSPN );
			wpCam->AddPos( { 0.0f, fDOWN_CAM, 0.0f } );

			m_upEnemy->SetPosition(
				vPosEnemyOld +
				D3DXVECTOR3( 0.0f, fDOWN_ENEMY, 0.0f ) );
			const float fMOVE = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->AddDistance( fMOVE, true );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_ROAD:
		{
			const float fPLAYER_SPEED_GO_ROAD = m_vecfOtherData[ iOtherDataIndex++ ];//通路を進んでいるとき.
			m_upPlayer->SetPosition(
				vPosPlayerOld +
				D3DXVECTOR3( fPLAYER_SPEED_GO_ROAD, 0.0f, 0.0f ) );

			m_upPlayer->IgnitionCoreBoost( true );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_PLAYER_UP:
		{
			const float fPLAYER_SPEED_GO_ROAD = m_vecfOtherData[ iOtherDataIndex++ ];//通路を進んでいるとき.
			m_upPlayer->SetPosition(
				vPosPlayerOld +
				D3DXVECTOR3( fPLAYER_SPEED_GO_ROAD, 0.0f, 0.0f ) );
			wpCam->AddPos( D3DXVECTOR3( fPLAYER_SPEED_GO_ROAD, 0.0f, 0.0f ) );
		
			const float fSPN = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->Spn( fSPN );
			const float fMOVE = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->AddDistance( fMOVE, true );

			const int iANIM_BOOST_NO = 7;
			m_upPlayer->m_pMesh->SetPartsAnimNo( enPARTS::LEG, iANIM_BOOST_NO );
			m_upPlayer->IgnitionCoreBoost( true );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_PLAYER_ROAD:
		{
			const float fPLAYER_SPEED_GO_ROAD = m_vecfOtherData[ iOtherDataIndex++ ];//通路を進んでいるとき.
			m_upPlayer->SetPosition(
				vPosPlayerOld +
				D3DXVECTOR3( fPLAYER_SPEED_GO_ROAD, 0.0f, 0.0f ) );
			m_upPlayer->IgnitionCoreBoost( true );
	
			wpCam->AddPos( D3DXVECTOR3( fPLAYER_SPEED_GO_ROAD, 0.0f, 0.0f ) );
			const float fMOVE = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->AddDistance( fMOVE, true );

			const float fDOOR_OPEN_FRAME = m_vecfOtherData[ iOtherDataIndex++ ];
			if( m_fMovieFrame >= fDOOR_OPEN_FRAME &&
				!m_isTrigger )
			{
				m_isTrigger = true;
				m_upStage->SetAnimDoor( clsStage::enDOOR_DOOR_1, clsStage::enDOOR_ANIM_OPENING );
				m_wpSound->PlaySE( enSE_DOOR_OPEN );
			}

			const float fSE_FRAME = m_vecfOtherData[ iOtherDataIndex++ ];
			if( m_fMovieFrame > fSE_FRAME )
			{
				if( m_wpSound->IsStoppedSE( enSE_PASS ) ){  
					m_wpSound->PlaySE( enSE_PASS );
					m_wpSound->StopSE( enSE_BOOSTER );
				}
			}
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_PLAYER_APP:
		{
			const float fPLAYER_SPEED_DOOR_APP = m_vecfOtherData[ iOtherDataIndex++ ];//ドアから現れるとき.
			m_upPlayer->SetPosition(
				vPosPlayerOld +
				D3DXVECTOR3( fPLAYER_SPEED_DOOR_APP, 0.0f, 0.0f ) );
			
			m_upPlayer->IgnitionCoreBoost( true );

			//プレイヤーの移動量.
			float fPLAYER_MOVE_X = m_upPlayer->GetPosition().x - vPosPlayerOld.x;

			wpCam->SetLookPos( {
				wpCam->GetLookPos().x + fPLAYER_MOVE_X,
				wpCam->GetLookPos().y,
				wpCam->GetLookPos().z } );

			//敵落下.
			const float fDOWN = m_vecfOtherData[ iOtherDataIndex++ ];
			m_upEnemy->SetPosition(
				vPosEnemyOld +
				D3DXVECTOR3( 0.0f, fDOWN, 0.0f ) );

		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_ENCOUNT:
		{
			const float fDOWN = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->AddPos( { 0.0f, fDOWN, 0.0f } );

			m_upEnemy->SetPosition(
				vPosEnemyOld +
				D3DXVECTOR3( 0.0f, fDOWN, 0.0f ) );

			const float fPLAYER_SPD = m_vecfOtherData[ iOtherDataIndex++ ];
			m_upPlayer->SetPosition(
				vPosPlayerOld +
				D3DXVECTOR3( fPLAYER_SPD, 0.0f, 0.0f ) );

			const float fANIM_FRAME = m_vecfOtherData[ iOtherDataIndex++ ];
			if( !m_isTrigger && m_fMovieFrame > fANIM_FRAME ){
				const int iARM_ANIM_INDEX = 1;
				m_upEnemy->m_pMesh->SetPartsAnimNo( enPARTS::ARM_L, iARM_ANIM_INDEX );
				m_upEnemy->m_pMesh->SetPartsAnimNo( enPARTS::ARM_R, iARM_ANIM_INDEX );
				m_wpSound->PlaySE( enSE_FIGHT_LEADY );
				m_isTrigger = true;
			}
			m_upPlayer->IgnitionCoreBoost( true );
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_ENEMY_LANDING:
		{
			const float fDOWN = m_vecfOtherData[ iOtherDataIndex++ ];
			const float fLANDING = m_vecfOtherData[ iOtherDataIndex++ ];

			//着地.
			if( vPosEnemyOld.y <= fLANDING ){
				m_upEnemy->SetPosition( 
					D3DXVECTOR3(
						vPosEnemyOld.x,
						fLANDING,
						vPosEnemyOld.z ) );

				if( !m_isTrigger ){
					m_upEnemy->Walk();
					const int iANIM_INDEX = m_vecfOtherData[ iOtherDataIndex++ ];
					m_upEnemy->m_pMesh->SetPartsAnimNo( enPARTS::LEG, iANIM_INDEX );
					m_upEnemy->m_pMesh->SetPartsAnimSpeed( enPARTS::LEG, g_dAnimSpeedReference / 4.0 );
					m_wpSound->PlaySE( enSE_LANDING );
					m_wpSound->StopSE( enSE_BOOSTER );
					m_isTrigger = true;
				}
			}
			//下降.
			else{
				m_upEnemy->SetPosition(
					vPosEnemyOld +
					D3DXVECTOR3( 0.0f, fDOWN, 0.0f ) );
			}
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_VS:
		{
			const float fDistance = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->CrabWalk( fDistance );
			
		}
		break;

	case clsSCENE_MOVIE_TAKEOFF::enCUT_END:
		{
			const float fDistance = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->CrabWalk( fDistance );

			iOtherDataIndex++;
			//プレーヤーは前進する.
			const float fPLAYER_SPEED = m_vecfOtherData[ iOtherDataIndex++ ];
			m_upPlayer->SetPosition(
				vPosPlayerOld +
				D3DXVECTOR3( fPLAYER_SPEED, 0.0f, 0.0f ) );
			wpCam->AddPos( D3DXVECTOR3( fPLAYER_SPEED, 0.0f, 0.0f ) );

			m_upPlayer->IgnitionCoreBoost( true );
		}
		break;
	}



	const float fSPN = 0.00625f;
	const float fMOVE = 0.25f;
	if( GetAsyncKeyState( 'A' ) & 0x8000 )wpCam->Spn( fSPN );
	if( GetAsyncKeyState( 'Z' ) & 0x8000 )wpCam->Spn(-fSPN );
	if( GetAsyncKeyState( 'S' ) & 0x8000 )wpCam->Turn( fSPN );
	if( GetAsyncKeyState( 'X' ) & 0x8000 )wpCam->Turn(-fSPN );
	if( GetAsyncKeyState( 'D' ) & 0x8000 )wpCam->Advancing( fMOVE );
	if( GetAsyncKeyState( 'C' ) & 0x8000 )wpCam->Advancing(-fMOVE );
	if( GetAsyncKeyState( 'F' ) & 0x8000 )wpCam->CrabWalk( fMOVE );
	if( GetAsyncKeyState( 'V' ) & 0x8000 )wpCam->CrabWalk(-fMOVE );
	if( GetAsyncKeyState( 'G' ) & 0x8000 )wpCam->SetDistance( fMOVE * 10.0f, true );
	if( GetAsyncKeyState( 'B' ) & 0x8000 )wpCam->SetDistance(-fMOVE * 10.0f, true );
	if( GetAsyncKeyState( 'H' ) & 0x8000 )wpCam->AddDistance( fMOVE, true );
	if( GetAsyncKeyState( 'N' ) & 0x8000 )wpCam->AddDistance(-fMOVE, true );


}

//指定した行のファイルデータをカメラに与える.
void clsSCENE_MOVIE_TAKEOFF::SetCamPosFromFile( const int iFileRow )
{
	clsCAMERA_TAKEOFF* wpCam = static_cast<clsCAMERA_TAKEOFF*>( m_wpCamera );

	clsFILE File;
	assert( File.Open( sFILE_PATH_CAMERA ) );
	if( File.GetSizeRow() <= iFileRow )	{ return; }

	D3DXVECTOR3 vPos;
	int iColIndex = 0;

	vPos.x = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.y = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.z = File.GetDataFloat( iFileRow, iColIndex++ ), 
	wpCam->SetPos( vPos );

	vPos.x = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.y = File.GetDataFloat( iFileRow, iColIndex++ ), 
	vPos.z = File.GetDataFloat( iFileRow, iColIndex++ ), 
	wpCam->SetLookPos( vPos );
}

//指定した行のファイルデータをギガポンたちに与える.
void clsSCENE_MOVIE_TAKEOFF::SetGigaponPosFromFile( const int iFileRow )
{
	clsFILE File;
	assert( File.Open( sFILE_PATH_GIGAPON ) );
	if( File.GetSizeRow() <= iFileRow )	{ return; }

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


void clsSCENE_MOVIE_TAKEOFF::SetOtherDataFromFile( const int iFileRow )
{
	clsFILE File;
	assert( File.Open( sFILE_PATH_OTHER ) );
	if( File.GetSizeRow() <= iFileRow )	{ return; }

	m_vecfOtherData.resize( File.GetSizeCol( iFileRow ) );
	for( unsigned int i=0; i<m_vecfOtherData.size(); i++ ){
		m_vecfOtherData[i] = File.GetDataFloat( iFileRow, i );
	}

}



//============================ デバッグテキスト ===========================//
#ifdef _DEBUG
void clsSCENE_MOVIE_TAKEOFF::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.

	sprintf_s( strDbgTxt, 
		"CameraPos : x[%f], y[%f], z[%f]",
		m_wpCamera->GetPos().x, m_wpCamera->GetPos().y, m_wpCamera->GetPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamLokPos : x[%f], y[%f], z[%f]",
		m_wpCamera->GetLookPos().x, m_wpCamera->GetLookPos().y, m_wpCamera->GetLookPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	iTxtY += iOFFSET;

	sprintf_s( strDbgTxt, 
		"PlayerPos : x[%f], y[%f], z[%f]",
		m_upPlayer->GetPosition().x, m_upPlayer->GetPosition().y, m_upPlayer->GetPosition().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"EnemyPos : x[%f], y[%f], z[%f]",
		m_upEnemy->GetPosition().x, m_upEnemy->GetPosition().y, m_upEnemy->GetPosition().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	iTxtY += iOFFSET;

	sprintf_s( strDbgTxt, 
		"Frame : [%f]",
		m_fMovieFrame );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );


	iTxtY += iOFFSET;

	switch( m_iCut )
	{
	case clsSCENE_MOVIE_TAKEOFF::enCUT_START:
		sprintf_s( strDbgTxt,"Cut : enCUT_START" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_RED_1:
		sprintf_s( strDbgTxt,"Cut : enCUT_RED_1" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_RED_2:
		sprintf_s( strDbgTxt,"Cut : enCUT_RED_2" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_RED_3:
		sprintf_s( strDbgTxt,"Cut : enCUT_RED_3" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_LIA_OPEN:
		sprintf_s( strDbgTxt,"Cut : enCUT_LIA_OPEN" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_ENEMY_APP:
		sprintf_s( strDbgTxt,"Cut : enCUT_ENEMY_APP" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_ROAD:
		sprintf_s( strDbgTxt,"Cut : enCUT_ROAD" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_PLAYER_UP:
		sprintf_s( strDbgTxt,"Cut : enCUT_PLAYER_UP" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_PLAYER_ROAD:
		sprintf_s( strDbgTxt,"Cut : enCUT_PLAYER_ROAD" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_PLAYER_APP:
		sprintf_s( strDbgTxt,"Cut : enCUT_PLAYER_APP" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_ENCOUNT:
		sprintf_s( strDbgTxt,"Cut : enCUT_ENCOUNT" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_ENEMY_LANDING:
		sprintf_s( strDbgTxt,"Cut : enCUT_ENEMY_LANDING" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_VS:
		sprintf_s( strDbgTxt,"Cut : enCUT_VS" );
		break;
	case clsSCENE_MOVIE_TAKEOFF::enCUT_END:
		sprintf_s( strDbgTxt,"Cut : enCUT_END" );
		break;
	}
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

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


	SCENE_CREATE_COUNT;
}
#endif //#ifdef _DEBUG
