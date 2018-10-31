#include "SceneAssemble.h"
#include "WindowBox.h"
#include "SceneAssembleInformation.h"

#include "MenuWindowAssembleBackTitle.h"
#include "MenuWindowAssembleTakeOff.h"
#include "MenuWindowAssembleColorChange.h"

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
const float fBOX_ALPHA = 0.75f;
const D3DXVECTOR3 vBOX_POS = { WND_W/2, WND_H/2, 0.0f };//シーン移動.
const D3DXVECTOR3 vBOX_SIZE = { WND_W/2, WND_H/2, 0.0f };
const D3DXVECTOR3 vBOX_POS_COLOR = { 380.0f, WND_H/2, 0.0f };//色替え.
const D3DXVECTOR3 vBOX_SIZE_COLOR = { 678.0f, 500.0f, 0.0f };
const float fBOX_BIG_SPD_W = 60.0f;
const float fBOX_BIG_SPD_H = 40.0f;
const clsLINE_BOX::encBEFOR_CHANGE encBOX_APPEAR_CHANGE_MODE = clsLINE_BOX::encBEFOR_CHANGE::WIDTH;
const float fBOX_SMALL_SPD_RATE = 1.0 / 16.0f;
const clsLINE_BOX::encBEFOR_CHANGE encBOX_DISAPPEAR_CHANGE_MODE = clsLINE_BOX::encBEFOR_CHANGE::BOTH;
const int iBOX_MESSAGE_LINE_GO_MISSION = 1;
const int iBOX_MESSAGE_LINE_BACK_TITLE = iBOX_MESSAGE_LINE_GO_MISSION + 1;
const int iBOX_MESSAGE_LINE_COLOR_CHANGE = iBOX_MESSAGE_LINE_BACK_TITLE + 1;


//色ゲージ枠.
const char sCOLOR_GAGE_PATH_BONE[] = "Data\\Image\\AssembleUi\\ColorGageBone.png";
const WHSIZE_FLOAT COLOR_GAGE_SIZE_BONE = { 480.0f, 32.0f };
const D3DXVECTOR3 vCOLOR_GAGE_POS_BONE  = { 420.0f, 240.0f, 0.0f };
const float fCOLOR_GAGE_OFFSET_BONE = COLOR_GAGE_SIZE_BONE.h + 20.0f;
const float fCOLOR_GAGE_OFFSET_BONE_2 =  24.0f;//パーツのベースと装甲の合間の追加オフセット.
//ゲージ本編.
const char sCOLOR_GAGE_PATH[] = "Data\\Image\\AssembleUi\\ColorGage.png";
const WHSIZE_FLOAT COLOR_GAGE_SIZE = { 32.0f, 32.0f };
const float fCOLOR_GAGE_ALPHA = 0.5f;
const D3DXVECTOR3 vCOLOR_GAGE_COLOR[] = {
	{ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f },
};

//YesNo.
//文字.
const int iMESSAGEBOX_YES_NO_INDEX_YES = 4;
const int iMESSAGEBOX_YES_NO_INDEX_NO = 5;
const D3DXVECTOR3 iMESSAGEBOX_YES_NO_YES_POS = { 525.0f, 450.0f, 0.0f };
const D3DXVECTOR3 iMESSAGEBOX_YES_NO_NO_POS  = { 725.0f, 450.0f, 0.0f };
const float fMESSAGE_BOX_YES_NO_SCALE = 18.0f;
//スプライト.
const D3DXVECTOR3 vMESSAGE_BOX_YES_NO_SCALE = { 2.5f, 1.25f, 0.0f };
const D3DXVECTOR3 vMESSAGE_BOX_YES_NO_OFFSET_POS_YES = { 29.0f, 11.0f, 0.0f };
const D3DXVECTOR3 vMESSAGE_BOX_YES_NO_OFFSET_POS_NO  = { 20.0f, 11.0f, 0.0f };


//日本語UI.
const char* sFONT_TEXT_PATH_ASSEMBLE = "Data\\Font\\Text\\TextAssemble.csv";
//ボタン説明.
const D3DXVECTOR3 vFONT_BUTTON_POS = { 650.0f, 40.0f, 0.0f };
const float fFONT_BUTTON_SCALE = 14.0f;
const int iFONT_BUTTON_LINE = 0;

//出撃メッセージ.
const D3DXVECTOR3 vFONT_MESSAGE_BOX_TITLE_POS = { 480.0f, 230.0f, 0.0f };
const float fFONT_MESSAGE_BOX_TITLE_SCALE = 24.0f;

//色替えメッセージ.
const D3DXVECTOR3 vFONT_MESSAGE_BOX_COLOR_CHANGE = { 250.0f, 140.0f, 0.0f };

//色替えtext.
const float fCOLOR_CHANGE_BOX_IN_TEXT_RGB_SCALE = 2.0f;
const float COLOR_CHANGE_BOX_IN_TEXT_RGB_OFFSET_X = -30.0f;
const float COLOR_CHANGE_BOX_IN_TEXT_RGB_OFFSET_Y = -10.0f;
const string sCOLOR_CHANGE_BOX_IN_TEXT_RGB_TEXT[] = {
	"R", "G", "B",
	"R", "G", "B",
}; 

const float fCOLOR_CHANGE_BOX_IN_TEXT_COLOR1_2_SCALE = 2.5f;
D3DXVECTOR2 vCOLOR_CHANGE_BOX_IN_TEXT_COLOR1_2_POS = { 60.0f, 190.0f };
float fCOLOR_CHANGE_BOX_IN_TEXT_COLOR1_2_POS_ADD_Y = fCOLOR_GAGE_OFFSET_BONE * 3 + fCOLOR_GAGE_OFFSET_BONE_2; 
const string sCOLOR_CHANGE_BOX_IN_TEXT_COLOR1_2_TEXT[] = {
	"Color1", "Color2"
};

const char sCOLOR_CHANGE_BOX_IN_SELECT_COLOR_PATH[] = "Data\\Image\\AssembleUi\\ColorSelect.png"; 
const WHSIZE_FLOAT COLOR_CHANGE_BOX_IN_SELECT_COLOR_SIZE = { 24.0f, 24.0f };

//色の戻るテキスト.
//本体.
const D3DXVECTOR3 vCOLOR_CHANGE_BACK_TEXT_POS = { 90.0f, 560.0f, 0.0f };
const float fCOLOR_CHANGE_BACK_TEXT_SCALE = 18.0f;
const int iCOLOR_CHANGE_BACK_TEXT_NUMBER = 6;
//背景.
const D3DXVECTOR3 vCOLOR_CHANGE_BACK_TEXT_BACK_COLOR_POS = { 112.0f, 570.0f, 0.0f };

//色の選択肢の数.
const char cCOLOR_CHANGE_INDEX_MAX = clsROBO_STATUS::enCOLOR_GAGE_size + 1;
const char cCOLOR_CHANGE_INDEX_DIS_WINDOW = clsROBO_STATUS::enCOLOR_GAGE_size;

//ズーム限界.
const float fZOOM_RIMIT_MIN = -100.0f;
const float fZOOM_RIMIT_MAX = 35.0f;



//================================//
//========== 組み換えクラス ==========//
//================================//
clsSCENE_ASSEMBLE::clsSCENE_ASSEMBLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_pAsmModel( nullptr )
	,m_pViewPortPartsWindow( nullptr )
	,m_pViewPortRoboWindow( nullptr )
	,m_pSelectParts( nullptr )
	,m_fDistanceAssembleModel( 0.0f )
{
	m_enSelectMode = clsASSEMBLE_UI::enSELECT_MODE::PARTS;

}

clsSCENE_ASSEMBLE::~clsSCENE_ASSEMBLE()
{

	SAFE_DELETE( m_pViewPortRoboWindow );
	SAFE_DELETE( m_pViewPortPartsWindow );
	SAFE_DELETE( m_pAsmModel );

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

	m_wpSound->PlayBGM( enBGM_RENGOKU0 );


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


	//UI.
	assert( !m_upUI );
	m_upUI = make_unique<clsASSEMBLE_UI>( m_wpFont );
	m_upUI->Create( m_wpDevice, m_wpContext, partsData );

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
	assert( !m_pSelectParts );
	m_pSelectParts = new clsPARTS_WINDOW_MODEL( m_wpResource, m_wpRoboStatus );


	//照合用情報の作成の為のファイルデータ取得.
	const char sTITLE_INFORMATION_DATA_PATH[] = "Data\\FileData\\Tahara\\AssembleMenuInformation.csv";
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	upFile->Open( sTITLE_INFORMATION_DATA_PATH );
	//照合用情報の作成.
	m_vecuiInformationDataArray.resize( enINFORMATION_INDEX_size );
	for( char i=0; i<enINFORMATION_INDEX_size; i++ ){
		const int iCOL = 0;
		assert( static_cast<unsigned int>( i ) < upFile->GetSizeRow() );
		m_vecuiInformationDataArray[i] = static_cast<unsigned int>( upFile->GetDataInt( i, iCOL ) );
	}
	upFile.reset();



}

void clsSCENE_ASSEMBLE::UpdateProduct( enSCENE &enNextScene )
{
	assert( m_wpSound );

	//暗転中は操作不能.
	bool isCanControl = true;
	if( m_wpBlackScreen->GetAlpha() ){
		isCanControl = false;
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


	if( m_upMenu ){
		MenuUpdate( enNextScene );
	}
	else{
		//操作.
		if( isCanControl ){
			//選択肢.
			if( isPressHoldRight()	)MoveCursorRight();
			if( isPressHoldLeft()	)MoveCursorLeft();
			if( isPressHoldUp()		)MoveCursorUp();
			if( isPressHoldDown()	)MoveCursorDown();
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
			//カラーチェンジ.
			if( m_wpXInput->isPressEnter( XINPUT_BACK ) ||
				GetAsyncKeyState( VK_SHIFT ) & 0x1 )
			{
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
			if( m_wpXInput->isPressEnter( XINPUT_Y ) ){
				SwitchDispStatus();
			}
			//ステータスのcomment切替.
			if( m_wpXInput->isPressEnter( XINPUT_X ) )
			{
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

	//モデル回転.
	const float fMODEL_SPN_SPD = 0.05f;
	if( m_wpXInput->isSlopeStay( XINPUT_RIGHT, false ) ){
		m_pAsmModel->AddRot( { 0.0f, -fMODEL_SPN_SPD, 0.0f } );
	}
	if( m_wpXInput->isSlopeStay( XINPUT_LEFT, false ) ){
		m_pAsmModel->AddRot( { 0.0f, fMODEL_SPN_SPD, 0.0f } );
	}
	//モデルズーム.
	const float fMODEL_MOVE_SPD = 5.0f;
	if( m_wpXInput->isSlopeStay( XINPUT_UP, false ) ){
		m_fDistanceAssembleModel += fMODEL_MOVE_SPD;
		if( m_fDistanceAssembleModel > fZOOM_RIMIT_MAX ){
			m_fDistanceAssembleModel = fZOOM_RIMIT_MAX;
		}
	}
	if( m_wpXInput->isSlopeStay( XINPUT_DOWN, false ) ){
		m_fDistanceAssembleModel -= fMODEL_MOVE_SPD;
		if( m_fDistanceAssembleModel < fZOOM_RIMIT_MIN ){
			m_fDistanceAssembleModel = fZOOM_RIMIT_MIN;
		}
	}


	assert( m_upUI );
	m_upUI->Input( m_wpXInput, m_wpDxInput );
	if( isMessageBoxClose() ){
		assert( m_vspFile[m_PartsSelect.Type] );
		m_upUI->Update( 
			m_enSelectMode,
			m_vspFile[m_PartsSelect.Type], 
			m_pAsmModel,
			m_PartsSelect.Type, 
			m_PartsSelect.Num[m_PartsSelect.Type], 
			iSTATUS_CUT_NUM );
	}
	else if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::MISSION_START ){
		m_upUI->Update( m_enSelectMode );
	}


	assert( m_pAsmModel );
	m_pAsmModel->UpDate();

	assert( m_pSelectParts );
	m_pSelectParts->SetColors( m_pAsmModel->GetColor() );
	m_pSelectParts->Update( m_PartsSelect.Type, m_PartsSelect.Num[ m_PartsSelect.Type ] );


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
	RoboViewCam.SetPos( vROBO_VIEW_CAM_POS );;
	RoboViewCam.AddPos( { 0.0f, 0.0f, m_fDistanceAssembleModel } );
	RoboViewCam.SetLookPos( vROBO_VIEW_CAM_LOOK );
	RoboViewCam.AddPos( m_pAsmModel->GetBonePos( enPARTS::LEG, sBONE_NAME_LEG_TO_CORE ) );


	//背景.
	assert( m_upBack );
	m_upBack->Render();

	//ほとんどのUI.
	assert( m_upUI );
	m_upUI->Render( m_enSelectMode, m_PartsSelect.Type, m_PartsSelect.Num[m_PartsSelect.Type] );


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
	const D3DXVECTOR4 vFONT_TEXT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_wpFont->SetColor( vFONT_TEXT_COLOR );
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
	assert( m_upUI );
	m_upUI->RenderPartsState( m_enSelectMode, m_PartsSelect.Type, m_PartsSelect.Num[m_PartsSelect.Type] );


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

		m_PartsSelect.Num[m_PartsSelect.Type] --;

		m_PartsSelect.Num[m_PartsSelect.Type] = 
			LoopRange( m_PartsSelect.Num[m_PartsSelect.Type], 0, m_vspFile[m_PartsSelect.Type]->GetSizeRow() );
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

		m_PartsSelect.Num[m_PartsSelect.Type] ++;

		m_PartsSelect.Num[m_PartsSelect.Type] = 
			LoopRange( m_PartsSelect.Num[m_PartsSelect.Type], 0, m_vspFile[m_PartsSelect.Type]->GetSizeRow() );
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
		m_upUI->AddCommentNoForChangePartsType();


		m_PartsSelect.Type = 
			LoopRange( m_PartsSelect.Type, 0, clsASSEMBLE_MODEL::ENUM_SIZE );
	}
}

void clsSCENE_ASSEMBLE::MoveCursorLeft()
{
	//パーツを選ぶ.
	if( isMessageBoxClose() ){
		m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		m_PartsSelect.Type --;
		m_upUI->AddCommentNoForChangePartsType();

		m_PartsSelect.Type = 
			LoopRange( m_PartsSelect.Type, 0, clsASSEMBLE_MODEL::ENUM_SIZE );
	}
}



//決定.
void clsSCENE_ASSEMBLE::Enter( enSCENE &enNextScene )
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

//色替え( 左右キーを押された ).
void clsSCENE_ASSEMBLE::AddRoboColor( const bool isIncrement )
{
	if( m_enColorGageIndex >= clsROBO_STATUS::enCOLOR_GAGE::enCOLOR_GAGE_size ){
		return;
	}

	assert( m_pAsmModel );
	//右.
	if( isIncrement ){
		if( m_pAsmModel->IncrementColor( m_enColorGageIndex ) ){
			//動いたなら音を鳴らす.
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		}
	}
	//左.
	else{
		if( m_pAsmModel->DecrementColor( m_enColorGageIndex ) ){
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		}
	}

	//色の保存.
	for( char i=0; i<clsROBO_STATUS::enCOLOR_GAGE_size; i++ ){
		clsROBO_STATUS::enCOLOR_GAGE tmpIndex = static_cast<clsROBO_STATUS::enCOLOR_GAGE>( i );
		int iColorRank = m_pAsmModel->GetColorRank( tmpIndex );
		m_wpRoboStatus->SetColorRank( tmpIndex, iColorRank );
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
//	m_wpSound->PlaySE( enSE_ENTER );

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
bool clsSCENE_ASSEMBLE::isMessageBoxClose()
{
	if( m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::MISSION_START ||
		m_enSelectMode == clsASSEMBLE_UI::enSELECT_MODE::TITLE_BACK ||
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
			m_wpPtrGroup, nullptr, &m_vecuiInformationDataArray );
	}
	else if( encMode == clsASSEMBLE_UI::enSELECT_MODE::TITLE_BACK ){
		m_upMenu = make_unique< clsMENU_WINDOW_ASSEMBLE_BACK_TITLE >(
			m_wpPtrGroup, nullptr, &m_vecuiInformationDataArray );
	}
	else if( encMode == clsASSEMBLE_UI::enSELECT_MODE::COLOR_CHANGE ){
		m_enColorGageIndex = static_cast<clsROBO_STATUS::enCOLOR_GAGE>( 0 );
		m_upMenu = make_unique< clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE >(
			m_wpPtrGroup, nullptr, &m_vecuiInformationDataArray, m_pAsmModel );
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
		m_upUI->GetUiPos().x, m_upUI->GetUiPos().y );
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



