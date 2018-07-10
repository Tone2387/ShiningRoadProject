#include "AssembleModel.h"

#include "OperationString.h"

using namespace std;

//配列の添え字.
const UCHAR ucLEG = static_cast<UCHAR>( enPARTS::LEG );
const UCHAR ucCORE = static_cast<UCHAR>( enPARTS::CORE );
const UCHAR ucHEAD = static_cast<UCHAR>( enPARTS::HEAD );
const UCHAR ucARM_L = static_cast<UCHAR>( enPARTS::ARM_L );
const UCHAR ucARM_R = static_cast<UCHAR>( enPARTS::ARM_R );
const UCHAR ucWEAPON_L = static_cast<UCHAR>( enPARTS::WEAPON_L );
const UCHAR ucWEAPON_R = static_cast<UCHAR>( enPARTS::WEAPON_R );

//パーツ種類の数.
const UCHAR	ucPARTS_MAX = static_cast<UCHAR>( enPARTS::MAX );
//パーツ名を渡すため.
const string sPARTS_NAME[ucPARTS_MAX] =
{
	"Leg", "Core", "Head", "ArmL", "ArmR", "WeaponL", "WeaponR"
};


//連結部分のボーン名.
#define BONE_NAME_LEG_TO_CORE		"LegJunctionCore"
#define BONE_NAME_CORE_TO_HEAD		"CoreJunctionHead"
#define BONE_NAME_CORE_TO_ARM_L	 	"CoreJunctionArmL"
#define BONE_NAME_CORE_TO_ARM_R	 	"CoreJunctionArmR"
#define BONE_NAME_ARM_L_TO_WEAPON_L "ArmLJunctionWeapon"
#define BONE_NAME_ARM_R_TO_WEAPON_R /*"ArmRJunctionWeapon"*/"JunctionWeapon"


#define BONE_NAME_WEAPON_VEC_ROOT /*"WeaponVecRoot"*/"JunctionWeapon"
#define BONE_NAME_WEAPON_VEC_END "WeaponVecEnd"

const double dANIM_SPD = 0.016;



clsASSEMBLE_MODEL::clsASSEMBLE_MODEL()
	:m_wpResource( nullptr )
	,m_upPartsFactory( nullptr )
	,m_vpParts()
	,m_dAnimSpd( 0.0 )
{
	m_dAnimSpd = dANIM_SPD;
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

//	SAFE_DELETE( m_upPartsFactory );
	if( m_upPartsFactory ){
		m_upPartsFactory.reset( nullptr );
	}

	m_wpResource = nullptr;
}



void clsASSEMBLE_MODEL::Create( clsResource* const pResource, clsROBO_STATUS* const pStatus )
{
	ASSERT_IF_NOT_NULL( m_upPartsFactory );
	assert( !m_vpParts.size() );
	ASSERT_IF_NOT_NULL( m_wpResource );

	m_wpResource = pResource;

	m_upPartsFactory = make_unique< clsFACTORY_PARTS >();

//	m_vpParts = new clsPARTS_BASE*[ucPARTS_MAX];
	m_vpParts.reserve( ucPARTS_MAX );
	for( UCHAR i=0; i<ucPARTS_MAX; i++ ){
		m_vpParts.push_back( nullptr );
		m_vpParts[i] = m_upPartsFactory->Create( static_cast<enPARTS>( i ) );
	}

	Init( pStatus );
}

//モデルの初期セット.
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

	SetPos( { 0.0f, 0.0f, 0.0f } );
	SetRot( { 0.0f, 0.0f, 0.0f } );
	SetScale( 1.0f );
	SetAnimSpd( dANIM_SPD );

//	AnimReSet();
}

void clsASSEMBLE_MODEL::UpDate()
{
	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		m_vpParts[i]->Update();
	}
}

void clsASSEMBLE_MODEL::Render(
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3& vLight, 
	const D3DXVECTOR3& vEye,
	const D3DXVECTOR4& vColor,
	const bool isAlpha )
{
	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		SetPos( GetPos() );
		m_vpParts[i]->ModelUpdate( m_vpParts[i]->m_Trans );
		m_vpParts[i]->ModelRender( mView, mProj, vLight, vEye, vColor, isAlpha );
	}
}


//モデルつけ変え.
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
	
	//パーツ名を教える( ボーンが無いよ用 ).
	clsOPERATION_STRING OprtStr;
	string tmpName = sPARTS_NAME[ucParts];						//どのパーツ?.
	tmpName = OprtStr.ConsolidatedNumber( tmpName, PartsNum );	//そのパーツの何番?.
	m_vpParts[ucParts]->SetPartsName( tmpName );				//名前教える.
//	AnimReSet();
}


//トランスフォーム.
void clsASSEMBLE_MODEL::SetPos( const D3DXVECTOR3 &vPos )
{
	m_Trans.vPos = vPos;

	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
//		m_wppParts[i]->SetPosition( m_Trans.vPos );
	}

	m_vpParts[ucLEG]->SetPosition( m_Trans.vPos );

	m_vpParts[ucCORE]->SetPosition( 
 		m_vpParts[ucLEG]->GetBonePos( BONE_NAME_LEG_TO_CORE ) );

	m_vpParts[ucHEAD]->SetPosition( 
		m_vpParts[ucCORE]->GetBonePos( BONE_NAME_CORE_TO_HEAD ) );

	m_vpParts[ucARM_L]->SetPosition( 
		m_vpParts[ucCORE]->GetBonePos( BONE_NAME_CORE_TO_ARM_L ) );

	m_vpParts[ucARM_R]->SetPosition( 
		m_vpParts[ucCORE]->GetBonePos( BONE_NAME_CORE_TO_ARM_R ) );

	m_vpParts[ucWEAPON_L]->SetPosition( 
		m_vpParts[ucARM_L]->GetBonePos( BONE_NAME_ARM_L_TO_WEAPON_L ) );
										   
	m_vpParts[ucWEAPON_R]->SetPosition( 
		m_vpParts[ucARM_R]->GetBonePos( BONE_NAME_ARM_R_TO_WEAPON_R ) );

	FitJointModel( m_vpParts[ucWEAPON_L], m_vpParts[ucARM_L],
		BONE_NAME_WEAPON_VEC_ROOT, BONE_NAME_WEAPON_VEC_END );//ArmLJunctionWeapon.ArmLJunctionCore
	FitJointModel( m_vpParts[ucWEAPON_R], m_vpParts[ucARM_R],
		BONE_NAME_WEAPON_VEC_ROOT, BONE_NAME_WEAPON_VEC_END );
}
void clsASSEMBLE_MODEL::AddPos( const D3DXVECTOR3 &vVec )
{
	SetPos( m_Trans.vPos + vVec );
}
D3DXVECTOR3 clsASSEMBLE_MODEL::GetPos() const
{
	return m_Trans.vPos;
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
void clsASSEMBLE_MODEL::AddRot( const D3DXVECTOR3 &vRot )
{
	SetRot( D3DXVECTOR3( m_Trans.fPitch, m_Trans.fYaw, m_Trans.fRoll ) + vRot );
}
D3DXVECTOR3 clsASSEMBLE_MODEL::GetRot() const
{
	return { m_Trans.fPitch, m_Trans.fYaw, m_Trans.fRoll };
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



//パーツのアニメーション変更.
bool clsASSEMBLE_MODEL::PartsAnimChange( const enPARTS enParts, const int iIndex )
{
	char cPartsIndex = static_cast<char>( enParts );
	assert( m_vpParts[ cPartsIndex ] );
	return m_vpParts[ cPartsIndex ]->PartsAnimChange( iIndex );
}



//回転値抑制.
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




//腕の角度を武器も模写する.
void clsASSEMBLE_MODEL::FitJointModel( 
	clsPARTS_BASE *pMover, clsPARTS_BASE *pBace,
	const char *RootBone, const char *EndBone )
{
	//ボーンのベクトルを出す( ローカル ).
	D3DXVECTOR3 vVecLocal = 
		pBace->GetBonePos( EndBone, true ) - 
		pBace->GetBonePos( RootBone, true );
	D3DXVec3Normalize( &vVecLocal, &vVecLocal );

	//ボーンのベクトルを出す( ワールド ).
	D3DXVECTOR3 vVecWorld = 
		pBace->GetBonePos( EndBone ) - 
		pBace->GetBonePos( RootBone );
	D3DXVec3Normalize( &vVecWorld, &vVecWorld );

	//ベクトルから回転値を求める.
	D3DXVECTOR3 vRot = { 0.0f, 0.0f, 0.0f };
//	vRot.x = atanf( vVec.y );//このゲームの仕様なら正解( 2018/06/19(火)現在 )( つまりゴリ押し ).
	vRot.x = atan2f( vVecLocal.y, -vVecLocal.z );//.
	vRot.y = atan2f( -vVecWorld.x, -vVecWorld.z );//( 何故、マイナスがかかっていたり、X,Zが入れ替わっているのかといえば、0度でモデルがこっちを向くから ).

	vRot.x = GuardDirOver( vRot.x );
	vRot.y = GuardDirOver( vRot.y );
//	vRot.z = GuardDirOver( vRot.z );

	pMover->SetRotation( vRot );
}


//アニメーションリセット.
void clsASSEMBLE_MODEL::AnimReSet()
{
	for( UINT i=0; i<m_vpParts.size(); i++ ){
		m_vpParts[i]->PartsAnimChange( 0 );
	}
}



#if _DEBUG
//各パーツのpos.
D3DXVECTOR3 clsASSEMBLE_MODEL::GetPartsPos( const UCHAR ucParts ) const
{
	ASSERT_IF_NULL( m_vpParts[ucParts] );
	return m_vpParts[ucParts]->GetPosition();
}
#endif//#if _DEBUG

