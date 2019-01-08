#ifndef PARTS_LEG_H_
#define PARTS_LEG_H_



#include "PartsBase.h"

//----- �� -----//.
#define sBONE_NAME_LEG_NULL			"null"
//��_�̃{�[����.
#define sBONE_NAME_LEG_POSITION_BASE "PositionBase"
//�A�������̃{�[����.
#define sBONE_NAME_LEG_TO_CORE		"JunctionCore"
//���̗�( 0 �` x ).
#define sBONE_NAME_LEG_SOLE			"Sole"
//�A�������̃{�[����.
#define sBONE_NAME_LEG_TO_CORE		"JunctionCore"

//�u�[�X�^�[( 00 �` xx ).
#define sBONE_NAME_LEG_BOOSTER_ROOT	"BoosterRoot"
#define sBONE_NAME_LEG_BOOSTER_END	"BoosterEnd"

//�֐�( 00 �` xx ).
#define sBONE_NAME_LEG_JOINT		"Joint"


class clsPARTS_LEG : public clsPARTS_BASE
{
public:
	clsPARTS_LEG();
	~clsPARTS_LEG();

	//�X�e�[�^�X�ϐ��̓Y����.
	enum enSTATUS : PARTS_STATUS_TYPE
	{
		HP = 0,		//�̗�.
		WALK_SPD,	//���s���x.
		STABILITY,	//���萫�\.
		TURN,		//���񐫔\.
		JUMP_POWER,	//�W�����v��.

		COL_SIZE,	//�����蔻��T�C�Y.
		WALK_ANIM_SPEED,//���s�A�j���[�V�������x.
						
		size
	};

	enum enLEG_BONE_POSITIONS : int
	{
		enLEG_BONE_POSITIONS_BOOSTER_ROOT = 0,
		enLEG_BONE_POSITIONS_BOOSTER_END,
		enLEG_BONE_POSITIONS_JOINT,
		enLEG_BONE_POSITIONS_JUNCTION_CORE,
		enLEG_BONE_POSITIONS_SOLE,
		enLEG_BONE_POSITIONS_POSITION_BASE,
		enLEG_BONE_POSITIONS_NULL,
			  
		enLEG_BONE_POSITIONS_size
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

	struct LEG_BONE_POSITIONS
	{
		std::vector<D3DXVECTOR3> vecvBoosterRoot;
		std::vector<D3DXVECTOR3> vecvBoosterEnd;
		std::vector<D3DXVECTOR3> vecvJoint;
		D3DXVECTOR3				 vJunctionCore;
		std::vector<D3DXVECTOR3> vecvSole;
		D3DXVECTOR3				 vPositionBase;
		D3DXVECTOR3				 vNull;
	}	m_BonePositions;


};

#endif//#ifndef PARTS_LEG_H_