#include "SceneAssemble.h"
#include "WindowBox.h"

using namespace std;


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




//メッセボックス.
const float fBOX_ALPHA = 0.5f;
const D3DXVECTOR3 vBOX_POS = { WND_W/2, WND_H/2, 0.0f };
const D3DXVECTOR3 vBOX_SIZE = { WND_W/2, WND_H/2, 0.0f };
const float fBOX_BIG_SPD_W = 60.0f;
const float fBOX_BIG_SPD_H = 40.0f;
const clsLINE_BOX::encBEFOR_CHANGE encBOX_APPEAR_CHANGE_MODE = clsLINE_BOX::encBEFOR_CHANGE::WIDTH;
const float fBOX_SMALL_SPD_RATE = 1.0 / 16.0f;
const clsLINE_BOX::encBEFOR_CHANGE encBOX_DISAPPEAR_CHANGE_MODE = clsLINE_BOX::encBEFOR_CHANGE::BOTH;
const int iBOX_MESSAGE_LINE_GO_MISSION = 1;
const int iBOX_MESSAGE_LINE_BACK_TITLE = iBOX_MESSAGE_LINE_GO_MISSION + 1;


//YesNo.
const float fMESSAGEBOX_BUTTON_X_OFFSET = 100.0f;
const float fMESSAGEBOX_BUTTON_Y = WND_H/2 + 100.0f;
const D3DXVECTOR3 vMESSAGEBOX_YES_BUTTON_POS = { WND_W/2 - fMESSAGEBOX_BUTTON_X_OFFSET, fMESSAGEBOX_BUTTON_Y, 0.0f };
const D3DXVECTOR3 vMESSAGEBOX_NO_BUTTON_POS =  { WND_W/2 + fMESSAGEBOX_BUTTON_X_OFFSET, fMESSAGEBOX_BUTTON_Y, 0.0f };
const WHSIZE_FLOAT MESSAGEBOX_BUTTON_YES_NO_DISP = { 80.0f, 60.0f }; 
const WHSIZE_FLOAT MESSAGEBOX_BUTTON_YES_NO_ANIM = { 2.0f, 2.0f }; 
const char* sMESSAGEBOX_BUTTON_YES_NO_PATH = "Data\\Image\\AssembleUi\\YesNo.png";
const POINTFLOAT MESSAGEBOX_YES_BUTTON_ANIM = { 0.0f, 0.0f };
const POINTFLOAT MESSAGEBOX_NO_BUTTON_ANIM = { 1.0f, 0.0f };
const float fMESSAGEBOX_YES_NO_BUTTON_SELECT_ANIM = 1.0f;



//日本語UI.
const char* sFONT_TEXT_PATH_ASSEMBLE = "Data\\Font\\Text\\TextAssemble.csv";
//ボタン説明.
const D3DXVECTOR3 vFONT_BUTTON_POS = { 650.0f, 40.0f, 0.0f };
const float fFONT_BUTTON_SCALE = 14.0f;
const int iFONT_BUTTON_LINE = 0;

//出撃メッセージ.
const D3DXVECTOR3 vFONT_MESSAGE_BOX_TITLE_POS = { 480.0f, 230.0f, 0.0f };
const float fFONT_MESSAGE_BOX_TITLE_SCALE = 24.0f;


//パーツ、ステータス説明.
const D3DXVECTOR3 vFONT_COMMENT_POS = { 28.0f, 680.0f, 0.0f };
const float fFONT_COMMENT_SCALE = 16.0f;
const int iFONT_COMMENT_LINE = 1;
const int iFONT_COMMENT_TEXT_SIZE = 128;




//================================//
//========== 組み換えクラス ==========//
//================================//
clsSCENE_ASSEMBLE::clsSCENE_ASSEMBLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_pAsmModel( nullptr )
	,m_pUI( nullptr )
	,m_pViewPortPartsWindow( nullptr )
	,m_pViewPortRoboWindow( nullptr )
	,m_pSelectParts( nullptr )
	,m_iMessageNum( 0 )
	,m_isMessageBoxYes( true )
	,m_isCanControl( false )
//	,m_enSelectMode()
{
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::PARTS;
}

clsSCENE_ASSEMBLE::~clsSCENE_ASSEMBLE()
{

	SAFE_DELETE( m_pViewPortRoboWindow );
	SAFE_DELETE( m_pViewPortPartsWindow );
	SAFE_DELETE( m_pAsmModel );
	SAFE_DELETE( m_pUI );

	for( unsigned int i=0; i<m_vspFile.size(); i++ ){
		if( m_vspFile[i] == nullptr ) continue;
		m_vspFile[i]->Close();
		m_vspFile[i].reset();
	}
	m_vspFile.clear();
	m_vspFile.shrink_to_fit();

	m_pSelectParts = nullptr;
}

void clsSCENE_ASSEMBLE::CreateProduct()
{
	m_wpFont->Create( sFONT_TEXT_PATH_ASSEMBLE );

	m_wpSound->PlayBGM( enBGM_MAFIA1 );

//	m_pTestChara = new clsCharaStatic;
//	m_pTestChara->AttachModel( 
//		m_wpResource->GetStaticModels( 
//		clsResource::enSTATIC_MODEL::enStaticModel_Ground ) );
//	m_pTestChara->Init();
//	m_pTestChara->SetPosition( D3DXVECTOR3( 1.0f, 0.0f, 50.0f ) );
//
//	m_pParts = new clsCharaStatic;
//	m_pParts->AttachModel( 
//		m_wpResource->GetStaticModels( 
//		clsResource::enSTATIC_MODEL::enStaticModel_Enemy ) );
//	m_pParts->Init();
//	m_pParts->SetPosition( D3DXVECTOR3( -2.0f, 1.0f, 0.0f ) );
//

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


	//UIの数用変数.
	clsASSEMBLE_UI::PARTS_NUM_DATA partsData;
	partsData.resize( clsASSEMBLE_MODEL::ENUM_SIZE );

	//パーツのステータス読み込み.
	UCHAR ucFileMax = clsASSEMBLE_MODEL::ENUM_SIZE;
	m_vspFile.resize( ucFileMax ); 
	for( UCHAR i=0; i<m_vspFile.size(); i++ ){
		if( m_vspFile[i] != nullptr ){
			assert( !"m_spFile[i]は作成済みです" );
			continue;
		}
		m_vspFile[i] = make_shared< clsFILE >();
		m_vspFile[i]->Open( sPARTS_STATUS_PASS[i] );

		partsData[i] = m_vspFile[i]->GetSizeRow();
	}

	//箱.
	assert( !m_upBox );
	m_upBox = make_unique< clsWINDOW_BOX >( m_wpDevice, m_wpContext );
	m_upBox->SetPos( vBOX_POS );
	m_upBox->SetSize( 0.0f );
	m_upBox->SetAlpha( fBOX_ALPHA );

	//yesno.
	assert( !m_upYesNo );
	ss.Disp = MESSAGEBOX_BUTTON_YES_NO_DISP;
	ss.Anim = MESSAGEBOX_BUTTON_YES_NO_ANIM;
	m_upYesNo = make_unique< clsSPRITE2D_CENTER >();
	m_upYesNo->Create( m_wpDevice, m_wpContext, sMESSAGEBOX_BUTTON_YES_NO_PATH, ss );


	//UI.
	assert( !m_pUI );
	m_pUI = new clsASSEMBLE_UI( m_wpFont );
	m_pUI->Create( m_wpDevice, m_wpContext, partsData );

	//モデルさん作成.
	assert( m_pAsmModel == nullptr );
	m_pAsmModel = new clsASSEMBLE_MODEL;
	m_pAsmModel->Create( m_wpResource, m_wpRoboStatus );
	m_pAsmModel->SetPos( vINIT_ROBO_POS );
	m_pAsmModel->SetRot( vINIT_ROBO_ROT );
	m_pAsmModel->SetScale( fINIT_ROBO_SCALE );

	assert( m_wpCamera );
	m_wpCamera->SetPos( vINIT_CAMERA_POS );
	m_wpCamera->SetLookPos( vINIT_CAMERA_LOOK_POS );

	//装備中のパーツを今の選択肢に適応する.
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::LEG ) ]		= m_wpRoboStatus->GetPartsNum( enPARTS::LEG );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::CORE ) ]		= m_wpRoboStatus->GetPartsNum( enPARTS::CORE );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::HEAD ) ]		= m_wpRoboStatus->GetPartsNum( enPARTS::HEAD );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::ARMS ) ]		= m_wpRoboStatus->GetPartsNum( enPARTS::HEAD );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::WEAPON_L ) ]	= m_wpRoboStatus->GetPartsNum( enPARTS::WEAPON_L );
	m_PartsSelect.Num[ static_cast<char>( clsASSEMBLE_MODEL::WEAPON_R ) ]	= m_wpRoboStatus->GetPartsNum( enPARTS::WEAPON_R );

	//ミッションシーンに引き継ぐ情報の初期化.
	assert( m_wpRoboStatus );
	m_wpRoboStatus->Clear();

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
	assert( !m_pSelectParts );
	m_pSelectParts = new clsPARTS_WINDOW_MODEL( m_wpResource, m_wpRoboStatus );
//	m_pSelectParts->AttachModel( m_wpResource->GetPartsModels(
//		static_cast< enPARTS >( m_PartsSelect.Type ),
//		static_cast< SKIN_ENUM_TYPE >( m_PartsSelect.Num[ m_PartsSelect.Type ] ) ) );

}

void clsSCENE_ASSEMBLE::UpdateProduct( enSCENE &enNextScene )
{
	assert( m_wpSound );

	int a = m_wpRoboStatus->GetRoboState( clsROBO_STATUS::HP );

	//暗転中は操作不能.
	if( m_wpBlackScreen->GetAlpha() ){
		m_isCanControl = false;
	}
	else{
		m_isCanControl = true;
	}

#if _DEBUG
	//テストモデル初期化 & パーツ切替.
	if( GetAsyncKeyState( 'Z' ) & 0x1 ){
#ifdef RESOURCE_READ_PARTS_MODEL_LOCK

		static int tmpI = 0; 
		tmpI ++;
		if( tmpI >= iTEST_ROBO_PARTS_MODEL_MAX ) tmpI = 0;

		m_pAsmModel->AttachModel( enPARTS::LEG, tmpI );
		m_pAsmModel->AttachModel( enPARTS::CORE, tmpI );
		m_pAsmModel->AttachModel( enPARTS::HEAD, tmpI );
		m_pAsmModel->AttachModel( enPARTS::ARM_L, tmpI );
		m_pAsmModel->AttachModel( enPARTS::ARM_R, tmpI );
		m_pAsmModel->AttachModel( enPARTS::WEAPON_L, tmpI );
		m_pAsmModel->AttachModel( enPARTS::WEAPON_R, tmpI );

#endif//#ifndef RESOURCE_READ_PARTS_MODEL_LOCK
		m_pAsmModel->SetPos( vINIT_ROBO_POS );
		m_pAsmModel->SetRot( vINIT_ROBO_ROT );
		m_pAsmModel->SetScale( fINIT_ROBO_SCALE );

		m_pAsmModel->SetRot( { 0.0f, 0.0f, 0.0f } );

		m_wpSound->StopAllSound();
	}

	//テストモデル移動.
	float fff = 1.0f;
	if( GetAsyncKeyState( 'W' ) & 0x8000 ) m_pAsmModel->AddPos( { 0.0f, fff, 0.0f } );
	if( GetAsyncKeyState( 'S' ) & 0x8000 ) m_pAsmModel->AddPos( { 0.0f, -fff, 0.0f } );
	if( GetAsyncKeyState( 'D' ) & 0x8000 ) m_pAsmModel->AddPos( { fff, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'A' ) & 0x8000 ) m_pAsmModel->AddPos( { -fff, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'E' ) & 0x8000 ) m_pAsmModel->AddPos( { 0.0f, 0.0f, fff } );
	if( GetAsyncKeyState( 'Q' ) & 0x8000 ) m_pAsmModel->AddPos( { 0.0f, 0.0f, -fff } );
	float rrr = 0.05f;
	if( GetAsyncKeyState( 'T' ) & 0x8000 ) m_pAsmModel->AddRot( { 0.0f, rrr, 0.0f } );
	if( GetAsyncKeyState( 'G' ) & 0x8000 ) m_pAsmModel->AddRot( { 0.0f, -rrr, 0.0f } );
	if( GetAsyncKeyState( 'F' ) & 0x8000 ) m_pAsmModel->AddRot( { rrr, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'H' ) & 0x8000 ) m_pAsmModel->AddRot( { -rrr, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'R' ) & 0x8000 ) m_pAsmModel->AddRot( { 0.0f, 0.0f, rrr } );
	if( GetAsyncKeyState( 'Y' ) & 0x8000 ) m_pAsmModel->AddRot( { 0.0f, 0.0f, -rrr } );


	if( GetAsyncKeyState( VK_F6 ) & 0x1 ){
//		enNextScene = enSCENE::MISSION;
		static int tmpLAnim = 0;
		m_pAsmModel->PartsAnimChange( static_cast<enPARTS>( m_PartsSelect.Type ), tmpLAnim++ );
		if( tmpLAnim >= 5 ) tmpLAnim = 0;
	}
	if( GetAsyncKeyState( VK_F7 ) & 0x1 ){
		static int siCORE_ANIM_NO = 0;
		m_pAsmModel->PartsAnimChange( enPARTS::LEG, siCORE_ANIM_NO++ );
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



#endif//#if _DEBUG

	//操作.
	if( m_isCanControl ){
		//選択肢.
		if( isPressRight()	)MoveCursorRight();
		if( isPressLeft()	)MoveCursorLeft();
		if( isPressUp()		)MoveCursorUp();
		if( isPressDown()	)MoveCursorDown();
		if( m_wpXInput->isPressEnter( XINPUT_B ) ||
			GetAsyncKeyState( VK_RETURN ) & 0x1 )
		{
			Enter( enNextScene );
		}
		if( isPressExit() ){
			Exit();
		}
		//次のシーンへのウィンドウを出す.
		if( m_wpXInput->isPressEnter( XINPUT_START ) || 
			GetAsyncKeyState( VK_SPACE ) & 0x1 )
		{
			//ウィンドウを出してない時は次のシーンへのウィンドウを出す.
			if( isMessageBoxClose() ){
				AppearMessageBox( clsASSEMBLE_UI::enSELECT_MODE::MISSION_START );
			}
			//ウィンドウを出しているときはそのウィンドウの決定ボタンにもなる.
			else{
				Enter( enNextScene );
			}
		}
		//ステータスウィンドウを隠す.
		if( m_wpXInput->isPressEnter( XINPUT_Y ) ){
			SwitchDispStatus();
		}
		//ステータスのcomment切替.
		if( m_wpXInput->isPressEnter( XINPUT_X ) )
		{
			if( m_pUI->isCanSwitchStatusComment() &&
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



	assert( m_pUI );
	m_pUI->Input( m_wpXInput, m_wpDxInput );
	if( isMessageBoxClose() ){
		assert( m_vspFile[m_PartsSelect.Type] );
		m_pUI->Update( 
			m_enSelectMode,
			m_vspFile[m_PartsSelect.Type], 
			m_pAsmModel,
			m_PartsSelect.Type, 
			m_PartsSelect.Num[m_PartsSelect.Type], 
			iSTATUS_CUT_NUM );
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::MISSION_START ){
		m_pUI->Update( m_enSelectMode );
	}

	assert( m_pAsmModel );
	m_pAsmModel->UpDate();

	assert( m_pSelectParts );
	m_pSelectParts->Update( m_PartsSelect.Type, m_PartsSelect.Num[ m_PartsSelect.Type ] );

	assert( m_upBox );
	m_upBox->Update();

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
	PartsViewCam.AddPos( m_pSelectParts->GetSelectPartsHeight() );

	//ロボ描画用.
	clsCAMERA_ASSEMBLE RoboViewCam;
	RoboViewCam.Create();
	RoboViewCam.SetPos( vROBO_VIEW_CAM_POS );
	RoboViewCam.SetLookPos( vROBO_VIEW_CAM_LOOK );
	RoboViewCam.AddPos( m_pAsmModel->GetBonePos( enPARTS::LEG, sBONE_NAME_LEG_TO_CORE ) );


	//背景.
	assert( m_upBack );
	m_upBack->Render();

	//ほとんどのUI.
	assert( m_pUI );
	m_pUI->Render( m_enSelectMode, m_PartsSelect.Type, m_PartsSelect.Num[m_PartsSelect.Type] );


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
	//各パーツ.m_vspFile
	m_upArrow->SetPos( vARROW_POS_PARTS );
	m_upArrow->SetRot( { 0.0f, 0.0f, fARROW_ROLL_PARTS } );
	//今選択しているパーツの最大数.
	const float fPARTS_MAX = static_cast<float>( m_vspFile[ m_PartsSelect.Type ]->GetSizeRow() );
	for( char i=0; i<cARROW_FOR_MAX_NUM; i++ ){
		m_upArrow->Render();
		m_upArrow->AddPos( { 0.0f, fARROW_POS_PARTS_OFFSET_Y, 0.0f } );
		m_upArrow->AddPos( { 
			0.0f, 
			fARROW_POS_PARTS_OFFSET_Y_RATE_PARTS_MAX * fPARTS_MAX, 
			0.0f } );
		m_upArrow->AddRot( { 0.0f, 0.0f, fARROW_ROLL_ADD } );
	}

	//ボタンの説明.
	assert( m_wpFont );
	m_wpFont->SetPos( vFONT_BUTTON_POS );
	m_wpFont->SetScale( fFONT_BUTTON_SCALE );
	m_wpFont->Render( iFONT_BUTTON_LINE );



	//パーツ描画.
	SetDepth( true );
	SetViewPort( 
		m_pViewPortPartsWindow, 
		PartsViewCam.GetPos(), PartsViewCam.GetLookPos(),
		m_pViewPortPartsWindow->Width, m_pViewPortPartsWindow->Height );
	assert( m_pSelectParts );
	m_pSelectParts->Render( m_mView, m_mProj, m_vLight, PartsViewCam.GetPos(), isMessageBoxClose() );


	//ロボ描画.
	SetViewPort( 
		m_pViewPortRoboWindow, 
		RoboViewCam.GetPos(), RoboViewCam.GetLookPos(),
		m_pViewPortRoboWindow->Width, m_pViewPortRoboWindow->Height );
	assert( m_pAsmModel );
	//パーツ選択中は選択しているパーツを光らせる.
	if( isMessageBoxClose() )
	{
		m_pAsmModel->Render( m_mView, m_mProj, m_vLight, RoboViewCam.GetPos(), 
			static_cast< clsASSEMBLE_MODEL::enPARTS_TYPES >( m_PartsSelect.Type ) );
	}
	else{
		m_pAsmModel->Render( m_mView, m_mProj, m_vLight, RoboViewCam.GetPos() );
	}
	SetDepth( false );



	//パーツのステータス.
	SetViewPort( 
		GetViewPortMainPtr(), 
		m_wpCamera->GetPos(), m_wpCamera->GetLookPos(),
		WND_W, WND_H );
	assert( m_pUI );
	m_pUI->RenderPartsState( m_enSelectMode, m_PartsSelect.Type, m_PartsSelect.Num[m_PartsSelect.Type] );


	//箱.
	assert( m_upBox );
	m_upBox->Render();

	//メッセボックスの上に置く文字たち.
	if( m_upBox->isStopChange() && m_upBox->GetSize().x ){
		//選択肢のタイトル.
		m_wpFont->SetPos( vFONT_MESSAGE_BOX_TITLE_POS );
		m_wpFont->SetScale( fFONT_MESSAGE_BOX_TITLE_SCALE );
		m_wpFont->Render( m_iMessageNum );

		//選択肢のボタン.
		//yes.
		POINTFLOAT tmpAnim = MESSAGEBOX_YES_BUTTON_ANIM;
		if( m_isMessageBoxYes ){
			tmpAnim.y = fMESSAGEBOX_YES_NO_BUTTON_SELECT_ANIM;
		}
		m_upYesNo->SetPos( vMESSAGEBOX_YES_BUTTON_POS );
		m_upYesNo->SetAnim( tmpAnim );
		m_upYesNo->Render();
		//no.
		tmpAnim = MESSAGEBOX_NO_BUTTON_ANIM;
		if( !m_isMessageBoxYes ){
			tmpAnim.y = fMESSAGEBOX_YES_NO_BUTTON_SELECT_ANIM;
		}
		m_upYesNo->SetPos( vMESSAGEBOX_NO_BUTTON_POS );
		m_upYesNo->SetAnim( tmpAnim );
		m_upYesNo->Render();
	}

}

//カーソル移動.
//カーソル移動の共通動作.
void clsSCENE_ASSEMBLE::MoveCursor()
{
//	m_wpSound->PlaySE( enSE_CURSOL_MOVE );
}


void clsSCENE_ASSEMBLE::MoveCursorUp()
{
	MoveCursor();

	//パーツカテゴリを選んでないならパーツを選ばせないよ.
	if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::PARTS ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );

		m_PartsSelect.Num[m_PartsSelect.Type] --;

		m_PartsSelect.Num[m_PartsSelect.Type] = 
			LoopRange( m_PartsSelect.Num[m_PartsSelect.Type], 0, m_vspFile[m_PartsSelect.Type]->GetSizeRow() );
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::STATUS ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		m_pUI->AddStatusCommentNo( false );
	}
}

void clsSCENE_ASSEMBLE::MoveCursorDown()
{
	MoveCursor();

	//パーツカテゴリを選んでないならパーツを選ばせないよ.
	if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::PARTS ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );

		m_PartsSelect.Num[m_PartsSelect.Type] ++;

		m_PartsSelect.Num[m_PartsSelect.Type] = 
			LoopRange( m_PartsSelect.Num[m_PartsSelect.Type], 0, m_vspFile[m_PartsSelect.Type]->GetSizeRow() );
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::STATUS ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		m_pUI->AddStatusCommentNo( true );
	}

}

void clsSCENE_ASSEMBLE::MoveCursorRight()
{
	MoveCursor();

	if( isMessageBoxClose() ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		m_PartsSelect.Type ++;
		m_pUI->AddCommentNoForChangePartsType();

//		//武器を超えたら.
//		if( m_PartsSelect.Type >= clsASSEMBLE_MODEL::ENUM_SIZE ){
//			//出撃.
//			m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::MISSION_START;
//		}

		m_PartsSelect.Type = 
			LoopRange( m_PartsSelect.Type, 0, clsASSEMBLE_MODEL::ENUM_SIZE );
	}
	//メッセボックスの選択肢.
	else{
		if( m_isMessageBoxYes ){
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
			m_isMessageBoxYes = false;
		}
	}
}

void clsSCENE_ASSEMBLE::MoveCursorLeft()
{
	MoveCursor();

	//パーツを選ぶ.
	if( isMessageBoxClose() ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		m_PartsSelect.Type --;
		m_pUI->AddCommentNoForChangePartsType();

		m_PartsSelect.Type = 
			LoopRange( m_PartsSelect.Type, 0, clsASSEMBLE_MODEL::ENUM_SIZE );
	}
	//メッセボックスの選択肢.
	else{
		if( !m_isMessageBoxYes ){
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
			m_isMessageBoxYes = true;
		}
	}
}

//決定.
void clsSCENE_ASSEMBLE::Enter( enSCENE &enNextScene )
{
//	m_wpSound->PlaySE( enSE::ENTER );

	if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::PARTS ){
		AssembleParts();
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::MISSION_START ){
		if( m_isMessageBoxYes ){ 
			MissionStart( enNextScene );
		}
		else{
			DisAppearMessageBox();
		}
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::TITLE_BACK ){
		if( m_isMessageBoxYes ){ 
			TitleBack( enNextScene );
		}
		else{
			DisAppearMessageBox();
		}
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

//出撃.
void clsSCENE_ASSEMBLE::MissionStart( enSCENE &enNextScene )
{
	m_wpSound->PlaySE( enSE_MISSION_START );
	m_wpSound->PlaySE( enSE_ENTER );

	enNextScene = enSCENE::MISSION;
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::MISSION_START;

	m_wpSound->PlaySE( enSE_MISSION_START );
}

//パーツ変更.
void clsSCENE_ASSEMBLE::AssembleParts()
{
	m_wpSound->PlaySE( enSE_ENTER );

	//ステータスが何項目あるのか.
	const int iStatusSize = m_vspFile[ m_PartsSelect.Type ]->GetSizeCol() - iSTATUS_CUT_NUM;

	//引数用変数.
	vector<int> tmpStatus;
	tmpStatus.reserve( iStatusSize );
	for( int i=0; i<iStatusSize; i++ ){
		//m_vspFile[]の添え字はどのパーツか、である.
		tmpStatus.push_back( 
			m_vspFile[ m_PartsSelect.Type ]->
				GetDataInt( m_PartsSelect.Num[m_PartsSelect.Type], i + iSTATUS_CUT_NUM ) );
		//GetDataInt()の第一引数は、そのパーツ部位の何番目の行を参照すればよいのか.
		//第二引数でiSTATUS_CUT_NUMを足しているのは、元の表にあるパーツ番号と名前はいらないからカットするためである.
	}

	//何度もキャストをするのは嫌なので.
	UCHAR tmpPartsNum = static_cast<UCHAR>( m_PartsSelect.Num[m_PartsSelect.Type] );

	switch( m_PartsSelect.Type )
	{
	case clsASSEMBLE_MODEL::LEG:
		m_wpRoboStatus->ReceiveLeg( tmpStatus,		tmpPartsNum );
		m_pAsmModel->AttachModel( enPARTS::LEG,		tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::CORE:
		m_wpRoboStatus->ReceiveCore( tmpStatus,		tmpPartsNum );
		m_pAsmModel->AttachModel( enPARTS::CORE,	tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::HEAD:
		m_wpRoboStatus->ReceiveHead( tmpStatus,		tmpPartsNum );
		m_pAsmModel->AttachModel( enPARTS::HEAD,	tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::ARMS:
		m_wpRoboStatus->ReceiveArms( tmpStatus,		tmpPartsNum );
		m_pAsmModel->AttachModel( enPARTS::ARM_L,	tmpPartsNum );
		m_pAsmModel->AttachModel( enPARTS::ARM_R,	tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::WEAPON_L:
		m_wpRoboStatus->ReceiveWeaponL( tmpStatus,	tmpPartsNum );
		m_pAsmModel->AttachModel( enPARTS::WEAPON_L,tmpPartsNum );
		break;
	case clsASSEMBLE_MODEL::WEAPON_R:
		m_wpRoboStatus->ReceiveWeaponR( tmpStatus,	tmpPartsNum );
		m_pAsmModel->AttachModel( enPARTS::WEAPON_R,tmpPartsNum );
		break;
	default:
		break;
	}

	//引数用変数の片づけ.
	tmpStatus.clear();
	tmpStatus.shrink_to_fit();
}


//戻る.
void clsSCENE_ASSEMBLE::TitleBack( enSCENE &enNextScene )
{
	m_wpSound->PlaySE( enSE_EXIT );
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
bool clsSCENE_ASSEMBLE::isMessageBoxClose()
{
	if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::MISSION_START ||
		m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::TITLE_BACK )
	{
		return false;
	}

	return true;
}

//メッセボックス出現.
void clsSCENE_ASSEMBLE::AppearMessageBox( 
	const clsASSEMBLE_UI::enSELECT_MODE encMode )
{
	m_upBox->SetSizeTarget( vBOX_SIZE );
	m_upBox->AddChangeData( fBOX_BIG_SPD_W, fBOX_BIG_SPD_H, encBOX_APPEAR_CHANGE_MODE );
	m_enSelectMode = encMode;
	if( encMode == clsASSEMBLE_UI::enSELECT_MODE::MISSION_START ){
		m_wpSound->PlaySE( enSE_WIN_APP );
//		m_wpSound->PlaySE( enSE_ENTER );
		m_iMessageNum = iBOX_MESSAGE_LINE_GO_MISSION;
	}
	else if( encMode == clsASSEMBLE_UI::enSELECT_MODE::TITLE_BACK ){
		m_wpSound->PlaySE( enSE_WIN_APP );
//		m_wpSound->PlaySE( enSE_EXIT );
		m_iMessageNum = iBOX_MESSAGE_LINE_BACK_TITLE;
	}
}
//メッセボックス消す.
void clsSCENE_ASSEMBLE::DisAppearMessageBox()
{
	m_wpSound->PlaySE( enSE_WIN_DISAPP );

	m_upBox->SetSizeTarget( { 0.0f, 0.0f, 0.0f } );
	m_upBox->AddChangeData( 
		m_upBox->GetSize().x * fBOX_SMALL_SPD_RATE, 
		m_upBox->GetSize().y * fBOX_SMALL_SPD_RATE, 
		encBOX_DISAPPEAR_CHANGE_MODE );
	//選択肢を選べるように戻す.
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::PARTS;
	//初期化.
	m_isMessageBoxYes = true;
}


//ステータスの表示非表示切替.
void clsSCENE_ASSEMBLE::SwitchDispStatus()
{
	//ウィンドウを出してるときは無視.
	if( !isMessageBoxClose() ){
		return;
	}

	m_wpSound->PlaySE( enSE_CURSOL_MOVE );
	m_pUI->SwitchDispStatusComment();
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::PARTS;
}

//パーツ選択とステータス選択の切り替え.
void clsSCENE_ASSEMBLE::ChangePartsSelect()
{
	m_wpSound->PlaySE( enSE_EXIT );
	m_pUI->SwitchStatusComment();
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::PARTS;
}

void clsSCENE_ASSEMBLE::ChangeStatusSelect()
{
	m_wpSound->PlaySE( enSE_ENTER );
	m_pUI->SwitchStatusComment();
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::STATUS;
}



//============================ デバッグテキスト ===========================//
#if _DEBUG
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
//		m_pAsmModel->GetPos().x, 
//		m_pAsmModel->GetPos().y, 
//		m_pAsmModel->GetPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"ModelRot : x[%f], y[%f], z[%f]",
//		m_pAsmModel->GetRot().x, 
//		m_pAsmModel->GetRot().y, 
//		m_pAsmModel->GetRot().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	//各パーツのpos.
//	for( UCHAR ucNo=0; ucNo<static_cast<UCHAR>( enPARTS::MAX ); ucNo++ ){
//		sprintf_s( strDbgTxt, 
//			"PartsPos : x[%f], y[%f], z[%f]",
//			m_pAsmModel->GetPartsPos( ucNo ).x, 
//			m_pAsmModel->GetPartsPos( ucNo ).y, 
//			m_pAsmModel->GetPartsPos( ucNo ).z );
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
		m_pUI->GetUiPos().x, m_pUI->GetUiPos().y );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

//	//日本語文字.[400:-400,200][200:-200,100][50:-50,25]
//	sprintf_s( strDbgTxt, 
//		"Font : x[%f], y[%f], Scale[%f]",
//		setumeiX, setumeiY, m_wpFont->GetScale() );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

//	//テスト用に数値を出す.
//	string tmpsString;
//	tmpsString = m_vspFile[m_PartsSelect.Type]->GetDataString( m_PartsSelect.Num[m_PartsSelect.Type] );
//	const char* tmpcString = tmpsString.c_str();
//	sprintf_s( strDbgTxt, 
//		tmpcString );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );



}
#endif //#if _DEBUG



