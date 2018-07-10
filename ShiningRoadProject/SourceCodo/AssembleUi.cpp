#include "AssembleUi.h"

#include <string>

using namespace std;

//パーツアイコンサイズ.
const WHSIZE_FLOAT PARTS_TYPE_SIZE = { 120.0f, 60.0f };

//パス.
const char* sIMAGE_PASS = "Data\\Image\\PartsIcon\\";
//エラー画像.
const string sNO_DATA_FILE_NAME = "NoData.png";
//パーツ種類の画像.
const string sPARTS_FILE_NAME[] =
{
	"Leg", "Core", "Head", "Arms", "Weapon"
};

//パーツカテゴリUIの基準位置.
const D3DXVECTOR3 vINIT_POS_PARTS_TYPE = { 21.75f, 95.0f, 0.0f };
//パーツカテゴリの座標の差.
const float fOFFSET_POS_X_PARTS_TYPE = PARTS_TYPE_SIZE.w + 4.0f;

//ステータスウィンドウ.
const D3DXVECTOR3 vINIT_POS_STATUS_WINDOW = { 98.0f, 183.25f, 0.0f };//179.5f;
const WHSIZE_FLOAT INIT_SIZE_STATUS_WINDOW  = { 415.0f, 468.0f };//360.0f.
const char* sPATH_STATUS_WINDOW = "Data\\Image\\AssembleUi\\StatusWindow.png";
const int iSTATUS_NUM_MAX = 12;//ステータスの最大数.

//パーツの姿のまどX座標.
float fOFFSET_PARTS_WINDOW_TO_STATUS_WINDOW = 8.0f;//ステータスウィンドウとの隙間.
//478.25f.
const D3DXVECTOR3 vINIT_POS_PARTS_WINDOW = { vINIT_POS_STATUS_WINDOW.x + INIT_SIZE_STATUS_WINDOW.w + fOFFSET_PARTS_WINDOW_TO_STATUS_WINDOW, 183.25f, 0.0f };
const WHSIZE_FLOAT INIT_SIZE_PARTS_WINDOW  = { 300.0f, 468.0f };
const char* sPATH_PARTS_WINDOW = "Data\\Image\\PartsIcon\\NoData.png";


//文字の大きさ.
const float fTEXT_SCALE = 1.5f;//2.0f.
//文字の座標.
const float fTEXT_POS_OFFSET_TO_STATUS_WINDOW_X = 5.0f;
const float fTEXT_POS_OFFSET_TO_STATUS_WINDOW_Y = 8.75f;
const D3DXVECTOR2 vTEXT_POS = {
	vINIT_POS_STATUS_WINDOW.x + fTEXT_POS_OFFSET_TO_STATUS_WINDOW_X,
	vINIT_POS_STATUS_WINDOW.y + fTEXT_POS_OFFSET_TO_STATUS_WINDOW_Y };
//二行目以降のずれ幅.
const float fTEXT_OFFSET_Y = INIT_SIZE_STATUS_WINDOW.h / iSTATUS_NUM_MAX;


#if _DEBUG
//目安.
const char* sPATH_DESIGN = "Data\\Image\\AssembleDesign.png";
const D3DXVECTOR3 vINIT_POS_DESIGN = { 0.0f, 0.0f, 0.0f };
const float fINIT_SCALE_DESIGN = 0.1875f;
#endif//#if _DEBUG



clsASSEMBLE_UI::clsASSEMBLE_UI()
{
}

clsASSEMBLE_UI::~clsASSEMBLE_UI()
{
#if _DEBUG
	if( m_upDegine ){
		m_upDegine.reset( nullptr );
	}
#endif//#if _DEBUG

	if( m_upText ){
		m_upText.reset( nullptr );
	}

	for( unsigned int i=0; i<m_pArrow.size(); i++ ){
		if( m_pArrow[i] ){
			m_pArrow[i].reset( nullptr );
		}
	}
	m_pArrow.clear();
	m_pArrow.shrink_to_fit();

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

	//ステータスが表示される.
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

	//文字.
	assert( !m_upText );
	m_upText = make_unique< clsUiText >();
	m_upText->Create( pContext, WND_W, WND_H, fTEXT_SCALE );
	m_upText->SetPos( vTEXT_POS );
	m_upText->SetText( "Magazine Load Time  12345 >> 12345" );

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
	if( GetAsyncKeyState( VK_RIGHT )& 0x8000 )	m_upStatusWindow->AddPos( { move, 0.0f, 0.0f } );
	if( GetAsyncKeyState( VK_LEFT ) & 0x8000 )	m_upStatusWindow->AddPos( {-move, 0.0f, 0.0f } );
	if( GetAsyncKeyState( VK_UP )	& 0x8000 )	m_upStatusWindow->AddPos( { 0.0f,-move, 0.0f } );
	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 )	m_upStatusWindow->AddPos( { 0.0f, move, 0.0f } );
	float scale = 0.01f;
	if( GetAsyncKeyState( 'D' ) & 0x8000 )	m_upStatusWindow->AddScale( { 1+scale, 1.0f, 0.0f } );
	if( GetAsyncKeyState( 'A' ) & 0x8000 )	m_upStatusWindow->AddScale( { 1-scale, 1.0f, 0.0f } );
	if( GetAsyncKeyState( 'W' ) & 0x8000 )	m_upStatusWindow->AddScale( { 1.0f, 1-scale, 0.0f } );
	if( GetAsyncKeyState( 'S' ) & 0x8000 )	m_upStatusWindow->AddScale( { 1.0f, 1+scale, 0.0f } );
	if( GetAsyncKeyState( 'E' ) & 0x8000 )	m_upStatusWindow->AddScale( 1+scale );
	if( GetAsyncKeyState( 'Q' ) & 0x8000 )	m_upStatusWindow->AddScale( 1-scale );
#endif//#if _DEBUG
}


void clsASSEMBLE_UI::Update()
{

}


void clsASSEMBLE_UI::Render()
{
#if _DEBUG
	m_upDegine->Render();
#endif//#if _DEBUG

	for( unsigned int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vupPartsType[i]->Render();
	}

	m_upStatusWindow->Render();
	m_upPartsWindow->Render();

	for( int i=0; i<iSTATUS_NUM_MAX; i++ ){
		m_upText->Render();
		m_upText->SetPos( { vTEXT_POS.x, vTEXT_POS.y + fTEXT_OFFSET_Y*i } );
	}
}




#if _DEBUG
//デバッグテキスト用.
D3DXVECTOR3 clsASSEMBLE_UI::GetUiPos()
{
	return m_upStatusWindow->GetPos();
}
#endif//#if _DEBUG
