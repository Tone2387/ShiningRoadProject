#ifndef PARTS_HEAD_H_
#define PARTS_HEAD_H_


#include "PartsBase.h"

//----- �� -----//.
#define sBONE_NAME_HEAD_CENTER "Center"


class clsPARTS_HEAD : public clsPARTS_BASE
{
public:
	clsPARTS_HEAD();
	~clsPARTS_HEAD();

	enum enSTATUS : PARTS_STATUS_TYPE
	{
		HP = 0,	//�̗�.

		SEARCH,		//���G���\.
		LOCK_ON_SPEED,	//���b�N���x.
		LOCK_ON_RANGE,	//���b�N����.

		COL_SIZE,	//�����蔻��T�C�Y.

		size
	};

	enum enHEAD_BONE_POSITIONS
	{
		enHEAD_BONE_POSITIONS_CENTER = 0,
			   
		enHEAD_BONE_POSITIONS_size
	};

	//���O�̃t���[���̃{�[���̍��W�������Ă���.
	D3DXVECTOR3 GetBonePosPreviosFrame( 
		const int enBoneName, 
		int iVecNum = 0 ) const override;
	//���Ŏg�����߂Ƀ{�[�����W���L�^����( Render�̒���Ɏg�� ).
	void UpdateBonePosPreviosFrame() override{
		m_BonePositions.vCenter	= this->GetBonePos( sBONE_NAME_HEAD_CENTER );
	};

private:
	//----- �e�p�[�c���Ƃ̊֐� -----//.
	void InitProduct() final;//�e�V�[����Init.
	void UpdateProduct() final;//�e�V�[����Update.
	//----- �e�p�[�c���Ƃ̊֐� -----//.

	struct HEAD_BONE_POSITIONS
	{
		D3DXVECTOR3 vCenter;
	}	m_BonePositions;

};

#endif//#ifndef PARTS_HEAD_H_