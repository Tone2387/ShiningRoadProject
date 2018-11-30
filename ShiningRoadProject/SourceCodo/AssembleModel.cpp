#include "AssembleModel.h"

#include "FactoryParts.h"

#include "OperationString.h"

using namespace std;

//leg�̃��f���̑��������������ꍇ.
//#define LEG_MODEL_POSITION_BASE_Y_OFFSET

#ifdef _DEBUG
#include "CharaStatic.h"
#endif//#ifdef _DEBUG

namespace{

#ifdef _DEBUG
		//����.
		std::unique_ptr<clsCharaStatic> m_upFootNull;
		std::unique_ptr<clsCharaStatic> m_upFootPosBase;
		std::unique_ptr<clsCharaStatic> m_upFootTrasnPos;
#endif//#ifdef _DEBUG


	//�z��̓Y����.
	const UCHAR ucLEG = static_cast<UCHAR>( enPARTS::LEG );
	const UCHAR ucCORE = static_cast<UCHAR>( enPARTS::CORE );
	const UCHAR ucHEAD = static_cast<UCHAR>( enPARTS::HEAD );
	const UCHAR ucARM_L = static_cast<UCHAR>( enPARTS::ARM_L );
	const UCHAR ucARM_R = static_cast<UCHAR>( enPARTS::ARM_R );
	const UCHAR ucWEAPON_L = static_cast<UCHAR>( enPARTS::WEAPON_L );
	const UCHAR ucWEAPON_R = static_cast<UCHAR>( enPARTS::WEAPON_R );

	//�p�[�c��ނ̐�.
	const UCHAR	ucPARTS_MAX = static_cast<UCHAR>( enPARTS::MAX );
	//�p�[�c����n������.
	const string sPARTS_NAME[ucPARTS_MAX] =
	{
		"Leg", "Core", "Head", "ArmL", "ArmR", "WeaponL", "WeaponR"
	};



	const double dANIM_SPD = 0.016;


	//�p�[�c���ߒl.
	const D3DXVECTOR4 vCOLOR_NORMAL = { 1.0f, 1.0f, 1.0f, 1.0f };
	const D3DXVECTOR4 vCOLOR_ALPHA =  { 10.0f, 10.0f, 0.0f, 0.65f };

	//�F�ύX�̃}�X�N���.
	const int iMASK_MAX_NUM = 2;

	//�F�̒i�K.gradation
	const int iCOLOR_GRADATION_MAX = 16;
	const int iCOLOR_GRADATION_MIN = 1;
}


clsASSEMBLE_MODEL::clsASSEMBLE_MODEL()
	:m_wpResource( nullptr )
	,m_vpParts()
	,m_dAnimSpd( 1.0 )
	,m_enPartsNum()
//	,m_iColorRank()
{
	m_dAnimSpd = dANIM_SPD;
	m_vecvColor.resize( iMASK_MAX_NUM, vCOLOR_NORMAL );
	for( char i=0; i<clsROBO_STATUS::enCOLOR_GAGE_size; i++ ){
		m_iColorRank[i] = iCOLOR_GRADATION_MIN;
		UpdateColor( static_cast<clsROBO_STATUS::enCOLOR_GAGE>( i ) );
	}
}

clsASSEMBLE_MODEL::~clsASSEMBLE_MODEL()
{
	for( unsigned int i=0; i<m_vpParts.size(); i++ ){
		if( m_vpParts[i] != nullptr ){
			m_vpParts[i]->DetatchModel();
			delete m_vpParts[i];
			m_vpParts[i] = nullptr;
		}
	}
	m_vpParts.clear();
	m_vpParts.shrink_to_fit();


	m_wpResource = nullptr;
}



void clsASSEMBLE_MODEL::Create( 
	clsResource* const pResource, 
	clsROBO_STATUS* const pStatus )
{
	assert( !m_vpParts.size() );
	assert( !m_wpResource );

	m_wpResource = pResource;

	unique_ptr< clsFACTORY_PARTS > upPartsFactory = make_unique< clsFACTORY_PARTS >();

	m_vpParts.resize( ucPARTS_MAX, nullptr );
	for( UCHAR i=0; i<ucPARTS_MAX; i++ ){
		m_vpParts[i] = upPartsFactory->Create( static_cast<enPARTS>( i ) );
	}


	Init( pStatus );

	CreateProduct( pStatus );

#ifdef _DEBUG
	float fRATE = 2.5f;
	//����.
	m_upFootTrasnPos = make_unique<clsCharaStatic>();
	m_upFootTrasnPos->AttachModel(
		m_wpResource->GetStaticModels( clsResource::enStaticModel_Building ) );
	m_upFootTrasnPos->AddRotationZ( static_cast<float>( M_PI ) );
	m_upFootTrasnPos->SetScale( 0.25f / fRATE );
	m_upFootTrasnPos->SetScale( { m_upFootTrasnPos->GetScale().x, 0.5f, m_upFootTrasnPos->GetScale().z } );

	m_upFootPosBase = make_unique<clsCharaStatic>();
	m_upFootPosBase->AttachModel(
		m_wpResource->GetStaticModels( clsResource::enStaticModel_Building ) );
	m_upFootPosBase->AddRotationZ( static_cast<float>( M_PI ) );
	m_upFootPosBase->SetScale( 0.2f / fRATE );

	m_upFootNull = make_unique<clsCharaStatic>();
	m_upFootNull->AttachModel(
		m_wpResource->GetStaticModels( clsResource::enStaticModel_Building ) );
	m_upFootNull->AddRotationZ( static_cast<float>( M_PI ) );
	m_upFootNull->SetScale( { 
		0.125f / fRATE, 
		0.5f / fRATE * 0.5f, 
		0.125f / fRATE } );
#endif//#ifdef _DEBUG

}

void clsASSEMBLE_MODEL::CreateProduct( clsROBO_STATUS* const pStatus )
{
}

//���f���̏����Z�b�g.
void clsASSEMBLE_MODEL::Init( clsROBO_STATUS* const pStatus )
{
	assert( pStatus );
	AttachModel( enPARTS::LEG,		pStatus->GetPartsNum( enPARTS::LEG ) );
	AttachModel( enPARTS::CORE,		pStatus->GetPartsNum( enPARTS::CORE ) );
	AttachModel( enPARTS::HEAD,		pStatus->GetPartsNum( enPARTS::HEAD ) );
	AttachModel( enPARTS::ARM_L,	pStatus->GetPartsNum( enPARTS::ARM_L ) );
	AttachModel( enPARTS::ARM_R,	pStatus->GetPartsNum( enPARTS::ARM_R ) );
	AttachModel( enPARTS::WEAPON_L, pStatus->GetPartsNum( enPARTS::WEAPON_L ) );
	AttachModel( enPARTS::WEAPON_R, pStatus->GetPartsNum( enPARTS::WEAPON_R ) );

	for( UCHAR i=0; i<ucPARTS_MAX; i++ ){
		m_vpParts[i]->PartsAnimChange( 0 );
	}


	SetPos( { 0.0f, 0.0f, 0.0f } );
	SetRot( { 0.0f, 0.0f, 0.0f } );
	SetScale( 1.0f );
	SetAnimSpd( dANIM_SPD );

	for( char i=0; i<clsROBO_STATUS::enCOLOR_GAGE_size; i++ ){
		clsROBO_STATUS::enCOLOR_GAGE tmpIndex = static_cast<clsROBO_STATUS::enCOLOR_GAGE>( i );
		m_iColorRank[i] = pStatus->GetColorRank( tmpIndex );
		UpdateColor( tmpIndex );
	}


	for( UCHAR i=0; i<ucPARTS_MAX; i++ ){
		m_vpParts[i]->Init();
	}

//	AnimReSet();
}

void clsASSEMBLE_MODEL::UpDate()
{
	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		m_vpParts[i]->Update();
	}
	UpdateProduct();
}


void clsASSEMBLE_MODEL::Render(
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3& vLight, 
	const D3DXVECTOR3& vEye,
	const enPARTS_TYPES AlphaParts,
	ID3D11DeviceContext* const pContext )
{
	D3DXVECTOR4 vTmpColorBase;
	D3DXVECTOR4 vTmpColorArmor;

#ifdef LEG_MODEL_POSITION_BASE_Y_OFFSET
	m_vpParts[ucLEG]->SetPosition( m_Trans.vPos );

	//���f���̑���.
//	D3DXVECTOR3 vLegPosPositionBase = m_vpParts[ucLEG]->GetBonePos( sBONE_NAME_LEG_POSITION_BASE );
//	D3DXVECTOR3 vLegPosNull = m_vpParts[ucLEG]->GetBonePos( sBONE_NAME_LEG_NULL );

	D3DXVECTOR3 vLegPosPositionBase = GetBonePosPreviosFrame( enPARTS_INDEX_LEG, clsPARTS_LEG::enLEG_BONE_POSITIONS_POSITION_BASE );
	D3DXVECTOR3 vLegPosNull			= GetBonePosPreviosFrame( enPARTS_INDEX_LEG, clsPARTS_LEG::enLEG_BONE_POSITIONS_NULL );

	const float fADD_POS_Y = vLegPosPositionBase.y - vLegPosNull.y;


	m_Trans.vPos += D3DXVECTOR3( 0.0f, fADD_POS_Y, 0.0f );

#endif

	D3DXVECTOR3 vLegPosPositionBase = GetBonePosPreviosFrame( enPARTS_INDEX_LEG, clsPARTS_LEG::enLEG_BONE_POSITIONS_POSITION_BASE );
	D3DXVECTOR3 vLegPosNull			= GetBonePosPreviosFrame( enPARTS_INDEX_LEG, clsPARTS_LEG::enLEG_BONE_POSITIONS_NULL );


	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		unsigned int uiMaskNum = 0;
		vTmpColorBase  = CreateColor( AlphaParts, i, uiMaskNum ++, pContext );
		vTmpColorArmor = CreateColor( AlphaParts, i, uiMaskNum ++, pContext );
		SetPartsFormalPos();
		m_vpParts[i]->ModelUpdate( m_vpParts[i]->m_Trans );
//		ModelUpdate();
		m_vpParts[i]->ModelRender( 
			mView, mProj, vLight, vEye, 
			vTmpColorBase, vTmpColorArmor, true );

		//�{�[���ʒu���������܂���.
		m_vpParts[i]->UpdateBonePosPreviosFrame();
	}

#ifdef LEG_MODEL_POSITION_BASE_Y_OFFSET
	m_Trans.vPos -= D3DXVECTOR3( 0.0f, fADD_POS_Y, 0.0f );

#endif//#define LEG_MODEL_POSITION_BASE_Y_OFFSET
#ifdef _DEBUG
	m_upFootNull->SetPosition( vLegPosNull );
	m_upFootNull->UpdatePos();
	m_upFootNull->Render( mView, mProj, vLight, vEye, { 10.0f, 0.0f, 0.0f, 0.5f }, true );
	
//	m_upFoot->SetPosition( m_Trans.vPos );
	m_upFootPosBase->SetPosition( vLegPosPositionBase );
	m_upFootPosBase->UpdatePos();
	m_upFootPosBase->Render( mView, mProj, vLight, vEye, { 0.0f, 10.0f, 0.0f, 0.5f }, true );
	
	m_upFootTrasnPos->SetPosition( m_vpParts[0]->GetPosition() );
	m_upFootTrasnPos->UpdatePos();
	m_upFootTrasnPos->Render( mView, mProj, vLight, vEye, { 0.0f, 0.0f, 10.0f, 0.5f }, true );
	
#endif//#ifdef _DEBUG

	//���C���[�t���[������.
	ChangeWireFrame( false, pContext );

}


//�F��f���o��.
D3DXVECTOR4 clsASSEMBLE_MODEL::CreateColor( 
	const enPARTS_TYPES AlphaParts, 
	const UINT uiIndex,
	const unsigned int uiMaskNum,
	ID3D11DeviceContext* const pContext  ) const
{
	D3DXVECTOR4 vReturn = vCOLOR_NORMAL;

	if( uiMaskNum < m_vecvColor.size() ){
		vReturn = m_vecvColor[ uiMaskNum ];
	}

	ChangeWireFrame( false, pContext );

	switch( AlphaParts )
	{
	case LEG:
		if( uiIndex == static_cast<UINT>( enPARTS::LEG ) ){
			ChangeWireFrame( true, pContext );
			vReturn = vCOLOR_ALPHA;
		}
		break;
	case CORE:
		if( uiIndex == static_cast<UINT>( enPARTS::CORE ) ){
			ChangeWireFrame( true, pContext );
			vReturn = vCOLOR_ALPHA;
		}
		break;
	case HEAD:
		if( uiIndex == static_cast<UINT>( enPARTS::HEAD ) ){
			ChangeWireFrame( true, pContext );
			vReturn = vCOLOR_ALPHA;
		}
		break;
	case ARMS:
		//���r�Ƃ�.
		if( uiIndex == static_cast<UINT>( enPARTS::ARM_L ) ||
			uiIndex == static_cast<UINT>( enPARTS::ARM_R ) )
		{
			ChangeWireFrame( true, pContext );
			vReturn = vCOLOR_ALPHA;
		}
		break;
	case WEAPON_L:
		if( uiIndex == static_cast<UINT>( enPARTS::WEAPON_L ) ){
			ChangeWireFrame( true, pContext );
			vReturn = vCOLOR_ALPHA;
		}
		break;
	case WEAPON_R:
		if( uiIndex == static_cast<UINT>( enPARTS::WEAPON_R ) ){
			ChangeWireFrame( true, pContext );
			vReturn = vCOLOR_ALPHA;
		}
		break;
	case enPARTS_TYPES::ENUM_SIZE:
	default:
		break;
	}

	return vReturn;
}

//���C���[�t���[���ؑ�.
void clsASSEMBLE_MODEL::ChangeWireFrame(
	const bool isWire,
	ID3D11DeviceContext* const  pContext ) const
{
	if( !pContext ){
		return;
	}

	//���X�^���C�Y(�ʂ̓h��Ԃ���)�ݒ�.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory( &rdc, sizeof( rdc ) );
	rdc.CullMode = D3D11_CULL_BACK;
					//D3D11_CULL_NONE :�J�����O��؂�(���w�ʂ�`�悷��).
					//D3D11_CULL_BACK :�w�ʂ�`�悵�Ȃ�.
					//D3D11_CULL_FRONT:���ʂ�`�悵�Ȃ�.
	rdc.FrontCounterClockwise	= FALSE;
								//�|���S���̕\�������肷��t���O.
								//TRUE :�����Ȃ�O����,�E���Ȃ������.
								//FALSE:���̋t�ɂȂ�.
	rdc.DepthClipEnable	= FALSE;	//�����ɂ��ẴN���b�s���O�L��.

	if( isWire ){
		rdc.FillMode = D3D11_FILL_WIREFRAME;//���C���[�t���[��.
	}
	else{
		rdc.FillMode = D3D11_FILL_SOLID;	//�h��Ԃ�(�\���b�h).
	}


	ID3D11Device* pDevice = nullptr;
	pContext->GetDevice( &pDevice );
	if( !pDevice ) return;

	ID3D11RasterizerState* pIr	= nullptr;
	pDevice->CreateRasterizerState( &rdc, &pIr );
	pContext->RSSetState( pIr );
	SAFE_RELEASE( pIr );

}


//���f�����ς�.
void clsASSEMBLE_MODEL::AttachModel( 
	const enPARTS enParts,
	const SKIN_ENUM_TYPE PartsNum )
{
	UCHAR ucParts = static_cast<UCHAR>( enParts );
	assert( m_vpParts[ucParts] );
	m_vpParts[ucParts]->DetatchModel();
	m_vpParts[ucParts]->AttachModel( 
		m_wpResource->GetPartsModels( enParts, PartsNum ) );
	m_vpParts[ucParts]->SetAnimSpeed( m_dAnimSpd );
	
	//�p�[�c����������( �{�[����������p ).
	clsOPERATION_STRING OprtStr;
	string tmpName = sPARTS_NAME[ucParts];						//�ǂ̃p�[�c?.
	tmpName = OprtStr.ConsolidatedNumber( tmpName, PartsNum );	//���̃p�[�c�̉���?.
	m_vpParts[ucParts]->SetPartsName( tmpName );				//���O������.
//	AnimReSet();


	//GetPartsNum�֐��p.
	int iIndex = static_cast<int>( enParts );
	if( enParts == enPARTS::ARM_L ||
		enParts == enPARTS::ARM_R )
	{
		iIndex = enPARTS_TYPES::ARMS;
	}
	else if( enParts == enPARTS::WEAPON_L ){
		iIndex = enPARTS_TYPES::WEAPON_L;
	}
	else if( enParts == enPARTS::WEAPON_R ){
		iIndex = enPARTS_TYPES::WEAPON_R;
	}
	m_enPartsNum[ iIndex ] = static_cast< enPARTS_TYPES >( PartsNum );
}



//���O�̃t���[���ł́A�w��p�[�c�̎w��{�[���̍��W��Ԃ�.
D3DXVECTOR3 clsASSEMBLE_MODEL::GetBonePosPreviosFrame( 
	const enPARTS_INDEX enParts, 
	const int enBoneName,
	int iVecNum ) const
{
	if( enParts < static_cast<enPARTS_INDEX>( m_vpParts.size() ) ){
		return m_vpParts[ enParts ]->GetBonePosPreviosFrame( enBoneName, iVecNum );
	}

	return D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}



//�g�����X�t�H�[��.
void clsASSEMBLE_MODEL::SetPos( const D3DXVECTOR3 &vPos )
{
	m_Trans.vPos = vPos;

	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
//		m_wppParts[i]->SetPosition( m_Trans.vPos );
	}

	//���f���̊�_.
	m_vpParts[ucLEG]->SetPosition( m_Trans.vPos );


	m_vpParts[ucCORE]->SetPosition( 
 		m_vpParts[ucLEG]->GetBonePos( sBONE_NAME_LEG_TO_CORE ) 
//		+ D3DXVECTOR3( 0.0f, fADD_POS_Y, 0.0f )
		);
//	m_vpParts[ucCORE]->AddPosition( D3DXVECTOR3( 0.0f, -fADD_POS_Y*5, 0.0f ) );

	m_vpParts[ucHEAD]->SetPosition( 
		m_vpParts[ucCORE]->GetBonePos( sBONE_NAME_CORE_TO_HEAD ) );

	m_vpParts[ucARM_L]->SetPosition( 
		m_vpParts[ucCORE]->GetBonePos( sBONE_NAME_CORE_TO_ARM_L ) );

	m_vpParts[ucARM_R]->SetPosition( 
		m_vpParts[ucCORE]->GetBonePos( sBONE_NAME_CORE_TO_ARM_R ) );

	m_vpParts[ucWEAPON_L]->SetPosition( 
		m_vpParts[ucARM_L]->GetBonePos( sBONE_NAME_ARM_TO_WEAPON ) );
										   
	m_vpParts[ucWEAPON_R]->SetPosition( 
		m_vpParts[ucARM_R]->GetBonePos( sBONE_NAME_ARM_TO_WEAPON ) );

	//����̊p�x.
	FitJointModel( m_vpParts[ucWEAPON_L], m_vpParts[ucARM_L],
		sBONE_NAME_ARM_WEAPON_VEC_ROOT, sBONE_NAME_ARM_WEAPON_VEC_END );//ArmLJunctionWeapon.ArmLJunctionCore
	FitJointModel( m_vpParts[ucWEAPON_R], m_vpParts[ucARM_R],
		sBONE_NAME_ARM_WEAPON_VEC_ROOT, sBONE_NAME_ARM_WEAPON_VEC_END );


	m_vpParts[ucWEAPON_L]->AddRotation( { m_vpParts[ucARM_L]->GetRotation().x, 0.0f, 0.0f } );
	m_vpParts[ucWEAPON_R]->AddRotation( { m_vpParts[ucARM_R]->GetRotation().x, 0.0f, 0.0f } );



}



void clsASSEMBLE_MODEL::SetRot( const D3DXVECTOR3 &vRot )
{
	D3DXVECTOR3 tmpRot = vRot;

	GuardDirOver( tmpRot.x );
	GuardDirOver( tmpRot.y );
	GuardDirOver( tmpRot.z );

	m_Trans.fPitch	= tmpRot.x;
	m_Trans.fYaw	= tmpRot.y;
	m_Trans.fRoll	= tmpRot.z;

	for( UCHAR i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		m_vpParts[i]->SetRotation( tmpRot );
	}
}


void clsASSEMBLE_MODEL::SetScale( const float fScale )
{
	m_Trans.vScale = { fScale, fScale, fScale };
	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		m_vpParts[i]->SetScale( fScale );
	}
}



void clsASSEMBLE_MODEL::SetAnimSpd( const double &dSpd )
{
	m_dAnimSpd = dSpd;
	for( UCHAR i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		m_vpParts[i]->SetAnimSpeed( m_dAnimSpd );
	}
}



//�p�[�c�̃A�j���[�V�����ύX.
bool clsASSEMBLE_MODEL::PartsAnimChange( const enPARTS enParts, const int iIndex ) const
{
	char cPartsIndex = static_cast<char>( enParts );
	assert( m_vpParts[ cPartsIndex ] );
	return m_vpParts[ cPartsIndex ]->PartsAnimChange( iIndex );
}




//�p�[�c�̃{�[���̍��W���擾.
D3DXVECTOR3 clsASSEMBLE_MODEL::GetBonePos( 
	const enPARTS enParts, const char* sBoneName ) const
{
	D3DXVECTOR3 vReturn = { 0.0f, 0.0f, 0.0f };
	char cTmpNum = static_cast<char>( enParts );

	assert( m_vpParts[ cTmpNum ] );
	vReturn = m_vpParts[ cTmpNum ]->GetBonePos( sBoneName );

	return vReturn;
}


//�{�[�������݂��邩.
bool clsASSEMBLE_MODEL::ExistsBone( const enPARTS enParts, const char* sBoneName ) const
{
	char cTmpNum = static_cast<char>( enParts );
	return m_vpParts[ cTmpNum ]->m_pMesh->ExistsBone( sBoneName );
}


//��]�l�}��.
float clsASSEMBLE_MODEL::GuardDirOver( float &outTheta ) const
{
	float fDIR_RIMIT = static_cast<float>( D3DX_PI * 2.0 ); 
	if( outTheta < 0.0 ){
		outTheta += fDIR_RIMIT;
	}
	else if( outTheta > fDIR_RIMIT ){
		outTheta -= fDIR_RIMIT;
	}
	else{
		return outTheta;
	}

	GuardDirOver( outTheta );

	return outTheta;
}




//�r�̊p�x�𕐊���͎ʂ���.
void clsASSEMBLE_MODEL::FitJointModel( 
	clsPARTS_BASE *pMover, clsPARTS_BASE *pBace,
	const char *RootBone, const char *EndBone ) const
{
	//�{�[���̃x�N�g�����o��( ���[�J�� ).
	D3DXVECTOR3 vVecLocal = 
		pBace->GetBonePos( EndBone, true ) - 
		pBace->GetBonePos( RootBone, true );
	D3DXVec3Normalize( &vVecLocal, &vVecLocal );

	//�{�[���̃x�N�g�����o��( ���[���h ).
	D3DXVECTOR3 vVecWorld = 
		pBace->GetBonePos( EndBone ) - 
		pBace->GetBonePos( RootBone );
	D3DXVec3Normalize( &vVecWorld, &vVecWorld );

	//�x�N�g�������]�l�����߂�.
	D3DXVECTOR3 vRot = { 0.0f, 0.0f, 0.0f };
//	vRot.x = atanf( vVec.y );//���̃Q�[���̎d�l�Ȃ琳��( 2018/06/19(��)���� )( �܂�S������ ).
	vRot.x = atan2f( vVecLocal.y, -vVecLocal.z );//.
	vRot.y = atan2f( -vVecWorld.x, -vVecWorld.z );//( ���́A�}�C�i�X���������Ă�����AX,Z������ւ���Ă���̂��Ƃ����΁A0�x�Ń��f�������������������� ).

	vRot.x = GuardDirOver( vRot.x );
	vRot.y = GuardDirOver( vRot.y );
//	vRot.z = GuardDirOver( vRot.z );

	pMover->SetRotation( vRot );
}


//�A�j���[�V�������Z�b�g.
void clsASSEMBLE_MODEL::AnimReSet() const
{
	for( UINT i=0; i<m_vpParts.size(); i++ ){
		m_vpParts[i]->PartsAnimChange( 0 );
	}
}


//�p�[�c�̐F�w��.
void clsASSEMBLE_MODEL::SetPartsColor( 
	const D3DXVECTOR4 &vColor, 
	const unsigned int uiMaskNum )
{
	if( uiMaskNum >= m_vecvColor.size() ){
		return;
	}
	m_vecvColor[ uiMaskNum ] = vColor;
}

D3DXVECTOR4 clsASSEMBLE_MODEL::GetPartsColor( const unsigned int uiMaskNum ) const
{
	if( uiMaskNum >= m_vecvColor.size() ){
		float fERROR_NUM = -1.0f;
		return D3DXVECTOR4( fERROR_NUM, fERROR_NUM, fERROR_NUM, fERROR_NUM );
	}
	return m_vecvColor[ uiMaskNum ];
}

bool clsASSEMBLE_MODEL::IncrementColor( 
	const clsROBO_STATUS::enCOLOR_GAGE enColorGage )
{
	bool isChange = false;

	m_iColorRank[ enColorGage ] ++;
	if( m_iColorRank[ enColorGage ] > iCOLOR_GRADATION_MAX ){
		m_iColorRank[ enColorGage ] = iCOLOR_GRADATION_MAX;
	}
	else if( m_iColorRank[ enColorGage ] < iCOLOR_GRADATION_MIN ){
		m_iColorRank[ enColorGage ] = iCOLOR_GRADATION_MIN;
	}
	else{
		isChange = true;
	}

	UpdateColor( enColorGage );

	return isChange;
}
bool clsASSEMBLE_MODEL::DecrementColor( 
	const clsROBO_STATUS::enCOLOR_GAGE enColorGage )
{
	bool isChange = false;

	m_iColorRank[ enColorGage ] --;
	if( m_iColorRank[ enColorGage ] < iCOLOR_GRADATION_MIN ){
		m_iColorRank[ enColorGage ] = iCOLOR_GRADATION_MIN;
	}
	else if( m_iColorRank[ enColorGage ] > iCOLOR_GRADATION_MAX ){
		m_iColorRank[ enColorGage ] = iCOLOR_GRADATION_MAX;
	}
	else{
		isChange = true;
	}

	UpdateColor( enColorGage );

	return isChange;
}

void clsASSEMBLE_MODEL::UpdateColor( const clsROBO_STATUS::enCOLOR_GAGE enColorGage )
{

	const int iBASE_NUMBER = 0;
	const int iARMOR_NUMBER = 1;
	const float fNEW_COLOR = 
		static_cast<float>( m_iColorRank[ enColorGage ] ) / 
		static_cast<float>( iCOLOR_GRADATION_MAX );

	switch( enColorGage )
	{
	case clsROBO_STATUS::enCOLOR_GAGE_BASE_R:
		m_vecvColor[ iBASE_NUMBER ].x = fNEW_COLOR;
		break;
	case clsROBO_STATUS::enCOLOR_GAGE_BASE_G:
		m_vecvColor[ iBASE_NUMBER ].y = fNEW_COLOR;
		break;
	case clsROBO_STATUS::enCOLOR_GAGE_BASE_B:
		m_vecvColor[ iBASE_NUMBER ].z = fNEW_COLOR;
		break;
	case clsROBO_STATUS::enCOLOR_GAGE_ARMOR_R:
		m_vecvColor[ iARMOR_NUMBER ].x = fNEW_COLOR;
		break;
	case clsROBO_STATUS::enCOLOR_GAGE_ARMOR_G:
		m_vecvColor[ iARMOR_NUMBER ].y = fNEW_COLOR;
		break;
	case clsROBO_STATUS::enCOLOR_GAGE_ARMOR_B:
		m_vecvColor[ iARMOR_NUMBER ].z = fNEW_COLOR;
		break;
	}
}



void clsASSEMBLE_MODEL::ModelUpdate() const
{
	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		m_vpParts[i]->ModelUpdate( m_vpParts[i]->m_Trans );
	}
}


float clsASSEMBLE_MODEL::GetColorGradation( 
	const clsROBO_STATUS::enCOLOR_GAGE enColorGage ) const
{
	const float fERROR = -1.0f; 
	if( enColorGage >= clsROBO_STATUS::enCOLOR_GAGE_size ){
		return fERROR;
	}
	else if( enColorGage < 0.0f ){
		return fERROR;
	}

	float fReturn = static_cast<float>( m_iColorRank[ enColorGage ] ) / static_cast<float>( iCOLOR_GRADATION_MAX );
	return fReturn;
}



//0~16�ŕԂ�.
int clsASSEMBLE_MODEL::GetColorRank( const clsROBO_STATUS::enCOLOR_GAGE enColorGage ) const
{
	if( enColorGage >= clsROBO_STATUS::enCOLOR_GAGE_size ||
		enColorGage < 0 )
	{
		return 0;
	}

	return m_iColorRank[ enColorGage ];
}





#ifdef _DEBUG
//�e�p�[�c��pos.
D3DXVECTOR3 clsASSEMBLE_MODEL::GetPartsPos( const UCHAR ucParts ) const
{
	assert( m_vpParts[ucParts] );
	return m_vpParts[ucParts]->GetPosition();
}
#endif//#ifdef _DEBUG

