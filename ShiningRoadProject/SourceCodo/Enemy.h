#pragma once

#include<random>
#include<vector>
#include<string>

#include"Global.h"
#include"Charactor.h"

const float g_fDisStandard = 0.1f;
const float g_fPushHulf = 0.5f;

class clsEnemyBase
{
public:
	clsEnemyBase();
	virtual ~clsEnemyBase();

	void Update();
	void SearchTarget(std::vector<clsCharactor*>);

protected:
	TRANSFORM* m_pTrans;

	clsCharactor* m_pChara;
	clsCharactor* m_pTarget;
	
	void SearchNear();

	bool SetMoveDir(float& fPush, float& fAngle);
	bool SetRotate(float& fPush, float& fAngle);
	bool SetLook(float& fPush, float& fAngle);

	bool IsJump();
	bool IsShot();

	struct BaseState
	{
		char strName[STR_BUFF_MAX];
		
		int iMoveCategoryVisType;//移動ステータスを切り替える方法.
		
		int iProcFrame;//
	};

	struct UpdateState
	{
		int iHorDirResult;
		D3DXVECTOR3 vHorMovePos;
		float fMoveDir;

		float fRotDir;
		float fVerLookDir;

		float fVerDis;
	};

	struct MoveState
	{
		int iHorDistance;
		int iHorDisRandMax;
		int iMoveDir;
		int iMoveDirRandMax;
		int iVerDistance;
		int iVerDistRandMax;
		int iActUpDateInterval;
	};

	struct ShotState
	{
		int iShotDisMin;
		int iShotDisMax;
		int iShotENLimitParcent;
	};

	struct VisibilityAreaState
	{
		int iVisType;
		int iVisDistance;
		int iVisAngle;
	};

	struct MoveData
	{
		int iCategory;
		std::vector<MoveState> v_MoveState;
	};

	struct ShotData
	{
		int iCategory;
		std::vector<ShotState> v_ShotState;
	};

	struct VisibilityAreaData
	{
		int iCategory;
		std::vector<VisibilityAreaState> v_VisAreaState;
	};

	int m_iProcessFrame;//処理更新時間.
	int m_iMoveCategoryNo;//現在の移動.

	BaseState m_BaseData;

	MoveData m_MoveData;
	ShotData m_ShotData;
	VisibilityAreaData m_visAreaData;
	UpdateState m_UpdateState;
};

