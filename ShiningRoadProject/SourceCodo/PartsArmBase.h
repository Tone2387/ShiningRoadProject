#ifndef PARTS_ARM_BASE_H_
#define PARTS_ARM_BASE_H_

#include "PartsBase.h"


//----- �r -----//.
//�u�[�X�^�[( 00 �` xx ).
#define sBONE_NAME_ARM_BOOSTER_FRONT_ROOT	"BoosterFrontRoot"
#define sBONE_NAME_ARM_BOOSTER_FRONT_END	"BoosterFrontEnd"
#define sBONE_NAME_ARM_BOOSTER_BACK_ROOT	"BoosterBackRoot"
#define sBONE_NAME_ARM_BOOSTER_BACK_END		"BoosterBackEnd"
#define sBONE_NAME_ARM_BOOSTER_SIDE_ROOT	"BoosterSideRoot"
#define sBONE_NAME_ARM_BOOSTER_SIDE_END		"BoosterSideEnd"
//�A�������̃{�[����.
#define sBONE_NAME_ARM_TO_WEAPON			"JunctionWeapon"
//����̉�]���m��Ɏg��.
#define sBONE_NAME_ARM_WEAPON_VEC_ROOT		sBONE_NAME_ARM_TO_WEAPON	
#define sBONE_NAME_ARM_WEAPON_VEC_END		"WeaponVec"		

//�֐�( 00 �` xx ).
#define sBONE_NAME_ARM_JOINT				"Joint"


class clsPARTS_ARM_BASE : public clsPARTS_BASE
{
public:
	clsPARTS_ARM_BASE();
	virtual ~clsPARTS_ARM_BASE();


	//�X�e�[�^�X�ϐ��̓Y����.
	enum enSTATUS : PARTS_STATUS_TYPE
	{
		HP = 0,	//�̗�.

		AIMING,	//�Ə����x( �G�C�� ).
		QUICK_THRUST,	//�N�C�b�N����.
		QUICK_COST,		//�N�C�b�N����G�l���M�[.
		QUICK_TIME,		//�N�C�b�N���ˎ���.
						
		COL_SIZE,	//�����蔻��T�C�Y.

		size
	};


	enum enARMS_BONE_POSITIONS
	{
		enARMS_BONE_POSITIONS_BOOSTER_FRONT_ROOT = 0,
		enARMS_BONE_POSITIONS_BOOSTER_FRONT_END,
		enARMS_BONE_POSITIONS_BOOSTER_BACK_ROOT,
		enARMS_BONE_POSITIONS_BOOSTER_BACK_END,
		enARMS_BONE_POSITIONS_BOOSTER_SIDE_ROOT,
		enARMS_BONE_POSITIONS_BOOSTER_SIDE_END,
			   
		enARMS_BONE_POSITIONS_JOINT,
		enARMS_BONE_POSITIONS_JUNCTION_WEAPON,
		enARMS_BONE_POSITIONS_WEAPON_VEC,
			   
		enARMS_BONE_POSITIONS_size
	};


	//���O�̃t���[���̃{�[���̍��W�������Ă���.
	D3DXVECTOR3 GetBonePosPreviosFrame( 
		const int enBoneName, 
		int iVecNum = 0 ) const override;
	//���Ŏg�����߂Ƀ{�[�����W���L�^����( Render�̒���Ɏg�� ).
	void UpdateBonePosPreviosFrame() override;

protected:

	//----- �e�p�[�c���Ƃ̊֐� -----//.
	virtual void InitProduct() override;//�e�V�[����Init.
	virtual void UpdateProduct() override;//�e�V�[����Update.
	//----- �e�p�[�c���Ƃ̊֐� -----//.

	struct ARMS_BONE_POSITIONS
	{
		std::vector<D3DXVECTOR3> vecvBoosterFontRoot;
		std::vector<D3DXVECTOR3> vecvBoosterFontEnd;
		std::vector<D3DXVECTOR3> vecvBoosterBackRoot;
		std::vector<D3DXVECTOR3> vecvBoosterBackEnd;
		std::vector<D3DXVECTOR3> vecvBoosterSideRoot;
		std::vector<D3DXVECTOR3> vecvBoosterSideEnd;
		std::vector<D3DXVECTOR3> vecvJoint;
		D3DXVECTOR3				 vJunctionWeapon;
		D3DXVECTOR3				 vWeaponVec;
	}	m_BonePositions;


private:


};

#endif//#ifndef PARTS_ARM_BASE_H_