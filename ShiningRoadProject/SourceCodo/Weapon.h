#pragma once

#include"Shot.h"

//�ˌ����s�����߂̏�����������N���X.

struct WeaponState
{
	BulletState  BState;
	ShootingState SState;

	int iAtk;
	int iReloadTime;
	int iLockSpeed;
	int iShotEN;
	
	int iStablity;//Robo��Arm�̏Ə����x�Ƃ̌v�Z���ʂ����Ă�������.

	int iMagazineReloadTime;
	int iBulletNumMax;
};

class clsWeapon
{
public:
	clsWeapon(clsPOINTER_GROUP* pPtrGroup);
	~clsWeapon();

	void Create(const WeaponState& State);
	void Update(clsStage* const pStage);
	bool Shot();

	int Hit(std::vector<clsObject::SPHERE>& v_TargetSphere);
	
	void Reload();

	bool IsNeedReload();//�c�e�����邩�Ŏˌ��������[�h���𔻒�.

	bool IsLock();

	void Lock(clsObject* pTargetObj = nullptr);//���b�N�͈͂ɓ����Ă��邩�m�F.
	void LockOut();//���b�N�������.

	int GetNowBulletNum();
	int GetMaxBulletNum();

	int GetShotEN() const;

private:
	WeaponState m_State;

	::Effekseer::Handle m_MuzzleEfc;
	::Effekseer::Handle m_AmmoEfc;

	clsObject* m_pTargetObj;

	clsShot** m_ppBullet;
	int m_iRemainingBullet;

	int m_iReloadCnt;
	int m_iMagazineReloadCnt;
	int m_iLockTime;

	bool m_bNeedReload;

	clsPOINTER_GROUP* m_pPtrGroup;
};

