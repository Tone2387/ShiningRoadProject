#ifndef PARTS_CORE_H_
#define PARTS_CORE_H_



#include "PartsBase.h"

//----- �R�A -----//.
#define sBONE_NAME_CORE_NULL		"null"
//�u�[�X�^�[( 00 �` xx ).
#define sBONE_NAME_CORE_BOOSTER_ROOT "BoosterRoot"
#define sBONE_NAME_CORE_BOOSTER_END	 "BoosterEnd"
//���f���̒��S.
#define sBONE_NAME_CORE_JENERATOR	"Jenerator"
//�A�������̃{�[����.
#define sBONE_NAME_CORE_TO_HEAD		"JunctionHead"
#define sBONE_NAME_CORE_TO_ARM_L	"JunctionArmL"
#define sBONE_NAME_CORE_TO_ARM_R	"JunctionArmR"


class clsPARTS_CORE : public clsPARTS_BASE
{
public:
	clsPARTS_CORE();
	~clsPARTS_CORE();


	//�X�e�[�^�X�ϐ��̓Y����.
	enum enSTATUS : PARTS_STATUS_TYPE
	{
		HP = 0,			//�̗�.

		EN_CAPA,		//�G�l���M�[�e�� �G�l���M�[ : energy  �e�� : capacity.
		EN_OUTPUT,		//�G�l���M�[�o��( �񕜗� ).
		BOOST_THRUST_H,	//�u�[�X�^�[����( ���� ).Horizontal
		BOOST_COST_H,	//�u�[�X�^�[����G�l���M�[( ���� ).
		BOOST_THRUST_V,	//�u�[�X�^�[����( ���� ).vertical
		BOOST_COST_V,	//�u�[�X�^�[����G�l���M�[( ���� ).
		ACT_TIME,		//��������(ActivityTime).
						
		COL_SIZE,	//�����蔻��T�C�Y.

		size
	};

	enum enCORE_BONE_POSITIONS : int
	{
		enCORE_BONE_POSITIONS_BOOSTER_ROOT = 0,
		enCORE_BONE_POSITIONS_BOOSTER_END,
		enCORE_BONE_POSITIONS_JENERATOR,
		enCORE_BONE_POSITIONS_JUNCTION_HEAD,
		enCORE_BONE_POSITIONS_JUNCTION_ARM_L,
		enCORE_BONE_POSITIONS_JUNCTION_ARM_R,
		
		enCORE_STATUS_size
	};

	//���O�̃t���[���̃{�[���̍��W�������Ă���.
	D3DXVECTOR3 GetBonePosPreviosFrame( 
		const int enBoneName, 
		int iVecNum = 0 ) const override;
	//���Ŏg�����߂Ƀ{�[�����W���L�^����( Render�̒���Ɏg�� ).
	void UpdateBonePosPreviosFrame() override;

private:
	//----- �e�p�[�c���Ƃ̊֐� -----//.
	void InitProduct() final;//�e�V�[����Init.
	void UpdateProduct() final;//�e�V�[����Update.
	//----- �e�p�[�c���Ƃ̊֐� -----//.

	struct CORE_BONE_POSITIONS
	{
		std::vector<D3DXVECTOR3> vecvBoosterRoot;
		std::vector<D3DXVECTOR3> vecvBoosterEnd;
		D3DXVECTOR3				 vJenerator;
		D3DXVECTOR3				 vJunctionHead;
		D3DXVECTOR3				 vJunctionArmL;
		D3DXVECTOR3				 vJunctionArmR;
	}	m_BonePositions;


};

#endif//#ifndef PARTS_CORE_H_