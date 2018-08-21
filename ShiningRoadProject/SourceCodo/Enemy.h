#pragma once

#include<random>
#include<vector>
#include<string>

#include"Global.h"
#include"Charactor.h"

const float g_fDisStandard = 0.1f;

class clsEnemyBase
{
public:
	TRANSFORM* m_pTrans;

	D3DXVECTOR3 m_vMovePlansDir;
	int m_iMoveCategoryNo;

	virtual void Init(
		LPSTR strEnemyFolderName, 
		clsCharactor* pChara, 
		std::vector<clsCharactor*> v_pEnemys);

	clsEnemyBase();
	virtual ~clsEnemyBase();

protected:
	clsCharactor* m_pChara;
	clsCharactor* m_pTarget;

	std::vector<clsCharactor*> m_v_pEnemys;

	void SearchTarget();
	void SearchNear();
	void SetMoveDir(float& fPush, float& fAngle);
	void SetRotate(float& fPush, float& fAngle);
	void SetLook(float& fPush, float& fAngle);
	bool IsJump();
	bool IsShot();

	struct BaseState
	{
		char strName[STR_BUFF_MAX];
		int iMoveCategoryVisType;
	};

	struct UpdateState
	{
		int iHorDirResult;
		D3DXVECTOR3 vHorMoveDir;
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
		int iVerMoveENLimitParcent;
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

	BaseState m_BaseData;

	MoveData m_MoveData;
	ShotData m_ShotData;
	VisibilityAreaData m_visAreaData;
	UpdateState m_UpdateState;
};

