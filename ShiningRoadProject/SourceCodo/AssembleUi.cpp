#include "AssembleUi.h"

#include "CXInput.h"
#include "DxInput.h"
#include "Sprite2DCenter.h"
#include "UiText.h"
#include "File.h"
#include "CFont.h"
#include "AssembleModel.h"
#include "WindowBox.h"

#include "OperationString.h"

//これがついているとフォントサイズは2.0f, 無しだと1.5f.
#define FONT_SIZA_BIG_VER


using namespace std;

#define DEBUG_SPRITE_NAME m_vecupStatusText[0]

namespace{



	//----- パーツカテゴリ -----//.
	//パーツカテゴリサイズ.
	const WHSIZE_FLOAT PARTS_TYPE_SIZE = { 102.0f, 70.0f };
	//パーツカテゴリUIの基準位置.
	const D3DXVECTOR3 vINIT_POS_PARTS_TYPE = { 32.0f, 87.0f, 0.0f };
	//パーツカテゴリの座標の差.
	const float fOFFSET_POS_X_PARTS_TYPE = PARTS_TYPE_SIZE.w + 18.0f;
	//パーツカテゴリアイコンへのパス.
	const char* sPATH_PARTS_TYPE_PATH = "Data\\Image\\AssembleUi\\IconEmpty.png";
	//----- パーツカテゴリ 終わり -----//.

	//選択中パーツカテゴリパス.
	const char* sPATH_SELECT_PARTS_TYPE = "Data\\Image\\AssembleUi\\SelectPartsType.png";
	const float fALPHA_SELECT_PARTS_TYPE = 0.5f;





	//----- 各パーツUI -----//.
	//サイズ.
	const WHSIZE_FLOAT PARTS_ICON_SIZE = PARTS_TYPE_SIZE;
	//UI同士の隙間.
	const float fPARTS_ICON_OFFSET = 18.0f;
	//カテゴリからの隙間.
	const float fPARTS_ICON_OFFSET_FROM_PARTS_TYPE = 18.0f;
	//基準位置.
	const D3DXVECTOR3 vINIT_POS_PARTS_ICON = { vINIT_POS_PARTS_TYPE.x, 176.0f, 0.0f };

	//各パーツへのアイコンパスへの道筋.
	const string sPATH_PARTS_ICON_ROOT = "Data\\RoboParts\\";
	//各パーツへのアイコンパスへの道筋( 各パーツ ).
	const string sPATH_PARTS_ICON_PARTS[] = {
		"Leg", "Core", "Head", "Arms", "Weapon", "Weapon"
	};
	//アイコンのファイル名.
	const char* sPATH_PARTS_ICON_END = "\\Icon.png";


	//エラー画像.
	const string sNO_DATA_FILE_NAME = "Data\\Image\\PartsIcon\\NoData.png";
	//----- 各パーツUI 終わり -----//.

	//選択中各パーツUIパス.
	const char* sPATH_SELECT_PARTS_NUM = "Data\\Image\\AssembleUi\\SelectPartsNum.png";
	const float fALPHA_SELECT_PARTS_NUM = fALPHA_SELECT_PARTS_TYPE;



	//----- パーツウィンドウ -----//.
	//478.25f.
	const D3DXVECTOR3 vINIT_POS_PARTS_WINDOW = 
		{ 153.0f, 176.0f, 0.0f };
	 const WHSIZE_FLOAT INIT_SIZE_PARTS_WINDOW  = { 576.0f, 482.0f };
	const char* sPATH_PARTS_WINDOW = "Data\\Image\\AssembleUi\\PartsWindow.png";

	//文字( ステータスウィンドウを基準にしている ).
	const float TEXT_SCALE_PARTS_NAME = 2.0f;
	const D3DXVECTOR2 vTEXT_POS_PARTS_NAME = {
		vINIT_POS_PARTS_WINDOW.x + 4.5f,
		vINIT_POS_PARTS_WINDOW.y + 16.25f 
	};
	const float fPARTS_WINDOW_ALPHA = 0.5f;
	//----- パーツウィンドウ終わり -----//.


	//----- ロボウィンドウ -----//.
	const char* sROBO_WINDOW_PATH = "Data\\Image\\AssembleUi\\RoboWindow.png";
	const WHSIZE_FLOAT INIT_SIZE_ROBO_WINDOW  = { 490.0f, 570.0f };
	const D3DXVECTOR3 INIT_POS_ROBO_WINDOW = { 753.0f, 87.0f, 0.0f };
	const float fROBO_WINDOW_ALPHA = 0.5f;
	//----- ロボウィンドウ 終わり -----//.




	//----- ステータスウィンドウ -----//.
	const WHSIZE_FLOAT INIT_SIZE_STATUS_WINDOW  = { INIT_SIZE_PARTS_WINDOW.w, 24.0f };//360.0f.
	const D3DXVECTOR3 vINIT_POS_STATUS_WINDOW = { 
		vINIT_POS_PARTS_WINDOW.x, 
		vINIT_POS_PARTS_WINDOW.y + INIT_SIZE_PARTS_WINDOW.h - INIT_SIZE_STATUS_WINDOW.h, 
		0.0f };//179.5f;
	const char* sPATH_STATUS_WINDOW = "Data\\Image\\AssembleUi\\StatusWindow.png";
	const float fSTATUS_WINDOW_ALPHA = 0.75f;
	const int iSTATUS_NUM_MAX = 11;//ステータスの最大数.

	//選択中ステータス.
	const char* sPATH_STATUS_COMMENT = "Data\\Image\\AssembleUi\\SelectStatus.png";
	const float fSTATUS_COMMENT_ALPHA = 0.5f;

	//ステータス文字.
	//この窓のタイトル.
	const float TEXT_SCALE_STATUS_TITLE = 2.0f;
	const D3DXVECTOR2 vTEXT_POS_STATUS_TITLE_OFFSET_TO_STATUS_WINDOW = { 4.5f, 8.25f };
	const D3DXVECTOR2 vTEXT_POS_STATUS_TITLE = {
		vINIT_POS_STATUS_WINDOW.x + vTEXT_POS_STATUS_TITLE_OFFSET_TO_STATUS_WINDOW.x,
		vINIT_POS_STATUS_WINDOW.y + vTEXT_POS_STATUS_TITLE_OFFSET_TO_STATUS_WINDOW.y };
	const char* sSTATUS_TITLE_TEXT = "Parts Status";

	
	//項目文字の座標.
	const D3DXVECTOR2 vTEXT_POS_OFFSET_TO_STATUS_WINDOW = { 5.0f, 8.75f };//窓からのずれ.
	const float fTEXT_POS_Y_OFFSET_STATUS = INIT_SIZE_STATUS_WINDOW.h;//二行目以降のずれ幅.
#ifdef FONT_SIZA_BIG_VER
	const float fTEXT_SCALE_STATUS = 2.0f;//文字サイズ.
	const float fTEXT_STTATUS_POS_Y = vINIT_POS_STATUS_WINDOW.y - vTEXT_POS_OFFSET_TO_STATUS_WINDOW.y - 4.0f;
#else //#ifdef FONT_SIZA_BIG_VER
	const float fTEXT_SCALE_STATUS = 1.5f;
	const float fTEXT_STTATUS_POS_Y = vINIT_POS_STATUS_WINDOW.y - vTEXT_POS_OFFSET_TO_STATUS_WINDOW.y;
#endif//#ifdef FONT_SIZA_BIG_VER
	const D3DXVECTOR2 vTEXT_POS_STATUS = {
		vINIT_POS_STATUS_WINDOW.x + vTEXT_POS_OFFSET_TO_STATUS_WINDOW.x,
		fTEXT_STTATUS_POS_Y
		+ ( 12.0f ) };
	//値文字の座標.
	const D3DXVECTOR2 vTEXT_POS_STATUS_NUM = 
		{ vTEXT_POS_STATUS.x + INIT_SIZE_STATUS_WINDOW.w - 12.0f, vTEXT_POS_STATUS.y };
	//ビフォアをどれくらい左にずらすか.
	const float fTEXT_POS_YX_OFFSET_STATUS_NOW = -64.0f;
	//今のステータスとみているパーツの間に挟むもの.
	const string sSTATUS_TEXT_MIDWAY = "     >>";
	//----- ステータスウィンドウ終わり -----//.




	//----- ヘッダーとフッター -----//.
	const char* sHEADER_FILE_PATH = "Data\\Image\\AssembleUi\\Header.png";
	const char* sFOOTER_FILE_PATH = "Data\\Image\\AssembleUi\\Footer.png";
	const WHSIZE_FLOAT INIT_SIZE_HEADER  = { WND_W, 54.0f };
	const WHSIZE_FLOAT INIT_SIZE_FOOTER  = { WND_W, 30.0f };
	const D3DXVECTOR3 INIT_POS_HEADER = { 0.0f, 14.0f, 0.0f };
	const D3DXVECTOR3 INIT_POS_FOOTER = { 0.0f, WND_H - INIT_SIZE_FOOTER.h - 15.0f, 0.0f };
	const float fHEADER_ALPHA = 0.5f;
	const float fFOOTER_ALPHA = 0.5f;
	//文字.
	const float fTEXT_SCALE_HEADER = 4.0f;
	const D3DXVECTOR2 vTEXT_POS_OFFSET_HEADER = { 57.0f, 7.0f };//画像とのずれ.
	const D3DXVECTOR2 vTEXT_POS_HEADER = { 
		INIT_POS_HEADER.x + vTEXT_POS_OFFSET_HEADER.x, 
		INIT_POS_HEADER.y + vTEXT_POS_OFFSET_HEADER.y };
	const char* sHEADER_TEXT = "ASSEMBLE";
	//----- ヘッダーとフッター 終わり -----//.




	#if _DEBUG
	//目安.
	const char* sPATH_DESIGN = "Data\\Image\\AssembleDesign.png";
	const D3DXVECTOR3 vINIT_POS_DESIGN = { 0.0f, 0.0f, 0.0f };
	const float fINIT_SCALE_DESIGN = 0.1875f;
	#endif//#if _DEBUG



	//パーツ名が格納されているデータ番号.
	const int iSTATUS_PARTS_NAME_NUM = 1;//.
	//隠さないステータスの数.
	const int iOPEN_STATUS_NUM[] =
	{ 5, 8, 4, 5, 9, 9 };
	//ステータスの名前.
	const char* sHP_NAME = "Armor Point";
	const string sSTATUS_NAME_LEG[] = 
		{ sHP_NAME, "Walk Speed", "Stability", "Turning", "Jump Power"  };
	const string sSTATUS_NAME_CORE[] = 
		{ sHP_NAME, "EN Capacity", "EN Output", "Boost Horizontal Power", "Boost Horizontal Cost", "Boost Vertical Power", "Boost Vertical Cost", "Activity Time" };
	const string sSTATUS_NAME_HEAD[] = 
		{ sHP_NAME, "Search", "Lock on Speed", "Lock on Range" };
	const string sSTATUS_NAME_ARMS[] = 
		{ sHP_NAME, "Aiming", "Boost Quick Power", "Boost Quick Cost", "Boost Quick Time" };
	const string sSTATUS_NAME_WEAPON[] = 
		{ "Attack Power", "Bullet Speed", "Bullet Range", "EN Cost", "Load Time", "Lock on Time", "Stability of Shooting", "Magazine Load Time", "Bullets Num" };


	//ステータスの色.
	const bool bRED_BIG_LEG[] =
		{ true, true, true, true, true };		
	const bool bRED_BIG_CORE[] =
		{ true, true, true, true, false, true, false, true };			
	const bool bRED_BIG_HEAD[] =
		{ true, true, true, true };			
	const bool bRED_BIG_ARMS[] =
		{ true, true, true, false, true };			
	const bool bRED_BIG_WEAPON[] =
		{ true, true, true, false, false, false, true, false, true };
	//集合体.
	const bool* bRED_BIG[] =
		{ bRED_BIG_LEG, bRED_BIG_CORE, bRED_BIG_HEAD, bRED_BIG_ARMS, bRED_BIG_WEAPON, bRED_BIG_WEAPON };


	//日本語UI.
	//シーンですでに作っている.
	//const char* sFONT_TEXT_PATH_ASSEMBLE = "Data\\Font\\Text\\TextAssemble.csv";
	//パーツ、ステータス説明.
	const D3DXVECTOR3 vFONT_COMMENT_POS = { 28.0f, 680.0f, 0.0f };
	const float fFONT_COMMENT_SCALE = 16.0f;
	//パーツ説明以外の行数.
	const int iFONT_COMMENT_LINE = 5 + 8 + 8;

	//パーツカテゴリの脚のindex.
	const int iFONT_TEXT_PARTS_TYPE_LEGS_INDEX = 5 + 8;
	const float fFONT_TEXT_PARTS_TYPE_SCALE = 18;
	//legs, core, 等の文字offset.
	const D3DXVECTOR3 vFONT_TEXT_PARTS_TYPE_POS_OFFSET[] =
	{
		{ 8.0f, 16.0f, 0.0f },
		{ 8.0f, 16.0f, 0.0f },
		{ 8.0f, 16.0f, 0.0f },
		{ 8.0f, 16.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
	};


	//ボタン.
	const float fBUTTON_SET_TEST_OFFSET = -90.0f;
	const char* sPATH_BUTONS = "Data\\Image\\Buttons.png";
	const float INIT_DISP_BUTTON_SPRITE = 32.0f;
	const WHSIZE_FLOAT INIT_ANIM_BUTTON_SPRITE = { 10.0f, 1.0f };
	const int iBUTTON_SPRITE_NUM = 6;
	const float fBUTTON_SPRITE_POS_Y = 32.0f;
	const D3DXVECTOR3 vBUTTON_SPRITE_POS[ iBUTTON_SPRITE_NUM ] =
	{
		{ fBUTTON_SET_TEST_OFFSET + 615.0f, fBUTTON_SPRITE_POS_Y, 0.0f },
		{ fBUTTON_SET_TEST_OFFSET + 695.0f, fBUTTON_SPRITE_POS_Y, 0.0f },
		{ fBUTTON_SET_TEST_OFFSET + 775.0f, fBUTTON_SPRITE_POS_Y, 0.0f },
		{ fBUTTON_SET_TEST_OFFSET + 950.0f, fBUTTON_SPRITE_POS_Y, 0.0f },
		{ fBUTTON_SET_TEST_OFFSET + 1110.0f,fBUTTON_SPRITE_POS_Y, 0.0f },
		{ fBUTTON_SET_TEST_OFFSET + 1270.0f,fBUTTON_SPRITE_POS_Y, 0.0f },
	};
	const POINTFLOAT vBUTTON_SPRITE_ANIM[ iBUTTON_SPRITE_NUM ] =
	{
		{ 4.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 3.0f, 0.0f },
		{ 2.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 5.0f, 0.0f },
	};
	//ボタン説明文.
	const D3DXVECTOR3 vFONT_BUTTON_POS = { fBUTTON_SET_TEST_OFFSET + 650.0f, 40.0f, 0.0f };
	const float fFONT_BUTTON_SCALE = 14.0f;
	const int iFONT_BUTTON_LINE = 1;

	//画面タイトル「ASSEMBLE」.
	const D3DXVECTOR3 vFONT_TEXT_TITLE_POS = { 57.0f, 21.0f, 1.0f };
	const float fFONT_TEXT_TITLE_SCALE = 32.0f;
	const int iFONT_TEXT_TITLE_LINE = 0;

}


clsASSEMBLE_UI::clsASSEMBLE_UI( clsFont* const pFont )
	:m_iStatusNum( 0 )
	,m_isDispStatus( true )
	,m_iStatusCommentNo( 0 )
	,m_enSelectMode( enSELECT_MODE::PARTS )
	,m_bStatusCommentOffset( false )
	,m_wpFont( pFont )
	,m_iReadLinePartsComment( 0 )
	,m_iReadNumPartsComment( 0 )
{
	//次のfor文用.
	const string* tmpStatusNamePtr[enPARTS_TYPE_SIZE] =
		{ &sSTATUS_NAME_LEG[0], &sSTATUS_NAME_CORE[0], &sSTATUS_NAME_HEAD[0], &sSTATUS_NAME_ARMS[0], &sSTATUS_NAME_WEAPON[0], &sSTATUS_NAME_WEAPON[0] };

	//表示する文字列のセット.
	for( int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		//表示ステータスの数をそろえる.
		m_vecsStatusNameBoxArray[i].resize( iOPEN_STATUS_NUM[i] );

		//ステータス名文字列をセット.
		for( int j=0; j<iOPEN_STATUS_NUM[i]; j++ ){
			m_vecsStatusNameBoxArray[i][j] = tmpStatusNamePtr[i][j];
		}
	}

	m_ViewPortPartsWindow.Width		= INIT_SIZE_PARTS_WINDOW.w;
	m_ViewPortPartsWindow.Height	= INIT_SIZE_PARTS_WINDOW.h;
	m_ViewPortPartsWindow.MinDepth	= 0.0f;
	m_ViewPortPartsWindow.MaxDepth	= 1.0f;
	m_ViewPortPartsWindow.TopLeftX	= vINIT_POS_PARTS_WINDOW.x;
	m_ViewPortPartsWindow.TopLeftY	= vINIT_POS_PARTS_WINDOW.y;
}

clsASSEMBLE_UI::~clsASSEMBLE_UI()
{
#if _DEBUG
	if( m_upDegine ){
		m_upDegine.reset( nullptr );
	}
#endif//#if _DEBUG

	for( unsigned int i=0; i<m_vecupStatusNumText.size(); i++ ){
		if( m_vecupStatusNumText[i] ){
			m_vecupStatusNumText[i].reset( nullptr );
		}
	}
	m_vecupStatusNumText.clear();
	m_vecupStatusNumText.shrink_to_fit();

	for( unsigned int i=0; i<m_vecupStatusText.size(); i++ ){
		if( m_vecupStatusText[i] ){
			m_vecupStatusText[i].reset( nullptr );
		}
	}

	m_vecupStatusText.clear();
	m_vecupStatusText.shrink_to_fit();

	m_vecupArrow.clear();
	m_vecupArrow.shrink_to_fit();


	m_vecupPartsType.clear();
	m_vecupPartsType.shrink_to_fit();



	for( int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vecsStatusNameBoxArray[i].clear();
		m_vecsStatusNameBoxArray[i].shrink_to_fit();
	}

	for( int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vecupPartsIconArray[i].clear();
		m_vecupPartsIconArray[i].shrink_to_fit();
	}

	m_iStatusNum = 0;

	m_wpFont = nullptr;
}



void clsASSEMBLE_UI::Create(
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext,
		PARTS_NUM_DATA data  )
{
	//どっちの腕に武器持たせるの?の窓.
	assert( !m_upWndBox );
	m_upWndBox = make_unique< clsWINDOW_BOX >( pDevice, pContext );

	//パーツ項目初期化.
	assert( m_vecupPartsType.size() == 0 );
	SPRITE_STATE ss;
	ss.Disp = PARTS_TYPE_SIZE;
	m_vecupPartsType.resize( enPARTS_TYPE_SIZE );
	for( unsigned int i=0; i<m_vecupPartsType.size(); i++ ){
		m_vecupPartsType[i] = make_unique< clsSprite2D >();

		m_vecupPartsType[i]->Create( pDevice, pContext, sPATH_PARTS_TYPE_PATH, ss );

		m_vecupPartsType[i]->SetPos( vINIT_POS_PARTS_TYPE );
		m_vecupPartsType[i]->AddPos( { fOFFSET_POS_X_PARTS_TYPE * static_cast<float>( i ), 0.0f, 0.0f } );
	}

	//選択中パーツ項目.
	assert( !m_upPartsTypeSelect );
	m_upPartsTypeSelect = make_unique< clsSprite2D >();
	m_upPartsTypeSelect->Create( pDevice, pContext, sPATH_SELECT_PARTS_TYPE, ss );
	m_upPartsTypeSelect->SetAlpha( fALPHA_SELECT_PARTS_TYPE );

	//各パーツUI.
	clsOPERATION_STRING OprtStr;
	ss.Disp = PARTS_ICON_SIZE;
	for( int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		assert( m_vecupPartsIconArray[i].size() == 0 );
		m_vecupPartsIconArray[i].resize( data[i] );
		for( int j=0; j<data[i]; j++ ){
			m_vecupPartsIconArray[i][j] = make_unique< clsSprite2D >();

			string sTmpPath;
			sTmpPath = sPATH_PARTS_ICON_ROOT + sPATH_PARTS_ICON_PARTS[i] + "\\" + sPATH_PARTS_ICON_PARTS[i];
			sTmpPath = OprtStr.ConsolidatedNumber( sTmpPath, j );//ディレクトリ番号番号連結.
			sTmpPath += sPATH_PARTS_ICON_END;//ファイル名.

			//アイコン画像が見つからなければNODATA画像を読み込む.
			if( FAILED( m_vecupPartsIconArray[i][j]->Create( pDevice, pContext, sTmpPath.c_str(), ss ) ) ){
				m_vecupPartsIconArray[i][j]->Create( pDevice, pContext, sNO_DATA_FILE_NAME.c_str(), ss );
			}

			m_vecupPartsIconArray[i][j]->SetPos( vINIT_POS_PARTS_ICON );
			m_vecupPartsIconArray[i][j]->AddPos( { 0.0f, ( fPARTS_ICON_OFFSET + PARTS_ICON_SIZE.h ) * static_cast<float>( j ), 0.0f } );
		}
	}
	//選択中各パーツアイコン.
	assert( !m_upPartsNumSelect );
	m_upPartsNumSelect = make_unique< clsSprite2D >();
	m_upPartsNumSelect->Create( pDevice, pContext, sPATH_SELECT_PARTS_NUM, ss );
	m_upPartsNumSelect->SetAlpha( fALPHA_SELECT_PARTS_NUM );


	//パーツの単体モデル表示される.
	assert( !m_upPartsWindow );
	ss.Disp = INIT_SIZE_PARTS_WINDOW;
	m_upPartsWindow = make_unique< clsSprite2D >();
	m_upPartsWindow->Create( pDevice, pContext, sPATH_PARTS_WINDOW, ss );
	m_upPartsWindow->SetPos( vINIT_POS_PARTS_WINDOW );
	m_upPartsWindow->SetAlpha( fPARTS_WINDOW_ALPHA );

	//ロボウィンドウ.
	assert( !m_upRoboWindow );
	ss.Disp = INIT_SIZE_ROBO_WINDOW;
	m_upRoboWindow = make_unique< clsSprite2D >();
	m_upRoboWindow->Create( pDevice, pContext, sROBO_WINDOW_PATH, ss );
	m_upRoboWindow->SetPos( INIT_POS_ROBO_WINDOW );
	m_upRoboWindow->SetAlpha( fROBO_WINDOW_ALPHA );

	//ヘッダー.
	assert( !m_upHeader );
	ss.Disp = INIT_SIZE_HEADER;
	m_upHeader = make_unique< clsSprite2D >();
	m_upHeader->Create( pDevice, pContext, sHEADER_FILE_PATH, ss );
	m_upHeader->SetPos( INIT_POS_HEADER );
	m_upHeader->SetAlpha( fHEADER_ALPHA );

	//フッター.
	assert( !m_upFooter );
	ss.Disp = INIT_SIZE_FOOTER;
	m_upFooter = make_unique< clsSprite2D >();
	m_upFooter->Create( pDevice, pContext, sFOOTER_FILE_PATH, ss );
	m_upFooter->SetPos( INIT_POS_FOOTER );
	m_upFooter->SetAlpha( fFOOTER_ALPHA );

//	//ヘッダー文字.
//	assert( !m_upHeaderText );
//	m_upHeaderText = make_unique< clsUiText >();
//	m_upHeaderText->Create( pContext, WND_W, WND_H, fTEXT_SCALE_HEADER );
//	m_upHeaderText->SetPos( vTEXT_POS_HEADER );
//	m_upHeaderText->SetText( sHEADER_TEXT );


	//ステータスが表示される窓.
	assert( !m_upStatusWindow );
	ss.Disp = INIT_SIZE_STATUS_WINDOW;
	m_upStatusWindow = make_unique< clsSprite2D >();
	m_upStatusWindow->Create( pDevice, pContext, sPATH_STATUS_WINDOW, ss );
	m_upStatusWindow->SetPos( vINIT_POS_STATUS_WINDOW );
	m_upStatusWindow->SetAlpha( fSTATUS_WINDOW_ALPHA );

	//選択中ステータスを示す.
	assert( !m_upSelectStatus );
	m_upSelectStatus = make_unique< clsSprite2D >();
	m_upSelectStatus->Create( pDevice, pContext, sPATH_STATUS_COMMENT, ss );//SelectStatus.png
	m_upSelectStatus->SetAlpha( fSTATUS_COMMENT_ALPHA );



	//ステータス項目.
	assert( !m_vecupStatusText.size() );
	m_vecupStatusText.resize( iSTATUS_NUM_MAX );
	for( int i=0; i<iSTATUS_NUM_MAX; i++ ){
		m_vecupStatusText[i] = make_unique< clsUiText >();
		m_vecupStatusText[i]->Create( pContext, WND_W, WND_H, fTEXT_SCALE_STATUS );
		m_vecupStatusText[i]->SetPos( vTEXT_POS_STATUS );
		m_vecupStatusText[i]->AddPos( { 0.0f, fTEXT_POS_Y_OFFSET_STATUS * static_cast<float>( i ) } );
	}
	m_vecupStatusText.shrink_to_fit();

	//ステータス値.
	assert( !m_vecupStatusNumText.size() );
	assert( !m_vecupStatusNumTextNow.size() );
	m_vecupStatusNumText.resize( iSTATUS_NUM_MAX );
	m_vecupStatusNumTextNow.resize( iSTATUS_NUM_MAX );
	for( int i=0; i<iSTATUS_NUM_MAX; i++ ){
		m_vecupStatusNumText[i] = make_unique< clsUiText >();
		m_vecupStatusNumText[i]->Create( pContext, WND_W, WND_H, fTEXT_SCALE_STATUS );
		m_vecupStatusNumText[i]->SetPos( vTEXT_POS_STATUS_NUM );
		m_vecupStatusNumText[i]->AddPos( { 0.0f, fTEXT_POS_Y_OFFSET_STATUS * static_cast<float>( i ) } );

		m_vecupStatusNumTextNow[i] = make_unique< clsUiText >();
		m_vecupStatusNumTextNow[i]->Create( pContext, WND_W, WND_H, fTEXT_SCALE_STATUS );
		m_vecupStatusNumTextNow[i]->SetPos( vTEXT_POS_STATUS_NUM );
		m_vecupStatusNumTextNow[i]->AddPos( { fTEXT_POS_YX_OFFSET_STATUS_NOW, fTEXT_POS_Y_OFFSET_STATUS * static_cast<float>( i ) } );
	}

	//パーツ名.
	assert( !m_upPartsNameText );
	m_upPartsNameText = make_unique< clsUiText >();
	m_upPartsNameText->Create( pContext, WND_W, WND_H, TEXT_SCALE_PARTS_NAME );
	m_upPartsNameText->SetPos( vTEXT_POS_PARTS_NAME );

#if _DEBUG
	ss.Disp = { WND_W, WND_H };
	m_upDegine = make_unique< clsSprite2D >();
	m_upDegine->Create( pDevice, pContext, sPATH_DESIGN, ss );
	m_upDegine->SetPos( vINIT_POS_DESIGN );
	m_upDegine->SetAlpha( 0.5f );
#endif//#if _DEBUG




	//ボタン.
	assert( !m_upButton );
	ss.Disp = { INIT_DISP_BUTTON_SPRITE, INIT_DISP_BUTTON_SPRITE };
	ss.Anim = INIT_ANIM_BUTTON_SPRITE;
	m_upButton = make_unique< clsSprite2D >();
	m_upButton->Create( pDevice, pContext, sPATH_BUTONS, ss );

}


void clsASSEMBLE_UI::Input(	
		const clsXInput* const pXInput,
		const clsDxInput* const pDxInput )
{
#if _DEBUG
	float move = 1.0f;
	if( GetAsyncKeyState( VK_RIGHT )& 0x8000 )	DEBUG_SPRITE_NAME->AddPos( { move, 0.0f/*, 0.0f*/ } );
	if( GetAsyncKeyState( VK_LEFT ) & 0x8000 )	DEBUG_SPRITE_NAME->AddPos( {-move, 0.0f/*, 0.0f*/ } );
	if( GetAsyncKeyState( VK_UP )	& 0x8000 )	DEBUG_SPRITE_NAME->AddPos( { 0.0f,-move/*, 0.0f*/ } );
	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 )	DEBUG_SPRITE_NAME->AddPos( { 0.0f, move/*, 0.0f*/ } );

//	float scale = 0.01f;
//	if( GetAsyncKeyState( 'D' ) & 0x8000 )	DEBUG_SPRITE_NAME->AddScale( { 1+scale, 1.0f, 0.0f } );
//	if( GetAsyncKeyState( 'A' ) & 0x8000 )	DEBUG_SPRITE_NAME->AddScale( { 1-scale, 1.0f, 0.0f } );
//	if( GetAsyncKeyState( 'W' ) & 0x8000 )	DEBUG_SPRITE_NAME->AddScale( { 1.0f, 1-scale, 0.0f } );
//	if( GetAsyncKeyState( 'S' ) & 0x8000 )	DEBUG_SPRITE_NAME->AddScale( { 1.0f, 1+scale, 0.0f } );
//	if( GetAsyncKeyState( 'E' ) & 0x8000 )	DEBUG_SPRITE_NAME->AddScale( 1+scale );
//	if( GetAsyncKeyState( 'Q' ) & 0x8000 )	DEBUG_SPRITE_NAME->AddScale( 1-scale );


#endif//#if _DEBUG



}


void clsASSEMBLE_UI::Update( 
	enSELECT_MODE enSelect,
	shared_ptr< clsFILE > const spFile, 
	clsASSEMBLE_MODEL* const pModel,
	const int iPartsType,
	const int iPartsNum,
	const int iStatusCutNum )
{
	m_enSelectMode = enSelect;
	//パーツ選択中でないし、ステータス説明確認でもないならカエレ.
	if( enSelect != enSELECT_MODE::PARTS &&
		enSelect != enSELECT_MODE::STATUS )
	{
		return;
	}

	//パーツ説明文表示文字数増加.
	const int iCOMMENT_STRING_NUM_MAX = 200;
	if( m_iReadNumPartsComment < iCOMMENT_STRING_NUM_MAX ){
		const int iCOMMENT_STRING_RENDER_NUM_ADD = 4;
		m_iReadNumPartsComment  += iCOMMENT_STRING_RENDER_NUM_ADD;
	}

	//選択中を示す半透明の板表示.
	assert( m_upPartsTypeSelect );
	m_upPartsTypeSelect->SetPos( m_vecupPartsType[ iPartsType ]->GetPos() );
	assert( m_upPartsNumSelect );
	m_upPartsNumSelect->SetPos( m_vecupPartsIconArray[ iPartsType ][ iPartsNum ]->GetPos() );

	//先への布石.
	int iOldStausNum = m_iStatusNum;

	//文字列.
	assert( spFile );
	m_iStatusNum = iOPEN_STATUS_NUM[ iPartsType ];//ステータスが何行あるかを取得.

	//ステータス項目,数値.
	assert( m_vecupStatusText[ iPartsType ] );
	assert( m_vecupStatusNumText[ iPartsType ] );
	const int iOFFSET_RATE_STATUS_TEXT_FOR_STATUS_WINDOW =  m_iStatusNum - 1;
	for( int i=0; i<iSTATUS_NUM_MAX; i++ ){
		m_vecupStatusText[i]->SetPos( vTEXT_POS_STATUS );
		m_vecupStatusText[i]->AddPos( { 0.0f, fTEXT_POS_Y_OFFSET_STATUS * static_cast<float>( i ) } );
		m_vecupStatusText[i]->AddPos( { 0.0f, -INIT_SIZE_STATUS_WINDOW.h * static_cast<float>( iOFFSET_RATE_STATUS_TEXT_FOR_STATUS_WINDOW ) } );
		m_vecupStatusNumText[i]->SetPos( vTEXT_POS_STATUS_NUM );
		m_vecupStatusNumText[i]->AddPos( { 0.0f, fTEXT_POS_Y_OFFSET_STATUS * static_cast<float>( i ) } );
		m_vecupStatusNumText[i]->AddPos( { 0.0f, -INIT_SIZE_STATUS_WINDOW.h * static_cast<float>( iOFFSET_RATE_STATUS_TEXT_FOR_STATUS_WINDOW ) } );
		m_vecupStatusNumTextNow[i]->SetPos( vTEXT_POS_STATUS_NUM );
		m_vecupStatusNumTextNow[i]->AddPos( { 0.0f, fTEXT_POS_Y_OFFSET_STATUS * static_cast<float>( i ) } );
		m_vecupStatusNumTextNow[i]->AddPos( { fTEXT_POS_YX_OFFSET_STATUS_NOW, -INIT_SIZE_STATUS_WINDOW.h * static_cast<float>( iOFFSET_RATE_STATUS_TEXT_FOR_STATUS_WINDOW ) } );
	}
	
	//ステータスの数の差によるずれを修正.
	if( m_bStatusCommentOffset ){
		m_bStatusCommentOffset = false;
		m_iStatusCommentNo += m_iStatusNum - iOldStausNum;
		//0に戻す.
		if( !m_isDispStatus ||
			m_enSelectMode == enSELECT_MODE::PARTS )
		{
			m_iStatusCommentNo = 0;
		}
	}

	//超えていたならループする.
	StatusNumLoopRange();

	//飛び出さない.
	if( static_cast< unsigned int >( m_iStatusNum ) > m_vecupStatusText.size() ||
		static_cast< unsigned int >( m_iStatusNum ) > m_vecupStatusNumText.size() )
	{
		m_iStatusNum = 0;
	}

	//パーツ名セット.
	assert( m_upPartsNameText );
	m_upPartsNameText->SetText( 
		spFile->GetDataString( iPartsNum, iSTATUS_PARTS_NAME_NUM ).c_str() );

	//ステータスウィンドウの文字列.
	for( int i=0; i<iOPEN_STATUS_NUM[iPartsType]; i++ ){
		//ステータス名セット.
		assert( m_vecupStatusText[i] );
		m_vecupStatusText[i]->SetText( m_vecsStatusNameBoxArray[ iPartsType ][i].c_str() );

		//色替えの為.
		int iBefor, iAfter;
		iBefor = spFile->GetDataInt( 
			pModel->GetPartsNum( static_cast< clsASSEMBLE_MODEL::enPARTS_TYPES >( iPartsType ) ),
			i + iStatusCutNum );
		iAfter = spFile->GetDataInt( iPartsNum, i + iStatusCutNum );
		D3DXVECTOR4 vColor = GetStatusColor( iBefor, iAfter,iPartsType, i );

		//ステータス数値セット.
		assert( m_vecupStatusNumText[i] );
		m_vecupStatusNumText[i]->SetText( 
			spFile->GetDataString( iPartsNum, i + iStatusCutNum ).c_str() );
		m_vecupStatusNumText[i]->SetColor( vColor );
		//今のステータス.
		assert( m_vecupStatusNumTextNow[i] );
		string tmpString = 
			spFile->GetDataString( 
				pModel->GetPartsNum( static_cast< clsASSEMBLE_MODEL::enPARTS_TYPES >( iPartsType ) ),
				i + iStatusCutNum )
			+ sSTATUS_TEXT_MIDWAY;
		m_vecupStatusNumTextNow[i]->SetText( tmpString.c_str() );
//		m_vecupStatusNumTextNow[i]->SetColor( vColor );
	}

	//日本語説明文を調整.
	SetReadLinePartsComment( iPartsType );


	//どっちのうで？.
	assert( m_upWndBox );
	m_upWndBox->Update();
}


void clsASSEMBLE_UI::Render( 
	enSELECT_MODE enSelect,
	const int iPartsType, 
	const int iPartsNum ) const
{

	//パーツカテゴリ.
	int iPartsTypeIndex = iFONT_TEXT_PARTS_TYPE_LEGS_INDEX;
	for( unsigned int i=0; i<m_vecupPartsType.size(); i++ ){
		assert( m_vecupPartsType[i] );
		m_vecupPartsType[i]->Render();
		//パーツカテゴリの文字.
		m_wpFont->SetPos( m_vecupPartsType[i]->GetPos() );
		m_wpFont->AddPos( vFONT_TEXT_PARTS_TYPE_POS_OFFSET[i] );
		m_wpFont->SetScale( fFONT_TEXT_PARTS_TYPE_SCALE );
	
		m_wpFont->Render( iPartsTypeIndex ++ );
	}


	//パーツアイコン.
	for( unsigned int j=0; j<m_vecupPartsIconArray[ iPartsType ].size(); j++ ){
		assert( m_vecupPartsIconArray[ iPartsType ][j] );
		m_vecupPartsIconArray[ iPartsType ][j]->Render();
	}


	assert( m_upHeader );
	m_upHeader->Render();

	assert( m_upFooter );
	m_upFooter->Render();

	assert( m_upPartsWindow );
	m_upPartsWindow->Render();
	assert( m_upRoboWindow );
	m_upRoboWindow->Render();


//	assert( m_upHeaderText );
//	m_upHeaderText->Render();
	assert( m_wpFont );
	//画面タイトル「ASSEMBLE」.
	const D3DXVECTOR4 vFONT_TEXT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_wpFont->SetColor( vFONT_TEXT_COLOR );
	m_wpFont->SetPos( vFONT_TEXT_TITLE_POS );
	m_wpFont->SetScale( fFONT_TEXT_TITLE_SCALE );
	m_wpFont->Render( iFONT_TEXT_TITLE_LINE );

	//ボタンの説明文.
	m_wpFont->SetPos( vFONT_BUTTON_POS );
	m_wpFont->SetScale( fFONT_BUTTON_SCALE );
	m_wpFont->Render( iFONT_BUTTON_LINE );

	assert( m_upButton );
	for( int i=0; i<iBUTTON_SPRITE_NUM; i++ ){
		m_upButton->SetPos( vBUTTON_SPRITE_POS[i] );
		m_upButton->SetAnim( vBUTTON_SPRITE_ANIM[i] );
		m_upButton->Render();
	}

	//パーツ選択中のみ描画.
	if( enSelect == enSELECT_MODE::PARTS ||
		enSelect == enSELECT_MODE::STATUS )
	{
		//選択中カテゴリ.
		assert( m_upPartsTypeSelect );
		m_upPartsTypeSelect->Render();
		assert( m_upPartsNumSelect );
		m_upPartsNumSelect->Render();
	}

	assert( m_upWndBox );
	m_upWndBox->Render();

#if _DEBUG
	static bool bDesignDisp = true;
	if( GetAsyncKeyState( 'M' ) & 0x1 ){
		if( bDesignDisp )	bDesignDisp = false;
		else				bDesignDisp = true;
	};
	if( bDesignDisp )m_upDegine->Render();
#endif//#if _DEBUG

}

void clsASSEMBLE_UI::RenderPartsState( 
	enSELECT_MODE enSelect, 
	const int iPartsType, 
	const int iPartsNum ) const//選択中パーツ番号.
{
	//パーツ選択中のみ描画.
	if( enSelect == enSELECT_MODE::PARTS ||
		enSelect == enSELECT_MODE::STATUS )
	{
		//パーツ名.
		assert( m_upPartsNameText );
		m_upPartsNameText->Render();

		//ステータスの説明.
		assert( m_wpFont );
		m_wpFont->SetPos( vFONT_COMMENT_POS );
		m_wpFont->SetScale( fFONT_COMMENT_SCALE );
		m_wpFont->Render( m_iReadLinePartsComment, m_iReadNumPartsComment );

		//ステータスを表示しないなら飛ばす.
		if( !m_isDispStatus ) return;

		//窓.
		assert( m_upStatusWindow );
		D3DXVECTOR3 vPos = m_upStatusWindow->GetPos();
		for( int i=0; i<m_iStatusNum; i++ ){
			m_upStatusWindow->Render();
			m_upStatusWindow->AddPos( { 0.0f, -INIT_SIZE_STATUS_WINDOW.h, 0.0f } );
		}
		m_upStatusWindow->SetPos( vPos );

		//ステータス名.
		if( enSelect == enSELECT_MODE::STATUS ){
			assert( m_upSelectStatus );
			int iTmp = m_iStatusNum - 1;
			m_upSelectStatus->SetPos( { 
				m_upStatusWindow->GetPos().x,
				m_upStatusWindow->GetPos().y + ( m_iStatusCommentNo - iTmp ) * INIT_SIZE_STATUS_WINDOW.h,//計算式はややゴリ押し.
				0.0f } );
			m_upSelectStatus->Render();
		}

		//数値.
		for( int i=0; i<m_iStatusNum; i++ ){
			assert( m_vecupStatusText[i] );
			m_vecupStatusText[i]->Render();
			assert( m_vecupStatusNumText[i] );
			m_vecupStatusNumText[i]->Render( clsUiText::enPOS::RIGHT );
			assert( m_vecupStatusNumTextNow[i] );
			m_vecupStatusNumTextNow[i]->Render( clsUiText::enPOS::RIGHT );
		}


	}

}


//説明文の行指定.
int clsASSEMBLE_UI::SetReadLinePartsComment(
	const int iPartsType )	//パーツ種類.
{
	//パーツ説明文の分.
	const int iPARTS_TEXT_LINE_MAX = 6;

	if( m_enSelectMode == enSELECT_MODE::PARTS ){
		m_iReadLinePartsComment = iPartsType + iFONT_COMMENT_LINE; 
	}
	else if( m_enSelectMode == enSELECT_MODE::STATUS ){
		m_iReadLinePartsComment = iFONT_COMMENT_LINE + iPARTS_TEXT_LINE_MAX; 
		
		int iTmp = iPartsType;
		if( iTmp >= iPARTS_TEXT_LINE_MAX - 1 ){
			iTmp --;
		}

		//パーツカテゴリ.
		for( int i=0; i<iTmp; i++ ){
			m_iReadLinePartsComment += iOPEN_STATUS_NUM[i];
		}

		//ステータスNo..
		m_iReadLinePartsComment += m_iStatusCommentNo;
	}
	
	return 0;
}


//ステータスウィンドウを隠す.
void clsASSEMBLE_UI::SwitchDispStatusComment()
{
	if( m_isDispStatus ){
		SetStatusComment( enSELECT_MODE::PARTS );
		m_isDispStatus = false;
	}
	else{
		SetStatusComment( enSELECT_MODE::PARTS );
		m_isDispStatus = true;
	}
}

//ステータスcommentの切り替え許可.
bool clsASSEMBLE_UI::isCanSwitchStatusComment()
{
	if( m_isDispStatus ){
		return true;
	}

	return false;
}

//ステータス詳細とパーツ選択の切り替え.
void clsASSEMBLE_UI::SwitchStatusComment()
{
	if( m_enSelectMode == enSELECT_MODE::PARTS ){
		m_isDispStatus = true;
		m_enSelectMode = enSELECT_MODE::STATUS;
	}
	else if( m_enSelectMode == enSELECT_MODE::STATUS ){
		m_enSelectMode = enSELECT_MODE::PARTS;
	}
	InitReadNumPartsComment();

}

//指定.
void clsASSEMBLE_UI::SetStatusComment( const enSELECT_MODE enMode )
{
	m_enSelectMode = enMode;

	m_isDispStatus = true;

//	if( m_enSelectMode == enSELECT_MODE::STATUS ){
//	}
//	else if( m_enSelectMode == enSELECT_MODE::PARTS ){
//	}
}

//.
void clsASSEMBLE_UI::AddStatusCommentNo( const bool isPlus )
{
	if( !m_isDispStatus ) return;

	InitReadNumPartsComment();

	if( isPlus ){
		m_iStatusCommentNo ++;
	}
	else{
		m_iStatusCommentNo --;
	}
}

void clsASSEMBLE_UI::StatusNumKeepRange()
{
	if( m_iStatusCommentNo >= m_iStatusNum )m_iStatusCommentNo = m_iStatusNum - 1;
	else if( m_iStatusCommentNo < 0 )		m_iStatusCommentNo = 0;

}
void clsASSEMBLE_UI::StatusNumLoopRange()
{
	if( m_iStatusCommentNo >= m_iStatusNum )m_iStatusCommentNo = 0;
	else if( m_iStatusCommentNo < 0 )		m_iStatusCommentNo = m_iStatusNum - 1;

}


//ゴリ押し気味.
//選択肢を横に持って行った時の調整.
void clsASSEMBLE_UI::AddCommentNoForChangePartsType( const int iPartsType )
{
//	m_iStatusCommentNo += m_iStatusNum;
	m_bStatusCommentOffset = true;
	InitReadNumPartsComment();

	//ステータスが何行あるかを取得.
	int iStatusNumOld = m_iStatusNum;
	m_iStatusNum = iOPEN_STATUS_NUM[ iPartsType ];

#if 0
	//ステ項目が増えたときに時に選択カーソルが上に上がるのを防ぐ.
	if( m_iStatusNum > iStatusNumOld ){
		m_iStatusCommentNo += m_iStatusNum - iStatusNumOld;
	}
	//逆に減ったときは下がるのを防ぐ.
	else if( m_iStatusNum < iStatusNumOld ){
		m_iStatusCommentNo -= iStatusNumOld - m_iStatusNum;
	}
#else
	//差分を追加する.
	if( m_iStatusNum != iStatusNumOld ){
		m_iStatusCommentNo += m_iStatusNum - iStatusNumOld;
	}
#endif

	//超えていたなら収める.
	StatusNumKeepRange();
}


D3DXVECTOR4 clsASSEMBLE_UI::GetStatusColor( 
	const int iBefore,		const int iAfter,
	const int iPartsType,	const int iStatusNum ) const
{
	const D3DXVECTOR4 vRED		= { 1.0f, 0.5f, 0.5f, 1.0f };
	const D3DXVECTOR4 vBLUE		= { 0.5f, 0.5f, 1.0f, 1.0f };
	const D3DXVECTOR4 vWHITE	= { 1.0f, 1.0f, 1.0f, 1.0f };
	bool isRed = false;

	if( iBefore == iAfter ){
		return vWHITE;
	}
	else if( iBefore > iAfter ){
		isRed = false;
	}
	else if( iBefore < iAfter ){
		isRed = true;
	}
	
	bool tmp = bRED_BIG[ iPartsType ][ iStatusNum ];
	if( !tmp ){
		if( isRed ) isRed = false;
		else		isRed = true;
	}

	if( isRed ){
		return vRED;
	}
	return vBLUE;
}


#if _DEBUG
//デバッグテキスト用.
D3DXVECTOR3 clsASSEMBLE_UI::GetUiPos()
{
	return DEBUG_SPRITE_NAME->GetPos();
}
#endif//#if _DEBUG
