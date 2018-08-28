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

	bool IsBoostOn();
	bool IsBoostOff();

	bool IsQuickTurn(float& fPush, float& fAngle);//ターゲット位置の方向が正面から一定以上離れてた場合、クイックターンを使用.
	bool IsQuickBoostApproach(float& fPush, float& fAngle);//クイックブーストを使用し、ターゲットとの距離を詰める.

	bool IsQuickBoostAvoid(float& fPush, float& fAngle);//クイックブーストによる回避.

	bool IsQuickBoostAvoidtoRockTime(float& fPush, float& fAngle);
	bool IsQuickBoostAvoidtoDamage(float& fPush, float& fAngle);

	bool IsShotR();
	bool IsShotL();

	ShotData m_LShotData;
	ShotData m_RShotData;

	struct QuickTrun
	{

	};
};