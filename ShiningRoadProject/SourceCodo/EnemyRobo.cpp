#include"EnemyRobo.h"

void clsEnemyRobo::Init(
	LPSTR strEnemyFolderName,
	clsRobo* pRobo,
	std::vector<clsCharactor*> v_pEnemys)
{
	m_pBody = pRobo;
	m_pChara = m_pBody;

	m_v_pEnemys = v_pEnemys;

	m_UpdateState.iHorDirResult = 0;
	m_UpdateState.vHorMoveDir = { 0.0f, 0.0f, 0.0f };

	ShotState SSTmp;
	SSTmp.iShotDisMax = 50.0f;
	SSTmp.iShotDisMin = 1.0f;

	MoveState MSTmp;
	MSTmp.iHorDisRandMax = 500;
	MSTmp.iHorDistance = 100;
	MSTmp.iMoveDir = 30;
	MSTmp.iMoveDirRandMax = 10;
	MSTmp.iVerDistance = 100;
	MSTmp.iVerDistRandMax = 200;
	MSTmp.iVerMoveENLimitParcent = 50;

	m_LShotData.iCategory = 1;
	m_LShotData.v_ShotState.push_back(SSTmp);

	m_RShotData.iCategory = 1;
	m_RShotData.v_ShotState.push_back(SSTmp);

	m_MoveData.iCategory = 1;
	m_MoveData.v_MoveState.push_back(MSTmp);
}

bool clsEnemyRobo::IsBoostOn()
{
	float fVerDis = m_pTarget->GetPosition().y - m_pBody->GetPosition().y;

	const int iHulf = 2;

	float fVerDestDis = m_UpdateState.fVerDis + (m_UpdateState.fVerDis / iHulf);

	if (fVerDis > 0.0f)
	{
		return true;
	}

	if (m_pBody->m_iEnelgy > (m_pBody->m_iEnelgyMax / iHulf))
	{
		return true;
	}

	return false;
}

bool clsEnemyRobo::IsBoostOff()
{
	float fVerDis = m_pTarget->GetPosition().y - m_pBody->GetPosition().y;

	const int iHulf = 2;

	float fVerDestDis = m_UpdateState.fVerDis + (m_UpdateState.fVerDis / iHulf);

	if (fVerDis < 0.0f)
	{
		if (fVerDis < fVerDestDis)
		{
			return true;
		}
	}

	if (m_pBody->m_iEnelgy < (m_pBody->m_iEnelgyMax / iHulf))
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

bool clsEnemyRobo::IsShotR()
{
	m_ShotData = m_RShotData;

	if (IsShot())
	{
		return true;
	}

	return false;
}

bool clsEnemyRobo::IsShotL()
{
	m_ShotData = m_LShotData;

	if (IsShot())
	{
		return true;
	}

	return false;
}

clsRoboCommand* clsEnemyRobo::MoveOperation(float& fPower, float& fAngle)
{
	if (SetMoveDir(fPower, fAngle))
	{
		return m_pComMove;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::MoveSwitchOperation()
{
	if (m_pBody->m_bBoost)
	{
		if (IsBoostOff())
		{
			return m_pComBoost;
		}
	}

	else
	{
		if (IsBoostOn())
		{
			return m_pComBoost;
		}
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::RotateOperation(float& fPower, float& fAngle)
{
	if (SetRotate(fPower,fAngle))
	{
		return m_pComRotate;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::LookOperation(float& fPower, float& fAngle)
{
	if (SetLook(fPower, fAngle))
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
	m_pComMove = new clsCommandMove;
	m_pComMoveSwitch = new clsCommandMoveSwitch;
	m_pComRotate = new clsCommandRotate;
	m_pComLook = new clsCommandLookVerMove;

	m_pComQuickBoost = new clsCommandQuickBoost;
	m_pComQuickTrun = new clsCommandQuickTurn;
	m_pComBoost = new clsCommandBoostRising;

	m_pComLShot = new clsCommandLShot;
	m_pComRShot = new clsCommandRShot;
}

clsEnemyRobo::~clsEnemyRobo()
{
	delete m_pComMove;
	delete m_pComMoveSwitch;
	delete m_pComRotate;
	delete m_pComLook;
	
	delete m_pComQuickBoost;
	delete m_pComQuickTrun;
	delete m_pComBoost;
	
	delete m_pComLShot;
	delete m_pComRShot;
}