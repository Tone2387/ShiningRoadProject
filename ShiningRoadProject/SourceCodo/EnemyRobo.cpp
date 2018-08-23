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

bool clsEnemyRobo::IsBoostOn()
{
	float fVerDis = m_pTarget->GetPosition().y - m_pBody->GetPosition().y;

	const int iHulf = 2;

	float fVerDestDis = m_UpdateState.fVerDis + (m_UpdateState.fVerDis / iHulf);

	if (fVerDis > 0.0f)
	{
		if (m_pBody->m_iEnelgy > m_pBody->m_iEnelgyMax / iHulf)
		{
			return true;
		}
	}

	return false;
}

bool clsEnemyRobo::IsBoostOff()
{
	float fVerDis = m_pTarget->GetPosition().y - m_pBody->GetPosition().y;

	const int iHulf = 2;

	float fVerDestDis = m_UpdateState.fVerDis + (m_UpdateState.fVerDis / iHulf);

	if (fVerDis > fVerDestDis)
	{
		return true;
	}

	if (m_pBody->m_iEnelgy < m_pBody->m_iEnelgyMax / iHulf)
	{
		return true;
	}

	return false;
}

bool clsEnemyRobo::IsQuickTurn()//ターゲット位置の方向が正面から一定以上離れてた場合、クイックターンを使用.
{
	/*D3DXVECTOR3 vForward = GetVec3Dir(m_pBody->GetRotation().y,g_vDirForward);

	D3DXVECTOR3 vDirTarget = m_pTarget->GetPosition() - m_pBody->GetPosition();

	D3DXVec3Normalize(&vDirTarget, &vDirTarget);

	float fDir = D3DXVec3Dot(&vForward, &vDirTarget);*/

	const float fVecX = m_pTarget->GetPosition().x - m_pBody->GetPosition().x;
	const float fVecZ = m_pTarget->GetPosition().z - m_pBody->GetPosition().z;

	float fRot = atan2f(fVecX, fVecZ) - m_pBody->GetRotation().y;

	ObjRollOverGuard(&fRot);

	if (fRot > D3DXToRadian(90))
	{
		return true;
	}

	return false;
}

bool clsEnemyRobo::IsQuickBoostApproach()//クイックブーストを使用し、ターゲットとの距離を詰める.
{
	float fDis = D3DXVec3Length(&(m_pTarget->GetPosition() - m_pBody->GetPosition()));

	if (fDis > 100.0f)
	{
		return true;
	}

	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoid()//クイックブーストによる回避.
{
	if (IsQuickBoostAvoidtoDamage())
	{
		return true;
	}

	if (IsQuickBoostAvoidtoRockTime())
	{
		return true;
	}

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

bool clsEnemyRobo::IsShotL()
{
	m_ShotData = m_LShotData;

	return IsShot();
}

bool clsEnemyRobo::IsShotR()
{
	m_ShotData = m_RShotData;

	return IsShot();
}

clsRoboCommand* clsEnemyRobo::MoveOperation(float& fPush, float& fAngle)
{
	SetMoveDir(fPush, fAngle);

	if (fPush > g_fPushHulf)
	{
		return m_pComMove;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::MoveSwitchOperation()
{
	

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::RotateOperation(float& fPush, float& fAngle)
{
	SetRotate(fPush, fAngle);

	if (fPush > g_fPushHulf)
	{
		return m_pComRotate;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::LookOperation(float& fPush, float& fAngle)
{
	SetLook(fPush, fAngle);

	if (fPush > g_fPushHulf)
	{
		return m_pComLook;
	}

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
	if (IsJump())
	{
		return m_pComBoost;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::LShotOperation()
{
	if (IsShotL())
	{
		return m_pComLShot;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::RShotOperation()
{
	if (IsShotR())
	{
		return m_pComRShot;
	}

	return nullptr;
}

clsEnemyRobo::clsEnemyRobo()
{

}

clsEnemyRobo::~clsEnemyRobo()
{

}