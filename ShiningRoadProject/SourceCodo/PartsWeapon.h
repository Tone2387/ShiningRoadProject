#ifndef PARTS_WEAPON_H_
#define PARTS_WEAPON_H_



#include "PartsBase.h"

//----- ���� -----//.
#define sBONE_NAME_WEAPON_MUZZLE_ROOT			"MuzzleRoot"
#define sBONE_NAME_WEAPON_MUZZLE_END			"MuzzleEnd"
#define sBONE_NAME_WEAPON_CARTRIDGE_INJECTION	"CartridgeInjection"




class clsPARTS_WEAPON : public clsPARTS_BASE
{
public:
	clsPARTS_WEAPON();
	~clsPARTS_WEAPON();

	//�X�e�[�^�X�ϐ��̓Y����.
	enum enSTATUS : PARTS_STATUS_TYPE
	{
		ATK = 0,			//�U����.
		BULLET_SPD,			//�e��.
		RANGE,				//�˒�����.
		COST,				//����G�l���M�[.
		LOAD_TIME,			//���U����.
		LOCK_ON_TIME,		//���b�N����.
		STABILITY,			//�ˌ�����.
		MAGAZINE_LOAD_TIME,	//�}�K�W�����U����.
		BULLETS_NUM,		//���e��.

		COL_SIZE,	//�e�̓����蔻��T�C�Y.
		EFC_BULLET,	//�e�{�̂̃G�t�F�N�g.
		EFC_LOCUS,	//�e�̋O�Ղ̃G�t�F�N�g.
		EFC_HIT,	//���e�̃G�t�F�N�g.
		EFC_CARTRIDGE,//��䰃G�t�F�N�g.
		SE_FIER,	//����SE.
		SE_HIT,		//���e��SE.

		size
	};

	enum enWEAPON_BONE_POSITIONS
	{
		enWEAPON_BONE_POSITIONS_MUZZLE_ROOT,
		enWEAPON_BONE_POSITIONS_MUZZLE_END,
		enWEAPON_BONE_POSITIONS_CARTRIDGE_INJECTION,
				 
		enWEAPON_BONE_POSITIONS_size
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

	struct WEAPON_BONE_POSITIONS
	{
		D3DXVECTOR3 vMuzzleRoot;
		D3DXVECTOR3 vMuzzleEnd;
		D3DXVECTOR3 vCartridgeInjection;
	}	m_BonePositions;


};

#endif//PARTS_WEAPON_H_