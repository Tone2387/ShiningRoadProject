#include "MissionModel.h"
#include "OperationString.h"

using namespace std;

//�W���C���g�{�[���̐����̌���.
const char cBONE_NAME_NUM_DIGIT_JOINT = 2;

//�p�[�c���󂯎��p�ϐ�.
static const char cPartsHeadNum = static_cast<char>(enPARTS::HEAD);
static const char cPartsCoreNum = static_cast<char>(enPARTS::CORE);
static const char cPartsArmLNum = static_cast<char>(enPARTS::ARM_L);
static const char cPartsArmRNum = static_cast<char>(enPARTS::ARM_R);
static const char cPartsWeaponLNum = static_cast<char>(enPARTS::WEAPON_L);
static const char cPartsWeaponRNum = static_cast<char>(enPARTS::WEAPON_R);
static const char cPartsLegNum = static_cast<char>(enPARTS::LEG);

//�����蔻��.
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

//���̃p�[�c�̓����蔻��̐���Ԃ�.
int clsMISSION_MODEL::CreateColStateBone( const enCOL_PARTS enColParts )
{
	int iReturn = 0;			//�߂�l.
	clsOPERATION_STRING OprtStr;//�{�[�����Ɣԍ����q�������.
	int iColNum = 0;			//m_iColNum�Ɋi�[���铖���蔻��̐�.
	int tmpIndex;				//m_vpParts�̓Y����.
	const BONE_SET INIT_BONE_SET;//�������p.
	 
	switch( enColParts )
	{
	case enCOL_PARTS_LEG:
		tmpIndex = static_cast< int >( enPARTS::LEG );
		//�{�[�������邾���J��Ԃ�.
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
		//���r.
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

		//�E�r.
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

//�{�[���̐����m�肷��.
void clsMISSION_MODEL::FixBoneStates()
{
	m_vColStates.shrink_to_fit();
	m_iColMax = 0;
	for( UCHAR i=0; i<enCOL_PARTS_size; i++ ){
		m_iColMax += m_iColNum[i];
	}

	m_vvColPos.resize( m_iColMax );
}

//�e�p�[�c�̓����蔻��̐���Ԃ�.
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

//�����蔻��̍��W�̔z������ׂĕԂ�.
void clsMISSION_MODEL::UpdateColPos()
{
	//�i�[����Ă���p�[�c�ԍ��͎�ނ�����ɂȂ��Ă���O��.
	int iTmpPartsNo = m_vColStates[0].iParts;
	int iVecNum = 0;

	for( int i=0; i<m_iColMax; i++ ){

		if (iTmpPartsNo != m_vColStates[i].iParts)
		{
			iTmpPartsNo = m_vColStates[i].iParts;
			iVecNum = 0;
		}

		//�{�[���̍��W���擾.
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

	clsOPERATION_STRING OprtStr;//�{�[�����Ɣԍ����q�������.

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

//�r�̊p�x�𕐊���͎ʂ���.
const D3DXVECTOR3 clsMISSION_MODEL::GetRotfromBone(const enPARTS PartsNum, const char* strBoneRootName, const char* strBoneEndName)
{
	char cTmpNum = static_cast<char>(PartsNum);
	//return m_vpParts[cTmpNum]->m_pMesh->ExistsBone(sBoneName);

	std::string strBoneRoot;
	std::string strBoneEnd;

	//�{�[���̃x�N�g�����o��( ���[�J�� ).
	D3DXVECTOR3 vVecLocal =
		m_vpParts[cTmpNum]->GetBonePos(strBoneEndName, true) -
		m_vpParts[cTmpNum]->GetBonePos(strBoneRootName, true);
	D3DXVec3Normalize(&vVecLocal, &vVecLocal);

	//�{�[���̃x�N�g�����o��( ���[���h ).
	D3DXVECTOR3 vVecWorld =
		m_vpParts[cTmpNum]->GetBonePos(strBoneEndName) -
		m_vpParts[cTmpNum]->GetBonePos(strBoneRootName);
	D3DXVec3Normalize(&vVecWorld, &vVecWorld);

	//�x�N�g�������]�l�����߂�.
	D3DXVECTOR3 vRot = { 0.0f, 0.0f, 0.0f };
	//	vRot.x = atanf( vVec.y );//���̃Q�[���̎d�l�Ȃ琳��( 2018/06/19(��)���� )( �܂�S������ ).
	vRot.x = atan2f(vVecLocal.y, -vVecLocal.z);//.
	vRot.y = atan2f(-vVecWorld.x, -vVecWorld.z);//( ���́A�}�C�i�X���������Ă�����AX,Z������ւ���Ă���̂��Ƃ����΁A0�x�Ń��f�������������������� ).

	vRot.x = GuardDirOver(vRot.x);
	vRot.y = GuardDirOver(vRot.y);
	//	vRot.z = GuardDirOver( vRot.z );

	return vRot;
}

const D3DXVECTOR3 clsMISSION_MODEL::GetRotfromVec(
	const D3DXVECTOR3 vRoot,
	const D3DXVECTOR3 vEnd)
{
	//�{�[���̃x�N�g�����o��( ���[�J�� ).
	D3DXVECTOR3 vRootLocal = vRoot - m_Trans.vPos;
	D3DXVECTOR3 vEndLocal = vEnd - m_Trans.vPos;

	D3DXVECTOR3 vVecLocal = vEndLocal - vRootLocal;
	D3DXVec3Normalize(&vVecLocal, &vVecLocal);

	//�{�[���̃x�N�g�����o��( ���[���h ).
	D3DXVECTOR3 vVecWorld = vEnd - vRoot;
	D3DXVec3Normalize(&vVecWorld, &vVecWorld);

	//�x�N�g�������]�l�����߂�.
	D3DXVECTOR3 vRot = { 0.0f, 0.0f, 0.0f };
	//	vRot.x = atanf( vVec.y );//���̃Q�[���̎d�l�Ȃ琳��( 2018/06/19(��)���� )( �܂�S������ ).
	vRot.x = atan2f(-vVecLocal.y, -vVecLocal.z);//.
	vRot.x = (vVecLocal.y * cosf(vRot.x)) - (vVecLocal.z * sinf(vRot.x));
	vRot.x = asin(vRot.x);
	vRot.y = atan2f(vVecWorld.x, vVecWorld.z);//( ���́A�}�C�i�X���������Ă�����AX,Z������ւ���Ă���̂��Ƃ����΁A0�x�Ń��f�������������������� ).

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

//�O�t���[���̃{�[���ʒu���󂯎�邽�߂̊֐��Q.
//�r�p�[�c.
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

//���p�[�c.
const D3DXVECTOR3 clsMISSION_MODEL::GetBonePosHeadCenter()
{
	return m_vpParts[cPartsHeadNum]->GetBonePosPreviosFrame(clsPARTS_HEAD::enHEAD_BONE_POSITIONS_CENTER);
}

//�R�A�p�[�c.
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

//���r�p�[�c.
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

//�E�r�p�[�c.
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

//������p�[�c.
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

//�E����p�[�c.
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