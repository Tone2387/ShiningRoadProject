#include "AssembleUi.h"

#include <string>

using namespace std;

//パーツアイコンサイズ.
const WHSIZE_FLOAT PARTS_TYPE_SIZE = { 128.0f, 64.0f };

//パス.
const string sIMAGE_PASS = "Data\\Image\\PartsIcon\\";
//エラー画像.
const string sNO_DATA_FILE_NAME = "NoData.png";
//パーツ種類の画像.
const string sPARTS_FILE_NAME[] =
{
	"Leg", "Core", "Head", "Arms", "Weapon"
};


clsASSEMBLE_UI::clsASSEMBLE_UI()
	:m_pStatusWindow( nullptr )
	,m_pPartsTypeSelect( nullptr )
	,m_pPartsNumSelect( nullptr )
{
}

clsASSEMBLE_UI::~clsASSEMBLE_UI()
{
	SAFE_DELETE( m_pPartsNumSelect );
	SAFE_DELETE( m_pPartsTypeSelect );
	SAFE_DELETE( m_pStatusWindow );
}



void clsASSEMBLE_UI::Create(
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext )
{
	string tmpString = sIMAGE_PASS;

	//パーツ項目初期化.
	assert( m_vpPartsType.size() == 0 );
	m_vpPartsType.reserve( enPARTS_TYPE_SIZE );
	SPRITE_STATE ss;
	ss.Disp = PARTS_TYPE_SIZE;
	for( unsigned int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vpPartsType.push_back( nullptr );
		m_vpPartsType[i] = new clsSprite2D;
		tmpString = sIMAGE_PASS + sNO_DATA_FILE_NAME;
		m_vpPartsType[i]->Create( pDevice, pContext, tmpString.c_str(), ss );
		m_vpPartsType[i]->SetPos( { (float)(i*128), (float)(i*128), 0.0f } );
	}

}


void clsASSEMBLE_UI::Input()
{

}


void clsASSEMBLE_UI::Update()
{

}


void clsASSEMBLE_UI::Render()
{
	for( unsigned int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vpPartsType[i]->Render();
	}

}




