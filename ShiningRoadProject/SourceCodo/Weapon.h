#pragma once

#include"Shot.h"

//射撃を行うための情報を持たせるクラス.

struct WeaponState
{
	BulletState  BState;
	ShootingState SState;

	int iReloadTime;
	int iLockSpeed;
	float iLockRange;

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
	void Updata();
	bool Shot(clsObject* pTargetObj = nullptr);//true : 射撃成功,false : 弾がないのでリロード.
	void Reload();

	clsObject** GetBulletObjs(int& iBulletNumMax) const;

private:
	WeaponState m_State;

	clsShot** m_ppBullet;
	int m_iRemainingBullet;

	clsPOINTER_GROUP* m_pPtrGroup;
};

