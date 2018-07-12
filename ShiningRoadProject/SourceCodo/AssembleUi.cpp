#include "AssembleUi.h"

#include <string>


//ファイルがあるか調べる機構を作る.


using namespace std;

//パス( エラーにつなげるために必要 ).
const char* sIMAGE_PASS = "Data\\Image\\PartsIcon\\";


//----- パーツカテゴリ -----//.
//パーツカテゴリサイズ.
const WHSIZE_FLOAT PARTS_TYPE_SIZE = { 120.0f, 60.0f };
//パーツカテゴリUIの基準位置.
const D3DXVECTOR3 vINIT_POS_PARTS_TYPE = { 21.75f, 95.0f, 0.0f };
//パーツカテゴリの座標の差.
const float fOFFSET_POS_X_PARTS_TYPE = PARTS_TYPE_SIZE.w + 4.0f;
//パーツカテゴリへのパス.
const char* sPATH_PARTS_TYPE = "Data\\Image\\AssembleUi\\";
//パーツカテゴリ画像パス( sIMAGE_PASSにくっつける ).
const string sPATH_PARTS_TYPE_CHILDREN[] =
{
	"LegType.png", "CoreType.png", "HeadType.png", "ArmsType.png", "WeaponType.png" 
};
//----- パーツカテゴリ 終わり -----//.

//選択中パーツカテゴリパス.
const char* sPATH_SELECT_PARTS_TYPE = "Data\\Image\\AssembleUi\\SelectPartsType.png";
const float fALPHA_SELECT_PARTS_TYPE = 0.5f;


//----- 各パーツUI -----//.
//エラー画像.
const string sNO_DATA_FILE_NAME = "NoData.png";
//サイズ.
const WHSIZE_FLOAT PARTS_ICON_SIZE = { 60.0f, 60.0f };
//UI同士の隙間.
const float fPARTS_ICON_OFFSET = 2.0f;
//カテゴリからの隙間.
const float fPARTS_ICON_OFFSET_FROM_PARTS_TYPE = 6.0f;
//基準位置.
const D3DXVECTOR3 vINIT_POS_PARTS_ICON = { vINIT_POS_PARTS_TYPE.x, vINIT_POS_PARTS_TYPE.y + PARTS_TYPE_SIZE.h + fPARTS_ICON_OFFSET_FROM_PARTS_TYPE, 0.0f };

//各パーツへのアイコンパスへの道筋.
const string sPATH_PARTS_ICON_ROOT = "Data\\RoboParts\\";
//各パーツへのアイコンパスへの道筋( 各パーツ ).
const string sPATH_PARTS_ICON_PARTS[] = {
	"Leg", "Core", "Head", "Arms", "Weapon"
};
//アイコンのファイル名.
const char* sPATH_PARTS_ICON_END = "\\Icon.png";



const string sNO_DATA_FILE_NAME_ = sIMAGE_PASS + sNO_DATA_FILE_NAME;//まだデータがないから必要なだけ ： 完成版ではなくなる.
//----- 各パーツUI 終わり -----//.

//選択中各パーツUIパス.
const char* sPATH_SELECT_PARTS_NUM = "Data\\Image\\AssembleUi\\SelectPartsNum.png";
const float fALPHA_SELECT_PARTS_NUM = fALPHA_SELECT_PARTS_TYPE;




//----- ステータスウィンドウ -----//.
const D3DXVECTOR3 vINIT_POS_STATUS_WINDOW = { 98.0f, vINIT_POS_PARTS_ICON.y, 0.0f };//179.5f;
const WHSIZE_FLOAT INIT_SIZE_STATUS_WINDOW  = { 415.0f, 468.0f };//360.0f.
const char* sPATH_STATUS_WINDOW = "Data\\Image\\AssembleUi\\StatusWindow.png";
const int iSTATUS_NUM_MAX = 11;//ステータスの最大数.

//ステータス文字.
//この窓のタイトル.
const float TEXT_SCALE_STATUS_TITLE = 2.0f;
const D3DXVECTOR2 vTEXT_POS_STATUS_TITLE_OFFSET_TO_STATUS_WINDOW = { 4.5f, 8.25f };
const D3DXVECTOR2 vTEXT_POS_STATUS_TITLE = {
	vINIT_POS_STATUS_WINDOW.x + vTEXT_POS_STATUS_TITLE_OFFSET_TO_STATUS_WINDOW.x,
	vINIT_POS_STATUS_WINDOW.y + vTEXT_POS_STATUS_TITLE_OFFSET_TO_STATUS_WINDOW.y };
const char* sSTATUS_TITLE_TEXT = "Parts Status";

//ステータス.
//二行目以降のずれ幅.
const float fTEXT_POS_Y_OFFSET_STATUS = INIT_SIZE_STATUS_WINDOW.h / ( iSTATUS_NUM_MAX + 1 );//この+1はステータスタイトルの為.
const float fTEXT_SCALE_STATUS = 1.5f;
//項目文字の座標.
const D3DXVECTOR2 vTEXT_POS_OFFSET_TO_STATUS_WINDOW = { 5.0f, 8.75f };//窓からのずれ.
const D3DXVECTOR2 vTEXT_POS_STATUS = {
	vINIT_POS_STATUS_WINDOW.x + vTEXT_POS_OFFSET_TO_STATUS_WINDOW.x,
	vINIT_POS_STATUS_WINDOW.y + vTEXT_POS_OFFSET_TO_STATUS_WINDOW.y + fTEXT_POS_Y_OFFSET_STATUS };
//値文字の座標.
const D3DXVECTOR2 vTEXT_POS_STATUS_NUM = 
	{ vTEXT_POS_STATUS.x + INIT_SIZE_STATUS_WINDOW.w - 12.0f, vTEXT_POS_STATUS.y };
//----- ステータスウィンドウ終わり -----//.


//----- パーツウィンドウ -----//.
//パーツの姿のまどX座標.
const float fOFFSET_PARTS_WINDOW_TO_STATUS_WINDOW = 8.0f;//ステータスウィンドウとの隙間.
//478.25f.
const D3DXVECTOR3 vINIT_POS_PARTS_WINDOW = { 
	vINIT_POS_STATUS_WINDOW.x + INIT_SIZE_STATUS_WINDOW.w + fOFFSET_PARTS_WINDOW_TO_STATUS_WINDOW, 
	vINIT_POS_STATUS_WINDOW.y, 
	0.0f };
const WHSIZE_FLOAT INIT_SIZE_PARTS_WINDOW  = { 300.0f, 468.0f };
const char* sPATH_PARTS_WINDOW = "Data\\Image\\PartsIcon\\NoData.png";

//文字( ステータスウィンドウを基準にしている ).
const float TEXT_SCALE_PARTS_NAME = TEXT_SCALE_STATUS_TITLE;
const D3DXVECTOR2 vTEXT_POS_PARTS_NAME = 
	{ vTEXT_POS_STATUS_TITLE.x + INIT_SIZE_STATUS_WINDOW.w + fOFFSET_PARTS_WINDOW_TO_STATUS_WINDOW, vTEXT_POS_STATUS_TITLE.y };

//----- パーツウィンドウ終わり -----//.






//----- ヘッダーとフッター -----//.
const WHSIZE_FLOAT INIT_SIZE_HEADER  = { WND_W, 60.0f };
const WHSIZE_FLOAT INIT_SIZE_FOOTER  = { WND_W, 24.0f };
const D3DXVECTOR3 INIT_POS_HEADER = { 0.0f, 20.0f, 0.0f };
const D3DXVECTOR3 INIT_POS_FOOTER = { 0.0f, WND_H - INIT_SIZE_FOOTER.h - 14.0f, 0.0f };
//文字.
const float fTEXT_SCALE_HEADER = 4.0f;
const float fTEXT_SCALE_FOOTER = 1.0f;
const D3DXVECTOR2 vTEXT_POS_OFFSET_HEADER = { 57.0f, 7.0f };//画像とのずれ.
const D3DXVECTOR2 vTEXT_POS_OFFSET_FOOTER = { 57.0f, 6.0f };
const D3DXVECTOR2 vTEXT_POS_HEADER = { INIT_POS_HEADER.x + vTEXT_POS_OFFSET_HEADER.x, INIT_POS_HEADER.y + vTEXT_POS_OFFSET_HEADER.y };
const D3DXVECTOR2 vTEXT_POS_FOOTER = { INIT_POS_FOOTER.x + vTEXT_POS_OFFSET_FOOTER.x, INIT_POS_FOOTER.y + vTEXT_POS_OFFSET_FOOTER.y };
const char* sHEADER_TEXT = "ASSEMBLE";
const char* sFOOTER_TEXT = "Enter : B";
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
{ 5, 11, 2, 2, 10 };
//ステータスの名前.
const string sSTATUS_NAME_LEG[] = 
	{ "HP", "Walk Speed", "Stability", "Turning", "Jump Power"  };
const string sSTATUS_NAME_CORE[] = 
	{ "HP", "EN Capacity", "EN Output", "Boost Horizontal Power", "Boost Horizontal Cost", "Boost Vertical Power", "Boost Vertical Cost", "Boost Quick Power", "Boost Quick Cost", "Boost Quick Time", "Activity Time" };
const string sSTATUS_NAME_HEAD[] = 
	{ "HP", "Search" };
const string sSTATUS_NAME_ARMS[] = 
	{ "HP", "Aiming" };
const string sSTATUS_NAME_WEAPON[] = 
	{ "Attack Power", "Bullet Speed", "Bullet Range", "Cost", "Load Time", "Lock Time", "Lock Range", "Stability", "Magazine Load Time", "Bullets Num" };




clsASSEMBLE_UI::clsASSEMBLE_UI()
	:m_iStatusNum( 0 )
{
	//次のfor文用.
	const string* tmpStatusNamePtr[enPARTS_TYPE_SIZE] =
		{ &sSTATUS_NAME_LEG[0], &sSTATUS_NAME_CORE[0], &sSTATUS_NAME_HEAD[0], &sSTATUS_NAME_ARMS[0], &sSTATUS_NAME_WEAPON[0] };

	//表示する文字列のセット.
	for( int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		//表示ステータスの数をそろえる.
		m_vsStatusNameBox[i].reserve( iOPEN_STATUS_NUM[i] );
		m_vsStatusNameBox[i].resize( iOPEN_STATUS_NUM[i] );

		//文字列をセット.
		for( int j=0; j<iOPEN_STATUS_NUM[i]; j++ ){
			m_vsStatusNameBox[i][j] = tmpStatusNamePtr[i][j];
		}
	}

}

clsASSEMBLE_UI::~clsASSEMBLE_UI()
{
#if _DEBUG
	if( m_upDegine ){
		m_upDegine.reset( nullptr );
	}
#endif//#if _DEBUG

	if( m_upPartsNameText){
		m_upPartsNameText.reset( nullptr );
	}

	if( m_upStatusTitleText ){
		m_upStatusTitleText.reset( nullptr );
	}

	for( unsigned int i=0; i<m_vupStatusNumText.size(); i++ ){
		if( m_vupStatusNumText[i] ){
			m_vupStatusNumText[i].reset( nullptr );
		}
	}

	for( unsigned int i=0; i<m_vupStatusText.size(); i++ ){
		if( m_vupStatusText[i] ){
			m_vupStatusText[i].reset( nullptr );
		}
	}

	if( m_upFooterText ){
		m_upFooterText.reset( nullptr );
	}

	if( m_upHeaderText ){
		m_upHeaderText.reset( nullptr );
	}

	for( unsigned int i=0; i<m_pArrow.size(); i++ ){
		if( m_pArrow[i] ){
			m_pArrow[i].reset( nullptr );
		}
	}
	m_pArrow.clear();
	m_pArrow.shrink_to_fit();

	if( m_upFooter ){
		m_upFooter.reset( nullptr );
	}
	if( m_upHeader ){
		m_upHeader.reset( nullptr );
	}


	for( unsigned int i=0; i<m_vupPartsType.size(); i++ ){
		if( m_vupPartsType[i] ){
			m_vupPartsType[i].reset( nullptr );
		}
	}
	m_vupPartsType.clear();
	m_vupPartsType.shrink_to_fit();


	if( m_upPartsNumSelect ){
		m_upPartsNumSelect.reset( nullptr );
	}

	if( m_upPartsTypeSelect ){
		m_upPartsTypeSelect.reset( nullptr );
	}

	if( m_upStatusWindow ){
		m_upStatusWindow.reset( nullptr );
	}

	if( m_upPartsWindow ){
		m_upPartsWindow.reset( nullptr );
	}

	for( int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vsStatusNameBox[i].clear();
		m_vsStatusNameBox[i].shrink_to_fit();
	}

	for( int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vupPartsIcon[i].clear();
		m_vupPartsIcon[i].shrink_to_fit();
	}

	m_iStatusNum = 0;
}



void clsASSEMBLE_UI::Create(
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext,
		PARTS_NUM_DATA data  )
{
	string tmpString = sIMAGE_PASS;

	//パーツ項目初期化.
	assert( m_vupPartsType.size() == 0 );
	m_vupPartsType.reserve( enPARTS_TYPE_SIZE );
	SPRITE_STATE ss;
	ss.Disp = PARTS_TYPE_SIZE;
	for( unsigned int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vupPartsType.push_back( nullptr );
		m_vupPartsType[i] = make_unique< clsSprite2D >();

		tmpString = sPATH_PARTS_TYPE + sPATH_PARTS_TYPE_CHILDREN[i];
		m_vupPartsType[i]->Create( pDevice, pContext, tmpString.c_str(), ss );

		m_vupPartsType[i]->SetPos( vINIT_POS_PARTS_TYPE );
		m_vupPartsType[i]->AddPos( { fOFFSET_POS_X_PARTS_TYPE * static_cast<float>( i ), 0.0f, 0.0f } );
	}

	//選択中パーツ項目.
	assert( !m_upPartsTypeSelect );
	m_upPartsTypeSelect = make_unique< clsSprite2D >();
	m_upPartsTypeSelect->Create( pDevice, pContext, sPATH_SELECT_PARTS_TYPE, ss );
	m_upPartsTypeSelect->SetAlpha( fALPHA_SELECT_PARTS_TYPE );

	//各パーツUI.
	ss.Disp = PARTS_ICON_SIZE;
	for( unsigned int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		assert( m_vupPartsIcon[i].size() == 0 );
		m_vupPartsIcon[i].reserve( data[i] );

		for( unsigned int j=0; j<data[i]; j++ ){
			m_vupPartsIcon[i].push_back( nullptr );
			m_vupPartsIcon[i][j] = make_unique< clsSprite2D >();

			tmpString = sPATH_PARTS_TYPE + sPATH_PARTS_TYPE_CHILDREN[i];
			m_vupPartsIcon[i][j]->Create( pDevice, pContext, tmpString.c_str(), ss );

			m_vupPartsIcon[i][j]->SetPos( vINIT_POS_PARTS_ICON );
			m_vupPartsIcon[i][j]->AddPos( { 0.0f, ( fPARTS_ICON_OFFSET + PARTS_ICON_SIZE.h ) * static_cast<float>( j ), 0.0f } );
		}
	}
	//選択中各パーツアイコン.
	assert( !m_upPartsNumSelect );
	m_upPartsNumSelect = make_unique< clsSprite2D >();
	m_upPartsNumSelect->Create( pDevice, pContext, sPATH_SELECT_PARTS_NUM, ss );
	m_upPartsNumSelect->SetAlpha( fALPHA_SELECT_PARTS_NUM );



	//ステータスが表示される窓.
	assert( !m_upStatusWindow );
	ss.Disp = INIT_SIZE_STATUS_WINDOW;
	m_upStatusWindow = make_unique< clsSprite2D >();
	m_upStatusWindow->Create( pDevice, pContext, sPATH_STATUS_WINDOW, ss );
	m_upStatusWindow->SetPos( vINIT_POS_STATUS_WINDOW );

	//パーツの単体モデル表示される.
	assert( !m_upPartsWindow );
	ss.Disp = INIT_SIZE_PARTS_WINDOW;
	m_upPartsWindow = make_unique< clsSprite2D >();
	m_upPartsWindow->Create( pDevice, pContext, sPATH_PARTS_WINDOW, ss );
	m_upPartsWindow->SetPos( vINIT_POS_PARTS_WINDOW );

	//ヘッダー.
	assert( !m_upHeader );
	ss.Disp = INIT_SIZE_HEADER;
	m_upHeader = make_unique< clsSprite2D >();
	m_upHeader->Create( pDevice, pContext, sNO_DATA_FILE_NAME_.c_str(), ss );
	m_upHeader->SetPos( INIT_POS_HEADER );

	//フッター.
	assert( !m_upFooter );
	ss.Disp = INIT_SIZE_FOOTER;
	m_upFooter = make_unique< clsSprite2D >();
	m_upFooter->Create( pDevice, pContext, sNO_DATA_FILE_NAME_.c_str(), ss );
	m_upFooter->SetPos( INIT_POS_FOOTER );


	//ヘッダー文字.
	assert( !m_upHeaderText );
	m_upHeaderText = make_unique< clsUiText >();
	m_upHeaderText->Create( pContext, WND_W, WND_H, fTEXT_SCALE_HEADER );
	m_upHeaderText->SetPos( vTEXT_POS_HEADER );
	m_upHeaderText->SetText( sHEADER_TEXT );

	//フッター文字.
	assert( !m_upFooterText );
	m_upFooterText = make_unique< clsUiText >();
	m_upFooterText->Create( pContext, WND_W, WND_H, fTEXT_SCALE_FOOTER );
	m_upFooterText->SetPos( vTEXT_POS_FOOTER );
	m_upFooterText->SetText( sFOOTER_TEXT );



	//ステータスタイトル.
	assert( !m_upStatusTitleText );
	m_upStatusTitleText = make_unique< clsUiText >();
	m_upStatusTitleText->Create( pContext, WND_W, WND_H, TEXT_SCALE_STATUS_TITLE );
	m_upStatusTitleText->SetPos( vTEXT_POS_STATUS_TITLE );
	m_upStatusTitleText->SetText( sSTATUS_TITLE_TEXT );

	//ステータス項目.
	assert( !m_vupStatusText.size() );
	m_vupStatusText.reserve( iSTATUS_NUM_MAX );
	m_vupStatusText.resize( iSTATUS_NUM_MAX );
	for( int i=0; i<iSTATUS_NUM_MAX; i++ ){
//		m_vupStatusText.push_back( nullptr );
		m_vupStatusText[i] = make_unique< clsUiText >();
		m_vupStatusText[i]->Create( pContext, WND_W, WND_H, fTEXT_SCALE_STATUS );
		m_vupStatusText[i]->SetPos( vTEXT_POS_STATUS );
		m_vupStatusText[i]->AddPos( { 0.0f, fTEXT_POS_Y_OFFSET_STATUS * static_cast<float>( i ) } );
		m_vupStatusText[i]->SetText( "Magazine Load Time" );
	}
	m_vupStatusText.shrink_to_fit();

	//ステータス値.
	assert( !m_vupStatusNumText.size() );
	m_vupStatusNumText.reserve( iSTATUS_NUM_MAX );
	m_vupStatusNumText.resize( iSTATUS_NUM_MAX );
	for( int i=0; i<iSTATUS_NUM_MAX; i++ ){
//		m_vupStatusNumText.push_back( nullptr );
		m_vupStatusNumText[i] = make_unique< clsUiText >();
		m_vupStatusNumText[i]->Create( pContext, WND_W, WND_H, fTEXT_SCALE_STATUS );
		m_vupStatusNumText[i]->SetPos( vTEXT_POS_STATUS_NUM );
		m_vupStatusNumText[i]->AddPos( { 0.0f, fTEXT_POS_Y_OFFSET_STATUS * static_cast<float>( i ) } );
		m_vupStatusNumText[i]->SetText( "12345 >> 12345" );
	}
	m_vupStatusNumText.shrink_to_fit();

	//パーツ名.
	assert( !m_upPartsNameText );
	m_upPartsNameText = make_unique< clsUiText >();
	m_upPartsNameText->Create( pContext, WND_W, WND_H, TEXT_SCALE_PARTS_NAME );
	m_upPartsNameText->SetPos( vTEXT_POS_PARTS_NAME );
	m_upPartsNameText->SetText( "PARTS_NAME" );



#if _DEBUG
	ss.Disp = { WND_W, WND_H };
	m_upDegine = make_unique< clsSprite2D >();
	m_upDegine->Create( pDevice, pContext, sPATH_DESIGN, ss );
	m_upDegine->SetPos( vINIT_POS_DESIGN );
	m_upDegine->SetAlpha( fINIT_SCALE_DESIGN );
#endif//#if _DEBUG
}


void clsASSEMBLE_UI::Input()
{
#if _DEBUG
	float move = 0.25f;
	if( GetAsyncKeyState( VK_RIGHT )& 0x8000 )	m_upFooter->AddPos( { move, 0.0f, 0.0f } );
	if( GetAsyncKeyState( VK_LEFT ) & 0x8000 )	m_upFooter->AddPos( {-move, 0.0f, 0.0f } );
	if( GetAsyncKeyState( VK_UP )	& 0x8000 )	m_upFooter->AddPos( { 0.0f,-move, 0.0f } );
	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 )	m_upFooter->AddPos( { 0.0f, move, 0.0f } );
	float scale = 0.01f;
	if( GetAsyncKeyState( 'D' ) & 0x8000 )	m_upFooter->AddScale( { 1+scale, 1.0f, 0.0f } );
	if( GetAsyncKeyState( 'A' ) & 0x8000 )	m_upFooter->AddScale( { 1-scale, 1.0f, 0.0f } );
	if( GetAsyncKeyState( 'W' ) & 0x8000 )	m_upFooter->AddScale( { 1.0f, 1-scale, 0.0f } );
	if( GetAsyncKeyState( 'S' ) & 0x8000 )	m_upFooter->AddScale( { 1.0f, 1+scale, 0.0f } );
	if( GetAsyncKeyState( 'E' ) & 0x8000 )	m_upFooter->AddScale( 1+scale );
	if( GetAsyncKeyState( 'Q' ) & 0x8000 )	m_upFooter->AddScale( 1-scale );
#endif//#if _DEBUG
}


void clsASSEMBLE_UI::Update( 
	shared_ptr< clsFILE > const spFile, 
	const int iPartsType,
	const int iPartsNum,
	const int iStatusCutNum )
{
	//選択中表示.
	m_upPartsTypeSelect->SetPos( m_vupPartsType[ iPartsType ]->GetPos() );
	m_upPartsNumSelect->SetPos( m_vupPartsIcon[ iPartsType ][ iPartsNum ]->GetPos() );

	//文字列.
	assert( spFile );
	m_iStatusNum = iOPEN_STATUS_NUM[ iPartsType ];//ステータスが何行あるかを取得.
	//飛び出さない.
	if( m_iStatusNum > m_vupStatusText.size() ||
		m_iStatusNum > m_vupStatusNumText.size() )
	{
		m_iStatusNum = 0;
	}

	//パーツ名セット.
	m_upPartsNameText->SetText( 
		spFile->GetDataString( iPartsNum, iSTATUS_PARTS_NAME_NUM ).c_str() );

	//ステータスウィンドウの文字列.
	for( int i=0; i<iOPEN_STATUS_NUM[iPartsType]; i++ ){
		//ステータス名セット.
		m_vupStatusText[i]->SetText( m_vsStatusNameBox[ iPartsType ][i].c_str() );
		//ステータス数値セット.
		m_vupStatusNumText[i]->SetText( spFile->GetDataString( iPartsNum, i + iStatusCutNum ).c_str() );
	}
}


void clsASSEMBLE_UI::Render( const int iPartsType, const int iPartsNum )
{
#if _DEBUG
	m_upDegine->Render();
#endif//#if _DEBUG

	//パーツカテゴリ.
	for( unsigned int i=0; i<m_vupPartsType.size(); i++ ){
		m_vupPartsType[i]->Render();
	}
	//選択中カテゴリ.
	m_upPartsTypeSelect->Render();

	//パーツアイコン.
	for( unsigned int j=0; j<m_vupPartsIcon[ iPartsType ].size(); j++ ){
		m_vupPartsIcon[ iPartsType ][j]->Render();
	}
	m_upPartsNumSelect->Render();

	m_upHeader->Render();
	m_upFooter->Render();

	m_upStatusWindow->Render();
	m_upPartsWindow->Render();

	m_upHeaderText->Render();
	m_upFooterText->Render();

	m_upPartsNameText->Render();

	m_upStatusTitleText->Render();
	for( int i=0; i<m_iStatusNum; i++ ){
//		m_vupStatusText->Render();
//		m_vupStatusText->SetPos( { vTEXT_POS_STATUS.x, vTEXT_POS_STATUS.y + fTEXT_OFFSET_Y*i } );
//		m_vupStatusNumText->Render( true );
//		m_vupStatusNumText->SetPos( { vTEXT_POS_STATUS_NUM.x, vTEXT_POS_STATUS_NUM.y + fTEXT_OFFSET_Y*i } );
		m_vupStatusText[i]->Render();
		m_vupStatusNumText[i]->Render( true );
	}
}




#if _DEBUG
//デバッグテキスト用.
D3DXVECTOR3 clsASSEMBLE_UI::GetUiPos()
{
	return m_upFooter->GetPos();
}
#endif//#if _DEBUG
