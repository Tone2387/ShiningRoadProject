#include "MissionModel.h"
#include "OperationString.h"

using namespace std;

//ジョイントボーンの数字の桁数.
const char cBONE_NAME_NUM_DIGIT_JOINT = 2;

//パーツ情報受け取り用変数.
static const char cPartsHeadNum = static_cast<char>(enPARTS::HEAD);
static const char cPartsCoreNum = static_cast<char>(enPARTS::CORE);
static const char cPartsArmLNum = static_cast<char>(enPARTS::ARM_L);
static const char cPartsArmRNum = static_cast<char>(enPARTS::ARM_R);
static const char cPartsWeaponLNum = static_cast<char>(enPARTS::WEAPON_L);
static const char cPartsWeaponRNum = static_cast<char>(enPARTS::WEAPON_R);
static const char cPartsLegNum = static_cast<char>(enPARTS::LEG);

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

void clsMISSION_MODEL::CreateProduct( clsROBO_STATUS* const pStatus )
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
	int tmpIndex;				//m_vpPartsの添え字.
	const BONE_SET INIT_BONE_SET;//初期化用.
	 
	switch( enColParts )
	{
	case enCOL_PARTS_LEG:
		tmpIndex = static_cast< int >( enPARTS::LEG );
		//ボーンがあるだけ繰り返し.
		while( m_vpParts[ tmpIndex ]->ExistsBone( OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT ).c_str() ) )
		{
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[m_iColStateIndex].iBoneNum = clsASSEMBLE_MODEL::enPARTS_INDEX_LEG;

			m_iColStateIndex ++;	
			iColNum ++;
		}
		m_iColNum[ enColParts ] = iReturn = iColNum;
		break;
	case enCOL_PARTS_CORE:
		tmpIndex = static_cast< int >( enPARTS::CORE );
		if( m_vpParts[ tmpIndex ]->ExistsBone( sBONE_NAME_COL_CORE ) ){
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[m_iColStateIndex].iBoneNum = clsASSEMBLE_MODEL::enPARTS_INDEX_CORE;

			m_iColStateIndex ++;	
			m_iColNum[ enColParts ] = iReturn = 1;
		}
		break;
	case enCOL_PARTS_HEAD:
		tmpIndex = static_cast< int >( enPARTS::HEAD );
		if( m_vpParts[ tmpIndex ]->ExistsBone( sBONE_NAME_COL_HEAD ) ){
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[m_iColStateIndex].iBoneNum = clsASSEMBLE_MODEL::enPARTS_INDEX_HEAD;

			m_iColStateIndex ++;	
			m_iColNum[ enColParts ] = iReturn = 1;
		}
		break;
	case enCOL_PARTS_ARMS:
		//左腕.
		tmpIndex = static_cast< int >( enPARTS::ARM_L );
		while( m_vpParts[ tmpIndex ]->ExistsBone( OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT ).c_str() ) )
		{
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[m_iColStateIndex].iBoneNum = clsASSEMBLE_MODEL::enPARTS_INDEX_ARM_L;

			m_iColStateIndex ++;	
			iColNum ++;
		}
		m_iColNum[ enColParts ] = iReturn = iColNum;

		//右腕.
		tmpIndex = static_cast< int >( enPARTS::ARM_R );
		iColNum = 0;
		while( m_vpParts[ tmpIndex ]->ExistsBone( OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT ).c_str() ) )
		{
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[m_iColStateIndex].iBoneNum = clsASSEMBLE_MODEL::enPARTS_INDEX_ARM_R;

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

vector< clsObject::SPHERE > clsMISSION_MODEL::GetColState(clsROBO_STATUS* const pStatus)
{
	vector< clsObject::SPHERE > v_Sphere;

	v_Sphere.resize(m_iColMax);

	for (int i = 0; i<m_iColMax; i++)
	{
		v_Sphere[i].vCenter = &m_vvColPos[i];
		
		switch (static_cast<enPARTS>(m_vColStates[i].iParts))
		{
		case enPARTS::LEG:
			v_Sphere[i].fRadius = pStatus->GetRoboState(clsROBO_STATUS::COL_SIZE_LEG);
			break;
		case enPARTS::CORE:
			v_Sphere[i].fRadius = pStatus->GetRoboState(clsROBO_STATUS::COL_SIZE_CORE);
			break;
		case enPARTS::HEAD:
			v_Sphere[i].fRadius = pStatus->GetRoboState(clsROBO_STATUS::COL_SIZE_HEAD);
			break;
		case enPARTS::ARM_L:
			v_Sphere[i].fRadius = pStatus->GetRoboState(clsROBO_STATUS::COL_SIZE_ARMS);
			break;
		case enPARTS::ARM_R:
			v_Sphere[i].fRadius = pStatus->GetRoboState(clsROBO_STATUS::COL_SIZE_ARMS);
			break;

		default:
			break;
		}
	}

	return v_Sphere;
}

//当たり判定の座標の配列をすべて返す.
void clsMISSION_MODEL::UpdateColPos()
{
	//格納されているパーツ番号は種類が順列になっている前提.
	int iTmpPartsNo = m_vColStates[0].iParts;
	int iVecNum = 0;

	for( int i=0; i<m_iColMax; i++ ){

		if (iTmpPartsNo != m_vColStates[i].iParts)
		{
			iTmpPartsNo = m_vColStates[i].iParts;
			iVecNum = 0;
		}

		//ボーンの座標を取得.
		switch (static_cast<enPARTS>(m_vColStates[i].iParts))
		{
		case enPARTS::LEG:
			m_vvColPos[i] = GetBonePosLegJoint(iVecNum);
			break;
		case enPARTS::CORE:
			m_vvColPos[i] = GetBonePosCoreJenerator();
			break;
		case enPARTS::HEAD:
			m_vvColPos[i] = GetBonePosHeadCenter();
			break;
		case enPARTS::ARM_L:
			m_vvColPos[i] = GetBonePosArmLJoint(iVecNum);
			break;
		case enPARTS::ARM_R:
			m_vvColPos[i] = GetBonePosArmRJoint(iVecNum);
			break;

		default:
			break;
		}
		
		++iVecNum;
	}
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

	assert(m_vpParts[cTmpNum]);
	m_vpParts[cTmpNum]->SetRotation(vRot);
}

//腕の角度を武器も模写する.
D3DXVECTOR3 clsMISSION_MODEL::GetDirfromBone(const enPARTS PartsNum, const char* strBoneRootName, const char* strBoneEndName)
{
	char cTmpNum = static_cast<char>(PartsNum);
	//return m_vpParts[cTmpNum]->m_pMesh->ExistsBone(sBoneName);

	std::string strBoneRoot;
	std::string strBoneEnd;

	//ボーンのベクトルを出す( ローカル ).
	D3DXVECTOR3 vVecLocal =
		m_vpParts[cTmpNum]->GetBonePos(strBoneEndName, true) -
		m_vpParts[cTmpNum]->GetBonePos(strBoneRootName, true);
	D3DXVec3Normalize(&vVecLocal, &vVecLocal);

	//ボーンのベクトルを出す( ワールド ).
	D3DXVECTOR3 vVecWorld =
		m_vpParts[cTmpNum]->GetBonePos(strBoneEndName) -
		m_vpParts[cTmpNum]->GetBonePos(strBoneRootName);
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

D3DXVECTOR3 clsMISSION_MODEL::GetDirfromBone(
	const enPARTS PartsNum,
	const int enBoneRootName,
	const int enBoneEndName,
	const int iVecNum)
{
	char cTmpNum = static_cast<char>(PartsNum);

	std::string strBoneRoot;
	std::string strBoneEnd;

	//ボーンのベクトルを出す( ローカル ).
	D3DXVECTOR3 vVecLocal =
		m_vpParts[cTmpNum]->GetBonePosPreviosFrame(enBoneEndName, iVecNum) -
		m_vpParts[cTmpNum]->GetBonePosPreviosFrame(enBoneRootName, iVecNum);
	D3DXVec3Normalize(&vVecLocal, &vVecLocal);

	//ボーンのベクトルを出す( ワールド ).
	D3DXVECTOR3 vVecWorld =
		m_vpParts[cTmpNum]->GetBonePosPreviosFrame(enBoneEndName, iVecNum) -
		m_vpParts[cTmpNum]->GetBonePosPreviosFrame(enBoneRootName, iVecNum);
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

	assert(m_vpParts[cTmpNum]);
	m_vpParts[cTmpNum]->SetAnimChange(iAnimIndex, dAnimTime);
}

void clsMISSION_MODEL::SetPartsAnimSpeed(const enPARTS PartsNum, const double dAnimSpeed)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vpParts[cTmpNum]);
	m_vpParts[cTmpNum]->SetAnimSpeed(dAnimSpeed);
}

void clsMISSION_MODEL::SetPartsAnimNormal(const enPARTS PartsNum, const bool bAnimTimeInit)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vpParts[cTmpNum]);
	m_vpParts[cTmpNum]->AnimNormal(bAnimTimeInit);
}

void clsMISSION_MODEL::SetPartsAnimReverce(const enPARTS PartsNum, const bool bAnimTimeInit)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vpParts[cTmpNum]);
	m_vpParts[cTmpNum]->AnimReverce(bAnimTimeInit);
}

const int clsMISSION_MODEL::GetPartsAnimNo(const enPARTS PartsNum)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vpParts[cTmpNum]);
	return m_vpParts[cTmpNum]->GetAnimNo();
}

const bool clsMISSION_MODEL::IsPartsAnimEnd(const enPARTS PartsNum)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vpParts[cTmpNum]);
	return m_vpParts[cTmpNum]->IsAnimTimeEnd();
}

const double clsMISSION_MODEL::GetPartsAnimNowTime(const enPARTS PartsNum)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vpParts[cTmpNum]);
	return m_vpParts[cTmpNum]->GetAnimTime();
}

const bool clsMISSION_MODEL::IsPartsAnimReverce(const enPARTS PartsNum)
{
	char cTmpNum = static_cast<char>(PartsNum);

	assert(m_vpParts[cTmpNum]);
	return m_vpParts[cTmpNum]->IsAnimReverce();
}

//前フレームのボーン位置を受け取るための関数群.
//脚パーツ.
const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosLegBoosterRoot(const int iBoneNo)
{
	return m_vpParts[cPartsLegNum]->GetBonePosPreviosFrame(clsPARTS_LEG::enLEG_BONE_POSITIONS_BOOSTER_ROOT, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosLegBoosterEnd(const int iBoneNo)
{
	return m_vpParts[cPartsLegNum]->GetBonePosPreviosFrame(clsPARTS_LEG::enLEG_BONE_POSITIONS_BOOSTER_END, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosLegJoint(const int iBoneNo)
{
	return m_vpParts[cPartsLegNum]->GetBonePosPreviosFrame(clsPARTS_LEG::enLEG_BONE_POSITIONS_JOINT, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosLegSole(const int iBoneNo)
{
	return m_vpParts[cPartsLegNum]->GetBonePosPreviosFrame(clsPARTS_LEG::enLEG_BONE_POSITIONS_SOLE, iBoneNo);
}

//頭パーツ.
const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosHeadCenter()
{
	return m_vpParts[cPartsHeadNum]->GetBonePosPreviosFrame(clsPARTS_HEAD::enHEAD_BONE_POSITIONS_CENTER);
}

//コアパーツ.
const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosCoreBoosterRoot(const int iBoneNo)
{
	return m_vpParts[cPartsCoreNum]->GetBonePosPreviosFrame(clsPARTS_CORE::enCORE_BONE_POSITIONS_BOOSTER_ROOT, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosCoreBoosterEnd(const int iBoneNo)
{
	return m_vpParts[cPartsCoreNum]->GetBonePosPreviosFrame(clsPARTS_CORE::enCORE_BONE_POSITIONS_BOOSTER_END, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosCoreJenerator()
{
	return m_vpParts[cPartsCoreNum]->GetBonePosPreviosFrame(clsPARTS_CORE::enCORE_BONE_POSITIONS_JENERATOR);
}

//左腕パーツ.
const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmLBoostFrontRoot(const int iBoneNo)
{
	return m_vpParts[cPartsArmLNum]->GetBonePosPreviosFrame(clsPARTS_ARM_L::enARMS_BONE_POSITIONS_BOOSTER_FRONT_ROOT, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmLBoostFrontEnd(const int iBoneNo)
{
	return m_vpParts[cPartsArmLNum]->GetBonePosPreviosFrame(clsPARTS_ARM_L::enARMS_BONE_POSITIONS_BOOSTER_FRONT_END, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmLBoostSideRoot(const int iBoneNo)
{
	return m_vpParts[cPartsArmLNum]->GetBonePosPreviosFrame(clsPARTS_ARM_L::enARMS_BONE_POSITIONS_BOOSTER_SIDE_ROOT, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmLBoostSideEnd(const int iBoneNo)
{
	return m_vpParts[cPartsArmLNum]->GetBonePosPreviosFrame(clsPARTS_ARM_L::enARMS_BONE_POSITIONS_BOOSTER_SIDE_END, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmLBoostBackRoot(const int iBoneNo)
{
	return m_vpParts[cPartsArmLNum]->GetBonePosPreviosFrame(clsPARTS_ARM_L::enARMS_BONE_POSITIONS_BOOSTER_BACK_ROOT, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmLBoostBackEnd(const int iBoneNo)
{
	return m_vpParts[cPartsArmLNum]->GetBonePosPreviosFrame(clsPARTS_ARM_L::enARMS_BONE_POSITIONS_BOOSTER_BACK_END, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmLJoint(const int iBoneNo)
{
	return m_vpParts[cPartsArmLNum]->GetBonePosPreviosFrame(clsPARTS_ARM_L::enARMS_BONE_POSITIONS_JOINT, iBoneNo);
}

//右腕パーツ.
const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmRBoostFrontRoot(const int iBoneNo)
{
	return m_vpParts[cPartsArmRNum]->GetBonePosPreviosFrame(clsPARTS_ARM_R::enARMS_BONE_POSITIONS_BOOSTER_FRONT_ROOT, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmRBoostFrontEnd(const int iBoneNo)
{
	return m_vpParts[cPartsArmRNum]->GetBonePosPreviosFrame(clsPARTS_ARM_R::enARMS_BONE_POSITIONS_BOOSTER_FRONT_END, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmRBoostSideRoot(const int iBoneNo)
{
	return m_vpParts[cPartsArmRNum]->GetBonePosPreviosFrame(clsPARTS_ARM_R::enARMS_BONE_POSITIONS_BOOSTER_SIDE_ROOT, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmRBoostSideEnd(const int iBoneNo)
{
	return m_vpParts[cPartsArmRNum]->GetBonePosPreviosFrame(clsPARTS_ARM_R::enARMS_BONE_POSITIONS_BOOSTER_SIDE_END, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmRBoostBackRoot(const int iBoneNo)
{
	return m_vpParts[cPartsArmRNum]->GetBonePosPreviosFrame(clsPARTS_ARM_R::enARMS_BONE_POSITIONS_BOOSTER_BACK_ROOT, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmRBoostBackEnd(const int iBoneNo)
{
	return m_vpParts[cPartsArmRNum]->GetBonePosPreviosFrame(clsPARTS_ARM_R::enARMS_BONE_POSITIONS_BOOSTER_BACK_END, iBoneNo);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosArmRJoint(const int iBoneNo)
{
	return m_vpParts[cPartsArmRNum]->GetBonePosPreviosFrame(clsPARTS_ARM_R::enARMS_BONE_POSITIONS_JOINT, iBoneNo);
}

//左武器パーツ.
const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosWeaponLMuzzleRoot()
{
	return m_vpParts[cPartsWeaponLNum]->GetBonePosPreviosFrame(clsPARTS_WEAPON::enWEAPON_BONE_POSITIONS_MUZZLE_ROOT);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosWeaponLMuzzleEnd()
{
	return m_vpParts[cPartsWeaponLNum]->GetBonePosPreviosFrame(clsPARTS_WEAPON::enWEAPON_BONE_POSITIONS_MUZZLE_END);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosWeaponLCartridgeInjection()
{
	return m_vpParts[cPartsWeaponLNum]->GetBonePosPreviosFrame(clsPARTS_WEAPON::enWEAPON_BONE_POSITIONS_CARTRIDGE_INJECTION);
}

//右武器パーツ.
const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosWeaponRMuzzleRoot()
{
	return m_vpParts[cPartsWeaponRNum]->GetBonePosPreviosFrame(clsPARTS_WEAPON::enWEAPON_BONE_POSITIONS_MUZZLE_ROOT);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosWeaponRMuzzleEnd()
{
	return m_vpParts[cPartsWeaponRNum]->GetBonePosPreviosFrame(clsPARTS_WEAPON::enWEAPON_BONE_POSITIONS_MUZZLE_END);
}

const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosWeaponRCartridgeInjection()
{
	return m_vpParts[cPartsWeaponRNum]->GetBonePosPreviosFrame(clsPARTS_WEAPON::enWEAPON_BONE_POSITIONS_CARTRIDGE_INJECTION);
}