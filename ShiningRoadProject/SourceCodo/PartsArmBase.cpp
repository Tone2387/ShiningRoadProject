#include "PartsArmBase.h"

#include "OperationString.h"

using namespace std;


clsPARTS_ARM_BASE::clsPARTS_ARM_BASE()
{
}

clsPARTS_ARM_BASE::~clsPARTS_ARM_BASE()
{
}

//作成時.
void clsPARTS_ARM_BASE::InitProduct()
{
	clsOPERATION_STRING OprtStr;

	//----- ブースターの数を調べる( 何番まであるか ) -----//.
	//前方.
	string sBoneName = sBONE_NAME_ARM_BOOSTER_FRONT_ROOT;
	int iBoosterNum = 0;
	//ボーンがある限り繰り返し : ボーン名を渡す : 基礎となるボーン名に数字を連結 : 引数はchar*型.
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
#ifdef _DEBUG
	//RootとEndと数が違うとエラー.
	int iBoosterNumOld = iBoosterNum;
	sBoneName = sBONE_NAME_ARM_BOOSTER_FRONT_END;
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
	m_BonePositions.vecvBoosterFontRoot.resize(iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_BonePositions.vecvBoosterFontEnd.resize( iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );


	//後方.
	sBoneName = sBONE_NAME_ARM_BOOSTER_BACK_ROOT;
	iBoosterNum = 0;
	//ボーンがある限り繰り返し : ボーン名を渡す : 基礎となるボーン名に数字を連結 : 引数はchar*型.
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
#ifdef _DEBUG
	//RootとEndと数が違うとエラー.
	iBoosterNumOld = iBoosterNum;
	sBoneName = sBONE_NAME_ARM_BOOSTER_BACK_END;
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
	m_BonePositions.vecvBoosterBackRoot.resize(iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_BonePositions.vecvBoosterBackEnd.resize( iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );


	//横.
	sBoneName = sBONE_NAME_ARM_BOOSTER_SIDE_ROOT;
	iBoosterNum = 0;
	//ボーンがある限り繰り返し : ボーン名を渡す : 基礎となるボーン名に数字を連結 : 引数はchar*型.
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
#ifdef _DEBUG
	//RootとEndと数が違うとエラー.
	iBoosterNumOld = iBoosterNum;
	sBoneName = sBONE_NAME_ARM_BOOSTER_SIDE_END;
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
	m_BonePositions.vecvBoosterSideRoot.resize(iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_BonePositions.vecvBoosterSideEnd.resize( iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );



	//関節.
	int iJointNum = 0;
	sBoneName = sBONE_NAME_ARM_JOINT;
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iJointNum, m_iDIGIT_JOINT_NUM ).c_str() ) )
	{
		iJointNum ++;
	}
	m_BonePositions.vecvJoint.resize( iJointNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );



}

//毎フレーム.
void clsPARTS_ARM_BASE::UpdateProduct()
{
}


//直前のフレームのボーンの座標を持ってくる.
D3DXVECTOR3 clsPARTS_ARM_BASE::GetBonePosPreviosFrame( 
	const int enBoneName, 
	int iVecNum )
{
	switch( enBoneName )
	{
	case enARMS_BONE_POSITIONS_BOOSTER_FRONT_ROOT:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterFontRoot.size() );
		return m_BonePositions.vecvBoosterFontRoot[ iVecNum ];

	case enARMS_BONE_POSITIONS_BOOSTER_FRONT_END:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterFontEnd.size() );
		return m_BonePositions.vecvBoosterFontEnd[ iVecNum ];

	case enARMS_BONE_POSITIONS_BOOSTER_BACK_ROOT:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterBackRoot.size() );
		return m_BonePositions.vecvBoosterBackRoot[ iVecNum ];

	case enARMS_BONE_POSITIONS_BOOSTER_BACK_END:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterBackEnd.size() );
		return m_BonePositions.vecvBoosterBackEnd[ iVecNum ];

	case enARMS_BONE_POSITIONS_BOOSTER_SIDE_ROOT:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterSideRoot.size() );
		return m_BonePositions.vecvBoosterSideRoot[ iVecNum ];

	case enARMS_BONE_POSITIONS_BOOSTER_SIDE_END:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvBoosterSideEnd.size() );
		return m_BonePositions.vecvBoosterSideEnd[ iVecNum ];

	case enARMS_BONE_POSITIONS_JOINT:
		IntOverGuird( &iVecNum, 0, m_BonePositions.vecvJoint.size() );
		return m_BonePositions.vecvJoint[ iVecNum ];

	case enARMS_BONE_POSITIONS_JUNCTION_WEAPON:
		return m_BonePositions.vJunctionWeapon;

	case enARMS_BONE_POSITIONS_WEAPON_VEC:
		return m_BonePositions.vWeaponVec;

	default:
		assert( !"不正な値" );
		GetBonePosPreviosFrame( 0, iVecNum );
		break;
	}


	D3DXVECTOR3 vReturnPos = { 0.0f, 0.0f, 0.0f };
	return vReturnPos;
}

//↑で使うためにボーン座標を記録する( Renderの直後に使う ).
void clsPARTS_ARM_BASE::UpdateBonePosPreviosFrame() 
{
	clsOPERATION_STRING OprtStr;
	//----- ブースター -----//.
	//前方.
	string sBoneNameRoot = sBONE_NAME_ARM_BOOSTER_FRONT_ROOT;
	string sBoneNameEnd	 = sBONE_NAME_ARM_BOOSTER_FRONT_END;
	for( int i=0; i<m_BonePositions.vecvBoosterFontEnd.size(); i++ ){
		m_BonePositions.vecvBoosterFontRoot[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameRoot, i, m_iDIGIT_BOOSTER_NUM ).c_str() );
		m_BonePositions.vecvBoosterFontEnd[i]  = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameEnd , i, m_iDIGIT_BOOSTER_NUM ).c_str() );
	}

	//後方.
	sBoneNameRoot = sBONE_NAME_ARM_BOOSTER_BACK_ROOT;
	sBoneNameEnd  = sBONE_NAME_ARM_BOOSTER_BACK_END;
	for( int i=0; i<m_BonePositions.vecvBoosterBackRoot.size(); i++ ){
		m_BonePositions.vecvBoosterBackRoot[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameRoot, i, m_iDIGIT_BOOSTER_NUM ).c_str() );
		m_BonePositions.vecvBoosterBackEnd[i]  = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameEnd , i, m_iDIGIT_BOOSTER_NUM ).c_str() );
	}

	//横.
	sBoneNameRoot = sBONE_NAME_ARM_BOOSTER_SIDE_ROOT;
	sBoneNameEnd  = sBONE_NAME_ARM_BOOSTER_SIDE_END;
	for( int i=0; i<m_BonePositions.vecvBoosterSideRoot.size(); i++ ){
		m_BonePositions.vecvBoosterSideRoot[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameRoot, i, m_iDIGIT_BOOSTER_NUM ).c_str() );
		m_BonePositions.vecvBoosterSideEnd[i]  = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameEnd , i, m_iDIGIT_BOOSTER_NUM ).c_str() );
	}

	//関節.
	for( int i=0; i<m_BonePositions.vecvJoint.size(); i++ ){
		m_BonePositions.vecvJoint[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBONE_NAME_ARM_JOINT, i, m_iDIGIT_JOINT_NUM ).c_str() );
	}



	m_BonePositions.vJunctionWeapon = this->GetBonePos( sBONE_NAME_ARM_TO_WEAPON );
	m_BonePositions.vWeaponVec			= this->GetBonePos( sBONE_NAME_ARM_JOINT );
}
