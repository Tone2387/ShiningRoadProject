#include "AssembleModel.h"

//配列の添え字.
const UCHAR ucLEG = static_cast<UCHAR>( enPARTS::LEG );
const UCHAR ucCORE = static_cast<UCHAR>( enPARTS::CORE );
const UCHAR ucHEAD = static_cast<UCHAR>( enPARTS::HEAD );
const UCHAR ucARM_L = static_cast<UCHAR>( enPARTS::ARM_L );
const UCHAR ucARM_R = static_cast<UCHAR>( enPARTS::ARM_R );
const UCHAR ucWEAPON_L = static_cast<UCHAR>( enPARTS::WEAPON_L );
const UCHAR ucWEAPON_R = static_cast<UCHAR>( enPARTS::WEAPON_R );


clsASSEMBLE_MODEL::clsASSEMBLE_MODEL() :
	m_wpResource( nullptr ),
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
	m_wpResource = nullptr;
}



void clsASSEMBLE_MODEL::Create( clsResource* const pResource )
{
	ASSERT_IF_NOT_NULL( m_pPartsFactory );
	ASSERT_IF_NOT_NULL( m_wppParts );
	ASSERT_IF_NOT_NULL( m_wpResource );

	m_wpResource = pResource;

	m_ucPartsMax = static_cast<UCHAR>( enPARTS::MAX );

	m_pPartsFactory = new clsFACTORY_PARTS;

	m_wppParts = new clsPARTS_BASE*[m_ucPartsMax];
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
//		m_wppParts[i] = nullptr;
		m_wppParts[i] = m_pPartsFactory->Create( static_cast<enPARTS>( i ) );
	}

	Init();
}

void clsASSEMBLE_MODEL::UpDate()
{
	ASSERT_IF_NULL( m_wppParts );
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
		ASSERT_IF_NULL( m_wppParts[i] );
//		m_wppParts[i]->SetPosition( { 0.0f, 1.0f, 0.0f } );

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


//モデルつけ変え.
void clsASSEMBLE_MODEL::AttachModel( 
	const enPARTS enParts,
	const SKIN_ENUM_TYPE PartsNum )
{
	UCHAR ucParts = static_cast<UCHAR>( enParts );
	ASSERT_IF_NULL( m_wppParts );
	ASSERT_IF_NULL( m_wppParts[ucParts] );
	m_wppParts[ucParts]->DetatchModel();
	m_wppParts[ucParts]->AttachModel( 
		m_wpResource->GetPartsModels( enParts, PartsNum ) );
//	m_wppParts[ucParts]->
}


//トランスフォーム.
void clsASSEMBLE_MODEL::SetPos( const D3DXVECTOR3 &vPos )
{
	ASSERT_IF_NULL( m_wppParts );
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
		ASSERT_IF_NULL( m_wppParts[i] );
		m_wppParts[i]->SetPosition( vPos );
	}

	m_wppParts[ucLEG]->SetPosition( vPos );
//	m_wppParts[ucCORE]->SetPosition( 
//		m_wppParts[ucLEG]->GetBonePos( "LegJunctionCore" ) );
//	m_wppParts[ucHEAD]->SetPosition( 
//		m_wppParts[ucCORE]->GetBonePos( "CoreJunctionHead" ) );
//	m_wppParts[ucARM_L]->SetPosition( 
//		m_wppParts[ucCORE]->GetBonePos( "CoreJunctionArmL" ) );
//	m_wppParts[ucARM_R]->SetPosition( 
//		m_wppParts[ucCORE]->GetBonePos( "CoreJunctionArmR" ) );
//	m_wppParts[ucWEAPON_L]->SetPosition( 
//		m_wppParts[ucARM_L]->GetBonePos( "ArmLJunctionWeapon" ) );
//	m_wppParts[ucWEAPON_R]->SetPosition( 
//		m_wppParts[ucARM_R]->GetBonePos( "ArmRJunctionWeapon" ) );
}

void clsASSEMBLE_MODEL::SetRot( const D3DXVECTOR3 &vYaw )
{
	ASSERT_IF_NULL( m_wppParts );
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
		ASSERT_IF_NULL( m_wppParts[i] );
		m_wppParts[i]->SetRotation( vYaw );
	}
}
void clsASSEMBLE_MODEL::AddRot( const D3DXVECTOR3 &vYaw )
{
	ASSERT_IF_NULL( m_wppParts );
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
		ASSERT_IF_NULL( m_wppParts[i] );
		m_wppParts[i]->SetRotation( 
			m_wppParts[i]->GetRotation() + vYaw );
	}
}

void clsASSEMBLE_MODEL::SetScale( const float fScale )
{
	ASSERT_IF_NULL( m_wppParts );
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
		ASSERT_IF_NULL( m_wppParts[i] );
		m_wppParts[i]->SetScale( fScale );
	}
}


//モデルの初期セット.
void clsASSEMBLE_MODEL::Init()
{
	AttachModel( enPARTS::LEG, 0 );
	AttachModel( enPARTS::CORE, 0 );
	AttachModel( enPARTS::HEAD, 0 );
	AttachModel( enPARTS::ARM_L, 0 );
	AttachModel( enPARTS::ARM_R, 0 );
	AttachModel( enPARTS::WEAPON_L, 0 );
	AttachModel( enPARTS::WEAPON_R, 0 );
	SetPos( { 0.0f, 0.0f, 0.0f } );
	SetRot( { 0.0f, 0.0f, 0.0f } );
	SetScale( 1.0f );

}

void clsASSEMBLE_MODEL::SetAnimSpd( const double &dSpd )
{
	ASSERT_IF_NULL( m_wppParts );
	for( UCHAR i=0; i<m_ucPartsMax; i++ ){
		ASSERT_IF_NULL( m_wppParts[i] );
		m_wppParts[0]->SetAnimSpeed( dSpd );
	}
}

