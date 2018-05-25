#include "AssembleModel.h"


clsASSEMBLE_MODEL::clsASSEMBLE_MODEL() :
	m_pPartsFactory( nullptr ),
	m_wppParts( nullptr )
{
}

clsASSEMBLE_MODEL::~clsASSEMBLE_MODEL()
{
	if( m_wppParts != nullptr ){
		for( UCHAR i=0; i<m_ucPartsMax; i++ ){
			if( m_wppParts[i] != nullptr ){
				m_wppParts[i]->DetatchModel();
				m_wppParts[i] = nullptr;
			}
		}
		delete[] m_wppParts;
		m_wppParts = nullptr;
	}
	m_ucPartsMax = 0;
	SAFE_DELETE( m_pPartsFactory );
}


void clsASSEMBLE_MODEL::Create()
{
	ASSERT_IF_NOT_NULL( m_pPartsFactory );
	ASSERT_IF_NOT_NULL( m_wppParts );

	m_ucPartsMax = static_cast<UCHAR>( enPARTS::MAX );

	m_pPartsFactory = new clsFACTORY_PARTS;

	m_wppParts = new clsPARTS_BASE*[m_ucPartsMax];
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
//		m_wppParts[i] = nullptr;
		m_wppParts[i] = m_pPartsFactory->Create( static_cast<enPARTS>( i ) );
	}
}

void clsASSEMBLE_MODEL::UpDate()
{
	ASSERT_IF_NULL( m_wppParts );
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
		ASSERT_IF_NULL( m_wppParts[i] );
		m_wppParts[i]->SetPosition( { 0.0f, 1.0f, 0.0f } );
	}
}

void clsASSEMBLE_MODEL::Render(
	const D3DXMATRIX& const mView, 
	const D3DXMATRIX& const mProj, 
	const D3DXVECTOR3& const vLight, 
	const D3DXVECTOR3& const vEye,
	const D3DXVECTOR4 &vColor,
	const bool isAlpha )
{
	ASSERT_IF_NULL( m_wppParts );
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
		ASSERT_IF_NULL( m_wppParts[i] );
		m_wppParts[i]->ModelRender( mView, mProj, vLight, vEye, vColor, isAlpha );
	}

}


//ƒ‚ƒfƒ‹‚Â‚¯•Ï‚¦.
void clsASSEMBLE_MODEL::AttachModel( 
	const enPARTS enParts,
	clsD3DXSKINMESH* const pModel )
{
	UCHAR ucParts = static_cast<UCHAR>( enParts );
	ASSERT_IF_NULL( m_wppParts );
	ASSERT_IF_NULL( m_wppParts[ucParts] );
	m_wppParts[ucParts]->DetatchModel();
	m_wppParts[ucParts]->AttachModel( pModel );
//	m_wppParts[ucParts]->
}


