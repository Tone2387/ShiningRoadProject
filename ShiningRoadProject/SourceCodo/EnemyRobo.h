#pragma once

#include"Enemy.h"
#include"Robo.h"

#define sShot

class clsEnemyRobo : public clsRobo
{
public:
	clsEnemyRobo();
	virtual ~clsEnemyRobo();

	void Action();
	
	bool IsQuickTurn();//ターゲット位置の方向が正面から一定以上離れてた場合、クイックターンを使用.
	bool IsQuickBoostApproach();//クイックブーストを使用し、ターゲットとの距離を詰める.
	
	bool IsQuickBoostAvoid();//クイックブーストによる回避.

	bool IsQuickBoostAvoidtoRockTime();
	bool IsQuickBoostAvoidtoDamage();

	bool IsShotR();
	bool IsShotL();



private:

};

clsEnemyRobo::clsEnemyRobo()
{
}

clsEnemyRobo::~clsEnemyRobo()
{
}