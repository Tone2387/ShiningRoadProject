#include "PartsHead.h"


clsPARTS_HEAD::clsPARTS_HEAD()
{
}

clsPARTS_HEAD::~clsPARTS_HEAD()
{
}

//�쐬��.
void clsPARTS_HEAD::InitProduct()
{
}

//���t���[��.
void clsPARTS_HEAD::UpdateProduct()
{
}



//���O�̃t���[���̃{�[���̍��W�������Ă���.
D3DXVECTOR3 clsPARTS_HEAD::GetBonePosPreviosFrame( 
	const int enBoneName, 
	int iVecNum ) const
{

	switch( enBoneName )
	{
	case enHEAD_BONE_POSITIONS_CENTER:
		return m_BonePositions.vCenter;


	default:
		assert( !"�s���Ȓl" );
		GetBonePosPreviosFrame( enBoneName, iVecNum );
		break;
	}


	D3DXVECTOR3 vReturnPos = { 0.0f, 0.0f, 0.0f };
	return vReturnPos;
}

////���Ŏg�����߂Ƀ{�[�����W���L�^����( Render�̒���Ɏg�� ).
//void clsPARTS_HEAD::UpdateBonePosPreviosFrame() 
//{
//	m_BonePositions.vCenter	= this->GetBonePos( sBONE_NAME_HEAD_CENTER );
//}
