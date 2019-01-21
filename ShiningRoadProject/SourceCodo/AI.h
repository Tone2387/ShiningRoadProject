#pragma once

#include<random>
#include<vector>
#include<string>

#include"Global.h"
#include"Charactor.h"

class clsEnemyBase
{
public:
	//clsEnemyBase(std::vector<clsCharactor*>& v_Enemys);
	clsEnemyBase();
	virtual ~clsEnemyBase();

	void Update();
	void SetData(std::string strEnemyFolderName);
	void SearchTarget(std::vector<clsCharactor*>);

protected:
	TRANSFORM* m_pTrans;

	clsCharactor* m_pChara;
	clsCharactor* m_pTarget;

	std::vector<clsCharactor*>* m_vp_pEnemys;
	
	enum enTargetEvaluationType
	{
		enTargetEvaluationTypeNear = 0,

		enTargetEvaluationTypeSize
	};

	enum enMoveSwitchType
	{
		enMoveSwitchTypeOrder = 0,
		enMoveSwitchTypeRandam,

		enMoveSwitchTypeSize
	};

	struct BaseState
	{
		std::string strEnemyFolderName;

		int iMoveSwitchType;//移動ステータスを切り替える方法.
		int iProcFrame;//視点調整を更新する時間.
		int iTargetEvaluationType;//ターゲット評価基準.
	};

	enum enBaseStateFileOrder
	{
		enBaseStateMoveSwichType,
		enBaseStateProcFrame,
		enBaseStateTargetEvaluationType,

		enBaseStateSize
	};

	struct MoveState
	{
		int iMoveUpdateInterval;
		int iHorDistance;
		int iHorDisRandMax;
		int iMoveDir;
		int iMoveDirRandMax;
		int iVerDistance;
		int iVerDistRandMax;
	};

	enum enMoveStateFileOrder
	{
		enMoveStateMoveUpdateInterval,
		enMoveStateHorDistance,
		enMoveStateHorDisRandMax,
		enMoveStateMoveDir,
		enMoveStateMoveDirRandMax,
		enMoveStateVerDistance,
		enMoveStateVerDistRandMax,

		enMoveStateSize
	};

	struct ShotState
	{
		int iShotDisMin;
		int iShotDisMax;
		int iShotENLimitParcent;
	};

	enum enShotStateFileOrder
	{
		enShotStateShotDisMin,
		enShotStateShotDisMax,
		enShotStateShotENLimitParcent,
		
		enShotStateSize
	};

	enum enVisibilityType
	{
		enVisibilityTypeNormal = 0,//壁に阻まれると視認できない.
		enVisibilityTypeOutlook,//壁に阻まれない.
		enVisibilityTypeAll,//無条件で認識.

		enVisibilityTypeSize
	};

	struct VisibilityAreaState
	{
		int iVisType;
		int iVisDistance;
		int iVisAngle;
	};

	enum enVisibilityAreaStateFileOrder
	{
		enVisibilityAreaVisType,
		enVisibilityAreaVisDistance,
		enVisibilityAreaVisAngle,
		
		enVisibilityAreaStateSize
	};

	BaseState m_BaseState;
	std::vector<MoveState> m_v_MoveState;
	std::vector<ShotState> m_v_ShotState;
	std::vector<VisibilityAreaState> m_v_VisAreaState;

	struct UpdateState
	{
		int iHorDirResult;
		D3DXVECTOR3 vHorMovePos;
		float fMoveDir;

		float fRotDir;
		float fVerLookDir;

		float fVerDis;

		int iMoveCategoryNo;//現在の移動.

		int iProcessCnt;//処理更新時間.
		int iMoveUpdateCnt;//移動ステータス更新間隔.
	};

	UpdateState m_UpdateState;

	void SetBaseData(std::string strEnemyFolderName);
	void SetMoveData();
	void SetVisibilityData();
	virtual void SetShotData();
	virtual void SetDataProduct();

	virtual void UpdateProduct();

	bool SetMoveDir(float& fPush, float& fAngle);
	void SwitchMoveState();
	bool SetRotate(float& fPush, float& fAngle);
	bool SetLook(float& fPush, float& fAngle);

	bool IsJump();
	bool IsShot();

	bool IsVisibilityArea(const D3DXVECTOR3& vEnemyPos, const VisibilityAreaState& VisAreaState);

	//ターゲット評価関数.
	bool IsTargetNear(const D3DXVECTOR3& vEnemyPos);
};

