#include "PartsArmBase.h"

#include "OperationString.h"

using namespace std;


clsPARTS_ARM_BASE::clsPARTS_ARM_BASE()
{
}

clsPARTS_ARM_BASE::~clsPARTS_ARM_BASE()
{
}

//�쐬��.
void clsPARTS_ARM_BASE::InitProduct()
{
	clsOPERATION_STRING OprtStr;

	//----- �u�[�X�^�[�̐��𒲂ׂ�( ���Ԃ܂ł��邩 ) -----//.
	//�O��.
	string sBoneName = sBONE_NAME_ARM_BOOSTER_FRONT_ROOT;
	int iBoosterNum = 0;
	//�{�[�����������J��Ԃ� : �{�[������n�� : ��b�ƂȂ�{�[�����ɐ�����A�� : ������char*�^.
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
#ifdef _DEBUG
	//Root��End�Ɛ����Ⴄ�ƃG���[.
	int iBoosterNumOld = iBoosterNum;
	sBoneName = sBONE_NAME_ARM_BOOSTER_FRONT_END;
	iBoosterNum = 0;
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
	if( iBoosterNum != iBoosterNumOld ){
		ERR_MSG( m_sPartsName.c_str(), "�u�[�X�^�[�̃{�[���̐���Root��End�ňႤ��" );
	}
#endif//#ifndef _DEBUG
	//while���𔲂���iBoosterNum�̔ԍ���Bone�͖��� = ���O�̔ԍ��܂ł͗L�� = iBoosterNum��size�ł���.
	m_BonePositions.vecvBoosterFontRoot.resize(iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_BonePositions.vecvBoosterFontEnd.resize( iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );


	//���.
	sBoneName = sBONE_NAME_ARM_BOOSTER_BACK_ROOT;
	iBoosterNum = 0;
	//�{�[�����������J��Ԃ� : �{�[������n�� : ��b�ƂȂ�{�[�����ɐ�����A�� : ������char*�^.
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
#ifdef _DEBUG
	//Root��End�Ɛ����Ⴄ�ƃG���[.
	iBoosterNumOld = iBoosterNum;
	sBoneName = sBONE_NAME_ARM_BOOSTER_BACK_END;
	iBoosterNum = 0;
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
	if( iBoosterNum != iBoosterNumOld ){
		ERR_MSG( m_sPartsName.c_str(), "�u�[�X�^�[�̃{�[���̐���Root��End�ňႤ��" );
	}
#endif//#ifndef _DEBUG
	//while���𔲂���iBoosterNum�̔ԍ���Bone�͖��� = ���O�̔ԍ��܂ł͗L�� = iBoosterNum��size�ł���.
	m_BonePositions.vecvBoosterBackRoot.resize(iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_BonePositions.vecvBoosterBackEnd.resize( iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );


	//��.
	sBoneName = sBONE_NAME_ARM_BOOSTER_SIDE_ROOT;
	iBoosterNum = 0;
	//�{�[�����������J��Ԃ� : �{�[������n�� : ��b�ƂȂ�{�[�����ɐ�����A�� : ������char*�^.
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
#ifdef _DEBUG
	//Root��End�Ɛ����Ⴄ�ƃG���[.
	iBoosterNumOld = iBoosterNum;
	sBoneName = sBONE_NAME_ARM_BOOSTER_SIDE_END;
	iBoosterNum = 0;
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}
	if( iBoosterNum != iBoosterNumOld ){
		ERR_MSG( m_sPartsName.c_str(), "�u�[�X�^�[�̃{�[���̐���Root��End�ňႤ��" );
	}
#endif//#ifndef _DEBUG
	//while���𔲂���iBoosterNum�̔ԍ���Bone�͖��� = ���O�̔ԍ��܂ł͗L�� = iBoosterNum��size�ł���.
	m_BonePositions.vecvBoosterSideRoot.resize(iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_BonePositions.vecvBoosterSideEnd.resize( iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );



	//�֐�.
	int iJointNum = 0;
	sBoneName = sBONE_NAME_ARM_JOINT;
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iJointNum, m_iDIGIT_JOINT_NUM ).c_str() ) )
	{
		iJointNum ++;
	}
	m_BonePositions.vecvJoint.resize( iJointNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );



}

//���t���[��.
void clsPARTS_ARM_BASE::UpdateProduct()
{
}


//���O�̃t���[���̃{�[���̍��W�������Ă���.
D3DXVECTOR3 clsPARTS_ARM_BASE::GetBonePosPreviosFrame( 
	const int enBoneName, 
	int iVecNum ) const
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
		assert( !"�s���Ȓl" );
		GetBonePosPreviosFrame( 0, iVecNum );
		break;
	}


	D3DXVECTOR3 vReturnPos = { 0.0f, 0.0f, 0.0f };
	return vReturnPos;
}

//���Ŏg�����߂Ƀ{�[�����W���L�^����( Render�̒���Ɏg�� ).
void clsPARTS_ARM_BASE::UpdateBonePosPreviosFrame() 
{
	clsOPERATION_STRING OprtStr;
	//----- �u�[�X�^�[ -----//.
	//�O��.
	string sBoneNameRoot = sBONE_NAME_ARM_BOOSTER_FRONT_ROOT;
	string sBoneNameEnd	 = sBONE_NAME_ARM_BOOSTER_FRONT_END;
	for( unsigned int i=0; i<m_BonePositions.vecvBoosterFontEnd.size(); i++ ){
		m_BonePositions.vecvBoosterFontRoot[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameRoot, i, m_iDIGIT_BOOSTER_NUM ).c_str() );
		m_BonePositions.vecvBoosterFontEnd[i]  = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameEnd , i, m_iDIGIT_BOOSTER_NUM ).c_str() );
	}

	//���.
	sBoneNameRoot = sBONE_NAME_ARM_BOOSTER_BACK_ROOT;
	sBoneNameEnd  = sBONE_NAME_ARM_BOOSTER_BACK_END;
	for( unsigned int i=0; i<m_BonePositions.vecvBoosterBackRoot.size(); i++ ){
		m_BonePositions.vecvBoosterBackRoot[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameRoot, i, m_iDIGIT_BOOSTER_NUM ).c_str() );
		m_BonePositions.vecvBoosterBackEnd[i]  = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameEnd , i, m_iDIGIT_BOOSTER_NUM ).c_str() );
	}

	//��.
	sBoneNameRoot = sBONE_NAME_ARM_BOOSTER_SIDE_ROOT;
	sBoneNameEnd  = sBONE_NAME_ARM_BOOSTER_SIDE_END;
	for( unsigned int i=0; i<m_BonePositions.vecvBoosterSideRoot.size(); i++ ){
		m_BonePositions.vecvBoosterSideRoot[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameRoot, i, m_iDIGIT_BOOSTER_NUM ).c_str() );
		m_BonePositions.vecvBoosterSideEnd[i]  = this->GetBonePos( OprtStr.ConsolidatedNumber( sBoneNameEnd , i, m_iDIGIT_BOOSTER_NUM ).c_str() );
	}

	//�֐�.
	for( unsigned int i=0; i<m_BonePositions.vecvJoint.size(); i++ ){
		m_BonePositions.vecvJoint[i] = this->GetBonePos( OprtStr.ConsolidatedNumber( sBONE_NAME_ARM_JOINT, i, m_iDIGIT_JOINT_NUM ).c_str() );
	}



	m_BonePositions.vJunctionWeapon = this->GetBonePos( sBONE_NAME_ARM_TO_WEAPON );
	m_BonePositions.vWeaponVec			= this->GetBonePos( sBONE_NAME_ARM_JOINT );
}
