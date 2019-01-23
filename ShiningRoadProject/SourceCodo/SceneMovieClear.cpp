#include "SceneMovieClear.h"
#include "Stage.h"
#include "RoboTakeoff.h"
#include "File.h"
#include "RoboStatusEnemy.h"
#include "CameraClear.h"
#include "Random.h"


using namespace std;



namespace
{
	const string sFILE_PATH_CUT_FRAME	= "Data\\FileData\\Tahara\\ClearCutFrame.csv";
	const string sFILE_PATH_CAMERA		= "Data\\FileData\\Tahara\\ClearCamPos.csv";
	const string sFILE_PATH_GIGAPON		= "Data\\FileData\\Tahara\\ClearGigaponPos.csv";
	const string sFILE_PATH_OTHER		= "Data\\FileData\\Tahara\\ClearOtherData.csv";

	const D3DXVECTOR4 vSTAGE_COLOR_RED = { 1.0f, 0.0f, 0.0f, 1.0f };
	const D3DXVECTOR4 vSTAGE_COLOR_BLUE= { 0.0f, 1.0f, 1.0f, 1.0f };
}


clsSCENE_MOVIE_CLEAR::clsSCENE_MOVIE_CLEAR( clsPOINTER_GROUP* const ptrGroup )
	:clsSCENE_MOVIE_BASE( ptrGroup, enSCENE::ENDING, sFILE_PATH_CUT_FRAME.c_str() )
	,m_fBomberEffectOffsetFrame( 0.0f )
{
}

clsSCENE_MOVIE_CLEAR::~clsSCENE_MOVIE_CLEAR()
{
}


void clsSCENE_MOVIE_CLEAR::CreateProduct()
{
	m_upStage = make_unique< clsStage >( m_wpPtrGroup );
	m_upStage->SetColorRed();

	m_upPlayer= make_unique< clsROBO_TAKEOFF >();
	m_upPlayer->RoboInit( m_wpPtrGroup, m_wpRoboStatus );


	unique_ptr< clsROBO_STATUS_ENEMY > upEnemyState;
	const int iMovieEnemyRow = 0;
	upEnemyState = make_unique< clsROBO_STATUS_ENEMY >( iMovieEnemyRow );

	m_upEnemy = make_unique< clsROBO_TAKEOFF >();
	m_upEnemy->RoboInit( m_wpPtrGroup, upEnemyState.get() );

	//向かい合う.
	m_upPlayer->SetRotation( { 0.0f, static_cast<float>( M_PI_2			), 0.0f } );
	m_upEnemy->SetRotation(  { 0.0f, static_cast<float>( M_PI_2 + M_PI  ), 0.0f } );

	//構える.
	const int iARM_ANIM_INDEX = 2;
	m_upPlayer->m_pMesh->SetPartsAnimNo( enPARTS::ARM_L, iARM_ANIM_INDEX );
	m_upPlayer->m_pMesh->SetPartsAnimNo( enPARTS::ARM_R, iARM_ANIM_INDEX );
	const int iARM_ANIM_ENEMY_INDEX = 4;
	m_upEnemy-> m_pMesh->SetPartsAnimNo( enPARTS::ARM_L, iARM_ANIM_ENEMY_INDEX );
	m_upEnemy-> m_pMesh->SetPartsAnimNo( enPARTS::ARM_R, iARM_ANIM_ENEMY_INDEX );
	m_wpSound->PlaySE( enSE_ARM_DOWN );


	SetCamPosFromFile(	   0 );
	SetGigaponPosFromFile( 0 );
	SetOtherDataFromFile(  0 );

//	if( m_wpSound ){
//		m_wpSound->PlayBGM( 0 );
//	}

	
	//高速明転.
	const float fDARK_SPD = fBLACK_SCREEN_DEFAULT_SPD * 2.0f;
	m_wpBlackScreen->SetChangeSpd( fDARK_SPD );


	m_wpEffects->Play( 0, m_upEnemy->GetPosition() );
	m_wpEffects->StopAll();

	clsSCENE_MOVIE_BASE::CreateProduct();
}



void clsSCENE_MOVIE_CLEAR::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	if( m_upStage ){	m_upStage-> Render( m_mView, m_mProj, m_vLight, vCamPos ); }
	if( m_upPlayer ){	m_upPlayer->Render( m_mView, m_mProj, m_vLight, vCamPos ); }
	if( m_upEnemy ){	m_upEnemy-> Render( m_mView, m_mProj, m_vLight, vCamPos ); }
}

void clsSCENE_MOVIE_CLEAR::RenderUi()
{
}



void clsSCENE_MOVIE_CLEAR::InitMovieProduct()
{
	clsCAMERA_CLEAR* wpCam = static_cast<clsCAMERA_CLEAR*>( m_wpCamera );

	SetCamPosFromFile( m_iCut );
	SetGigaponPosFromFile( m_iCut );
	SetOtherDataFromFile( m_iCut );

	switch( m_iCut )
	{
	case clsSCENE_MOVIE_CLEAR::enCUT_START:
		{
		}
		break;

	case clsSCENE_MOVIE_CLEAR::enCUT_END:
		{
			//低速暗転.
			const int iINDEX = 6;
			const float fRATE = m_vecfOtherData[ iINDEX ];
			const float fDARK_SPD = fBLACK_SCREEN_DEFAULT_SPD * fRATE;
			m_wpBlackScreen->SetChangeSpd( fDARK_SPD );
		}
		break;

	}

}


void clsSCENE_MOVIE_CLEAR::UpdateMovieProduct( int iOtherDataIndex )
{
	clsCAMERA_CLEAR* wpCam = static_cast<clsCAMERA_CLEAR*>( m_wpCamera );

	D3DXVECTOR3 vPosPlayerOld = m_upPlayer->GetPosition();
	D3DXVECTOR3 vPosEnemyOld = m_upEnemy->GetPosition();

	m_upPlayer->UpdateProduct( m_upStage.get() );
	m_upPlayer->ModelUpdate();


	switch( m_iCut )
	{
	case clsSCENE_MOVIE_CLEAR::enCUT_START:
		{
			//敵のアニメ用( 次のカットでも回してしまうと立ち上がってしまう ).
			m_upEnemy->UpdateProduct( m_upStage.get() );
			m_upEnemy->ModelUpdate();

			const float fSPN = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->Spn( fSPN );

			const float fMOVE = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->CrabWalk( fMOVE );

			//膝附モーション開始.
			const float fANIM_FRAME = m_vecfOtherData[ iOtherDataIndex++ ];
			if( !m_upEnemy->m_pMesh->GetPartsAnimNo( enPARTS::LEG ) && 
				m_fMovieFrame > fANIM_FRAME )
			{
				const int iANIM_NO = 14;
				m_upEnemy-> m_pMesh->SetPartsAnimNo( enPARTS::LEG, iANIM_NO );
			}

			//膝附SE.
			const float fSE_KNEE_FRAME = m_vecfOtherData[ iOtherDataIndex++ ];
			if( !m_isTrigger && 
				m_fMovieFrame > fSE_KNEE_FRAME )
			{
				m_isTrigger = true;
				m_wpSound->PlaySE( enSE_KNEE );
			}

			//爆発.
			const float fSE_BOMBER_FRAME = m_vecfOtherData[ iOtherDataIndex++ ];
			if( m_wpSound->IsStoppedSE( enSE_BOMBER ) && 
				m_fMovieFrame > fSE_BOMBER_FRAME )
			{
				m_isTrigger = true;
				m_wpSound->PlaySE( enSE_BOMBER );
				m_wpEffects->Play( 0, m_upEnemy->GetPosition() );
			}

		}//sv.
		break;

	case clsSCENE_MOVIE_CLEAR::enCUT_END:
		{

			++ m_fBomberEffectOffsetFrame;
			const float fSPN = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->Spn( fSPN );

			const float fMOVE = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->Advancing( fMOVE );

			const float fCLUB = m_vecfOtherData[ iOtherDataIndex++ ];
			wpCam->CrabWalk( fCLUB );

			//爆発.
			const float fBOM_OFFSET_TIME= m_vecfOtherData[ iOtherDataIndex++ ];
			const int iBOM_OFFSET_POS = static_cast<int>( m_vecfOtherData[ iOtherDataIndex++ ] );
			const float fBOM_POS_RATE = 1.0f / 100.0f;
			if( m_fBomberEffectOffsetFrame > fBOM_OFFSET_TIME ){
				m_fBomberEffectOffsetFrame = 0.0f;

				D3DXVECTOR3 vRanPos = { 0.0f, 0.0f, 0.0f };
				vRanPos.x = static_cast<float>( clsRANDOM::GetRandom( -iBOM_OFFSET_POS,				  0 ) ) * fBOM_POS_RATE;
				vRanPos.y = static_cast<float>( clsRANDOM::GetRandom( 0,				iBOM_OFFSET_POS ) ) * fBOM_POS_RATE;
				vRanPos.z = static_cast<float>( clsRANDOM::GetRandom( -iBOM_OFFSET_POS, iBOM_OFFSET_POS ) ) * fBOM_POS_RATE;
				m_wpEffects->Play( 0, m_upEnemy->GetPosition() + vRanPos );
			}

			//敵が消える.
			const float fENEMY_DELETE_TIME= m_vecfOtherData[ iOtherDataIndex++ ];
			if( m_fMovieFrame > fENEMY_DELETE_TIME ){
				m_upEnemy->SetScale( 0.0f );
			}
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
void clsSCENE_MOVIE_CLEAR::SetCamPosFromFile( const int iFileRow )
{
	clsCAMERA_CLEAR* wpCam = static_cast<clsCAMERA_CLEAR*>( m_wpCamera );

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
void clsSCENE_MOVIE_CLEAR::SetGigaponPosFromFile( const int iFileRow )
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


void clsSCENE_MOVIE_CLEAR::SetOtherDataFromFile( const int iFileRow )
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
void clsSCENE_MOVIE_CLEAR::RenderDebugText()
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
		"Frame : [%f]",
		m_fMovieFrame );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

//	sprintf_s( strDbgTxt, 
//		"PlayerPos : x[%f], y[%f], z[%f]",
//		m_upPlayer->GetPosition().x, m_upPlayer->GetPosition().y, m_upPlayer->GetPosition().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"EnemyPos : x[%f], y[%f], z[%f]",
//		m_upEnemy->GetPosition().x, m_upEnemy->GetPosition().y, m_upEnemy->GetPosition().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	iTxtY += iOFFSET;
//
//	sprintf_s( strDbgTxt, 
//		"Frame : [%f]",
//		m_fMovieFrame );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//
//	iTxtY += iOFFSET;
//
//	switch( m_iCut )
//	{
//	case clsSCENE_MOVIE_CLEAR::enCUT_START:
//		sprintf_s( strDbgTxt,"Cut : enCUT_START" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_RED_1:
//		sprintf_s( strDbgTxt,"Cut : enCUT_RED_1" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_RED_2:
//		sprintf_s( strDbgTxt,"Cut : enCUT_RED_2" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_RED_3:
//		sprintf_s( strDbgTxt,"Cut : enCUT_RED_3" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_LIA_OPEN:
//		sprintf_s( strDbgTxt,"Cut : enCUT_LIA_OPEN" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_ENEMY_APP:
//		sprintf_s( strDbgTxt,"Cut : enCUT_ENEMY_APP" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_PLAYER_UP:
//		sprintf_s( strDbgTxt,"Cut : enCUT_PLAYER_UP" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_PLAYER_ROAD:
//		sprintf_s( strDbgTxt,"Cut : enCUT_PLAYER_ROAD" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_PLAYER_APP:
//		sprintf_s( strDbgTxt,"Cut : enCUT_PLAYER_APP" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_ENCOUNT:
//		sprintf_s( strDbgTxt,"Cut : enCUT_ENCOUNT" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_ENEMY_LANDING:
//		sprintf_s( strDbgTxt,"Cut : enCUT_ENEMY_LANDING" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_VS:
//		sprintf_s( strDbgTxt,"Cut : enCUT_VS" );
//		break;
//	case clsSCENE_MOVIE_CLEAR::enCUT_END:
//		sprintf_s( strDbgTxt,"Cut : enCUT_END" );
//		break;
//	}
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
