#include "RoboStatusBase.h"

#include "PartsLeg.h"
#include "PartsCore.h"
#include "PartsHead.h"
#include "PartsArmL.h"
#include "PartsArmR.h"
#include "PartsWeapon.h"


using namespace std;

namespace{

	const int iCOLOR_RANK_INIT = 16;

}



clsROBO_STATUS::clsROBO_STATUS()
	:m_iFILE_VAR_ROW( 0 )//��s�������݂��Ȃ�.
	,m_iFILE_INDEX_COL_LEG( 0 )
	,m_iFILE_INDEX_COL_CORE( 1 )
	,m_iFILE_INDEX_COL_HEAD( 2 )
	,m_iFILE_INDEX_COL_ARM_L( 3 )
	,m_iFILE_INDEX_COL_ARM_R( 4 )
	,m_iFILE_INDEX_COL_WEAPON_L( 5 )
	,m_iFILE_INDEX_COL_WEAPON_R( 6 )
	,m_iFILE_INDEX_COL_COLOR_ARMOR_R( 7 )
	,m_iFILE_INDEX_COL_COLOR_ARMOR_G( 8 )
	,m_iFILE_INDEX_COL_COLOR_ARMOR_B( 9 )
	,m_iFILE_INDEX_COL_COLOR_BASE_R( 10 )
	,m_iFILE_INDEX_COL_COLOR_BASE_G( 11 )
	,m_iFILE_INDEX_COL_COLOR_BASE_B( 12 )
{	 
	Clear();


	UCHAR tmpSize = sizeof( m_RoboStateData.ucPartsModelNum ) / sizeof( m_RoboStateData.ucPartsModelNum[0] );
	for( UCHAR i=0; i<tmpSize; i++ ){
		m_RoboStateData.ucPartsModelNum[i] = 0;
	}

	for( char i=0; i<enCOLOR_GAGE_size; i++ ){
		m_RoboStateData.iColorRank[i] = iCOLOR_RANK_INIT;
	}

}

clsROBO_STATUS::~clsROBO_STATUS()
{
	Clear();
}

//�܂�����ɂ���.
void clsROBO_STATUS::Clear()
{
	for( UCHAR i=0; i<enROBO_STATE_SIZE; i++ ){
		m_iRoboState[i] = 0;
	}

	for( UCHAR i=0; i<enWEAPON_STATE_SIZE; i++ ){
		m_iWeaponState[enWEAPON_NUM::LEFT][i] = 0;
		m_iWeaponState[enWEAPON_NUM::RIGHT][i] = 0;
	}
	for( UCHAR i=0; i<enHAVE_HP_PARTS_SIZE; i++ ){
		m_iRoboHp[i] = 0;
	}
	

}



//���{�̏���f���o��.
int clsROBO_STATUS::GetRoboState( const enROBO_STATE enStateNum )
{
	if( enStateNum >= enROBO_STATE_SIZE ){
		assert( !"�s���ȏ����擾���悤�Ƃ���܂���" );
		return 0;
	}

	//HP���Z.
	if( enStateNum == enROBO_STATE::HP ){
		m_iRoboState[enStateNum] = 0;
		for( UCHAR i=0; i<enHAVE_HP_PARTS_SIZE; i++ ){
			m_iRoboState[enStateNum] += m_iRoboHp[i];
		}
	}

	return m_iRoboState[enStateNum];
}
//����̏���f���o��.
int clsROBO_STATUS::GetWeaponState( 
	const enWEAPON_NUM enArmLR, const enWEAPON_STATE enStateNum ) const
{
	if( enArmLR >= enWEAPON_NUM_SIZE ){
		assert( !"�s���Ȑ��̕��킪�w�肳��܂���" );
		return 0;
	}

	if( enStateNum >= enWEAPON_STATE_SIZE ){
		assert( !"�s���ȕ�������擾���悤�Ƃ���܂���" );
		return 0;
	}

	return m_iWeaponState[enArmLR][enStateNum];
}


//�p�[�c�ԍ���Ԃ�( ���ܑ������Ă���p�[�c�����Ԃ� ).
UCHAR clsROBO_STATUS::GetPartsNum( const enPARTS PartsType ) const
{
	return m_RoboStateData.ucPartsModelNum[ static_cast<int>( PartsType ) ];
}



//�f�[�^�̎󂯎��.
void clsROBO_STATUS::ReceiveLeg( const vector<int> &LegDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( LegDatas.size() > clsPARTS_LEG::size ){
		assert( !"�󂯎����vector�f�[�^��size���傫�����܂�" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::LEG]		  = LegDatas[clsPARTS_LEG::HP];			//�l�̃p�[�c�ɉe�������.
	
	m_iRoboState[enROBO_STATE::WALK_SPD]  = LegDatas[clsPARTS_LEG::WALK_SPD];	//���s���x.
	m_iRoboState[enROBO_STATE::STABILITY] = LegDatas[clsPARTS_LEG::STABILITY];	//���萫�\.
	m_iRoboState[enROBO_STATE::TURN]	  = LegDatas[clsPARTS_LEG::TURN];		//���񐫔\.
	m_iRoboState[enROBO_STATE::JUMP_POWER]= LegDatas[clsPARTS_LEG::JUMP_POWER];	//�W�����v��.

	m_iRoboState[enROBO_STATE::COL_SIZE_LEG]	= LegDatas[clsPARTS_LEG::COL_SIZE];//�����蔻��T�C�Y.
	m_iRoboState[enROBO_STATE::WALK_ANIM_SPEED] = LegDatas[clsPARTS_LEG::WALK_ANIM_SPEED];//���s�A�j�����x

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::LEG ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveCore( const vector<int> &CoreDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( CoreDatas.size() > clsPARTS_CORE::size ){
		assert( !"�󂯎����vector�f�[�^��size���傫�����܂�" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::CORE]			= CoreDatas[clsPARTS_CORE::HP];				//�l�̃p�[�c�ɉe�������.
	
	m_iRoboState[enROBO_STATE::EN_CAPA]			= CoreDatas[clsPARTS_CORE::EN_CAPA];		//�G�l���M�[�e�� �G�l���M�[ : energy  �e�� : capacity.
	m_iRoboState[enROBO_STATE::EN_OUTPUT]		= CoreDatas[clsPARTS_CORE::EN_OUTPUT];		//�G�l���M�[�o��( �񕜗� ).
	m_iRoboState[enROBO_STATE::BOOST_THRUST_H]	= CoreDatas[clsPARTS_CORE::BOOST_THRUST_H];	//�u�[�X�^�[����( ���� ).
	m_iRoboState[enROBO_STATE::BOOST_COST_H]	= CoreDatas[clsPARTS_CORE::BOOST_COST_H];	//�u�[�X�^�[����G�l���M�[( ���� ).
	m_iRoboState[enROBO_STATE::BOOST_THRUST_V]	= CoreDatas[clsPARTS_CORE::BOOST_THRUST_V];	//�u�[�X�^�[����( ���� ).
	m_iRoboState[enROBO_STATE::BOOST_COST_V]	= CoreDatas[clsPARTS_CORE::BOOST_COST_V];	//�u�[�X�^�[����G�l���M�[( ���� ).
	m_iRoboState[enROBO_STATE::ACT_TIME]		= CoreDatas[clsPARTS_CORE::ACT_TIME];		//��������(ActivityTime).

	m_iRoboState[enROBO_STATE::COL_SIZE_CORE]	= CoreDatas[clsPARTS_CORE::COL_SIZE];	//�����蔻��T�C�Y.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::CORE ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveHead( const vector<int> &HeadDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( HeadDatas.size() > clsPARTS_HEAD::size ){
		assert( !"�󂯎����vector�f�[�^��size���傫�����܂�" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::HEAD]  = HeadDatas[clsPARTS_HEAD::HP];		//�l�̃p�[�c�ɉe�������.
	
	m_iRoboState[enROBO_STATE::SEARCH]			= HeadDatas[clsPARTS_HEAD::SEARCH];		//���G���\.
	m_iRoboState[enROBO_STATE::LOCK_ON_SPEED]	= HeadDatas[clsPARTS_HEAD::LOCK_ON_SPEED];	//���b�N���x.
	m_iRoboState[enROBO_STATE::LOCK_ON_RANGE]	= HeadDatas[clsPARTS_HEAD::LOCK_ON_RANGE];	//���b�N����.

	m_iRoboState[enROBO_STATE::COL_SIZE_HEAD]= HeadDatas[clsPARTS_HEAD::COL_SIZE];	//�����蔻��T�C�Y.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::HEAD ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveArms( const vector<int> &ArmsDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( ArmsDatas.size() > clsPARTS_ARM_BASE::size ){
		assert( !"�󂯎����vector�f�[�^��size���傫�����܂�" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::ARMS]		= ArmsDatas[clsPARTS_ARM_BASE::HP];		//�l�̃p�[�c�ɉe�������.
	
	m_iRoboState[enROBO_STATE::AIMING]		= ArmsDatas[clsPARTS_ARM_BASE::AIMING];			//�Ə����x( �G�C�� ).
	m_iRoboState[enROBO_STATE::QUICK_THRUST]= ArmsDatas[clsPARTS_ARM_BASE::QUICK_THRUST];	//�N�C�b�N����.
	m_iRoboState[enROBO_STATE::QUICK_COST]	= ArmsDatas[clsPARTS_ARM_BASE::QUICK_COST];		//�N�C�b�N����G�l���M�[.
	m_iRoboState[enROBO_STATE::QUICK_TIME]	= ArmsDatas[clsPARTS_ARM_BASE::QUICK_TIME];		//�N�C�b�N���ˎ���.

	m_iRoboState[enROBO_STATE::COL_SIZE_ARMS] = ArmsDatas[clsPARTS_ARM_BASE::COL_SIZE]; //�����蔻��T�C�Y.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::ARM_L ) ] = static_cast<UCHAR>( PartsNum );
	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::ARM_R ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveWeaponL( const vector<int> &WeaponLDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( WeaponLDatas.size() > clsPARTS_WEAPON::size ){
		assert( !"�󂯎����vector�f�[�^��size���傫�����܂�" );
	}

	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::ATK]					= WeaponLDatas[clsPARTS_WEAPON::ATK];				//�U����.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::BULLET_SPD]			= WeaponLDatas[clsPARTS_WEAPON::BULLET_SPD];		//�e��.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::RANGE]				= WeaponLDatas[clsPARTS_WEAPON::RANGE];				//�˒�����.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::COST]				= WeaponLDatas[clsPARTS_WEAPON::COST];				//����G�l���M�[.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::LOAD_TIME]			= WeaponLDatas[clsPARTS_WEAPON::LOAD_TIME];			//���U����.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::LOCK_ON_TIME]		= WeaponLDatas[clsPARTS_WEAPON::LOCK_ON_TIME];		//���b�N����.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::SHOT_STABILITY]		= WeaponLDatas[clsPARTS_WEAPON::STABILITY];			//�ˌ�����.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::MAGAZINE_LOAD_TIME]	= WeaponLDatas[clsPARTS_WEAPON::MAGAZINE_LOAD_TIME];//�}�K�W�����U����.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::BULLETS_NUM]			= WeaponLDatas[clsPARTS_WEAPON::BULLETS_NUM];		//���e��.

	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::COL_SIZE]	= WeaponLDatas[clsPARTS_WEAPON::COL_SIZE];	//�e�̓����蔻��T�C�Y.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::EFC_BULLET]	= WeaponLDatas[clsPARTS_WEAPON::EFC_BULLET];//�e�{�̂̃G�t�F�N�g.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::EFC_LOCUS]	= WeaponLDatas[clsPARTS_WEAPON::EFC_LOCUS];	//�e�̋O�Ղ̃G�t�F�N�g.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::EFC_HIT]		= WeaponLDatas[clsPARTS_WEAPON::EFC_HIT];	//���e�̃G�t�F�N�g.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::EFC_CARTRIDGE]=WeaponLDatas[clsPARTS_WEAPON::EFC_HIT];	//���e�̃G�t�F�N�g.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::SE_FIER]		= WeaponLDatas[clsPARTS_WEAPON::SE_FIER];	//����SE.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::SE_HIT]		= WeaponLDatas[clsPARTS_WEAPON::SE_HIT];	//���e��SE.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::WEAPON_L ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveWeaponR( const vector<int> &WeaponRDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( WeaponRDatas.size() > clsPARTS_WEAPON::size ){
		assert( !"�󂯎����vector�f�[�^��size���傫�����܂�" );
	}

	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::ATK]				= WeaponRDatas[clsPARTS_WEAPON::ATK];				//�U����.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::BULLET_SPD]			= WeaponRDatas[clsPARTS_WEAPON::BULLET_SPD];		//�e��.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::RANGE]				= WeaponRDatas[clsPARTS_WEAPON::RANGE];				//�˒�����.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::COST]				= WeaponRDatas[clsPARTS_WEAPON::COST];				//����G�l���M�[.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::LOAD_TIME]			= WeaponRDatas[clsPARTS_WEAPON::LOAD_TIME];			//���U����.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::LOCK_ON_TIME]		= WeaponRDatas[clsPARTS_WEAPON::LOCK_ON_TIME];		//���b�N����.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::SHOT_STABILITY]		= WeaponRDatas[clsPARTS_WEAPON::STABILITY];			//�ˌ�����.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::MAGAZINE_LOAD_TIME]	= WeaponRDatas[clsPARTS_WEAPON::MAGAZINE_LOAD_TIME];//�}�K�W�����U����.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::BULLETS_NUM]		= WeaponRDatas[clsPARTS_WEAPON::BULLETS_NUM];		//���e��.

	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::COL_SIZE]	= WeaponRDatas[clsPARTS_WEAPON::COL_SIZE];	//�e�̓����蔻��T�C�Y.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::EFC_BULLET]	= WeaponRDatas[clsPARTS_WEAPON::EFC_BULLET];//�e�{�̂̃G�t�F�N�g.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::EFC_LOCUS]	= WeaponRDatas[clsPARTS_WEAPON::EFC_LOCUS];	//�e�̋O�Ղ̃G�t�F�N�g.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::EFC_HIT]	= WeaponRDatas[clsPARTS_WEAPON::EFC_HIT];	//���e�̃G�t�F�N�g.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::EFC_CARTRIDGE]=WeaponRDatas[clsPARTS_WEAPON::EFC_HIT];	//���e�̃G�t�F�N�g.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::SE_FIER]	= WeaponRDatas[clsPARTS_WEAPON::SE_FIER];	//����SE.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::SE_HIT]		= WeaponRDatas[clsPARTS_WEAPON::SE_HIT];	//���e��SE.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::WEAPON_R ) ] = static_cast<UCHAR>( PartsNum );
}


//�F�t���O�̂����.
void clsROBO_STATUS::SetColorRank( 
	const enCOLOR_GAGE enColorNum, 
	const int iColorRate )
{
	assert( enColorNum >= 0  );
	assert( enColorNum < enCOLOR_GAGE_size );
	m_RoboStateData.iColorRank[ enColorNum ] = iColorRate;
}


int clsROBO_STATUS::GetColorRank( const enCOLOR_GAGE enColorNum ) const
{
	assert( enColorNum >= 0  );
	assert( enColorNum < enCOLOR_GAGE_size );
	return m_RoboStateData.iColorRank[ enColorNum ];
}
