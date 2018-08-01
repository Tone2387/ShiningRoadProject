#include "PartsWindowModel.h"

using namespace std;

//Žó‚¯Žæ‚é‘I‘ðŽˆ.
const short SELECT_TYPE_LEG		= 0;
const short SELECT_TYPE_CORE	= 1;
const short SELECT_TYPE_HEAD	= 2;
const short SELECT_TYPE_ARMS	= 3;
const short SELECT_TYPE_WEP_L	= 4;
const short SELECT_TYPE_WEP_R	= 5;

clsPARTS_WINDOW_MODEL::clsPARTS_WINDOW_MODEL( clsResource* const pResource )
	:m_wpResource( pResource )
{
	m_upSelectParts = make_unique< clsASSEMBLE_MODEL >();
}

clsPARTS_WINDOW_MODEL::~clsPARTS_WINDOW_MODEL()
{
	m_wpResource = nullptr;
}



void clsPARTS_WINDOW_MODEL::Update( const short Type, const short Num )
{
	enPARTS tmpParts;

	switch( Type )
	{
	case SELECT_TYPE_LEG:
		tmpParts = enPARTS::LEG;
		break;
	case SELECT_TYPE_CORE:
		tmpParts = enPARTS::CORE;
		break;
	case SELECT_TYPE_HEAD:
		tmpParts = enPARTS::HEAD;
		break;
	case SELECT_TYPE_WEP_L:
		tmpParts = enPARTS::WEAPON_L;
		break;
	case SELECT_TYPE_WEP_R:
		tmpParts = enPARTS::WEAPON_R;
		break;

	case SELECT_TYPE_ARMS:
		tmpParts = enPARTS::ARM_L;
		break;

	default:
		return;
	}

//	m_upSelectParts->AttachModel(
//		m_wpResource->GetPartsModels(
//		tmpParts, static_cast< SKIN_ENUM_TYPE >( Num ) ) );
//
//	if(){}
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

