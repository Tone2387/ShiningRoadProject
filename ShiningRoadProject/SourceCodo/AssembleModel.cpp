#include "AssembleModel.h"

#include "OperationString.h"

using namespace std;

//legのモデルの足元がおかしい場合.
//#define LEG_MODEL_POSITION_BASE_Y_OFFSET

#if _DEBUG
#include "CharaStatic.h"
#endif//#if _DEBUG

namespace{

#if _DEBUG
		//足元.
		std::unique_ptr<clsCharaStatic> m_upFootNull;
		std::unique_ptr<clsCharaStatic> m_upFootPosBase;
		std::unique_ptr<clsCharaStatic> m_upFootTrasnPos;
#endif//#if _DEBUG


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



	const double dANIM_SPD = 0.016;


	//パーツ透過値.
	const D3DXVECTOR4 vCOLOR_NORMAL = { 1.0f, 1.0f, 1.0f, 1.0f };
	const D3DXVECTOR4 vCOLOR_ALPHA =  { 10.0f, 10.0f, 0.0f, 0.65f };

	//色変更のマスク種類.
	const int iMASK_MAX_NUM = 2;

	//色の段階.gradation
	const int iCOLOR_GRADATION_MAX = 16;
	const int iCOLOR_GRADATION_MIN = 1;
}


clsASSEMBLE_MODEL::clsASSEMBLE_MODEL()
	:m_wpResource( nullptr )
	,m_upPartsFactory( nullptr )
	,m_vpParts()
	,m_dAnimSpd( 1.0 )
	,m_enPartsNum()
	,m_iColorRank()
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

//	SAFE_DELETE( m_upPartsFactory );
	if( m_upPartsFactory ){
		m_upPartsFactory.reset( nullptr );
	}

	m_wpResource = nullptr;
}



void clsASSEMBLE_MODEL::Create( 
	clsResource* const pResource, 
	clsROBO_STATUS* const pStatus )
{
	assert( !m_upPartsFactory );
	assert( !m_vpParts.size() );
	assert( !m_wpResource );

	m_wpResource = pResource;

	m_upPartsFactory = make_unique< clsFACTORY_PARTS >();

	m_vpParts.resize( ucPARTS_MAX, nullptr );
	for( UCHAR i=0; i<ucPARTS_MAX; i++ ){
		m_vpParts[i] = m_upPartsFactory->Create( static_cast<enPARTS>( i ) );
	}


	Init( pStatus );

	CreateProduct( pStatus );

#if _DEBUG
	float fRATE = 2.5f;
	//足元.
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
#endif//#if _DEBUG

}

void clsASSEMBLE_MODEL::CreateProduct( clsROBO_STATUS* const pStatus )
{
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

void clsASSEMBLE_MODEL::UpdateProduct()
{}

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

	//モデルの足元.
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

		//ボーン位置を教え込ませる.
		m_vpParts[i]->UpdateBonePosPreviosFrame();
	}

#ifdef LEG_MODEL_POSITION_BASE_Y_OFFSET
	m_Trans.vPos -= D3DXVECTOR3( 0.0f, fADD_POS_Y, 0.0f );

#endif//#define LEG_MODEL_POSITION_BASE_Y_OFFSET
#if _DEBUG
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
	
#endif//#if _DEBUG

	//ワイヤーフレーム解除.
	ChangeWireFrame( false, pContext );

}


//色を吐き出す.
D3DXVECTOR4 clsASSEMBLE_MODEL::CreateColor( 
	const enPARTS_TYPES AlphaParts, 
	const UINT uiIndex,
	const unsigned int uiMaskNum,
	ID3D11DeviceContext* const pContext  )
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
		//両腕とも.
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

//ワイヤーフレーム切替.
void clsASSEMBLE_MODEL::ChangeWireFrame(
	const bool isWire,
	ID3D11DeviceContext* const  pContext )
{
	if( !pContext ){
		return;
	}

	//ラスタライズ(面の塗りつぶし方)設定.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory( &rdc, sizeof( rdc ) );
	rdc.CullMode = D3D11_CULL_BACK;
					//D3D11_CULL_NONE :カリングを切る(正背面を描画する).
					//D3D11_CULL_BACK :背面を描画しない.
					//D3D11_CULL_FRONT:正面を描画しない.
	rdc.FrontCounterClockwise	= FALSE;
								//ポリゴンの表裏を決定するフラグ.
								//TRUE :左回りなら前向き,右回りなら後ろ向き.
								//FALSE:↑の逆になる.
	rdc.DepthClipEnable	= FALSE;	//距離についてのクリッピング有効.

	if( isWire ){
		rdc.FillMode = D3D11_FILL_WIREFRAME;//ワイヤーフレーム.
	}
	else{
		rdc.FillMode = D3D11_FILL_SOLID;	//塗りつぶし(ソリッド).
	}


	ID3D11Device* pDevice = nullptr;
	pContext->GetDevice( &pDevice );
	if( !pDevice ) return;

	ID3D11RasterizerState* pIr	= nullptr;
	pDevice->CreateRasterizerState( &rdc, &pIr );
	pContext->RSSetState( pIr );
	SAFE_RELEASE( pIr );

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


	//GetPartsNum関数用.
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



//直前のフレームでの、指定パーツの指定ボーンの座標を返す.
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



//トランスフォーム.
void clsASSEMBLE_MODEL::SetPos( const D3DXVECTOR3 &vPos )
{
	m_Trans.vPos = vPos;

	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
//		m_wppParts[i]->SetPosition( m_Trans.vPos );
	}

	//モデルの基点.
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

	//武器の角度.
	FitJointModel( m_vpParts[ucWEAPON_L], m_vpParts[ucARM_L],
		sBONE_NAME_ARM_WEAPON_VEC_ROOT, sBONE_NAME_ARM_WEAPON_VEC_END );//ArmLJunctionWeapon.ArmLJunctionCore
	FitJointModel( m_vpParts[ucWEAPON_R], m_vpParts[ucARM_R],
		sBONE_NAME_ARM_WEAPON_VEC_ROOT, sBONE_NAME_ARM_WEAPON_VEC_END );


	m_vpParts[ucWEAPON_L]->AddRotation( { m_vpParts[ucARM_L]->GetRotation().x, 0.0f, 0.0f } );
	m_vpParts[ucWEAPON_R]->AddRotation( { m_vpParts[ucARM_R]->GetRotation().x, 0.0f, 0.0f } );



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


int clsASSEMBLE_MODEL::GetPartsNum( const enPARTS_TYPES enPartsType )
{
	return m_enPartsNum[ enPartsType ];
}


//パーツのアニメーション変更.
bool clsASSEMBLE_MODEL::PartsAnimChange( const enPARTS enParts, const int iIndex )
{
	char cPartsIndex = static_cast<char>( enParts );
	assert( m_vpParts[ cPartsIndex ] );
	return m_vpParts[ cPartsIndex ]->PartsAnimChange( iIndex );
}




//パーツのボーンの座標を取得.
D3DXVECTOR3 clsASSEMBLE_MODEL::GetBonePos( 
	const enPARTS enParts, const char* sBoneName )
{
	D3DXVECTOR3 vReturn = { 0.0f, 0.0f, 0.0f };
	char cTmpNum = static_cast<char>( enParts );

	assert( m_vpParts[ cTmpNum ] );
	vReturn = m_vpParts[ cTmpNum ]->GetBonePos( sBoneName );

	return vReturn;
}


//ボーンが存在するか.
bool clsASSEMBLE_MODEL::ExistsBone( const enPARTS enParts, const char* sBoneName )
{
	char cTmpNum = static_cast<char>( enParts );
	return m_vpParts[ cTmpNum ]->m_pMesh->ExistsBone( sBoneName );
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


//パーツの色指定.
void clsASSEMBLE_MODEL::SetPartsColor( 
	const D3DXVECTOR4 &vColor, 
	const unsigned int uiMaskNum )
{
	if( uiMaskNum >= m_vecvColor.size() ){
		return;
	}
	m_vecvColor[ uiMaskNum ] = vColor;
}

D3DXVECTOR4 clsASSEMBLE_MODEL::GetPartsColor( const unsigned int uiMaskNum )
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



void clsASSEMBLE_MODEL::ModelUpdate()
{
	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		m_vpParts[i]->ModelUpdate( m_vpParts[i]->m_Trans );
	}
}


float clsASSEMBLE_MODEL::GetColorGradation( const clsROBO_STATUS::enCOLOR_GAGE enColorGage )
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

std::vector< D3DXVECTOR4 > clsASSEMBLE_MODEL::GetColor()
{
	return m_vecvColor;
}


//0~16で返す.
int clsASSEMBLE_MODEL::GetColorRank( const clsROBO_STATUS::enCOLOR_GAGE enColorGage )
{
	if( enColorGage >= clsROBO_STATUS::enCOLOR_GAGE_size ||
		enColorGage < 0 )
	{
		return 0;
	}

	return m_iColorRank[ enColorGage ];
}





#if _DEBUG
//各パーツのpos.
D3DXVECTOR3 clsASSEMBLE_MODEL::GetPartsPos( const UCHAR ucParts ) const
{
	assert( m_vpParts[ucParts] );
	return m_vpParts[ucParts]->GetPosition();
}
#endif//#if _DEBUG

