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
	struct QuickBoostApproach
	{
		int iDis;
	};

	struct QuickTrun
	{
		int iDir;
	};

	enum enAvoidCategory
	{
		enAvoidLockTime = 0,
		enAvoidDamage,

		enAvoidSize
	};

	struct QuickBoostAvoid
	{
		enAvoidCategory iAvoidNum;//回避条件カテゴリナンバー.
		int iUpdateTime;
		int iAvoidDir;
		int iLockTimeorDamage;
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

	int m_iAvoidDamageUpdateTime;
	int m_iDamage;
	int m_iLockTime;

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

	ShotData m_LShotData;
	ShotData m_RShotData;

	

	std::vector<QuickBoostApproach> m_v_QuickAppState;
	std::vector<QuickTrun> m_v_QuickTrunState;
	std::vector<QuickBoostAvoid> m_v_QuickAvoidState;
};