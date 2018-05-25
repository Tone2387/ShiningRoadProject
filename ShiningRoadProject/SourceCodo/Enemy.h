#pragma once

#include"Global.h"

const float g_fDisStandard = 0.1f;
const float g_fPercentage = 0.01f;

class clsEnemyBase
{
public:


	clsEnemyBase();
	~clsEnemyBase();

private:

public:
	struct EnemyUpdateState
	{
		D3DXVECTOR3 vTargetPos;
		float fTargetYaw;

		D3DXVECTOR3 vTargetMoveDir;
		float fTargetMoveSpeed;

		float fVerPower;
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
};

