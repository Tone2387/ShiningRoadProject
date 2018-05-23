//#pragma once

#ifndef CHARACTOR
#define CHARACTOR

#include"Global.h"
#include"DX9Mesh.h"
#include"Object.h"

class clsCharactor : public clsObject
{

public:
	//共通ステータス.
	int HP;//体力.
	int m_MaxHP;//最大体力.

	bool m_bDeadFlg;//死亡フラグ.

	//入力関係.

	float fPushMin;

	//移動関係.
	bool m_bMoving;

	float m_fMoveSpeed;//最終的に加算されるスピード.
	float m_fMoveSpeedMax;//スピードの最大値.

	int m_iTopSpeedFrame;//↑に達するまでのフレーム値.
	float m_fMoveAccele;// = m_fMoveSpeedMax / m_fTopSpeedFrame;
	
	int m_iStopFrame;//完全に停止するまでのフレーム値.
	float m_fMoveDecele;// = m_fMoveSpeed / m_iStopFrame;

	D3DXVECTOR3 m_vMoveDir;

	void Shot();

	//移動関係.
	void Move(const float fAngle, const float fPush);
	bool IsMoveing();
	bool IsMoveControl();
	void SetMoveDir(const float fAngle);
	void MoveControl();
	void Accele(const float fPower);
	void Decele();

	void SetMoveAcceleSpeed(
		float fMoveSpeedMax, 
		int iTopSpeedFrame)//加速.
	{
		m_fMoveSpeedMax = fMoveSpeedMax;
		m_iTopSpeedFrame = iTopSpeedFrame;

		m_fMoveAccele = m_fMoveSpeedMax / m_iTopSpeedFrame;

		SetMoveDecelerationSpeed(m_iTopSpeedFrame);
	}

	void SetMoveDecelerationSpeed(const int iStopFrame)//減速.
	{
		m_iStopFrame = iStopFrame;

		m_fMoveDecele = abs(m_fMoveSpeed) / m_iStopFrame;
	}

	//回転.
	float m_fRotSpd;

	void SetRotationSpeed(const float fSpd);
	void Rotate(const float fAngle, const float fPush);

	void Spin(
		float& fNowYaw,
		const float fTargetYaw,
		const float fTurnSpd,
		const float fTurnStop);

	//空中関係.
	float m_fJumpPower;

	void SetJumpPower(const float fPower)
	{
		m_fJumpPower = fPower;
	}

	void Jump();

	float fJumpPower;

	//当たり判定関係.

	//スフィア.

	//スフィア構造体.
	struct SPHERE
	{
		D3DXVECTOR3 vCenter;//中心.
		float fRadius;	//半径.
	};

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

private:
	LPD3DXMESH m_pMeshForRay;//ﾚｲのためのﾒｯｼｭ用.
};

#endif