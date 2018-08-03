#include "PartsWindowModel.h"

using namespace std;

//受け取る選択肢.
const short SELECT_TYPE_LEG		= 0;
const short SELECT_TYPE_CORE	= 1;
const short SELECT_TYPE_HEAD	= 2;
const short SELECT_TYPE_ARMS	= 3;
const short SELECT_TYPE_WEP_L	= 4;
const short SELECT_TYPE_WEP_R	= 5;

//モデル回転速度.
const float fSPN_SPD = 3.14f * 1.5f / 180.0f;

//各パーツの中心位置ボーン名( 添え字は上記「受け取る選択肢」 ).
const string sBOPNE_NAME_PARTS_CENTER[] =
{
	"JunctionCore",
	"Jenerator",
	"Center",
	"Joint02",
	"null",
	"null"
};



clsPARTS_WINDOW_MODEL::clsPARTS_WINDOW_MODEL( clsResource* const pResource, clsROBO_STATUS* const pStatus )
	:m_wpResource( pResource )
{
	m_upSelectParts = make_unique< clsASSEMBLE_MODEL >();
	m_upSelectParts->Create( pResource, pStatus );
}

clsPARTS_WINDOW_MODEL::~clsPARTS_WINDOW_MODEL()
{
	m_wpResource = nullptr;
}



void clsPARTS_WINDOW_MODEL::Update( const short Type, const short Num )
{
	//回転.
	m_upSelectParts->AddRot( { 0.0f, fSPN_SPD, 0.0f } );

	m_SelectNum = static_cast< SKIN_ENUM_TYPE >( Num );

	switch( Type )
	{
	case SELECT_TYPE_LEG:
		m_SelectType = enPARTS::LEG;
		break;
	case SELECT_TYPE_CORE:
		m_SelectType = enPARTS::CORE;
		break;
	case SELECT_TYPE_HEAD:
		m_SelectType = enPARTS::HEAD;
		break;
	case SELECT_TYPE_WEP_L:
		m_SelectType = enPARTS::WEAPON_L;
		break;
	case SELECT_TYPE_WEP_R:
		m_SelectType = enPARTS::WEAPON_R;
		break;

	case SELECT_TYPE_ARMS:
		m_SelectType = enPARTS::ARM_L;
		break;

	default:
		return;
	}

	m_upSelectParts->AttachModel(
		m_SelectType, m_SelectNum );

	//右腕も変更する.
	if( Type == SELECT_TYPE_ARMS ){
		m_SelectType = enPARTS::ARM_R;
		m_upSelectParts->AttachModel(
			m_SelectType, m_SelectNum );
		m_SelectType = enPARTS::ARM_L;
	}
}

void clsPARTS_WINDOW_MODEL::Render(
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3& vLight, 
	const D3DXVECTOR3& vEye )
{
	assert( m_upSelectParts );
//	m_upSelectParts->ModelUpdate();

	m_upSelectParts->Render( mView, mProj, vLight, vEye );
}


//アセンブルシーンのパーツウィンドウのカメラの高さの基準にする.
D3DXVECTOR3 clsPARTS_WINDOW_MODEL::GetSelectPartsHeight()
{
	D3DXVECTOR3 vReturn = { 0.0f, 0.0f, 0.0f };

	short tmpIndex = static_cast<short>( m_SelectType );

	switch( tmpIndex )
	{
	case SELECT_TYPE_LEG:
		tmpIndex = SELECT_TYPE_LEG;
		break;
	case SELECT_TYPE_CORE:
		tmpIndex = SELECT_TYPE_CORE;
		break;
	case SELECT_TYPE_HEAD:
		tmpIndex = SELECT_TYPE_HEAD;
		break;
	case SELECT_TYPE_ARMS:
		tmpIndex = SELECT_TYPE_ARMS;
		break;
	case SELECT_TYPE_WEP_L:
	case SELECT_TYPE_WEP_R:
	default:
		tmpIndex = SELECT_TYPE_WEP_L;
		m_SelectType = enPARTS::WEAPON_L;
		break;
	}

	//高さだけ.
	vReturn.y = m_upSelectParts->GetBonePos( 
		m_SelectType, sBOPNE_NAME_PARTS_CENTER[ tmpIndex ].c_str() ).y;

	return vReturn;
}
