#include "AssembleUi.h"

#include <string>

using namespace std;


const WHSIZE_FLOAT PARTS_TYPE_SIZE = { 64.0f, 128.0f };


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
	//ÉpÅ[ÉcçÄñ⁄èâä˙âª.
	assert( m_vpPartsType.size() == 0 );
	m_vpPartsType.reserve( enPARTS_TYPE_SIZE );
	SPRITE_STATE ss;
	ss.Disp = PARTS_TYPE_SIZE;
	for( unsigned int i=0; i<enPARTS_TYPE_SIZE; i++ ){
		m_vpPartsType.push_back( nullptr );
		m_vpPartsType[i] = new clsSprite2D;
		m_vpPartsType[i]->Create( pDevice, pContext, "", ss );
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

}




