#include "SceneAssemble.h"
//#include "DxInput.h"
#include "WindowBox.h"
#include "SceneAssembleInformation.h"

#include "MenuWindowAssembleBackTitle.h"
#include "MenuWindowAssembleTakeOff.h"
#include "MenuWindowAssembleColorChange.h"

#include "CameraAssemble.h"

#include "PartsWindowModel.h"

#include "File.h"

using namespace std;

//#include "RoboStatusEnemy.h"


namespace{

	//要素数は<clsSCENE_ASSEMBLE::ENUM_SIZE>.
	const string sPARTS_STATUS_PASS[] =
	{
		"Data\\RoboParts\\Leg\\RoboPartsData.csv",
		"Data\\RoboParts\\Core\\RoboPartsData.csv",
		"Data\\RoboParts\\Head\\RoboPartsData.csv",
		"Data\\RoboParts\\Arms\\RoboPartsData.csv",
		"Data\\RoboParts\\Weapon\\RoboPartsData.csv",
		"Data\\RoboParts\\Weapon\\RoboPartsData.csv",
	};

	//モデルさんの初期位置.
	//const D3DXVECTOR3 vINIT_ROBO_POS = { 52.0f, -35.0f, 17.0f };
	//const D3DXVECTOR3 vINIT_ROBO_ROT = { 6.03318501f, 0.649538994f, 6.18318605f };
	const D3DXVECTOR3 vINIT_ROBO_POS = { 0.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 vINIT_ROBO_ROT = { 0.0f, static_cast<float>( M_PI_4 ) * 0.5f, 0.0f };
	const float fINIT_ROBO_SCALE = 0.75f;

	//カメラの初期位置.
	const D3DXVECTOR3 vINIT_CAMERA_POS = { 0.0f, 0.0f, -100.0f };
	const D3DXVECTOR3 vINIT_CAMERA_LOOK_POS = { 0.0f, 0.0f, 0.0f };


	//ステータスの、CSVから削る行数.
	const int iSTATUS_CUT_NUM = 2;//番号と名前.

	//----- パーツウィンドウ用 -----//.
	//ビューポート.
	const FLOAT INIT_VP_PARTS_W = 576.0f;
	const FLOAT INIT_VP_PARTS_H = 482.0f;
	const FLOAT INIT_VP_PARTS_X = 153.0f;
	const FLOAT INIT_VP_PARTS_Y = 176.0f;
	const FLOAT INIT_VP_PARTS_MIN =	0.0f;
	const FLOAT INIT_VP_PARTS_MAX =	1.0f;
	//カメラ.
	const D3DXVECTOR3 vPARTS_VIEW_CAM_POS  = { 0.0f, 0.0f, -100.0f };
	const D3DXVECTOR3 vPARTS_VIEW_CAM_LOOK = { 0.0f, 0.0f, 0.0f };
	//----- パーツウィンドウ用 -----//.

	//----- ロボウィンドウ用 -----//.
	//ビューポート.
	const FLOAT INIT_VP_ROBO_W = 490.0f;
	const FLOAT INIT_VP_ROBO_H = 570.0f;
	const FLOAT INIT_VP_ROBO_X = 753.0f;
	const FLOAT INIT_VP_ROBO_Y = 87.0f;
	const FLOAT INIT_VP_ROBO_MIN =	0.0f;
	const FLOAT INIT_VP_ROBO_MAX =	1.0f;
	//カメラ.
	const D3DXVECTOR3 vROBO_VIEW_CAM_POS  = { 0.0f, 10.0f, -100.0f };
	const D3DXVECTOR3 vROBO_VIEW_CAM_LOOK = { 0.0f,-10.0f, 0.0f };
	//----- ロボウィンドウ用 -----//.

	//----- 背景 -----//.
	const char* sBACK_SPRITE_PATH = "Data\\Image\\AssembleUi\\AssembleBack.png";
	const D3DXVECTOR3 vBACK_POS = { 0.0f, 0.0f, 0.0f };

	//----- 背景 -----//.


	////----- 矢印 -----//.
	//m_upArrow
	const WHSIZE_FLOAT ARROW_DISP = { 14.0f, 36.0f };
	const char* sPATH_ARROW = "Data\\Image\\AssembleUi\\Arrow.png";
	const char cARROW_FOR_MAX_NUM = 2;

	const D3DXVECTOR3 vARROW_POS_TYPE = { -6.0f + 32.0f - ( ARROW_DISP.w * 0.5f ), 122.0f, 0.0f };
	const float fARROW_POS_TYPE_OFFSET_X = 6.0f + 6.0f * 120.0f;
	const float fARROW_ROLL_ADD = static_cast<float>( M_PI );

	const float fARROW_ROLL_PARTS = static_cast<float>( M_PI_2 );
	const D3DXVECTOR3 vARROW_POS_PARTS = { 83.0f, 172.0f - ARROW_DISP.w * 0.5f, 0.0f };
	const float fARROW_POS_PARTS_OFFSET_Y = ( 4.0f );
	const float fARROW_POS_PARTS_OFFSET_Y_RATE_PARTS_MAX = ( 70.0f + 18.0f );
	////----- 矢印 終わり -----//.




	//色替えメニュー座標.
	const D3DXVECTOR3 vBOX_POS_COLOR = { 380.0f, WND_H/2, 0.0f };//色替え.


	//日本語UI.
	const char* sFONT_TEXT_PATH_ASSEMBLE = "Data\\Font\\Text\\TextAssemble.csv";



	//ズーム限界.
	const float fZOOM_RIMIT_MIN = -100.0f;
	const float fZOOM_RIMIT_MAX = 64.0f;

}

//================================//
//========== 組み換えクラス ==========//
//================================//
clsSCENE_ASSEMBLE::clsSCENE_ASSEMBLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_spAsmModel( nullptr )
	,m_pViewPortPartsWindow( nullptr )
	,m_pViewPortRoboWindow( nullptr )
	,m_enSelectMode( clsASSEMBLE_UI::enSELECT_MODE::PARTS )
	,m_vRoboViewOffsetPos( { 0.0f, 0.0f, 0.0f } )
{
//	clsROBO_STATUS_ENEMY* ppp;
//	ppp = new clsROBO_STATUS_ENEMY( 0 );
//	delete ppp;
}

clsSCENE_ASSEMBLE::~clsSCENE_ASSEMBLE()
{

	SAFE_DELETE( m_pViewPortRoboWindow );
	SAFE_DELETE( m_pViewPortPartsWindow );
	SAFE_DELETE( m_spAsmModel );

	for( unsigned int i=0; i<m_vecspFile.size(); i++ ){
		if( m_vecspFile[i] == nullptr ) continue;
		m_vecspFile[i]->Close();
		m_vecspFile[i].reset();
	}
	m_vecspFile.clear();
	m_vecspFile.shrink_to_fit();

}

void clsSCENE_ASSEMBLE::CreateProduct()
{
	m_wpFont->Create( sFONT_TEXT_PATH_ASSEMBLE );

	if( m_wpSound ){
		m_wpSound->PlayBGM( enBGM_RENGOKU0 );
	}

	//背景.
	SPRITE_STATE ss;
	ss.Disp = { WND_W, WND_H };
	m_upBack = make_unique< clsSprite2D >();
	m_upBack->Create( m_wpDevice, m_wpContext, sBACK_SPRITE_PATH, ss );
	m_upBack->SetPos( vBACK_POS );

	//矢印.
	assert( !m_upArrow );
	ss.Disp = ARROW_DISP;
	m_upArrow = make_unique< clsSPRITE2D_CENTER >();
	m_upArrow->Create( m_wpDevice, m_wpContext, sPATH_ARROW, ss );


	//ミッションシーンに引き継ぐ情報の初期化.
	assert( m_wpRoboStatus );
	m_wpRoboStatus->Clear();

	//UIの数用変数.
	clsASSEMBLE_UI::PARTS_NUM_DATA partsData;
	partsData.resize( clsASSEMBLE_MODEL::ENUM_SIZE );

	//パーツのステータス読み込み.
	UCHAR ucFileMax = clsASSEMBLE_MODEL::ENUM_SIZE;
	m_vecspFile.resize( ucFileMax ); 
	for( UCHAR i=0; i<m_vecspFile.size(); i++ ){
		if( m_vecspFile[i] != nullptr ){
			assert( !"m_spFile[i]は作成済みです" );
			continue;
		}
		m_vecspFile[i] = make_shared< clsFILE >();
		m_vecspFile[i]->Open( sPARTS_STATUS_PASS[i] );

		partsData[i] = m_vecspFile[i]->GetSizeRow();
	}


	//UI.
	assert( !m_upUI );
	m_upUI = make_unique<clsASSEMBLE_UI>( m_wpFont );
	m_upUI->Create( m_wpDevice, m_wpContext, partsData );

	//モデルさん作成.
	assert( m_spAsmModel == nullptr );
	m_spAsmModel = new clsASSEMBLE_MODEL;
	m_spAsmModel->Create( m_wpResource, m_wpRoboStatus );
	m_spAsmModel->SetPos( vINIT_ROBO_POS );
	m_spAsmModel->SetRot( vINIT_ROBO_ROT );
	m_spAsmModel->SetScale( fINIT_ROBO_SCALE );

	assert( m_wpCamera );
	m_wpCamera->SetPos( vINIT_CAMERA_POS );
	m_wpCamera->SetLookPos( vINIT_CAMERA_LOOK_POS );

	//装備中のパーツを今の選択肢に適応する.
	assert( m_wpRoboStatus->GetPartsNum( enPARTS::ARM_L ) == m_wpRoboStatus->GetPartsNum( enPARTS::ARM_R ) );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::LEG ) ]		= m_wpRoboStatus->GetPartsNum( enPARTS::LEG );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::CORE ) ]		= m_wpRoboStatus->GetPartsNum( enPARTS::CORE );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::HEAD ) ]		= m_wpRoboStatus->GetPartsNum( enPARTS::HEAD );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::ARMS ) ]		= m_wpRoboStatus->GetPartsNum( enPARTS::ARM_L );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::WEAPON_L ) ]	= m_wpRoboStatus->GetPartsNum( enPARTS::WEAPON_L );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::WEAPON_R ) ]	= m_wpRoboStatus->GetPartsNum( enPARTS::WEAPON_R );

	//ステータスの初期化.
	for( m_PartsSelect.Type=0; m_PartsSelect.Type<clsASSEMBLE_MODEL::ENUM_SIZE; m_PartsSelect.Type++ ){
		AssembleParts();
	}
	m_PartsSelect.Type = 0;

	//パーツビュー.
	assert( !m_pViewPortPartsWindow );
	m_pViewPortPartsWindow = new D3D11_VIEWPORT;
	m_pViewPortPartsWindow->Width	 = INIT_VP_PARTS_W;
	m_pViewPortPartsWindow->Height	 = INIT_VP_PARTS_H;
	m_pViewPortPartsWindow->TopLeftX = INIT_VP_PARTS_X;
	m_pViewPortPartsWindow->TopLeftY = INIT_VP_PARTS_Y;
	m_pViewPortPartsWindow->MinDepth = INIT_VP_PARTS_MIN;
	m_pViewPortPartsWindow->MaxDepth = INIT_VP_PARTS_MAX;

	//ロボビュー.
	assert( !m_pViewPortRoboWindow );
	m_pViewPortRoboWindow = new D3D11_VIEWPORT;
	m_pViewPortRoboWindow->Width	= INIT_VP_ROBO_W;
	m_pViewPortRoboWindow->Height	= INIT_VP_ROBO_H;
	m_pViewPortRoboWindow->TopLeftX = INIT_VP_ROBO_X;
	m_pViewPortRoboWindow->TopLeftY = INIT_VP_ROBO_Y;
	m_pViewPortRoboWindow->MinDepth = INIT_VP_ROBO_MIN;
	m_pViewPortRoboWindow->MaxDepth = INIT_VP_ROBO_MAX;


	//パーツビューに置くパーツ.
	assert( !m_upSelectParts );
	m_upSelectParts = make_unique< clsPARTS_WINDOW_MODEL >( m_wpResource, m_wpRoboStatus );


	//メニューの為のデータ取得&作成.
	clsMENU_WINDOW_ASSEMBLE_BASE::CreateInformation( &m_vecuiInformationDataArray, enINFORMATION_INDEX_size );



}

void clsSCENE_ASSEMBLE::UpdateProduct( enSCENE &enNextScene )
{
	assert( m_wpSound );

	//暗転中は操作不能.
	bool isCanControl = true;
	if( m_wpBlackScreen->GetAlpha() ){
		isCanControl = false;
	}

#ifdef _DEBUG

#ifdef RESOURCE_READ_PARTS_MODEL_LOCK
	//テストモデル初期化 & パーツ切替.
	if( GetAsyncKeyState( 'Z' ) & 0x1 ){

		static int tmpI = 0; 
		tmpI ++;
		if( tmpI >= iTEST_ROBO_PARTS_MODEL_MAX ) tmpI = 0;

		m_spAsmModel->AttachModel( enPARTS::LEG, tmpI );
		m_spAsmModel->AttachModel( enPARTS::CORE, tmpI );
		m_spAsmModel->AttachModel( enPARTS::HEAD, tmpI );
		m_spAsmModel->AttachModel( enPARTS::ARM_L, tmpI );
		m_spAsmModel->AttachModel( enPARTS::ARM_R, tmpI );
		m_spAsmModel->AttachModel( enPARTS::WEAPON_L, tmpI );
		m_spAsmModel->AttachModel( enPARTS::WEAPON_R, tmpI );

	}
#endif//#ifndef RESOURCE_READ_PARTS_MODEL_LOCK



	if( GetAsyncKeyState( VK_F6 ) & 0x1 ){
		static int tmpLAnim = 0;
		m_spAsmModel->PartsAnimChange( static_cast<enPARTS>( m_PartsSelect.Type ), tmpLAnim++ );
		if( tmpLAnim >= 10 ) tmpLAnim = 0;
	}
	if( GetAsyncKeyState( VK_F7 ) & 0x1 ){
		static int tmpRAnim = 0;
		m_spAsmModel->PartsAnimChange( enPARTS::LEG, tmpRAnim++ );
		if( tmpRAnim > 10 ) tmpRAnim = 0;
	}
	if( GetAsyncKeyState( VK_F8 ) & 0x1 ){
		static int siCORE_ANIM_NO = 0;
		m_spAsmModel->PartsAnimChange( enPARTS::LEG, siCORE_ANIM_NO++ );
		if( siCORE_ANIM_NO > 1 ) siCORE_ANIM_NO = 0;
	}



	if( GetAsyncKeyState( 'Z' ) & 0x1 )m_wpSound->PlaySE( 0 );
	if( GetAsyncKeyState( 'X' ) & 0x1 )m_wpSound->PlaySE( 1 );
	if( GetAsyncKeyState( 'C' ) & 0x1 )m_wpSound->PlaySE( 2 );
	if( GetAsyncKeyState( 'V' ) & 0x1 )m_wpSound->PlaySE( 3 );
	if( GetAsyncKeyState( 'B' ) & 0x1 )m_wpSound->PlaySE( 4 );
	if( GetAsyncKeyState( 'N' ) & 0x1 ){
		m_ehHibana = m_wpEffects->Play( 
			2, { 0.0f, 20.0f, 0.0f } );
	}



#endif//#ifdef _DEBUG


	//操作.
	if( isCanControl )
	{
		if( m_upMenu ){
			MenuUpdate( enNextScene );
		}
		else{
			//スティックの動き( ロボの回転 ).
			MoveRoboStick();
			//選択肢.
			if( isPressHoldRight( false ) )	MoveCursorRight();
			if( isPressHoldLeft	( false ) )	MoveCursorLeft();
			if( isPressHoldUp	( false ) )	MoveCursorUp();
			if( isPressHoldDown	( false ) )	MoveCursorDown();
			if( isPressExit() )				Exit();
			if( isPressButtonB() )			Enter( enNextScene );
			//次のシーンへのウィンドウを出す.
			if( isPressButtonStart() ){
				//ウィンドウを出してない時は次のシーンへのウィンドウを出す.
				if( isMessageBoxClose() ){
					AppearMessageBox( clsASSEMBLE_UI::enSELECT_MODE::MISSION_START );
				}
				//ウィンドウを出しているときはそのウィンドウの決定ボタンにもなる.
				else{
					Enter( enNextScene );
				}
			}
			//カラーチェンジ.
			if( isPressButtonBack() ){
				//開けるなら開く.
				if( isMessageBoxClose() ){
					AppearMessageBox( clsASSEMBLE_UI::enSELECT_MODE::COLOR_CHANGE );
				}
				//既に開いていているなら閉じる.
				else{
					DisAppearMessageBox();
				}
			}
			//ステータスウィンドウを隠す.
			if( isPressButtonY() ){
				SwitchDispStatus();
			}
			//ステータスのcomment切替.
			if( isPressButtonX() ){
				if( m_upUI->isCanSwitchStatusComment() &&
					isMessageBoxClose() )
				{
					if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::PARTS ){
						ChangeStatusSelect();
					}
					else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::STATUS ){
						ChangePartsSelect();
					}
				}
			}
		}
	}



	assert( m_upUI );
	m_upUI->Input( m_wpXInput, m_wpDxInput );
	if( isMessageBoxClose() ){
		assert( m_vecspFile[m_PartsSelect.Type] );
		m_upUI->Update( 
			m_enSelectMode,
			m_vecspFile[m_PartsSelect.Type], 
			m_spAsmModel,
			m_PartsSelect.Type, 
			m_PartsSelect.Num[m_PartsSelect.Type], 
			iSTATUS_CUT_NUM );
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::MISSION_START ){
		m_upUI->Update( m_enSelectMode );
	}


	assert( m_spAsmModel );
	m_spAsmModel->UpDate();

	assert( m_upSelectParts );
	m_upSelectParts->SetColors( m_spAsmModel->GetColor() );
	m_upSelectParts->Update( m_PartsSelect.Type, m_PartsSelect.Num[ m_PartsSelect.Type ] );


}

//右スティックの動き( ロボの回転 ).
void clsSCENE_ASSEMBLE::MoveRoboStick()
{
	//左スティックはカメラ移動.
	const float fOFFSET_LIMIT_X = 25.0f;
	const float fOFFSET_LIMIT_Y_UP = 25.0f;
	const float fOFFSET_LIMIT_Y_DOWN = 50.0f;
	const float fMODEL_MOVE_SPD = 1.5f;
	if( m_wpXInput->isSlopeStay( XINPUT_LEFT ) ){
		m_vRoboViewOffsetPos.x += fMODEL_MOVE_SPD;
		if( m_vRoboViewOffsetPos.x > fOFFSET_LIMIT_X ){
			m_vRoboViewOffsetPos.x = fOFFSET_LIMIT_X;
		}
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_RIGHT ) ){
		m_vRoboViewOffsetPos.x -= fMODEL_MOVE_SPD;
		if( m_vRoboViewOffsetPos.x < -fOFFSET_LIMIT_X ){
			m_vRoboViewOffsetPos.x = -fOFFSET_LIMIT_X;
		}
	}
	if( m_wpXInput->isSlopeStay( XINPUT_DOWN ) ){
		m_vRoboViewOffsetPos.y += fMODEL_MOVE_SPD;
		if( m_vRoboViewOffsetPos.y > fOFFSET_LIMIT_Y_UP ){
			m_vRoboViewOffsetPos.y = fOFFSET_LIMIT_Y_UP;
		}
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_UP ) ){
		m_vRoboViewOffsetPos.y -= fMODEL_MOVE_SPD;
		if( m_vRoboViewOffsetPos.y < -fOFFSET_LIMIT_Y_DOWN ){
			m_vRoboViewOffsetPos.y = -fOFFSET_LIMIT_Y_DOWN;
		}
	}
	//右スティックは回転とズーム.
	//モデル回転.
	const float fMODEL_SPN_SPD = 0.05f;
	if( isRightStickStayRight() ){
		m_spAsmModel->AddRot( { 0.0f, fMODEL_SPN_SPD, 0.0f } );
	}
	else if( isRightStickStayLeft() ){
		m_spAsmModel->AddRot( { 0.0f, -fMODEL_SPN_SPD, 0.0f } );
	}
	//モデルズーム.
	const float fMODEL_ZOOM_SPD = 5.0f;
	if( isRightStickStayUp() ){
		m_vRoboViewOffsetPos.z += fMODEL_ZOOM_SPD;
		if( m_vRoboViewOffsetPos.z > fZOOM_RIMIT_MAX ){
			m_vRoboViewOffsetPos.z = fZOOM_RIMIT_MAX;
		}
	}
	else if( isRightStickStayDown() ){
		m_vRoboViewOffsetPos.z -= fMODEL_MOVE_SPD;
		if( m_vRoboViewOffsetPos.z < fZOOM_RIMIT_MIN ){
			m_vRoboViewOffsetPos.z = fZOOM_RIMIT_MIN;
		}
	}
}



void clsSCENE_ASSEMBLE::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
}

void clsSCENE_ASSEMBLE::RenderUi()
{
	//パーツ描画用.
	clsCAMERA_ASSEMBLE PartsViewCam;
	PartsViewCam.Create();
	PartsViewCam.SetPos( vPARTS_VIEW_CAM_POS );
	PartsViewCam.SetLookPos( vPARTS_VIEW_CAM_LOOK );
	PartsViewCam.AddPos( m_upSelectParts->GetSelectPartsHeight() );

	//ロボ描画用.
	clsCAMERA_ASSEMBLE RoboViewCam;
	RoboViewCam.Create();
	RoboViewCam.SetPos( vROBO_VIEW_CAM_POS );;
//	RoboViewCam.AddPos( { 0.0f, 0.0f, m_fDistanceAssembleModel } );
	RoboViewCam.SetLookPos( vROBO_VIEW_CAM_LOOK );
	RoboViewCam.AddPos( m_spAsmModel->GetBonePos( enPARTS::LEG, sBONE_NAME_LEG_TO_CORE ) );
	RoboViewCam.AddPos( m_vRoboViewOffsetPos, true );


	//背景.
	assert( m_upBack );
	m_upBack->Render();

	//ほとんどのUI.
	assert( m_upUI );
	m_upUI->Render( 
		m_enSelectMode, 
		m_PartsSelect.Type, 
		m_PartsSelect.Num[ m_PartsSelect.Type ],
		m_wpXInput->isConnected() );


	//矢印.
	assert( m_upArrow );
	//パーツタイプ.
	m_upArrow->SetPos( vARROW_POS_TYPE );
	m_upArrow->SetRot( { 0.0f, 0.0f, 0.0f } );
	for( char i=0; i<cARROW_FOR_MAX_NUM; i++ ){
		m_upArrow->Render();
		m_upArrow->AddPos( { fARROW_POS_TYPE_OFFSET_X, 0.0f, 0.0f } );
		m_upArrow->AddRot( { 0.0f, 0.0f, fARROW_ROLL_ADD } );
	}
	//各パーツ.m_vecspFile
	m_upArrow->SetPos( vARROW_POS_PARTS );
	m_upArrow->SetRot( { 0.0f, 0.0f, fARROW_ROLL_PARTS } );
	//今選択しているパーツの最大数.
	const float fPARTS_MAX = static_cast<float>( m_vecspFile[ m_PartsSelect.Type ]->GetSizeRow() );
	for( char i=0; i<cARROW_FOR_MAX_NUM; i++ ){
		m_upArrow->Render();
		m_upArrow->AddPos( { 0.0f, fARROW_POS_PARTS_OFFSET_Y, 0.0f } );
		m_upArrow->AddPos( { 
			0.0f, 
			fARROW_POS_PARTS_OFFSET_Y_RATE_PARTS_MAX * fPARTS_MAX, 
			0.0f } );
		m_upArrow->AddRot( { 0.0f, 0.0f, fARROW_ROLL_ADD } );
	}




	//パーツ描画.
	SetDepth( true );
	SetViewPort( 
		m_pViewPortPartsWindow, 
		PartsViewCam.GetPos(), PartsViewCam.GetLookPos(),
		m_pViewPortPartsWindow->Width, m_pViewPortPartsWindow->Height,
		m_fRenderLimit );
	assert( m_upSelectParts );
	m_upSelectParts->Render( m_mView, m_mProj, m_vLight, PartsViewCam.GetPos(), isMessageBoxClose() );


	//ロボ描画.
	SetViewPort( 
		m_pViewPortRoboWindow, 
		RoboViewCam.GetPos(), RoboViewCam.GetLookPos(),
		m_pViewPortRoboWindow->Width, m_pViewPortRoboWindow->Height,
		m_fRenderLimit );
	assert( m_spAsmModel );
	//パーツ選択中は選択しているパーツを光らせる.
	if( isMessageBoxClose() )
	{
		m_spAsmModel->Render( m_mView, m_mProj, m_vLight, RoboViewCam.GetPos(), 
			static_cast< clsASSEMBLE_MODEL::enPARTS_TYPES >( m_PartsSelect.Type ),
			m_wpContext );
	}
	else{
		m_spAsmModel->Render( m_mView, m_mProj, m_vLight, RoboViewCam.GetPos() );
	}
	SetDepth( false );



	//パーツのステータス.
	SetViewPort( 
		GetViewPortMainPtr(), 
		m_wpCamera->GetPos(), m_wpCamera->GetLookPos(),
		WND_W, WND_H );
	assert( m_upUI );
	m_upUI->RenderPartsState( m_enSelectMode, m_PartsSelect.Type, m_PartsSelect.Num[ m_PartsSelect.Type ] );


	if( m_upMenu ){
		m_upMenu->Render();
	}

}

//----- カーソル移動 -----//.
void clsSCENE_ASSEMBLE::MoveCursorUp()
{
	//パーツカテゴリを選んでないならパーツを選ばせないよ.
	if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::PARTS ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		m_upUI->InitReadNumPartsComment();

		m_PartsSelect.Num[ m_PartsSelect.Type ] --;

		m_PartsSelect.Num[ m_PartsSelect.Type ] = 
			LoopRange( m_PartsSelect.Num[ m_PartsSelect.Type ], 0, m_vecspFile[ m_PartsSelect.Type ]->GetSizeRow() );
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::STATUS ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		m_upUI->AddStatusCommentNo( false );
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::COLOR_CHANGE ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );

	}
}

void clsSCENE_ASSEMBLE::MoveCursorDown()
{
	//パーツカテゴリを選んでないならパーツを選ばせないよ.
	if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::PARTS ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		m_upUI->InitReadNumPartsComment();

		m_PartsSelect.Num[ m_PartsSelect.Type ] ++;

		m_PartsSelect.Num[ m_PartsSelect.Type ] = 
			LoopRange( m_PartsSelect.Num[ m_PartsSelect.Type ], 0, m_vecspFile[ m_PartsSelect.Type ]->GetSizeRow() );
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::STATUS ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		m_upUI->AddStatusCommentNo( true );
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::COLOR_CHANGE ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
	}

}

void clsSCENE_ASSEMBLE::MoveCursorRight()
{
	//パーツ選択.
	if( isMessageBoxClose() ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );

		m_PartsSelect.Type ++;
		m_PartsSelect.Type = 
			LoopRange( m_PartsSelect.Type, 0, clsASSEMBLE_MODEL::ENUM_SIZE );

		m_upUI->AddCommentNoForChangePartsType( m_PartsSelect.Type );
	}
}

void clsSCENE_ASSEMBLE::MoveCursorLeft()
{
	//パーツを選ぶ.
	if( isMessageBoxClose() ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );

		m_PartsSelect.Type --;
		m_PartsSelect.Type = 
			LoopRange( m_PartsSelect.Type, 0, clsASSEMBLE_MODEL::ENUM_SIZE );

		m_upUI->AddCommentNoForChangePartsType( m_PartsSelect.Type );
	}
}



//決定.
void clsSCENE_ASSEMBLE::Enter( enSCENE &enNextScene )const
{
	if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::PARTS ){
		m_wpSound->PlaySE( enSE_ENTER );
		AssembleParts();
	}
}
//キャンセル.
void clsSCENE_ASSEMBLE::Exit()
{
	//ないとき.
	if( isMessageBoxClose() ){
		//パーツを選んでいるなら.
		if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::PARTS ){
			//ウィンドウを出す.
			AppearMessageBox( clsASSEMBLE_UI::enSELECT_MODE::TITLE_BACK );
		}
		//ステータスを見ているなら.
		else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::STATUS ){
			//パーツセレクトに戻る.
			ChangePartsSelect();
		}
	}
	//ウィンドウがあるとき.
	else{
		//ウィンドウを消す.
		DisAppearMessageBox();
	}
}

//Xinputのボタン入力.
//bool clsSCENE_ASSEMBLE::isPressButtonA()
//{
//	return false;
//}
bool clsSCENE_ASSEMBLE::isPressButtonB()
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_B ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKeyEnter( (enPKey)enGIGANT_WEAPONS_CONTROLLER_B ) ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_RETURN ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_ASSEMBLE::isPressButtonX()
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_X ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKeyEnter( (enPKey)enGIGANT_WEAPONS_CONTROLLER_X ) ){
			return true;
		}
	}

	return false;
}
bool clsSCENE_ASSEMBLE::isPressButtonY()
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_Y ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKeyEnter( (enPKey)enGIGANT_WEAPONS_CONTROLLER_Y ) ){
			return true;
		}
	}

	return false;
}
bool clsSCENE_ASSEMBLE::isPressButtonStart()
{
	if( isPressStart() ){
		return true;
	}
	else if( m_wpDxInput->IsPressKeyEnter( (enPKey)enGIGANT_WEAPONS_CONTROLLER_START ) ){
		return true;
	}


	return false;
}
bool clsSCENE_ASSEMBLE::isPressButtonBack()
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_BACK ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKeyEnter( (enPKey)enGIGANT_WEAPONS_CONTROLLER_BACK ) ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_SHIFT ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_ASSEMBLE::isRightStickStayUp()
{
	if( m_wpXInput->isConnected() ){
		if( m_wpXInput->isSlopeStay( XINPUT_UP, false ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKey( enPKey_RUp ) ){
			return true;
		}
	}

	return false;
}
bool clsSCENE_ASSEMBLE::isRightStickStayDown()
{
	if( m_wpXInput->isConnected() ){
		if( m_wpXInput->isSlopeStay( XINPUT_DOWN, false ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKey( enPKey_RDown ) ){
			return true;
		}
	}

	return false;
}
bool clsSCENE_ASSEMBLE::isRightStickStayRight()
{
	if( m_wpXInput->isConnected() ){
		if( m_wpXInput->isSlopeStay( XINPUT_RIGHT, false ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKey( enPKey_RRight ) ){
			return true;
		}
	}

	return false;
}
bool clsSCENE_ASSEMBLE::isRightStickStayLeft()
{
	if( m_wpXInput->isConnected() ){
		if( m_wpXInput->isSlopeStay( XINPUT_LEFT, false ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKey( enPKey_RLeft ) ){
			return true;
		}
	}

	return false;
}







//色替え( 左右キーを押された ).
void clsSCENE_ASSEMBLE::AddRoboColor( const bool isIncrement )const
{
	if( m_enColorGageIndex >= clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_size ){
		return;
	}

	assert( m_spAsmModel );
	//右.
	if( isIncrement ){
		if( m_spAsmModel->IncrementColor( m_enColorGageIndex ) ){
			//動いたなら音を鳴らす.
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		}
	}
	//左.
	else{
		if( m_spAsmModel->DecrementColor( m_enColorGageIndex ) ){
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		}
	}

	//色の保存.
	for( char i=0; i<clsROBO_STATUS::enCOLOR_GAGE_size; i++ ){
		clsROBO_STATUS::enCOLOR_GAGE tmpIndex = static_cast<clsROBO_STATUS::enCOLOR_GAGE>( i );
		int iColorRank = m_spAsmModel->GetColorRank( tmpIndex );
		m_wpRoboStatus->SetColorRank( tmpIndex, iColorRank );
	}
}



//出撃.
void clsSCENE_ASSEMBLE::MissionStart( enSCENE &enNextScene )
{
	m_wpSound->PlaySE( enSE_MISSION_START );
//	m_wpSound->PlaySE( enSE_ENTER );

	enNextScene = enSCENE::MISSION;
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::MISSION_START;

	m_wpSound->PlaySE( enSE_MISSION_START );
}

//パーツ変更.
void clsSCENE_ASSEMBLE::AssembleParts()const
{
//	m_wpSound->PlaySE( enSE_ENTER );

	//ステータスが何項目あるのか.
	const int iStatusSize = m_vecspFile[ m_PartsSelect.Type ]->GetSizeCol() - iSTATUS_CUT_NUM;

	//引数用変数.
	vector<int> tmpStatus;
	tmpStatus.reserve( iStatusSize );
	for( int i=0; i<iStatusSize; i++ ){
		//m_vecspFile[]の添え字はどのパーツか、である.
		tmpStatus.push_back( 
			m_vecspFile[ m_PartsSelect.Type ]->
				GetDataInt( m_PartsSelect.Num[ m_PartsSelect.Type ], i + iSTATUS_CUT_NUM ) );
		//GetDataInt()の第一引数は、そのパーツ部位の何番目の行を参照すればよいのか.
		//第二引数でiSTATUS_CUT_NUMを足しているのは、元の表にあるパーツ番号と名前はいらないからカットするためである.
	}

	//何度もキャストをするのは嫌なので.
	UCHAR tmpPartsNum = static_cast<UCHAR>( m_PartsSelect.Num[ m_PartsSelect.Type ] );

	switch( m_PartsSelect.Type )
	{
	case clsASSEMBLE_MODEL::LEG:
		m_wpRoboStatus->ReceiveLeg( tmpStatus,		tmpPartsNum );
		m_spAsmModel->AttachModel( enPARTS::LEG,	tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::CORE:
		m_wpRoboStatus->ReceiveCore( tmpStatus,		tmpPartsNum );
		m_spAsmModel->AttachModel( enPARTS::CORE,	tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::HEAD:
		m_wpRoboStatus->ReceiveHead( tmpStatus,		tmpPartsNum );
		m_spAsmModel->AttachModel( enPARTS::HEAD,	tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::ARMS:
		m_wpRoboStatus->ReceiveArms( tmpStatus,		tmpPartsNum );
		m_spAsmModel->AttachModel( enPARTS::ARM_L,	tmpPartsNum );
		m_spAsmModel->AttachModel( enPARTS::ARM_R,	tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::WEAPON_L:
		m_wpRoboStatus->ReceiveWeaponL( tmpStatus,	tmpPartsNum );
		m_spAsmModel->AttachModel( enPARTS::WEAPON_L,tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::WEAPON_R:
		m_wpRoboStatus->ReceiveWeaponR( tmpStatus,	tmpPartsNum );
		m_spAsmModel->AttachModel( enPARTS::WEAPON_R,tmpPartsNum );
		break;
	default:
		break;
	}

	//引数用変数の片づけ.
	tmpStatus.clear();
	tmpStatus.shrink_to_fit();
}


//戻る.
void clsSCENE_ASSEMBLE::TitleBack( enSCENE &enNextScene )const
{
//	m_wpSound->PlaySE( enSE_EXIT );
	enNextScene = enSCENE::TITLE;
}








//範囲内に収める( パーツの選択肢がオーバーしないようにする ).
//minはその数値より小さくならない、maxはそれ以上にはならない.
// min <= t < max.
template<class T, class MIN, class MAX >
T clsSCENE_ASSEMBLE::LoopRange( T t, const MIN min, const MAX max ) const
{
	int num = static_cast<int>( t );
	int Min = static_cast<int>( min );
	int Max = static_cast<int>( max );
	
//	if( Min > num ){
//		num = Min;
//	}
//	else if( num >= Max ){
//		num = Max - 1;
//	}
	if( Min > num ){
		num = Max - 1;
	}
	else if( num >= Max ){
		num = Min;
	}

	return static_cast<T>( num );
}

//カーソルを出撃に合わせているならtrue.
bool clsSCENE_ASSEMBLE::isMessageBoxClose()const
{
	if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::MISSION_START	||
		m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::TITLE_BACK		||
		m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::COLOR_CHANGE )
	{
		return false;
	}

	return true;
}

//メッセボックス出現.
void clsSCENE_ASSEMBLE::AppearMessageBox( 
	const clsASSEMBLE_UI::enSELECT_MODE encMode )
{
	m_enSelectMode = encMode;
	if( encMode == clsASSEMBLE_UI::enSELECT_MODE::MISSION_START ){
		m_upMenu = make_unique< clsMENU_WINDOW_ASSEMBLE_TAKE_OFF >(
			m_hWnd, m_wpPtrGroup, nullptr, &m_vecuiInformationDataArray );
	}
	else if( encMode == clsASSEMBLE_UI::enSELECT_MODE::TITLE_BACK ){
		m_upMenu = make_unique< clsMENU_WINDOW_ASSEMBLE_BACK_TITLE >(
			m_hWnd, m_wpPtrGroup, nullptr, &m_vecuiInformationDataArray );
	}
	else if( encMode == clsASSEMBLE_UI::enSELECT_MODE::COLOR_CHANGE ){
		m_enColorGageIndex = static_cast<clsROBO_STATUS::enCOLOR_GAGE>( 0 );
		m_upMenu = make_unique< clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE >(
			m_hWnd, m_wpPtrGroup, nullptr, &m_vecuiInformationDataArray, m_spAsmModel );
		m_upMenu->SetPos( vBOX_POS_COLOR );
	}
}
//メッセボックス消す.
void clsSCENE_ASSEMBLE::DisAppearMessageBox()
{

	//選択肢を選べるように戻す.
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::PARTS;

}


//ステータスの表示非表示切替.
void clsSCENE_ASSEMBLE::SwitchDispStatus()
{
	//ウィンドウを出してるときは無視.
	if( !isMessageBoxClose() ){
		return;
	}

	m_wpSound->PlaySE( enSE_CURSOL_MOVE );
	m_upUI->SwitchDispStatusComment();
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::PARTS;
}

//パーツ選択とステータス選択の切り替え.
void clsSCENE_ASSEMBLE::ChangePartsSelect()
{
	m_wpSound->PlaySE( enSE_EXIT );
	m_upUI->SwitchStatusComment();
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::PARTS;
}

void clsSCENE_ASSEMBLE::ChangeStatusSelect()
{
	m_wpSound->PlaySE( enSE_ENTER );
	m_upUI->SwitchStatusComment();
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::STATUS;
}




//メニューの動き.
void clsSCENE_ASSEMBLE::MenuUpdate( enSCENE &enNextScene )
{
	m_upMenu->Update();
	//メニューが何か返してくる.
	unsigned int uiReceiveInformation = m_upMenu->GetInformation();
	if( uiReceiveInformation )
	{
		char cInformationIndex = -1;
		for( char i=0; i<enINFORMATION_INDEX_size; i++ ){
			//有用な情報と合致したなら.
			if( uiReceiveInformation == m_vecuiInformationDataArray[i] ){
				cInformationIndex = i;
			}
		}
		switch( cInformationIndex )
		{
		case enINFORMATION_INDEX_GO_TITLE:
			TitleBack( enNextScene );
			break;
		case enINFORMATION_INDEX_GO_MISSION:
			MissionStart( enNextScene );
			break;
		case enINFORMATION_INDEX_CLOSE_WINDOW:
			m_upMenu->Close();
			DisAppearMessageBox();
			break;

		case enINFORMATION_INDEX_COLOR_1_R_INCREMENT:
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_BASE_R;
			AddRoboColor( true );
			break;
		case enINFORMATION_INDEX_COLOR_1_R_DECREMENT:
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_BASE_R;
			AddRoboColor( false );
			break;

		case enINFORMATION_INDEX_COLOR_1_G_INCREMENT:
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_BASE_G;
			AddRoboColor( true );
			break;
		case enINFORMATION_INDEX_COLOR_1_G_DECREMENT:
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_BASE_G;
			AddRoboColor( false );
			break;

		case enINFORMATION_INDEX_COLOR_1_B_INCREMENT:
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_BASE_B;
			AddRoboColor( true );
			break;
		case enINFORMATION_INDEX_COLOR_1_B_DECREMENT:
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_BASE_B;
			AddRoboColor( false );
			break;


		case enINFORMATION_INDEX_COLOR_2_R_INCREMENT:
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_ARMOR_R;
			AddRoboColor( true );											
			break;															
		case enINFORMATION_INDEX_COLOR_2_R_DECREMENT:						
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_ARMOR_R;
			AddRoboColor( false );											
			break;															
																			
		case enINFORMATION_INDEX_COLOR_2_G_INCREMENT:						
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_ARMOR_G;
			AddRoboColor( true );											
			break;															
		case enINFORMATION_INDEX_COLOR_2_G_DECREMENT:						
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_ARMOR_G;
			AddRoboColor( false );											
			break;															
																			
		case enINFORMATION_INDEX_COLOR_2_B_INCREMENT:						
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_ARMOR_B;
			AddRoboColor( true );											
			break;															
		case enINFORMATION_INDEX_COLOR_2_B_DECREMENT:						
			m_enColorGageIndex = clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_ARMOR_B;
			AddRoboColor( false );
			break;

		default:
			assert( !"不正な情報が返されました" );
			break;
		}
	}

	//消える許可が出たら消える.
	if( m_upMenu->isDeletePermission() ){
		m_upMenu.reset( nullptr );
	}

}



//============================ デバッグテキスト ===========================//
#ifdef _DEBUG
void clsSCENE_ASSEMBLE::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.
	
//	//モデルのpos.
//	sprintf_s( strDbgTxt, 
//		"ModelPos : x[%f], y[%f], z[%f]",
//		m_spAsmModel->GetPos().x, 
//		m_spAsmModel->GetPos().y, 
//		m_spAsmModel->GetPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"ModelRot : x[%f], y[%f], z[%f]",
//		m_spAsmModel->GetRot().x, 
//		m_spAsmModel->GetRot().y, 
//		m_spAsmModel->GetRot().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	//各パーツのpos.
//	for( UCHAR ucNo=0; ucNo<static_cast<UCHAR>( enPARTS::MAX ); ucNo++ ){
//		sprintf_s( strDbgTxt, 
//			"PartsPos : x[%f], y[%f], z[%f]",
//			m_spAsmModel->GetPartsPos( ucNo ).x, 
//			m_spAsmModel->GetPartsPos( ucNo ).y, 
//			m_spAsmModel->GetPartsPos( ucNo ).z );
//		m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//	}
//
//	//カメラ.
//	sprintf_s( strDbgTxt, 
//		"CameraPos : x[%f], y[%f], z[%f]",
//		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"CamLokPos : x[%f], y[%f], z[%f]",
//		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	//ライト.
//	sprintf_s( strDbgTxt, 
//		"Light : x[%f], y[%f], z[%f]",
//		m_vLight.x, m_vLight.y, m_vLight.z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

//	//ライト.
//	sprintf_s( strDbgTxt, 
//		"Light : x[%f], y[%f], z[%f]",
//		m_vLight.x, m_vLight.y, m_vLight.z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	//UI.
	sprintf_s( strDbgTxt, 
		"SpritePos : x[%f], y[%f]",
		m_upUI->GetUiPos().x, m_upUI->GetUiPos().y );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

//	//日本語文字.[400:-400,200][200:-200,100][50:-50,25]
//	sprintf_s( strDbgTxt, 
//		"Font : x[%f], y[%f], Scale[%f]",
//		setumeiX, setumeiY, m_wpFont->GetScale() );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

//	//テスト用に数値を出す.
//	string tmpsString;
//	tmpsString = m_vecspFile[m_PartsSelect.Type]->GetDataString( m_PartsSelect.Num[m_PartsSelect.Type] );
//	const char* tmpcString = tmpsString.c_str();
//	sprintf_s( strDbgTxt, 
//		tmpcString );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );



}
#endif //#ifdef _DEBUG



