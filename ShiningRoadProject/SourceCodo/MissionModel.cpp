#include "MissionModel.h"
#include "OperationString.h"

using namespace std;

//ジョイントボーンの数字の桁数.
const char cBONE_NAME_NUM_DIGIT_JOINT = 2;

//当たり判定.
const int iRESURVE_SIZE_COL = 64;

clsMISSION_MODEL::clsMISSION_MODEL()
	:m_iColNum()
	,m_iColMax( 0 )
	,m_iColStateIndex( 0 )
{
	m_vColStates.reserve( iRESURVE_SIZE_COL );
}

clsMISSION_MODEL::~clsMISSION_MODEL()
{
}

void clsMISSION_MODEL::CreateProduct()
{
	CreateColStateBones();
	FixBoneStates();
}

int clsMISSION_MODEL::CreateColStateBones()
{
	int iReturn = 0;
	iReturn += CreateColStateBone( clsMISSION_MODEL::enCOL_PARTS_LEG );
	iReturn += CreateColStateBone( clsMISSION_MODEL::enCOL_PARTS_CORE );
	iReturn += CreateColStateBone( clsMISSION_MODEL::enCOL_PARTS_HEAD );
	iReturn += CreateColStateBone( clsMISSION_MODEL::enCOL_PARTS_ARMS );

	return iReturn;
}

//そのパーツの当たり判定の数を返す.
int clsMISSION_MODEL::CreateColStateBone( const enCOL_PARTS enColParts )
{
	int iReturn = 0;			//戻り値.
	clsOPERATION_STRING OprtStr;//ボーン名と番号を繋げる役割.
	int iColNum = 0;			//m_iColNumに格納する当たり判定の数.
	int tmpIndex;				//m_vecpPartsの添え字.
	const BONE_SET INIT_BONE_SET;//初期化用.
	 
	switch( enColParts )
	{
	case enCOL_PARTS_LEG:
		tmpIndex = static_cast< int >( enPARTS::LEG );
		//ボーンがあるだけ繰り返し.
		while( m_vecpParts[ tmpIndex ]->ExistsBone( OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT ).c_str() ) )
		{
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT );

			m_iColStateIndex ++;	
			iColNum ++;
		}
		m_iColNum[ enColParts ] = iReturn = iColNum;
		break;
	case enCOL_PARTS_CORE:
		tmpIndex = static_cast< int >( enPARTS::CORE );
		if( m_vecpParts[ tmpIndex ]->ExistsBone( sBONE_NAME_COL_CORE ) ){
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = sBONE_NAME_COL_CORE;

			m_iColStateIndex ++;	
			m_iColNum[ enColParts ] = iReturn = 1;
		}
		break;
	case enCOL_PARTS_HEAD:
		tmpIndex = static_cast< int >( enPARTS::HEAD );
		if( m_vecpParts[ tmpIndex ]->ExistsBone( sBONE_NAME_COL_HEAD ) ){
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = sBONE_NAME_COL_HEAD;

			m_iColStateIndex ++;	
			m_iColNum[ enColParts ] = iReturn = 1;
		}
		break;
	case enCOL_PARTS_ARMS:
		//左腕.
		tmpIndex = static_cast< int >( enPARTS::ARM_L );
		while( m_vecpParts[ tmpIndex ]->ExistsBone( OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT ).c_str() ) )
		{
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT );

			m_iColStateIndex ++;	
			iColNum ++;
		}
		m_iColNum[ enColParts ] = iReturn = iColNum;

		//右腕.
		tmpIndex = static_cast< int >( enPARTS::ARM_R );
		iColNum = 0;
		while( m_vecpParts[ tmpIndex ]->ExistsBone( OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT ).c_str() ) )
		{
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT );

			m_iColStateIndex ++;	
			iColNum ++;
		}
		m_iColNum[ enColParts ] = iReturn += iColNum;
		break;
	default:
		return -1;
	}

	return iReturn;
}

//ボーンの数を確定する.
void clsMISSION_MODEL::FixBoneStates()
{
	m_vColStates.shrink_to_fit();
	m_iColMax = 0;
	for( UCHAR i=0; i<enCOL_PARTS_size; i++ ){
		m_iColMax += m_iColNum[i];
	}

	m_vvColPos.resize( m_iColMax );
}

//各パーツの当たり判定の数を返す.
int clsMISSION_MODEL::GetColNum( const enCOL_PARTS enColParts )
{
	switch( enColParts )
	{
	case enCOL_PARTS_LEG:
	case enCOL_PARTS_CORE:
	case enCOL_PARTS_HEAD:
	case enCOL_PARTS_ARMS:
		return m_iColNum[ enColParts ];
	}

	return -1;
}

//当たり判定の座標の配列をすべて返す.
shared_ptr< vector< D3DXVECTOR3 > > clsMISSION_MODEL::GetColPosPtr()
{
	for( int i=0; i<m_iColMax; i++ ){
		//ボーンの座標を取得.
		m_vvColPos[i] = GetBonePos( 
			static_cast< enPARTS >( m_vColStates[i].iParts ), 
			m_vColStates[i].sName.c_str() );
	}

	shared_ptr< vector< D3DXVECTOR3 > > spvvReturn( &m_vvColPos );

	return spvvReturn;
}

int clsMISSION_MODEL::GetSimilarityNameBoneNum(const enPARTS PartsNum, const char* strBoneName)
{
	int iResult = 0;
	int iSearchNum = 0;

	std::string strBoneNameTmp;

	clsOPERATION_STRING OprtStr;//ボーン名と番号を繋げる役割.

	while (1)
	{
		strBoneNameTmp = strBoneName;
		strBoneNameTmp = OprtStr.ConsolidatedNumber(strBoneNameTmp, iSearchNum, cBONE_NAME_NUM_DIGIT_JOINT);
		if (!ExistsBone(PartsNum, strBoneNameTmp.c_str()))
		{
			break;
		}

		iResult++;
		iSearchNum++;
	}

	return iResult;
}

void clsMISSION_MODEL::SetPartsRotate(const enPARTS PartsNum, const D3DXVECTOR3 vRot)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vecpParts[cTmpNum]);
	m_vecpParts[cTmpNum]->SetRotation(vRot);
}

//腕の角度を武器も模写する.
D3DXVECTOR3 clsMISSION_MODEL::GetDirfromBone(const enPARTS PartsNum, const char* strBoneRootName, const char* strBoneEndName)
{
	char cTmpNum = static_cast<char>(PartsNum);
	//return m_vecpParts[cTmpNum]->m_pMesh->ExistsBone(sBoneName);

	std::string strBoneRoot;
	std::string strBoneEnd;

	//ボーンのベクトルを出す( ローカル ).
	D3DXVECTOR3 vVecLocal =
		m_vecpParts[cTmpNum]->GetBonePos(strBoneEndName, true) -
		m_vecpParts[cTmpNum]->GetBonePos(strBoneRootName, true);
	D3DXVec3Normalize(&vVecLocal, &vVecLocal);

	//ボーンのベクトルを出す( ワールド ).
	D3DXVECTOR3 vVecWorld =
		m_vecpParts[cTmpNum]->GetBonePos(strBoneEndName) -
		m_vecpParts[cTmpNum]->GetBonePos(strBoneRootName);
	D3DXVec3Normalize(&vVecWorld, &vVecWorld);

	//ベクトルから回転値を求める.
	D3DXVECTOR3 vRot = { 0.0f, 0.0f, 0.0f };
	//	vRot.x = atanf( vVec.y );//このゲームの仕様なら正解( 2018/06/19(火)現在 )( つまりゴリ押し ).
	vRot.x = atan2f(vVecLocal.y, -vVecLocal.z);//.
	vRot.y = atan2f(-vVecWorld.x, -vVecWorld.z);//( 何故、マイナスがかかっていたり、X,Zが入れ替わっているのかといえば、0度でモデルがこっちを向くから ).

	vRot.x = GuardDirOver(vRot.x);
	vRot.y = GuardDirOver(vRot.y);
	//	vRot.z = GuardDirOver( vRot.z );

	return vRot;
}

void clsMISSION_MODEL::SetPartsAnimNo(const enPARTS PartsNum, const int iAnimIndex, const double dAnimTime)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vecpParts[cTmpNum]);
	m_vecpParts[cTmpNum]->SetAnimChange(iAnimIndex, dAnimTime);
}

void clsMISSION_MODEL::SetPartsAnimSpeed(const enPARTS PartsNum, const double dAnimSpeed)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vecpParts[cTmpNum]);
	m_vecpParts[cTmpNum]->SetAnimSpeed(dAnimSpeed);
}

void clsMISSION_MODEL::SetPartsAnimNormal(const enPARTS PartsNum, const bool bAnimTimeInit)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vecpParts[cTmpNum]);
	m_vecpParts[cTmpNum]->AnimNormal(bAnimTimeInit);
}

void clsMISSION_MODEL::SetPartsAnimReverce(const enPARTS PartsNum, const bool bAnimTimeInit)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vecpParts[cTmpNum]);
	m_vecpParts[cTmpNum]->AnimReverce(bAnimTimeInit);
}

const int clsMISSION_MODEL::GetPartsAnimNo(const enPARTS PartsNum)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vecpParts[cTmpNum]);
	return m_vecpParts[cTmpNum]->GetAnimNo();
}

const bool clsMISSION_MODEL::IsPartsAnimEnd(const enPARTS PartsNum)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vecpParts[cTmpNum]);
	return m_vecpParts[cTmpNum]->IsAnimTimeEnd();
}

const double clsMISSION_MODEL::GetPartsAnimNowTime(const enPARTS PartsNum)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vecpParts[cTmpNum]);
	return m_vecpParts[cTmpNum]->GetAnimTime();
}

const bool clsMISSION_MODEL::IsPartsAnimReverce(const enPARTS PartsNum)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vecpParts[cTmpNum]);
	return m_vecpParts[cTmpNum]->IsAnimReverce();
}