#include "PartsWindowModelForArms.h"

//using namespace std;

//パーツ透過値.
const D3DXVECTOR4 vCOLOR_NORMAL = { 1.0f, 1.0f, 1.0f, 1.0f };
const D3DXVECTOR4 vCOLOR_ALPHA =  { 0.0f, 0.0f, 0.0f, 1.0f };


clsPARTS_WINDOW_MODEL_FOR_ARMS::clsPARTS_WINDOW_MODEL_FOR_ARMS()
{
}

clsPARTS_WINDOW_MODEL_FOR_ARMS::~clsPARTS_WINDOW_MODEL_FOR_ARMS()
{
}

void clsPARTS_WINDOW_MODEL_FOR_ARMS::UpdateProduct()
{
	ModelUpdate();
}


void clsPARTS_WINDOW_MODEL_FOR_ARMS::Render(
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3& vLight, 
	const D3DXVECTOR3& vEye,
	const enPARTS_TYPES RenderParts/*,
	const D3DXVECTOR4& vColor,
	const bool isAlpha*/ )
{
	D3DXVECTOR4 vTmpColor;

	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		vTmpColor = CreateColor( RenderParts, i );
		SetPos( GetPos() );
		m_vpParts[i]->ModelUpdate( m_vpParts[i]->m_Trans );
		m_vpParts[i]->ModelRender( mView, mProj, vLight, vEye, vTmpColor, true );
	}
}

//色を吐き出す.
D3DXVECTOR4 clsPARTS_WINDOW_MODEL_FOR_ARMS::CreateColor( const enPARTS_TYPES AlphaParts, const UINT uiIndex )
{
	D3DXVECTOR4 vReturn = vCOLOR_ALPHA;

	switch( AlphaParts )
	{
	case LEG:
		if( uiIndex == static_cast<UINT>( enPARTS::LEG ) ){
			vReturn = vCOLOR_NORMAL;
		}
		break;
	case CORE:
		if( uiIndex == static_cast<UINT>( enPARTS::CORE ) ){
			vReturn = vCOLOR_NORMAL;
		}
		break;
	case HEAD:
		if( uiIndex == static_cast<UINT>( enPARTS::HEAD ) ){
			vReturn = vCOLOR_NORMAL;
		}
		break;
	case ARMS:
		//両腕とも.
		if( uiIndex == static_cast<UINT>( enPARTS::ARM_L ) ||
			uiIndex == static_cast<UINT>( enPARTS::ARM_R ) )
		{
			vReturn = vCOLOR_NORMAL;
		}
		break;
	case WEAPON_L:
		if( uiIndex == static_cast<UINT>( enPARTS::WEAPON_L ) ){
			vReturn = vCOLOR_NORMAL;
		}
		break;
	case WEAPON_R:
		if( uiIndex == static_cast<UINT>( enPARTS::WEAPON_R ) ){
			vReturn = vCOLOR_NORMAL;
		}
		break;
	case enPARTS_TYPES::ENUM_SIZE:
	default:
		break;
	}

	return vReturn;
}
