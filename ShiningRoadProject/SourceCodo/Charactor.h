//#pragma once

#ifndef CHARACTOR
#define CHARACTOR

#include"Global.h"
#include"Object.h"
#include"Shot.h"

class clsCharactor : public clsObject
{

	struct WeaponState
	{
		ShotState BulletState;//弾に持たせる情報.
		ShotFormState ShootingState;//射撃時に必要な情報.

		int iShotNumMax;//装弾数.
		int iRemainingShotNumNow;//残弾数.
	};

public:
	//共通ステータス.
	int HP;//体力.
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

	float m_fJumpPower;

	virtual void Shot();
	void ShotInit();
	void Reload();

	
	int m_iWeaponNum;

	//移動関係.
	void Move(const float fAngle, const float fPush);
	
	bool IsMoveing();
	bool IsMoveControl();

	void SetMoveDir(const float fAngle);
	void MoveControl();

	void MoveAccele(const float fPower);
	void MoveDecele();

	void SetMoveAcceleSpeed(const float fMoveSpeedMax, const int iTopSpeedFrame);//加速.
	void SetMoveDeceleSpeed(const int iMoveStopFrame);//減速.
	

	//回転.
	void Rotate(const float fAngle, const float fPush);
	bool IsRotate();
	bool IsRotControl();
	void RotAccele(const float fPower);
	void RotDecele();
	void SetRotAcceleSpeed(const float fRotSpeedMax, const int iTopRotSpdFrame);
	void SetRotDeceleSpeed(const int iRotStopFrame);
	void SetRotDir(float Angle);
	
	void SetRotationSpeed(const float fSpd);

	void Spin(
		float& fNowYaw,
		const float fTargetYaw,
		const float fTurnSpd,
		const float fTurnStop);

	//空中関係.
	

	void SetJumpPower(const float fPower)
	{
		m_fJumpPower = fPower;
	}

	void Jump();

	//当たり判定関係.

	//スフィア.

	

	bool PointIntersect(
		const D3DXVECTOR3 StartPos,	//基準の位置.
		const D3DXVECTOR3 EndPos,		//標的の位置.
		const clsDX9Mesh* pTarget		//障害物の物体.
		);
	
	//ﾚｲのﾋｯﾄ判定の長さ.
	float RaySpece;

	clsCharactor();
	virtual ~clsCharactor();

	bool RecoLange(
		const D3DXVECTOR3 CenterPos, 
		const D3DXVECTOR3 TargetPos, 
		const float Range);//円の範囲判定.
protected:

private:
	LPD3DXMESH m_pMeshForRay;//ﾚｲのためのﾒｯｼｭ用.
};

#endif