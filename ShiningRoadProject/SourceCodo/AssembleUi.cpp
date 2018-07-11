#include "AssembleUi.h"

#include <string>

using namespace std;

//パーツアイコンサイズ.
const WHSIZE_FLOAT PARTS_TYPE_SIZE = { 120.0f, 60.0f };

//パス( パーツアイコンにつなげるために必要 ).
const char* sIMAGE_PASS = "Data\\Image\\PartsIcon\\";
//エラー画像.
const string sNO_DATA_FILE_NAME = "NoData.png";
const string sNO_DATA_FILE_NAME_ = sIMAGE_PASS + sNO_DATA_FILE_NAME;//まだデータがないから必要なだけ ： 完成版ではなくなる.
//パーツ種類の画像.
const string sPARTS_FILE_NAME[] =
{
	"Leg", "Core", "Head", "Arms", "Weapon"
};

//パーツカテゴリUIの基準位置.
const D3DXVECTOR3 vINIT_POS_PARTS_TYPE = { 21.75f, 95.0f, 0.0f };
//パーツカテゴリの座標の差.
const float fOFFSET_POS_X_PARTS_TYPE = PARTS_TYPE_SIZE.w + 4.0f;

//----- ステータスウィンドウ -----//.
const D3DXVECTOR3 vINIT_POS_STATUS_WINDOW = { 98.0f, 183.25f, 0.0f };//179.5f;
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
const D3DXVECTOR3 vINIT_POS_PARTS_WINDOW = { vINIT_POS_STATUS_WINDOW.x + INIT_SIZE_STATUS_WINDOW.w + fOFFSET_PARTS_WINDOW_TO_STATUS_WINDOW, 183.25f, 0.0f };
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




clsASSEMBLE_UI::clsASSEMBLE_UI()
	:m_iStatusNum( 0 )
{
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


	m_iStatusNum = 0;
}



void clsASSEMBLE_UI::Create(
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext )
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

		tmpString = sIMAGE_PASS + sNO_DATA_FILE_NAME;
		m_vupPartsType[i]->Create( pDevice, pContext, tmpString.c_str(), ss );

		m_vupPartsType[i]->SetPos( vINIT_POS_PARTS_TYPE );//148.25f.
		m_vupPartsType[i]->AddPos( { fOFFSET_POS_X_PARTS_TYPE * static_cast<float>( i ), 0.0f, 0.0f } );
	}

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
//	m_vupStatusText.resize( iSTATUS_NUM_MAX, nullptr );
	for( int i=0; i<iSTATUS_NUM_MAX; i++ ){
		m_vupStatusText.push_back( nullptr );
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
//	m_vupStatusNumText.resize( iSTATUS_NUM_MAX, nullptr );
	for( int i=0; i<iSTATUS_NUM_MAX; i++ ){
		m_vupStatusNumText.push_back( nullptr );
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
	std::shared_ptr< clsFILE > const spFile, 
	const int iPartsNum,
	const int iCutNum )
{
	assert( spFile );
	m_iStatusNum = spFile->GetSizeCol() - iCutNum;//ステータスが何行あるかを取得.
	//飛び出さない.
	if( m_iStatusNum >= m_vupStatusText.size() ||
		m_iStatusNum >= m_vupStatusNumText.size() )
	{
		m_iStatusNum = 0;
	}

	//パーツ名セット.
	m_upPartsNameText->SetText( 
		spFile->GetDataString( iPartsNum, iSTATUS_PARTS_NAME_NUM ).c_str() );
}


void clsASSEMBLE_UI::Render()
{
#if _DEBUG
	m_upDegine->Render();
#endif//#if _DEBUG

	for( unsigned int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vupPartsType[i]->Render();
	}

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
