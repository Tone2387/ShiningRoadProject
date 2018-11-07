#pragma once

#include"Enemy.h"
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
	};

	enum enQuickTrunFileOrder
	{
		enQuickTrunUpdateTime = 0,
		enQuickTrunDir,
		enQuickTrunENLimit,

		enQuickTrunSize
	};

	enum enAvoidCategory
	{
		enAvoidUpdateTime = 0,
		enAvoidLockTime,
		enAvoidDamage,

		enAvoidSize
	};

	struct QuickBoostAvoid
	{
		int iUpdateTime;
		int iAvoidNum;//回避条件カテゴリナンバー.
		int iLockTimeorDamage;
		int iAvoidDir;
		int iENLimit;

		//以下、毎フレーム更新のための変数.
		int iAvoidDamageUpdateTime;
		int iDamage;
		int iLockTime;
	};

	enum enQuickBoostAvoidFileOrder
	{
		QuickBoostAvoidUpdateTime = 0,
		QuickBoostAvoidAvoidNum,
		QuickBoostAvoidLockTimeorDamage,
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

	bool IsQuickBoostAvoidtoLockTime(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	bool IsQuickBoostAvoidtoDamage(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	void ResetAvoidtoDamage();

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

	std::vector<QuickBoostApproach> m_v_QuickAppState;
	std::vector<QuickTrun> m_v_QuickTrunState;
	std::vector<QuickBoostAvoid> m_v_QuickBoostAvoidState;

	bool m_bENSaving;
};