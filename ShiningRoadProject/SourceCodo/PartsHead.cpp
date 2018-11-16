#include "PartsHead.h"


clsPARTS_HEAD::clsPARTS_HEAD()
{
}

clsPARTS_HEAD::~clsPARTS_HEAD()
{
}

//作成時.
void clsPARTS_HEAD::InitProduct()
{
}

//毎フレーム.
void clsPARTS_HEAD::UpdateProduct()
{
}



//直前のフレームのボーンの座標を持ってくる.
D3DXVECTOR3 clsPARTS_HEAD::GetBonePosPreviosFrame( 
	const int enBoneName, 
	int iVecNum ) const
{

	switch( enBoneName )
	{
	case enHEAD_BONE_POSITIONS_CENTER:
		return m_BonePositions.vCenter;


	default:
		assert( !"不正な値" );
		GetBonePosPreviosFrame( enBoneName, iVecNum );
		break;
	}


	D3DXVECTOR3 vReturnPos = { 0.0f, 0.0f, 0.0f };
	return vReturnPos;
}

////↑で使うためにボーン座標を記録する( Renderの直後に使う ).
//void clsPARTS_HEAD::UpdateBonePosPreviosFrame() 
//{
//	m_BonePositions.vCenter	= this->GetBonePos( sBONE_NAME_HEAD_CENTER );
//}
