#include "PartsBase.h"

using namespace std;

//�{�[����������Ȃ��Ƃ��Ƀ��b�Z�[�W���o���Ȃ����.
//#define ERROR_MESSAGE_MACRO_BONE_NAME_NOT_FIND

clsPARTS_BASE::clsPARTS_BASE()
	:m_iDIGIT_BOOSTER_NUM( 2 )
	,m_iDIGIT_SOLE_NUM( 1 )
	,m_iDIGIT_JOINT_NUM( 2 )
	,m_sPartsName( "NoName" )
{	
	D3DXVECTOR3 vINIT_SCALE = { 1.0f, 1.0f, 1.0f };
	m_Trans.vScale = vINIT_SCALE;

}

clsPARTS_BASE::~clsPARTS_BASE()
{
}




//���t���[��.
void clsPARTS_BASE::Update()
{
#ifdef _DEBUG
	ModelUpdate( m_Trans );
#endif//#ifdef _DEBUG
	UpdateBonePos();
	UpdateProduct();
	ModelUpdate( m_Trans );
}




D3DXVECTOR3 clsPARTS_BASE::GetBonePos( const char* sBoneName, const bool isLocalPos )
{
	D3DXVECTOR3 vBonePos;

	//���ꂪ�����ƍ��E�̕��킪�����ꍇ�ɉE����Ɏ����Ă������.
	ModelUpdate( m_Trans );

	//�{�[���̍��W�����.
	if( !m_pMesh->GetPosFromBone( sBoneName, &vBonePos, isLocalPos ) ){
		//�{�[����������Ȃ����.
		string tmpS = m_sPartsName + "��" +  sBoneName;
#ifdef ERROR_MESSAGE_MACRO_BONE_NAME_NOT_FIND
		ERR_MSG( tmpS.c_str(), "���̃��f���ɂ��̖��O�̃{�[���͑��݂��܂���" );
#endif//#ifdef ERROR_MESSAGE_MACRO_BONE_NAME_NOT_FIND
	}

	return vBonePos;
}

