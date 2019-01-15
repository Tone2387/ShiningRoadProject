#ifndef ROBO_STATUS_H_
#define ROBO_STATUS_H_


class clsFILE;
#include "Global.h"


//�A�Z���u���V�[���ł̑I�����̌^.
using ASSEMBLE_SCENE_SELECT_TYPE = short;


//�Q�[���N���X�ō쐬�A.
//�A�Z���u���V�[���Œl��ݒ�A.
//�~�b�V���V�[���ŏ������̒i�K�Ń��{�ɓn��.
class clsROBO_STATUS
{
public:
	clsROBO_STATUS();
	virtual ~clsROBO_STATUS();

	//�F�̖_.
	enum enCOLOR_GAGE : char
	{
		enCOLOR_GAGE_BASE_R = 0,
		enCOLOR_GAGE_BASE_G,
		enCOLOR_GAGE_BASE_B,

		enCOLOR_GAGE_ARMOR_R,
		enCOLOR_GAGE_ARMOR_G,
		enCOLOR_GAGE_ARMOR_B,

		enCOLOR_GAGE_size
	};
	 
	//���{�̃X�e�[�^�X�z��̈���.
	enum enROBO_STATE : UCHAR
	{
		HP = 0,		//�̗�.

		WALK_SPD,	//���s���x.
		STABILITY,	//���萫�\.
		TURN,		//���񐫔\.
		JUMP_POWER,	//�W�����v��.

		EN_CAPA,		//�G�l���M�[�e�� �G�l���M�[ : energy  �e�� : capacity.
		EN_OUTPUT,		//�G�l���M�[�o��( �񕜗� ).
		BOOST_THRUST_H,	//�u�[�X�^�[����( ���� ).
		BOOST_COST_H,	//�u�[�X�^�[����G�l���M�[( ���� ).
		BOOST_THRUST_V,	//�u�[�X�^�[����( ���� ).
		BOOST_COST_V,	//�u�[�X�^�[����G�l���M�[( ���� ).
		ACT_TIME,		//��������(ActivityTime).

		SEARCH,			//���G���\.
		LOCK_ON_SPEED,	//���b�N���x.
		LOCK_ON_RANGE,	//���b�N����.

		AIMING,			//�Ə����x( �G�C�� ).
		QUICK_THRUST,	//�N�C�b�N����.
		QUICK_COST,		//�N�C�b�N����G�l���M�[.
		QUICK_TIME,		//�N�C�b�N���ˎ���.

		//�B���X�e�[�^�X.
		COL_SIZE_LEG,	//�r�����蔻��T�C�Y.
		COL_SIZE_CORE,	//�R�A�����蔻��T�C�Y.
		COL_SIZE_HEAD,	//�������蔻��T�C�Y.
		COL_SIZE_ARMS,	//�r�����蔻��T�C�Y.
		WALK_ANIM_SPEED,//�r���s�A�j�����x.

		enROBO_STATE_SIZE
	};

	//����̃X�e�[�^�X�z��̈���.
	enum enWEAPON_STATE : UCHAR
	{
		ATK = 0,			//�U����.
		BULLET_SPD,			//�e��.
		RANGE,				//�˒�����.
		COST,				//����G�l���M�[.
		LOAD_TIME,			//���U����.
		LOCK_ON_TIME,		//���b�N����.
		SHOT_STABILITY,		//�ˌ�����.
		MAGAZINE_LOAD_TIME,	//�}�K�W�����U����.
		BULLETS_NUM,		//���e��.

		//�B���X�e�[�^�X.
		COL_SIZE,	//�e�̓����蔻��T�C�Y.
		EFC_BULLET,	//�e�{�̂̃G�t�F�N�g.
		EFC_LOCUS,	//�e�̋O�Ղ̃G�t�F�N�g.
		EFC_HIT,	//���e�̃G�t�F�N�g.
		EFC_MUZZLE,//�}�Y���G�t�F�N�g.
		SE_FIER,	//����SE.
		SE_HIT,		//���e��SE.

		enWEAPON_STATE_SIZE
	};

	//����̃X�e�[�^�X���E�̈���.
	enum enWEAPON_NUM : UCHAR
	{
		LEFT = 0,
		RIGHT= 1,

		enWEAPON_NUM_SIZE
	};


	//�܂�����ɂ���( �A�Z���u���V�[���ł̏������Ƃ��Ďg�� ).
	void Clear();

	//�f�[�^�̎󂯎��.
	//�����̊֐��Ńf�[�^���󂯎��O��Clear()���Ă�������.
	//������ : �U���͂Ȃǂ̃p�����[�^�Q.
	//������ : ���f���ԍ�.
	void ReceiveLeg(	const std::vector<int> &LegDatas,		const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveCore(	const std::vector<int> &CoreDatas,		const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveHead(	const std::vector<int> &HeadDatas,		const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveArms(	const std::vector<int> &ArmsDatas,		const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveWeaponL(const std::vector<int> &WeaponLDatas,	const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveWeaponR(const std::vector<int> &WeaponRDatas,	const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );


	//���{�̏���f���o��.
	int GetRoboState( const enROBO_STATE enStateNum );
	//����̏���f���o��.
	int GetWeaponState( const enWEAPON_NUM enArmLR, const enWEAPON_STATE enStateNum ) const;

	//�p�[�c�ԍ���Ԃ�( ���ܑ������Ă���p�[�c�����Ԃ� ).//#define SKIN_ENUM_TYPE UCHAR.
	//clsASSEMBLE_MODEL�N���X��AttachModel�̑������Ƃ��Ďg���܂�.
	UCHAR GetPartsNum( const enPARTS PartsType ) const;


	//�F�t���O�̂����.
	void SetColorRank( const enCOLOR_GAGE enColorNum, const int iColorRate );
	int GetColorRank( const enCOLOR_GAGE enColorNum ) const;

protected:


	//�p����ŊO������f�[�^��ǂݍ��܂���.
	virtual void LoadFileData( const char* sFilePath ) = 0;

	//�p�[�c�̃X�e�[�^�X�f�[�^��ǂݍ���.
	void LoadPartsData( std::vector< std::unique_ptr< clsFILE > >& PartsFile );

	//�ǂݍ��񂾔ԍ��̃f�[�^���擾����.
	void AttachData( const std::vector< std::unique_ptr< clsFILE > >& PartsFile );

	//ReceiveLeg()���̊֐��Ɋi�[���邽�߂̃f�[�^�����A�f���o��.
	std::vector<int> CreateDataForReceive( 
		const std::vector< std::unique_ptr< clsFILE > >& PartsFile,
		const int PartsType,
		const int PartsNum );

protected:

	//�V�[�����܂����ŕK�v�ȃ��{�̃f�[�^.
	struct ROBO_STATE_DATA
	{
		//�p�[�c�ԍ��̔z��.
		UCHAR ucPartsModelNum[ static_cast<int>( enPARTS::MAX ) ];

		//�F�̒i�K.
		int iColorRank[ enCOLOR_GAGE_size ];
	}	m_RoboStateData;


	//�p���N���X�Ŏg��.
	const int m_iFILE_VAR_ROW;
	const int m_iFILE_INDEX_COL_LEG;
	const int m_iFILE_INDEX_COL_CORE;
	const int m_iFILE_INDEX_COL_HEAD;
	const int m_iFILE_INDEX_COL_ARM_L;
	const int m_iFILE_INDEX_COL_ARM_R;
	const int m_iFILE_INDEX_COL_WEAPON_L;
	const int m_iFILE_INDEX_COL_WEAPON_R;
	const int m_iFILE_INDEX_COL_COLOR_ARMOR_R;
	const int m_iFILE_INDEX_COL_COLOR_ARMOR_G;
	const int m_iFILE_INDEX_COL_COLOR_ARMOR_B;
	const int m_iFILE_INDEX_COL_COLOR_BASE_R;
	const int m_iFILE_INDEX_COL_COLOR_BASE_G;
	const int m_iFILE_INDEX_COL_COLOR_BASE_B;



private:

	//HP�����p�[�c.
	enum enHAVE_HP_PARTS
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,

		enHAVE_HP_PARTS_SIZE
	};

	//���{�b�g�{�̂̃p�����[�^.
	int m_iRoboState[ enROBO_STATE_SIZE ];

	//����̃p�����[�^.
	int m_iWeaponState[ enWEAPON_NUM_SIZE ][ enWEAPON_STATE_SIZE ];

	//���{��HP����ō��Z���邽�߂̕ϐ�.
	int m_iRoboHp[ enHAVE_HP_PARTS_SIZE ];

};
#endif//#ifndef ROBO_STATUS_H_