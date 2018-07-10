#pragma once

#include<random>

#include"Global.h"
#include"Object.h"

const float g_fDisStandard = 0.1f;

class clsEnemyBase
{
public:
	TRANSFORM* m_pTrans;

	D3DXVECTOR3 m_vMovePlansDir;
	int m_iMoveCategoryNo;

	void Init(LPSTR strEnemyFolderName);
	//virtual void Update();

	void SearchTarget(clsObject* pObj);
	void SearchNear(clsObject* pObj);
	void SetMoveDir();
	bool IsShot();

	clsEnemyBase();
	~clsEnemyBase();

private:
	clsObject* m_pTarget;

public:
	struct EnemyBaseState
	{
		char strName[256];
		int iMoveCategoryVisType;
	};

	struct EnemyUpdateState
	{
		D3DXVECTOR3 vHorMoveDir;
		float fVerDis;
	};

	struct EnemyMoveState
	{
		int iHorDistance;
		int iHorDisRandMax;
		int iMoveDir;
		int iMoveDirRandMax;
		int iVerDistance;
		int iVerDistRandMax;
		int iVerMoveENLimitParcent;
		int iActUpDateInterval;
	};

	struct EnemyShotState
	{
		int iShotDisMin;
		int iShotDisMax;
		int iShotENLimitParcent;
	};

	struct EnemyVisibilityAreaState
	{
		int iVisType;
		int iVisDistance;
		int iVisAngle;
	};

	struct EnemyMoveData
	{
		int iCategory;
		EnemyMoveState** MoveState;
	};

	struct EnemyShotData
	{
		int iCategory;
		EnemyShotState** ShotState;

		~EnemyShotData()
		{
			for (int i = 0; i < iCategory; i++)
			{
				if (ShotState[iCategory])
				{
					delete ShotState[iCategory];
					ShotState[iCategory] = nullptr;
				}
			}

			if (ShotState)
			{
				delete[] ShotState;
				ShotState = nullptr;
			}
		};
	};

	struct EnemyVisibilityAreaData
	{
		int iCategory;
		EnemyVisibilityAreaState** VisAreaState;
	};

	EnemyBaseState m_BaseData;

	EnemyMoveData m_MoveData;
	EnemyShotData m_ShotData;
	EnemyVisibilityAreaData m_visAreaData;
	EnemyUpdateState m_UpdateState;
};

