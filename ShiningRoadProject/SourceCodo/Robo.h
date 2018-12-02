#pragma once

#include"Charactor.h"
#include"SkinMesh.h"

//�����̏�����������.
#ifdef Tahara
#include "PtrGroup.h"
#include "MissionModel.h"

//2018/05/16( �� ).
	//�֐��uRoboInit�v�ɁuclsPOINTER_GROUP�v�^�̈�����ǉ����A.
	//�ǉ����������o�ϐ�( m_wpResource�Am_wpEffects�Am_wpSound ).
	//�ɃA�h���X�����蓖�Ă鏈����ǉ����܂���.
	
	//�����q�̓s���ケ�̃N���X�́uRender�v�֐���.
	//�uclsSkinMesh�v�N���X�́uModelRender�v�֐���.
	//�uclsD3DXSKINMESH�v�N���X�́uRender�v�֐���.
	//������const��t���܂���.
	
	//Zeromemory�̓|�C���^����\�ꂷ��̂ŏ����������s���܂���.
	//���uclsCharactor�v�ƁuclsObject�v�uclsSkinMesh�v��.

#endif//#ifdef Tahara

class clsRobo : public clsCharactor
{
public:
	enum enWeaponType
	{
		enWeaponLHand,
		enWeaponRHand,

		enWeaponTypeSize
	};

	clsMISSION_MODEL* m_pMesh;

	void RoboInit(clsPOINTER_GROUP* const pPtrGroup, 
		clsROBO_STATUS* const pRobo);

	void ModelUpdate()
	{
		D3DXVECTOR3 vTmp;

		vTmp = GetRotation();
		
		SetRotateHeadParts();
		SetRotateArmParts();
		SetRotateCoreParts();
		SetRotateLegParts();

		vTmp = GetPosition();
		m_pMesh->SetPos(vTmp);

		m_pMesh->SetScale(m_Trans.vScale.x);

		m_pMesh->UpDate();
	}

	void SetRotateHeadParts();
	void SetRotateArmParts();
	void SetRotateCoreParts();
	void SetRotateLegParts();

	void Render( 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye) override
	{
		m_mProj = mProj;
		ModelUpdate();
		m_pMesh->Render(mView, mProj, vLight, vEye);
		UpdatePosfromBone();
		m_pMesh->UpdateColPos();
	}

	void UpdateCollsion();

	bool m_bBoost;//true:�u�[�X�^�[�W�J/false:��W�J.

	//Head�p�[�c���琔�l���擾����ϐ��Ɗ֘A����ϐ�//

	//Core�p�[�c���琔�l���擾����ϐ��Ɗ֘A����ϐ�//
	int m_iEnelgyMax;//�G�l���M�[�ő�e��.
	int m_iEnelgy;//���݃G�l���M�[�c��.
	int m_iEnelgyOutput;//�G�l���M�[�o��(�񕜗�)
	int m_iEnelgyRecoveryPoint;//���݂̃G�l���M�[�񕜗�.
	int m_iBoostFloatRecovery;//�󒆂Ńu�[�X�g�W�J���̃G�l���M�[�g�p��.

	void EnelgyRecovery();
	void SetEnelgyRecoveryAmount();
	bool IsEnelgyRamaining(const int iConsumption);//�G�l���M�[������ʂ𒴂��Ă��邩?true:�����Ă���.
	bool EnelgyConsumption(const int iConsumption);//�G�l���M�[����͂�������.

	float m_fBoostMoveSpeedMax;
	int m_iBoostMoveCost;

	float m_fQuickBoostSpeedMax;
	int m_iQuickBoostEnelgyCost;
	int m_iQuickBoostTopSpeedTime;//�ō�����ۂt���[���l.
	int m_iQuickBoostDecStartTime;//�c�N�C�b�N���ˎ���.

	float m_fQuickTrunSpeedMax;
	float m_fQuickTrunDir;
	int m_iQuickTrunEnelgyCost;
	int m_iQuickTrunTopSpeedTime;//�ō�����ۂt���[���l.
	int m_iQuickTrunDecStartTime;//�c�N�C�b�N���ˎ���.

	int m_iQuickInterbal;//�N�C�b�N�u�[�X�g�̍ĕ��˂܂ł̃t���[��.

	float m_fBoostRisingSpeedMax;//�u�[�X�^�[�����ړ��̍ō���.
	int m_iBoostRisingTopSpeedFrame;//���ɒB����܂ł̃t���[���l.
	float m_fBoostRisingAccele;// = m_fMoveSpeedMax / m_fTopSpeedFrame;
	int m_iBoostRisingyCost;
	float m_fBoostFollRes;//�u�[�X�^�[�W�J���ɗ����鑬�x.

	int m_iActivityLimitTime;//�������E����.
	bool m_bTimeUp;//�������E.

	//Arm�p�[�c���琔�l���擾����ϐ��Ɗ֘A����ϐ�//

	//Leg�p�[�c���琔�l���擾����ϐ��Ɗ֘A����ϐ�//
	float m_fWalktMoveSpeedMax;//Leg�p�[�c����擾������s���x.
	int m_iWalkTopSpeedFrame;//���萫�\����v�Z.
	
	int m_iBoostTopSpeedFrame;//���萫�\����v�Z.

	void Walk();
	void Boost();
	void MoveSwitch();
	void BoostRising();
	void QuickBoost();
	void SetDirQuickBoost(const float fAngle);
	void QuickTurn();
	void SetDirQuickTurn(const float fAngle);

	void ShotLWeapon();
	void ShotRWeapon();
	bool IsLWeaponLock();
	bool IsRWeaponLock();

	virtual void UpdateProduct(clsStage* pStage) override;
	void UpdataQuick();
	void UpdataLimitTime();
	void UpdataBoost();

	void UpdatePosfromBone();

	clsRobo();
	~clsRobo();

	

private:
	void Down() final;

	bool m_bStopComShotL;
	bool m_bStopComShotR;

	D3DXVECTOR3 m_vMoveDirforBoost;

	//�E�r�u�[�X�^�[�G�t�F�N�g.
	std::vector<::Effekseer::Handle> m_v_RHandFrontBoostEfc;
	std::vector<::Effekseer::Handle> m_v_RHandSideBoostEfc;
	std::vector<::Effekseer::Handle> m_v_RHandBackBoostEfc;

	//���r�u�[�X�^�[�G�t�F�N�g.
	std::vector<::Effekseer::Handle> m_v_LHandFrontBoostEfc;
	std::vector<::Effekseer::Handle> m_v_LHandSideBoostEfc;
	std::vector<::Effekseer::Handle> m_v_LHandBackBoostEfc;

	//�R�A�u�[�X�^�[�G�t�F�N�g.
	std::vector<::Effekseer::Handle> m_v_CoreBoostEfc;

	//�r�u�[�X�^�[�G�t�F�N�g.
	std::vector<::Effekseer::Handle> m_v_LegBoostEfc;

	void SetBoostEffect();
	
	void SetRHandFrontBoostEffect();
	void SetRHandSideBoostEffect();
	void SetRHandBackBoostEffect();

	void SetLHandFrontBoostEffect();
	void SetLHandSideBoostEffect();
	void SetLHandBackBoostEffect();

	void SetCoreBoostEffect();

	void SetLegBoostEffect();

	void PlayBoostEfc();
	
	void PlayFrontBoostEfc();
	void PlayRightBoostEfc();
	void PlayLeftBoostEfc();
	void PlayBackBoostEfc();

	void PlayCoreBoostEfc();

	void PlayLegBoostEfc();

	int GetBoostEfcNum(enPARTS PartsNum,const char* strBoostPosition);

	virtual void UpdateLookStartingPos() override;

	enum enAnimNoLeg
	{
		enAnimNoLegWait = 0,
		enAnimNoLegWalkStart,
		enAnimNoLegWalkRight,
		enAnimNoLegWalkLeft,
		enAnimNoLegWalkEndRight,
		enAnimNoLegWalkEndLeft,
		enAnimNoLegBoostStart,
		enAnimNoLegBoost,
		enAnimNoLegBoostEnd,
		enAnimNoLegJumpStart,
		enAnimNoLegJumpUp,
		enAnimNoLegJump,
		enAnimNoLegJumpDown,
		enAnimNoLegJumpEnd,
		enAnimNoLegDown
	};

	enum enAnimNoArm
	{
		enAnimNoArmWait = 0,
		enAnimNoArmWeaponHoldStart,
		enAnimNoArmWeaponHoldAct,
		enAnimNoArmWeaponShot,
		enAnimNoArmWeaponHoldEnd,
		enAnimNoArmWeaponReloadStart,
		enAnimNoArmWeaponReload,
		enAnimNoArmWeaponReloadEnd,
		enAnimNoArmDown,
	};

	enum enAnimNoCore
	{
		enAnimNoCoreWait = 0,
		enAnimNoCoreDown,
		enAnimNoCoreRadiatorOpen,
		enAnimNoCoreRadiatorAct,
		enAnimNoCoreRadiatorClose
	};

	enum enAnimNoHead
	{
		enAnimNoHeadWait = 0,
		enAnimNoHeadDown
	};

	enum enAnimNoWeapon
	{
		enAnimNoWeaponWait = 0,
		enAnimNoWeaponShot,
		enAnimNoWeaponReload
	};

	void AnimUpdate();

	void AnimUpdateLeg();
	void AnimUpdateCore();
	void AnimUpdateHead();
	void AnimUpdateArmL();
	void AnimUpdateArmR();
	void AnimUpdateWeaponL();
	void AnimUpdateWeaponR();

	const bool IsLegPartsAnimBoost();//�r�p�[�c�̃A�j���[�V�������u�[�X�^�[�֌W�Ȃ�true.

	const bool IsArmLPartsAnimReload();//���r�p�[�c�̃A�j���[�V�����������[�h�֌W�Ȃ�true.
	const bool IsArmRPartsAnimReload();//�E�r�p�[�c�̃A�j���[�V�����������[�h�֌W�Ȃ�true.

	void AnimChangeLeg(enAnimNoLeg enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeCore(enAnimNoCore enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeHead(enAnimNoHead enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeArmL(enAnimNoArm enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeArmR(enAnimNoArm enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeWeaponL(enAnimNoWeapon enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeWeaponR(enAnimNoWeapon enChangeAnimNo, double dAnimTime = 0.0);

//�����̏�����������.
#ifdef Tahara
	//�����Ƃ�delete���Ȃ���null���Ă�.
	clsResource*		m_wpResource;
	clsEffects*			m_wpEffects;
	clsSOUND_MANAGER_BASE*	m_wpSound;

	//�����蔻��̃|�C���^.
//	std::shared_ptr< std::vector< D3DXVECTOR3 > > m_spColPoss;

	//���{���f��.
//	std::unique_ptr< clsMISSION_MODEL > m_upMissModel;

#endif//#ifdef Tahara


};


