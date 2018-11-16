#pragma once

#include"AI.h"
#include"RoboCommand.h"

#define sShot

class clsEnemyRobo : public clsEnemyBase
{
public:
	clsEnemyRobo();
	virtual ~clsEnemyRobo();

	void Init(
		LPSTR strEnemyFolderName,
		clsRobo* pRobo);

	clsRoboCommand* MoveOperation(float& fPower, float& fAngle);
	clsRoboCommand* MoveSwitchOperation();
	clsRoboCommand* RotateOperation(float& fPower, float& fAngle);
	clsRoboCommand* LookOperation(float& fPower, float& fAngle);

	clsRoboCommand* QuickTurnOperation(float& fPower, float& fAngle);
	clsRoboCommand* QuickBoostOperation(float& fPower, float& fAngle);
	clsRoboCommand* BoostOperation();

	clsRoboCommand* LShotOperation();
	clsRoboCommand* RShotOperation();
	
private:
	struct Boost
	{
		int iRisingENParcent;//上昇開始していいEN残量率.
		int iENLimitParcent;//上昇をやめるEN残量率.
	};

	enum enBoostFileOrder
	{
		enBoostRisingENParcent = 0,
		enBoostENLimitParcent,

		enBoostSize
	};

	struct QuickBoostApproach
	{
		int iUpdateTime;
		int iDis;
		int iENLimit;

		//以下、毎フレーム更新のための変数.
		int iUpdateCnt;
	};

	enum enQuickBoostApproachFileOrder
	{
		enQuickBoostAppUpdateTime = 0,
		enQuickBoostAppDis,
		enQuickBoostAppENLimit,

		enQuickBoostAppSize
	};

	struct QuickTrun
	{
		int iUpdateTime;
		int iDir;
		int iENLimit;

		//以下、毎フレーム更新のための変数.
		int iUpdateCnt;
	};

	enum enQuickTrunFileOrder
	{
		enQuickTrunUpdateTime = 0,
		enQuickTrunDir,
		enQuickTrunENLimit,

		enQuickTrunSize
	};

	enum enQuickBoostAvoidType
	{
		enQuickBoostAvoidTypeLookTime = 0,//被ロックオン時間による実行.
		enQuickBoostAvoidTypeLookTimePriorityUpdateTime,//実行間隔を優先.
		enQuickBoostAvoidTypeDamage,//被ダメージによる実行.
		enQuickBoostAvoidTypeDamagePriorityUpdateTime,//実行間隔を優先.

		enQuickBoostAvoidTypeSize
	};

	struct QuickBoostAvoid
	{
		int iUpdateTime;//更新間隔or実行間隔.
		int iAvoidType;//回避条件カテゴリナンバー.
		int iConditions;//回避条件の許容値.
		int iAvoidDir;//回避する方向.
		int iENLimit;//回避時EN消費

		//以下、毎フレーム更新のための変数.
		int iUpdateCnt;//更新間隔or実行間隔カウント.
		int iConditionsCnt;//条件の蓄積.
	};

	enum enQuickBoostAvoidFileOrder
	{
		enQuickBoostAvoidUpdateTime = 0,
		enQuickBoostAvoidType,
		enQuickBoostAvoidLockTimeorDamage,
		enQuickBoostAvoidDir,
		enQuickBoostAvoidENLimit,

		enQuickBoostAvoidSize
	};

	clsRobo* m_pBody;

	clsRoboCommand* m_pComMove;
	clsRoboCommand* m_pComMoveSwitch;
	clsRoboCommand* m_pComRotate;
	clsRoboCommand* m_pComLook;

	clsRoboCommand* m_pComQuickBoost;
	clsRoboCommand* m_pComQuickTrun;
	clsRoboCommand* m_pComBoost;

	clsRoboCommand* m_pComLShot;
	clsRoboCommand* m_pComRShot;

	void IsENSaving();//ブースト上昇を行っていいほどENが回復したかを判定.
	bool IsBoostRising();

	bool IsBoostOn();
	bool IsBoostOff();

	bool IsQuickTurn(float& fPush, float& fAngle);//ターゲット位置の方向が正面から一定以上離れてた場合、クイックターンを使用.
	bool IsQuickBoostApproach(float& fPush, float& fAngle);//クイックブーストを使用し、ターゲットとの距離を詰める.

	bool IsQuickBoostAvoid(float& fPush, float& fAngle);//クイックブーストによる回避.

	//回避条件判定関数群.
	//被ロックオン時間(回避条件優先)
	bool IsQuickBoostAvoidLockTime(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	//被ロックオン時間(実行間隔優先)
	bool IsQuickBoostAvoidLockTimePriorityUpdateTime(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	//被ダメージ量(回避条件優先)
	bool IsQuickBoostAvoidDamage(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	//被ダメージ量(実行間隔優先)
	bool IsQuickBoostAvoidDamagePriorityUpdateTime(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	//void ResetAvoidtoDamage();

	bool IsShotR();
	bool IsShotL();

	virtual void SetDataProduct();
	virtual void SetShotData();

	void SetBoostData();

	void SetQuickBoostAppData();
	void SetQuickTrunData();
	void SetQuickBoostAvoidData();

	std::vector<ShotState> m_v_LShotState;
	std::vector<ShotState> m_v_RShotState;

	Boost m_BoostState;

	std::vector<QuickBoostApproach> m_v_QuickBoostAppState;
	std::vector<QuickTrun> m_v_QuickTrunState;
	std::vector<QuickBoostAvoid> m_v_QuickBoostAvoidState;

	bool m_bENSaving;
};