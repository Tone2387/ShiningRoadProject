#include"EnemyCharactor.h"

clsCharactorCommand* clsCharactorEnemy::MoveOperation(float& fPush, float& fAngle)
{
	SetMoveDir(fPush, fAngle);

	if (fPush < g_fPushHulf)
	{
		return nullptr;
	}

	return m_pComMove;
}

clsCharactorCommand* clsCharactorEnemy::RotateOperation(float& fPush, float& fAngle)
{
	SetRotate(fPush, fAngle);

	if (fPush < g_fPushHulf)
	{
		return nullptr;
	}

	return m_pComRotate;
}

clsCharactorCommand* clsCharactorEnemy::LookOperation(float& fPush, float& fAngle)
{
	SetLook(fPush, fAngle);

	if (fPush < g_fPushHulf)
	{
		return nullptr;
	}

	return m_pComLook;
}

clsCharactorCommand* clsCharactorEnemy::JumpOperation()
{
	if (!IsJump())
	{
		return nullptr;
	}

	return m_pComJump;
}

clsCharactorCommand* clsCharactorEnemy::ShotOperation()
{
	if (!IsShot())
	{
		return nullptr;
	}

	return m_pComShot;
}

clsCharactorEnemy::clsCharactorEnemy()
{
	m_pComMove = new clsCommandMove;
	m_pComRotate = new clsCommandRotation;
	m_pComLook = new clsCommandLookUp;
	m_pComJump = new clsCommandJump;
	m_pComShot = new clsCommandShot;
}

clsCharactorEnemy::~clsCharactorEnemy()
{
	delete m_pComMove;
	delete m_pComRotate;
	delete m_pComLook;
	delete m_pComJump;
	delete m_pComShot;
}