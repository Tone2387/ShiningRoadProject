#include "PartsWindowModel.h"

using namespace std;

namespace{

	//�󂯎��I����.
	const short SELECT_TYPE_LEG		= 0;
	const short SELECT_TYPE_CORE	= 1;
	const short SELECT_TYPE_HEAD	= 2;
	const short SELECT_TYPE_ARMS	= 3;
	const short SELECT_TYPE_WEP_L	= 4;
	const short SELECT_TYPE_WEP_R	= 5;

	//���f����]���x.
	const float fSPN_SPD = 3.14f * 1.5f / 180.0f;

	//�e�p�[�c�̒��S�ʒu�{�[����( �Y�����͏�L�u�󂯎��I�����v ).
	const string sBONE_NAME_PARTS_CENTER[] =
	{
		"JunctionCore",
		"Jenerator",
		"Center",
		"null",
		"MuzzleRoot",
		"MuzzleRoot"
	};
	//�r�̉��[.
	const string sBONE_NAME_PARTS_ARM_HAND = "JunctionWeapon";
	//����̒[.
	const char* sBONE_NAME_PARTS_WEP_ROOT = "null";
	const char* sBONE_NAME_PARTS_WEP_END = "MuzzleEnd";

	//�J�����ɓn��vec3�̕␳( �Y�����͏�L�u�󂯎��I�����v{ �uSELECT_TYPE_WEP_L�v�܂� } ).
	const D3DXVECTOR3 vCAM_OFFSET[4] =
	{
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
		D3DXVECTOR3( 0.0f, 0.0f, 30.0f ),
		D3DXVECTOR3( 0.0f, 0.0f, 50.0f ),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f )
	};

	//�}�X�N.
	const int iMASK_ARMOR = 0;
	const int iMASK_BASE = 0;
	const int iMASK_MAX = 2;

}


clsPARTS_WINDOW_MODEL::clsPARTS_WINDOW_MODEL( clsResource* const pResource, clsROBO_STATUS* const pStatus )
	:m_wpResource( pResource )
{
	m_upRoboModel = make_unique< clsPARTS_WINDOW_MODEL_FOR_ARMS >();
	m_upRoboModel->Create( pResource, pStatus );
	m_upRoboModel->SetPos( { 0.0f, 0.0f, 0.0f } );

	m_upWeapon = make_unique< clsPARTS_WEAPON >();
	m_upWeapon->Init();
	m_upWeapon->SetPosition( { 0.0f, 0.0f, 0.0f } );
}

clsPARTS_WINDOW_MODEL::~clsPARTS_WINDOW_MODEL()
{
	m_wpResource = nullptr;
}



void clsPARTS_WINDOW_MODEL::Update( const short Type, const short Num )
{
	//��].
	m_upRoboModel->AddRot( { 0.0f, fSPN_SPD, 0.0f } );
	m_upWeapon->SetRotation( m_upWeapon->GetRotation() + D3DXVECTOR3( 0.0f, fSPN_SPD, 0.0f ) );

	//�Y����.
	m_SelectNum = static_cast< SKIN_ENUM_TYPE >( Num );

	//�ǂ̃p�[�c?.
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

	case SELECT_TYPE_ARMS://�E�r�͌�ł��.
		m_SelectType = enPARTS::ARM_L;
		break;

	default:
		return;
	}

	//�S�g.
	m_upRoboModel->AttachModel(
		m_SelectType, m_SelectNum );

	//�E�r���ύX����.
	if( Type == SELECT_TYPE_ARMS ){
		m_SelectType = enPARTS::ARM_R;
		m_upRoboModel->AttachModel(
			m_SelectType, m_SelectNum );
		m_SelectType = enPARTS::ARM_L;
	}
	m_upRoboModel->UpDate();
	for( int i=0; i<iMASK_MAX; i++ ){
		m_upRoboModel->SetPartsColor( m_vecvColors[i], i );
	}

	//����.
	m_upWeapon->AttachModel( 
		m_wpResource->GetPartsModels(
			enPARTS::WEAPON_L, m_SelectNum ) );
}

void clsPARTS_WINDOW_MODEL::Render(
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3& vLight, 
	const D3DXVECTOR3& vEye,
	const bool isRender ) const
{
	if( !isRender ) return;

	switch( m_SelectType )
	{
	case enPARTS::LEG:
	case enPARTS::CORE:
	case enPARTS::HEAD:
	case enPARTS::ARM_L:
	case enPARTS::ARM_R:
		assert( m_upRoboModel );
		m_upRoboModel->Render( mView, mProj, vLight, vEye, 
			static_cast< clsASSEMBLE_MODEL::enPARTS_TYPES >( m_SelectType ) );
		break;
	case enPARTS::WEAPON_L:
	case enPARTS::WEAPON_R:
		assert( m_upWeapon );
		m_upWeapon->ModelUpdate( m_upWeapon->m_Trans );
		m_upWeapon->Render( mView, mProj, vLight, vEye, m_vecvColors[0], m_vecvColors[1] );
		break;
	}

}


//�A�Z���u���V�[���̃p�[�c�E�B���h�E�̃J�����̍����̊�ɂ���.
D3DXVECTOR3 clsPARTS_WINDOW_MODEL::GetSelectPartsHeight()
{
	D3DXVECTOR3 vReturn = { 0.0f, 0.0f, 0.0f };

	//�{�[�����̔z��̓Y����.
	short tmpIndex = static_cast< short >( m_SelectType );

	//����͑S�č��Ƃ���( �킴�킴switch���ɂ��Ă���̂́A���̃p�[�c����������\�������������� ).
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

	//���̃p�[�c�̒��S���E�B���h�E�̒��S�Ɏ����Ă���( �������� ).
	//����.
	if( tmpIndex == SELECT_TYPE_WEP_L ){
		m_upWeapon->ModelUpdate( m_upWeapon->m_Trans );
		
	}
	//����ȊO.
	else{
		m_upRoboModel->UpDate();
		vReturn.y = m_upRoboModel->GetBonePos( 
			m_SelectType, sBONE_NAME_PARTS_CENTER[ tmpIndex ].c_str() ).y;
	}

	//�r�͓���.
	if( tmpIndex == SELECT_TYPE_LEG ){
		const float fHARH = 0.5f;
		vReturn.y *= fHARH;
	}
	//�r������.
	else if( tmpIndex == SELECT_TYPE_ARMS ){
		float fHandY = m_upRoboModel->GetBonePos( 
			m_SelectType, sBONE_NAME_PARTS_ARM_HAND.c_str() ).y;
		const float fHARH = 0.5f;
		vReturn.y = fHARH * ( vReturn.y + fHandY );
	}
	//���������.
	else if( tmpIndex == SELECT_TYPE_WEP_L ){
		const float fHARH = 0.5f;
		D3DXVECTOR3 vWepCenter;
		vWepCenter = 
			m_upWeapon->GetBonePos( sBONE_NAME_PARTS_WEP_ROOT ) + 
			m_upWeapon->GetBonePos( sBONE_NAME_PARTS_WEP_END );
		vWepCenter *= fHARH;
		vReturn = vWepCenter;
	}

	//�g�k�I��.
	vReturn += vCAM_OFFSET[ tmpIndex ];

	return vReturn;
}

