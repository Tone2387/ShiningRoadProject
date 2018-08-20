#include"EnemyRobo.h"

void clsEnemyRobo::Init(
	LPSTR strEnemyFolderName,
	clsCharactor* pChara,
	std::vector<clsCharactor*> v_pEnemys)
{

}

void clsEnemyRobo::Action()
{

}

bool clsEnemyRobo::IsQuickTurn()//ターゲット位置の方向が正面から一定以上離れてた場合、クイックターンを使用.
{


	return false;
}

bool clsEnemyRobo::IsQuickBoostApproach()//クイックブーストを使用し、ターゲットとの距離を詰める.
{


	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoid()//クイックブーストによる回避.
{


	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoidtoRockTime()
{


	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoidtoDamage()
{


	return false;
}

bool clsEnemyRobo::IsShotR()
{


	return false;
}

bool clsEnemyRobo::IsShotL()
{


	return false;
}

clsRoboCommand* clsEnemyRobo::MoveOperation(float& fPower, float& fAngle)
{


	return nullptr;
}

clsRoboCommand* clsEnemyRobo::MoveSwitchOperation()
{


	return nullptr;
}

clsRoboCommand* clsEnemyRobo::RotateOperation(float& fPower, float& fAngle)
{


	return nullptr;
}

clsRoboCommand* clsEnemyRobo::LookOperation(float& fPower, float& fAngle)
{


	return nullptr;
}

clsRoboCommand* clsEnemyRobo::QuickTurnOperation()
{


	return nullptr;
}

clsRoboCommand* clsEnemyRobo::QuickBoostOperation()
{


	return nullptr;
}

clsRoboCommand* clsEnemyRobo::BoostOperation()
{


	return nullptr;
}

clsRoboCommand* clsEnemyRobo::LShotOperation()
{


	return nullptr;
}

clsRoboCommand* clsEnemyRobo::RShotOperation()
{


	return nullptr;
}

clsEnemyRobo::clsEnemyRobo()
{
}

clsEnemyRobo::~clsEnemyRobo()
{
}