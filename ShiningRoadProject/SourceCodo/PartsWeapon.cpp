#include "PartsWeapon.h"


clsPARTS_WEAPON::clsPARTS_WEAPON()
{
}

clsPARTS_WEAPON::~clsPARTS_WEAPON()
{
}

//�쐬��.
void clsPARTS_WEAPON::InitProduct()
{
}

//���t���[��.
void clsPARTS_WEAPON::UpdateProduct()
{
}



//���O�̃t���[���̃{�[���̍��W�������Ă���.
D3DXVECTOR3 clsPARTS_WEAPON::GetBonePosPreviosFrame( 
	const int enBoneName, 
	int iVecNum ) const
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
		assert( !"�s���Ȓl" );
		GetBonePosPreviosFrame( enBoneName, iVecNum );
		break;
	}


	D3DXVECTOR3 vReturnPos = { 0.0f, 0.0f, 0.0f };
	return vReturnPos;
}

//���Ŏg�����߂Ƀ{�[�����W���L�^����( Render�̒���Ɏg�� ).
void clsPARTS_WEAPON::UpdateBonePosPreviosFrame() 
{
	m_BonePositions.vMuzzleRoot			= this->GetBonePos( sBONE_NAME_WEAPON_MUZZLE_ROOT );
	m_BonePositions.vMuzzleEnd			= this->GetBonePos( sBONE_NAME_WEAPON_MUZZLE_END );
	m_BonePositions.vCartridgeInjection	= this->GetBonePos( sBONE_NAME_WEAPON_CARTRIDGE_INJECTION );
}
