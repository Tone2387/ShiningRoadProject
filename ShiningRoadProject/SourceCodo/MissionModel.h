#pragma once 


#include "AssembleModel.h"


#define sBONE_NAME_COL_JOINT	"Joint"//00�`xx.

#define sBONE_NAME_COL_CORE "Jenerator"//�R�A�̒��S.
#define sBONE_NAME_COL_HEAD "Center"//���̒��S.



//�~�b�V�������Ƀ��{�N���X���g���A�p�[�c�̏W����.
class clsMISSION_MODEL : public clsASSEMBLE_MODEL
{
public:

	clsMISSION_MODEL();
	~clsMISSION_MODEL();

	//�{�[���ɕK�v�ȏ��.
	struct BONE_SET
	{
		int			iBoneNum;
		int			iParts;//���̃p�[�c��(enPARTS���L���X�g���Ă��̕ϐ��ɓ����).
	};

	enum enCOL_PARTS : UCHAR
	{
		enCOL_PARTS_LEG = 0,
		enCOL_PARTS_CORE,
		enCOL_PARTS_HEAD,
		enCOL_PARTS_ARMS,

		enCOL_PARTS_size
	};

	//Similarity:�����A�ގ���.
	//�ԍ����Ⴄ�����̖��O�̃{�[���̐���Ԃ�.
	int GetSimilarityNameBoneNum(const enPARTS PartsNum, const char* strBoneName);

	//�e�p�[�c�̓����蔻��̐���Ԃ�.
	int GetColNum( const enCOL_PARTS enColParts );

	const D3DXVECTOR3 GetRotfromBone(
		const enPARTS PartsNum,
		const char* strBoneRootName, 
		const char* strBoneEndName);

	const D3DXVECTOR3 GetRotfromVec(
		D3DXVECTOR3 vRoot,
		D3DXVECTOR3 vEnd);

	//�����蔻��̍��W�̔z������ׂĕԂ�.
	std::vector< clsObject::SPHERE > GetColState(clsROBO_STATUS* const pStatus);

	void UpdateColPos();

	void AnimPause()
	{
		for (char i = 0; i < enPARTS_INDEX_size; i++)
		{
			assert(m_vpParts[i]);
			m_vpParts[i]->AnimPause();
		}
	}

	void AnimPlay()
	{
		for (char i = 0; i < enPARTS_INDEX_size; i++)
		{
			assert(m_vpParts[i]);
			m_vpParts[i]->AnimPlay();
		}
	}

	//�w�肵���p�[�c�̉�]�l��ύX����.
	void SetPartsRotate(const enPARTS PartsNum, const D3DXVECTOR3 vRot);
	//�w�肵���p�[�c�̃A�j���[�V������ς���.
	void SetPartsAnimNo(const enPARTS PartsNum, const int iAnimIndex, const double dAnimTime = 0.0);
	//�w�肵���p�[�c�̃A�j���[�V�������x��ς���.
	void SetPartsAnimSpeed(const enPARTS PartsNum, const double dAnimSpeed);
	//�w�肵���p�[�c�̃A�j���[�V������ʏ�Đ��ɂ���.
	void SetPartsAnimNormal(const enPARTS PartsNum, const bool bAnimTimeInit = false);
	//�w�肵���p�[�c�̃A�j���[�V�������t�Đ��ɂ���.
	void SetPartsAnimReverce(const enPARTS PartsNum, const bool bAnimTimeInit = false);

	//�w�肵���p�[�c�̃A�j���[�V�����ԍ���n��.
	const int GetPartsAnimNo(const enPARTS PartsNum);
	//�w�肵���p�[�c�̃A�j���[�V�����I����n�� true:�A�j���[�V�����I��.
	const bool IsPartsAnimEnd(const enPARTS PartsNum);
	//�w�肵���p�[�c�̃A�j���[�V�����Đ����Ԃ�n��.
	const double GetPartsAnimNowTime(const enPARTS PartsNum);
	//�w�肵���p�[�c�̃A�j���[�V�����Đ���Ԃ�n�� true:�t�Đ�.
	const bool IsPartsAnimReverce(const enPARTS PartsNum);

	//�O�t���[���̃{�[���ʒu���󂯎�邽�߂̊֐��Q.
	//�r�p�[�c.
	const D3DXVECTOR3 GetBonePosLegBoosterRoot(const int);
	const D3DXVECTOR3 GetBonePosLegBoosterEnd(const int);

	const D3DXVECTOR3 GetBonePosLegJoint(const int);

	const D3DXVECTOR3 GetBonePosLegSole(const int);

	//���p�[�c.
	const D3DXVECTOR3 GetBonePosHeadCenter();

	//�R�A�p�[�c.
	const D3DXVECTOR3 GetBonePosCoreBoosterRoot(const int);
	const D3DXVECTOR3 GetBonePosCoreBoosterEnd(const int);

	const D3DXVECTOR3 GetBonePosCoreJenerator();
	
	//���r�p�[�c.
	const D3DXVECTOR3 GetBonePosArmLBoostFrontRoot(const int);
	const D3DXVECTOR3 GetBonePosArmLBoostFrontEnd(const int);

	const D3DXVECTOR3 GetBonePosArmLBoostSideRoot(const int);
	const D3DXVECTOR3 GetBonePosArmLBoostSideEnd(const int);

	const D3DXVECTOR3 GetBonePosArmLBoostBackRoot(const int);
	const D3DXVECTOR3 GetBonePosArmLBoostBackEnd(const int);

	const D3DXVECTOR3 GetBonePosArmLJoint(const int);

	//�E�r�p�[�c.
	const D3DXVECTOR3 GetBonePosArmRBoostFrontRoot(const int);
	const D3DXVECTOR3 GetBonePosArmRBoostFrontEnd(const int);

	const D3DXVECTOR3 GetBonePosArmRBoostSideRoot(const int);
	const D3DXVECTOR3 GetBonePosArmRBoostSideEnd(const int);

	const D3DXVECTOR3 GetBonePosArmRBoostBackRoot(const int);
	const D3DXVECTOR3 GetBonePosArmRBoostBackEnd(const int);

	const D3DXVECTOR3 GetBonePosArmRJoint(const int);

	//������p�[�c.
	const D3DXVECTOR3 GetBonePosWeaponLMuzzleRoot();
	const D3DXVECTOR3 GetBonePosWeaponLMuzzleEnd();

	const D3DXVECTOR3 GetBonePosWeaponLCartridgeInjection();

	//�E����p�[�c.
	const D3DXVECTOR3 GetBonePosWeaponRMuzzleRoot();
	const D3DXVECTOR3 GetBonePosWeaponRMuzzleEnd();

	const D3DXVECTOR3 GetBonePosWeaponRCartridgeInjection();

private:

	//�����̃p�[�c�̓����蔻��̏��쐬.
	//�߂�l�S�p�[�c�̓����蔻��̐�.
	int CreateColStateBones();

	//�{�[���̐����m�肷��( CreateColStateBones�֐��̌�Ɏg�� ).
	void FixBoneStates();
	//.
	//�߂�l���̃p�[�c�̓����蔻��̐�.
	int CreateColStateBone( const enCOL_PARTS enColParts );

	//Create�֐��̒��ɂ���.
	void CreateProduct( clsROBO_STATUS* const pStatus ) override;

	//�{�[�����.
	std::vector< BONE_SET > m_vColStates;
	int						m_iColStateIndex;
	std::vector< D3DXVECTOR3 > m_vvColPos;//�֐��œn���p�̕ϐ�.

	int m_iColNum[ enCOL_PARTS_size ];//�e�p�[�c�̓����蔻��̐�.

	int m_iColMax;//�����蔻��̍��v��.

};

