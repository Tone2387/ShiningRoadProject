#include "PartsCore.h"
#include "OperationString.h"

using namespace std;


clsPARTS_CORE::clsPARTS_CORE()
{
}

clsPARTS_CORE::~clsPARTS_CORE()
{
}

//�쐬��.
void clsPARTS_CORE::InitProduct()
{
	clsOPERATION_STRING OprtStr;

	//�u�[�X�^�[�̐��𒲂ׂ�( ���Ԃ܂ł��邩 ).
	string sBoneName = sBONE_NAME_CORE_BOOSTER_ROOT;
	int iBoosterNum = 0;
	//�{�[�����������J��Ԃ� : �{�[������n�� : ��b�ƂȂ�{�[�����ɐ�����A�� : ������char*�^.
	while( this->ExistsBone( OprtStr.ConsolidatedNumber( sBoneName, iBoosterNum, m_iDIGIT_BOOSTER_NUM ).c_str() ) )
	{
		iBoosterNum ++;
	}

#ifdef _DEBUG
	//Root��End�Ɛ����Ⴄ�ƃG���[.
	int iBoosterNumOld = iBoosterNum;
	sBoneName = sBONE_NAME_CORE_BOOSTER_END;
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
	m_BonePositions.vecvBoosterRoot.resize(iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_BonePositions.vecvBoosterEnd.resize( iBoosterNum, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
}

//���t���[��.
void clsPARTS_CORE::UpdateProduct()
{
}



//���O�̃t���[���̃{�[���̍��W�������Ă���.
D3DXVECTOR3 clsPARTS_CORE::GetBonePosPreviosFrame( 
	const int enBoneName, 
	int iVecNum ) const
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
		assert( !"�s���Ȓl" );
		GetBonePosPreviosFrame( 0, iVecNum );
		break;
	}


	const D3DXVECTOR3 vRETURN_POS = { 0.0f, 0.0f, 0.0f };
	return vRETURN_POS;
}

//���Ŏg�����߂Ƀ{�[�����W���L�^����( Render�̒���Ɏg�� ).
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
