#include "PartsLeg.h"

#include "OperationString.h"

using namespace std;


clsPARTS_LEG::clsPARTS_LEG()
{
}

clsPARTS_LEG::~clsPARTS_LEG()
{
}

//作成時.
void clsPARTS_LEG::InitProduct()
{
	clsOPERATION_STRING OprtStr;

	//ブースターの数を調べる( 何番まであるか ).
	string sBoneName = sBONE_NAME_LEG_BOOSTER_ROOT;
	int iBoosterNum = 0;
	//ボーンがある限り繰り返し : ボーン名を渡す : 基礎となるボーン名に数字を連結 : 引数はchar*型.
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}

#ifdef _DEBUG
	//RootとEndと数が違うとエラー.
	int iBoosterNumOld = iBoosterNum;
	sBoneName = sBONE_NAME_LEG_BOOSTER_END;
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

	//関節.
	int iJointNum = 0;
	sBoneName = sBONE_NAME_LEG_JOINT;
	iJointNum = 0;
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iJointNum, m_iDIGIT_JOINT_NUM ).c_str() ) )
	{
		iJointNum ++;
	}
	m_BonePositions.vecvJoint.resize( iJointNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	//足の裏.
	int iSoleNum = 0;
	sBoneName = sBONE_NAME_LEG_SOLE;
	iSoleNum = 0;
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iSoleNum, m_iDIGIT_SOLE_NUM ).c_str() ) )
	{
		iSoleNum ++;
	}
	m_BonePositions.vecvSole.resize( iSoleNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );


}

//毎フレーム.
void clsPARTS_LEG::UpdateProduct()
{
}


//直前のフレームのボーンの座標を持ってくる.
D3DXVECTOR3 clsPARTS_LEG::GetBonePosPreviosFrame( 
	const int enBoneName, 
	int iVecNum )
{
	if( iVecNum >= m_BonePositions.vecvBoosterRoot.size() ){
		iVecNum = m_BonePositions.vecvBoosterRoot.size() - 1;
	}
	else if( iVecNum < 0 ){
		iVecNum = 0;
	}

	switch( enBoneName )
	{
	case enLEG_BONE_POSITIONS_BOOSTER_ROOT:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterRoot.size() );
		return m_BonePositions.vecvBoosterRoot[ iVecNum ];

	case enLEG_BONE_POSITIONS_BOOSTER_END:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterEnd.size() );
		return m_BonePositions.vecvBoosterEnd[ iVecNum ];

	case enLEG_BONE_POSITIONS_JOINT:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvJoint.size() );
		return m_BonePositions.vecvJoint[ iVecNum ];

	case enLEG_BONE_POSITIONS_JUNCTION_CORE:
		return m_BonePositions.vJunctionCore;

	case enLEG_BONE_POSITIONS_SOLE:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvSole.size() );
		return m_BonePositions.vecvSole[ iVecNum ];

	case enLEG_BONE_POSITIONS_POSITION_BASE:
		return m_BonePositions.vPositionBase;

	case enLEG_BONE_POSITIONS_NULL:
		return m_BonePositions.vNull;

	default:
		assert( !"不正な値" );
		GetBonePosPreviosFrame( 0, iVecNum );
		break;
	}


	D3DXVECTOR3 vReturnPos = { 0.0f, 0.0f, 0.0f };
	return vReturnPos;
}

//↑で使うためにボーン座標を記録する( Renderの直後に使う ).
void clsPARTS_LEG::UpdateBonePosPreviosFrame() 
{
	clsOPERATION_STRING OprtStr;
	//ブースター.
	string sBoneNameRoot = sBONE_NAME_LEG_BOOSTER_ROOT;
	string sBoneNameEnd	 = sBONE_NAME_LEG_BOOSTER_END;
	for( int i=0; i<m_BonePositions.vecvBoosterRoot.size(); i++ ){
		m_BonePositions.vecvBoosterRoot[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameRoot, i, m_iDIGIT_BOOSTER_NUM ).c_str() );
		m_BonePositions.vecvBoosterEnd[i]  = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameEnd , i, m_iDIGIT_BOOSTER_NUM ).c_str() );
	}

	//関節.
	string sBoneName = sBONE_NAME_LEG_JOINT;
	for( int i=0; i<m_BonePositions.vecvJoint.size(); i++ ){
		m_BonePositions.vecvJoint[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneName, i, m_iDIGIT_JOINT_NUM ).c_str() );
	}

	//足の裏.
	sBoneName = sBONE_NAME_LEG_SOLE;
	for( int i=0; i<m_BonePositions.vecvSole.size(); i++ ){
		m_BonePositions.vecvSole[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneName, i, m_iDIGIT_SOLE_NUM ).c_str() );
	}


	m_BonePositions.vNull		  = this->GetBonePos( sBONE_NAME_LEG_NULL );
	m_BonePositions.vJunctionCore = this->GetBonePos( sBONE_NAME_LEG_TO_CORE );
	m_BonePositions.vPositionBase = this->GetBonePos( sBONE_NAME_LEG_POSITION_BASE );
}
