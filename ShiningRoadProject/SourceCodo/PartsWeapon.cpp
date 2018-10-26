#include "PartsWeapon.h"


clsPARTS_WEAPON::clsPARTS_WEAPON()
{
}

clsPARTS_WEAPON::~clsPARTS_WEAPON()
{
}

//作成時.
void clsPARTS_WEAPON::CreateProduct()
{
}

//毎フレーム.
void clsPARTS_WEAPON::UpdateProduct()
{
}



//直前のフレームのボーンの座標を持ってくる.
D3DXVECTOR3 clsPARTS_WEAPON::GetBonePosPreviosFrame( 
	const int enBoneName, 
	int iVecNum )
{

	switch( enBoneName )
	{
	case enWEAPON_BONE_POSITIONS_MUZZLE_ROOT:
		return m_BonePositions.vMuzzleRoot;

	case enWEAPON_BONE_POSITIONS_MUZZLE_END:
		return m_BonePositions.vMuzzleEnd;

	case enWEAPON_BONE_POSITIONS_CARTRIDGE_INJECTION:
		return m_BonePositions.vCartridgeInjection;


	default:
		assert( !"不正な値" );
		GetBonePosPreviosFrame( enBoneName, iVecNum );
		break;
	}


	D3DXVECTOR3 vReturnPos = { 0.0f, 0.0f, 0.0f };
	return vReturnPos;
}

//↑で使うためにボーン座標を記録する( Renderの直後に使う ).
void clsPARTS_WEAPON::UpdateBonePosPreviosFrame() 
{
	m_BonePositions.vMuzzleRoot			= this->GetBonePos( sBONE_NAME_WEAPON_MUZZLE_ROOT );
	m_BonePositions.vMuzzleEnd			= this->GetBonePos( sBONE_NAME_WEAPON_MUZZLE_END );
	m_BonePositions.vCartridgeInjection	= this->GetBonePos( sBONE_NAME_WEAPON_CARTRIDGE_INJECTION );
}
