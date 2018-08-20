#pragma once

#include"Enemy.h"
#include"CharactorCommand.h"

class clsCharactorEnemy : public clsEnemyBase
{
public:
	clsCharactorEnemy();
	~clsCharactorEnemy();

	clsCharactorCommand* MoveOperation(float& fPower, float& fAngle);
	clsCharactorCommand* RotateOperation(float& fPower, float& fAngle);
	clsCharactorCommand* LookOperation(float& fPower, float& fAngle);

	clsCharactorCommand* JumpOperation();
	clsCharactorCommand* ShotOperation();

private:
	clsCharactorCommand* m_pComMove;
	clsCharactorCommand* m_pComRotate;
	clsCharactorCommand* m_pComLook;
		
	clsCharactorCommand* m_pComShot;
	clsCharactorCommand* m_pComJump;
};

