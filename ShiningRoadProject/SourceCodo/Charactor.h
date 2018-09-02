//#pragma once

#ifndef CHARACTOR
#define CHARACTOR

#include"Global.h"
#include"Object.h"
#include"Weapon.h"

struct HitState//弾が当たった時に相手側に送る情報のまとめ.
{
	bool bHit;
	int iDamage;
	float fInpuct;
	D3DXVECTOR3 vInpuctDir;

	void Clear();
};

class clsCharactor : public clsObject
{
public:
	//共通ステータス.
	int m_HP;//体力.
	int m_MaxHP;//最大体力.

	bool m_bDeadFlg;//死亡フラグ.

	//入力関係.
	float fPushMin;

	//移動関係.
	bool m_bMoving;
	
	float m_fMoveSpeedMax;//スピードの最大値.

	int m_iTopMoveSpeedFrame;//↑に達するまでのフレーム値.
	float m_fMoveAccele;// = m_fMoveSpeedMax / m_fTopSpeedFrame;
	
	int m_iMoveStopFrame;//完全に停止するまでのフレーム値.
	float m_fMoveDecele;// = m_fMoveSpeed / m_iStopFrame;

	int m_iMoveReverseDirInertia;

	bool m_bRotation;

	float m_fRotSpeed;
	float m_fRotSpeedMax;

	int m_iTopRotSpeedFrame;
	float m_fRotAccele;

	int m_iRotStopFrame;
	float m_fRotDecele;

	float m_fRotDir;

	float m_fLookUpDir;

	float m_fJumpPower;

	bool m_bLockStartingPosXSwitch;
	float m_bLockStartingPosX;

	D3DXVECTOR3 m_vLockRangePos;//ロックオン判定の開始座標.
	D3DXVECTOR3 m_vLockRangeDir;//ロックオン判定が伸びる方向.
	float m_fLockRange;//ロックオン距離.
	float m_fLockCircleRadius;//ロックオン判定の半径.

	std::vector<clsWeapon*> m_v_pWeapons;
	std::vector<D3DXVECTOR3> m_v_vMuzzlePos;
	std::vector<D3DXVECTOR3> m_v_vShotDir;
	std::vector<clsCharactor*> m_v_pEnemys;

	void SetEnemys(std::vector<clsCharactor*> v_pEnemys);//敵の認識.

	int m_iWeaponNum;
	int m_iWeaponNumMax;

	bool Shot();
	bool Reload();

	HitState BulletHit(std::vector<clsObject::SPHERE> v_TargetSphere);
	bool Damage(HitState);//ダメージと衝撃力.

	void WeaponInit(clsPOINTER_GROUP* pPrt, WeaponState* pWeapon,const int iWeaponMax);//pWeaponには配列のポインターを入れてください.
	
	void WeaponUpdate();

	void LockChara();
	bool IsInLockRange();
	void SetLockRangeDir();
	bool IsInLockRange(D3DXVECTOR3 vTargetPos);

	void Lock();
	void LockOut();

	clsCharactor* m_pTargetChara;
	int m_iTargetNo;

	bool m_bRadarWarning;//ロックされてる.

	void CharactorUpdate();

	//移動関係.
	void Move();

	void AddMoveAccele(const float fAngle, const float fPush);//移動加速度加算.
	
	bool IsMoveing();
	bool IsMoveControl();

	void SetMoveDir(const float fAngle);
	void MoveControl();

	void MoveAccele(const float fPower);
	void MoveDecele();

	void SetMoveAcceleSpeed(const float fMoveSpeedMax, const int iTopSpeedFrame);//加速.
	void SetMoveDeceleSpeed(const int iMoveStopFrame);//減速.

	//回転.
	void Rotate();

	void AddRotAccele(const float fAngle, const float fPush);//回転速度加算.

	bool IsRotate();
	bool IsRotControl();

	void RotAccele(const float fPower);
	void RotDecele();

	void SetRotDir(float Angle);

	void SetRotAcceleSpeed(const float fRotSpeedMax, const int iTopRotSpdFrame);
	void SetRotDeceleSpeed(const int iRotStopFrame);

	void Spin(//角度に回転速度の加算を行う.
		float& fNowYaw,
		const float fTargetYaw,
		const float fTurnSpd);

	void LookUp(const float fAngle, const float fPush);//上下視点.

	//ジャンプ.
	
	void SetJumpPower(const float fPower)
	{
		m_fJumpPower = fPower;
	}

	void Jump();

	bool PointIntersect(
		const D3DXVECTOR3 StartPos,	//基準の位置.
		const D3DXVECTOR3 EndPos,		//標的の位置.
		const clsDX9Mesh* pTarget		//障害物の物体.
		);


	clsCharactor();
	virtual ~clsCharactor();

	bool RecoLange(
		const D3DXVECTOR3 CenterPos, 
		const D3DXVECTOR3 TargetPos, 
		const float Range);//円の範囲判定.

protected:
	void ShotSwich(const int iWeaponNum);//複数ある武器から使用する武器を決める.

private:
	bool m_bMoveAcceleOlder;//移動速度加算を行ったかどうか.
	bool m_bRotAcceleOlder;//回転速度加算を行ったかどうか.

};

#endif