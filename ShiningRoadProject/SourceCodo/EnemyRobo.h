#pragma once

#include"Enemy.h"
#include"RoboCommand.h"

#define sShot

class clsEnemyRobo : public clsEnemyBase
{
public:
	clsEnemyRobo();
	virtual ~clsEnemyRobo();

	void Action();

	virtual void Init(
		LPSTR strEnemyFolderName,
		clsCharactor* pChara,
		std::vector<clsCharactor*> v_pEnemys);

	bool IsBoostOn();
	bool IsBoostOff();
	
	bool IsQuickTurn();//ターゲット位置の方向が正面から一定以上離れてた場合、クイックターンを使用.
	bool IsQuickBoostApproach();//クイックブーストを使用し、ターゲットとの距離を詰める.
	
	bool IsQuickBoostAvoid();//クイックブーストによる回避.

	bool IsQuickBoostAvoidtoRockTime();
	bool IsQuickBoostAvoidtoDamage();

	bool IsShotR();
	bool IsShotL();

private:
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

	void RoboAIOperation();

	clsRoboCommand* MoveOperation(float& fPower, float& fAngle);
	clsRoboCommand* MoveSwitchOperation();
	clsRoboCommand* RotateOperation(float& fPower, float& fAngle);
	clsRoboCommand* LookOperation(float& fPower, float& fAngle);

	clsRoboCommand* QuickTurnOperation();
	clsRoboCommand* QuickBoostOperation();
	clsRoboCommand* BoostOperation();

	clsRoboCommand* LShotOperation();
	clsRoboCommand* RShotOperation();

	ShotData m_LShotData;
	ShotData m_RShotData;

	struct QuickTrun
	{
		int iRangeDir;
	};

	struct QuickBoostApproach
	{

	};
	
	struct QuickBoostAvoid
	{

	};
};