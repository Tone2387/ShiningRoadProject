#include "PartsWindowModelForArms.h"

//using namespace std;



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
	bool bRender;

	for( UINT i=0; i<m_vpParts.size(); i++ ){
		assert( m_vpParts[i] );
		//ï`âÊÇ∑ÇÈÇ©ÇµÇ»Ç¢Ç©ÇåàÇﬂÇÈ.
		bRender = isRender( RenderParts, i );
		//ÉÇÉfÉãòAåã.
		SetPos( GetPos() );
		m_vpParts[i]->ModelUpdate( m_vpParts[i]->m_Trans );
		//ï`âÊ.
		if( !bRender ) continue;
		m_vpParts[i]->ModelRender( mView, mProj, vLight, vEye, m_vecvColor[0], m_vecvColor[1] );
	}
}

//êFÇìfÇ´èoÇ∑.
bool clsPARTS_WINDOW_MODEL_FOR_ARMS::isRender( const enPARTS_TYPES AlphaParts, const UINT uiIndex )
{
	switch( AlphaParts )
	{
	case LEG:
		if( uiIndex == static_cast<UINT>( enPARTS::LEG ) ){
			return true;
		}
		break;
	case CORE:
		if( uiIndex == static_cast<UINT>( enPARTS::CORE ) ){
			return true;
		}
		break;
	case HEAD:
		if( uiIndex == static_cast<UINT>( enPARTS::HEAD ) ){
			return true;
		}
		break;
	case ARMS:
		//óºòrÇ∆Ç‡.
		if( uiIndex == static_cast<UINT>( enPARTS::ARM_L ) ||
			uiIndex == static_cast<UINT>( enPARTS::ARM_R ) )
		{
			return true;
		}
		break;
	case WEAPON_L:
		if( uiIndex == static_cast<UINT>( enPARTS::WEAPON_L ) ){
			return true;
		}
		break;
	case WEAPON_R:
		if( uiIndex == static_cast<UINT>( enPARTS::WEAPON_R ) ){
			return true;
		}
		break;
	case enPARTS_TYPES::ENUM_SIZE:
	default:
		break;
	}

	return false;
}
