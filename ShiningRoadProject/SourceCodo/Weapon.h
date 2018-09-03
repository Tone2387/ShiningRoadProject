#pragma once

#include"Shot.h"

//射撃を行うための情報を持たせるクラス.

struct WeaponState
{
	BulletState  BState;
	ShootingState SState;

	int iAtk;
	int iReloadTime;
	int iLockSpeed;
	int iShotEN;
	
	int iStablity;//RoboはArmの照準精度との計算結果を入れてください.

	int MagazineReloadTime;
	int iBulletNumMax;
};

class clsWeapon
{
public:
	clsWeapon(clsPOINTER_GROUP* pPtrGroup);
	~clsWeapon();

	void Create(WeaponState State);
	void Update();
	bool Shot();

	int Hit(std::vector<clsObject::SPHERE> v_TargetSphere);
	
	void Reload();

	bool IsNeedReload();//残弾があるかで射撃かリロードかを判定.

	bool IsLock();

	void Lock(clsObject* pTargetObj = nullptr);//ロック範囲に入っているか確認.
	void LockOut();//ロック判定解除.

	int GetNowBulletNum();
	int GetMaxBulletNum();

	int GetShotEN() const;

private:
	WeaponState m_State;

	clsObject* m_pTargetObj;

	clsShot** m_ppBullet;
	int m_iRemainingBullet;

	int m_iReloadCnt;
	int m_iLockTime;

	clsPOINTER_GROUP* m_pPtrGroup;
};

