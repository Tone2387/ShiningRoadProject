#include "PartsCore.h"
#include "OperationString.h"

using namespace std;


clsPARTS_CORE::clsPARTS_CORE()
{
}

clsPARTS_CORE::~clsPARTS_CORE()
{
}

//作成時.
void clsPARTS_CORE::InitProduct()
{
	clsOPERATION_STRING OprtStr;

	//ブースターの数を調べる( 何番まであるか ).
	string sBoneName = sBONE_NAME_CORE_BOOSTER_ROOT;
	int iBoosterNum = 0;
	//ボーンがある限り繰り返し : ボーン名を渡す : 基礎となるボーン名に数字を連結 : 引数はchar*型.
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}

#ifdef _DEBUG
	//RootとEndと数が違うとエラー.
	int iBoosterNumOld = iBoosterNum;
	sBoneName = sBONE_NAME_CORE_BOOSTER_END;
	iBoosterNum = 0;
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
	if( iBoosterNum != iBoosterNumOld ){
		ERR_MSG( m_sPartsName.c_str(), "ブースターのボーンの数がRootとEndで違うぞ" );
	}
#endif//#ifndef _DEBUG

	//while文を抜けたiBoosterNumの番号のBoneは無い = 直前の番号までは有る = iBoosterNumはsizeである.
	m_BonePositions.vecvBoosterRoot.resize(iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_BonePositions.vecvBoosterEnd.resize( iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
}

//毎フレーム.
void clsPARTS_CORE::UpdateProduct()
{
}



//直前のフレームのボーンの座標を持ってくる.
D3DXVECTOR3 clsPARTS_CORE::GetBonePosPreviosFrame( 
	const int enBoneName, 
	int iVecNum )
{
//	if( iVecNum >= m_BonePositions.vecvBoosterRoot.size() ){
//		iVecNum = m_BonePositions.vecvBoosterRoot.size() - 1;
//	}
//	else if( iVecNum < 0 ){
//		iVecNum = 0;
//	}

	switch( enBoneName )
	{
	case enCORE_BONE_POSITIONS_BOOSTER_ROOT:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterRoot.size() );
		return m_BonePositions.vecvBoosterRoot[ iVecNum ];

	case enCORE_BONE_POSITIONS_BOOSTER_END:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterRoot.size() );
		return m_BonePositions.vecvBoosterEnd[ iVecNum ];

	case enCORE_BONE_POSITIONS_JENERATOR:
		return m_BonePositions.vJenerator;

	case enCORE_BONE_POSITIONS_JUNCTION_HEAD:
		return m_BonePositions.vJunctionHead;

	case enCORE_BONE_POSITIONS_JUNCTION_ARM_L:
		return m_BonePositions.vJunctionArmL;

	case enCORE_BONE_POSITIONS_JUNCTION_ARM_R:
		return m_BonePositions.vJunctionArmR;

	default:
		assert( !"不正な値" );
		GetBonePosPreviosFrame( 0, iVecNum );
		break;
	}


	D3DXVECTOR3 vReturnPos = { 0.0f, 0.0f, 0.0f };
	return vReturnPos;
}

//↑で使うためにボーン座標を記録する( Renderの直後に使う ).
void clsPARTS_CORE::UpdateBonePosPreviosFrame() 
{
	clsOPERATION_STRING OprtStr;
	string sBoneNameRoot = sBONE_NAME_CORE_BOOSTER_ROOT;
	string sBoneNameEnd	 = sBONE_NAME_CORE_BOOSTER_END;
	for( int i=0; i<m_BonePositions.vecvBoosterRoot.size(); i++ ){
		m_BonePositions.vecvBoosterRoot[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameRoot, i, m_iDIGIT_BOOSTER_NUM ).c_str() );
		m_BonePositions.vecvBoosterEnd[i]  = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameEnd , i, m_iDIGIT_BOOSTER_NUM ).c_str() );
	}

	m_BonePositions.vJenerator		= this->GetBonePos( sBONE_NAME_CORE_JENERATOR );
	m_BonePositions.vJunctionHead	= this->GetBonePos( sBONE_NAME_CORE_TO_HEAD );
	m_BonePositions.vJunctionArmL	= this->GetBonePos( sBONE_NAME_CORE_TO_ARM_L );
	m_BonePositions.vJunctionArmR	= this->GetBonePos( sBONE_NAME_CORE_TO_ARM_R );
}
