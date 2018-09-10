#include"EnemyRobo.h"

void clsEnemyRobo::Init(
	LPSTR strEnemyFolderName,
	clsRobo* pRobo)
{
	m_pBody = pRobo;
	m_pChara = m_pBody;

	m_UpdateState.iHorDirResult = 0;
	m_UpdateState.vHorMovePos = { 0.0f, 0.0f, 0.0f };

	ShotState SSTmp;
	SSTmp.iShotDisMax = 50;
	SSTmp.iShotDisMin = 1;

	MoveState MSTmp;
	MSTmp.iHorDisRandMax = 500;
	MSTmp.iHorDistance = 100;
	MSTmp.iMoveDir = 0;
	MSTmp.iMoveDirRandMax = 0;
	MSTmp.iVerDistance = 5000;
	MSTmp.iVerDistRandMax = 500;
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
	if (m_pTarget)
	{
		float fVerDis = m_pBody->GetPosition().y - m_pTarget->GetPosition().y;

		const int iHulf = 2;

		float fVerDestDis = m_UpdateState.fVerDis + (m_UpdateState.fVerDis / iHulf);

		if (fVerDis > 0.0f)//敵が上にいる.
		{
			return true;
		}

		if (m_pBody->m_iEnelgy > (m_pBody->m_iEnelgyMax / iHulf))//EN残量が十分にある.
		{
			return true;
		}
	}

	return false;
}

bool clsEnemyRobo::IsBoostOff()
{
	if (m_pTarget)
	{
		float fVerDis = m_pBody->GetPosition().y - m_pTarget->GetPosition().y;

		const int iHulf = 2;

		float fVerDestDis = m_UpdateState.fVerDis + (m_UpdateState.fVerDis / iHulf);

		if (fVerDis < 0.0f)//敵が下にいる.
		{
			if (abs(fVerDis) > abs(fVerDestDis))//敵との高さの差が大きすぎる.
			{
				return true;
			}
		}

		if (m_pBody->m_iEnelgy < (m_pBody->m_iEnelgyMax / iHulf))//EN残量が少ない.
		{
			return true;
		}
	}

	return false;
}

bool clsEnemyRobo::IsQuickTurn(float& fPush, float& fAngle)//ターゲット位置の方向が正面から一定以上離れてた場合、クイックターンを使用.
{
	/*D3DXVECTOR3 vForward = GetVec3Dir(m_pBody->GetRotation().y,g_vDirForward);

	D3DXVECTOR3 vDirTarget = m_pTarget->GetPosition() - m_pBody->GetPosition();

	D3DXVec3Normalize(&vDirTarget, &vDirTarget);

	float fDir = D3DXVec3Dot(&vForward, &vDirTarget);*/

	fPush = 0.0f;
	fAngle = 0.0f;

	if (m_pTarget)
	{
		const float fVecX = m_pTarget->GetPosition().x - m_pBody->GetPosition().x;
		const float fVecZ = m_pTarget->GetPosition().z - m_pBody->GetPosition().z;

		float fRot = atan2f(fVecX, fVecZ) - m_pBody->GetRotation().y;

		int iDirHulf = 180;
		int iDirHFull = 360;

		if (fRot > static_cast<float>D3DXToRadian(iDirHulf))
		{
			fRot -= static_cast<float>D3DXToRadian(iDirHFull);
		}
		else if (fRot < -static_cast<float>D3DXToRadian(iDirHulf))
		{
			fRot += static_cast<float>D3DXToRadian(iDirHFull);
		}

		ObjRollOverGuard(&fRot);

		if (abs(fRot) > D3DXToRadian(150))
		{
			const float fPushFull = 1.0f;

			if (fRot > 0.0f)
			{
				fPush = fPushFull;
			}

			else
			{
				fPush = -fPushFull;
			}

			fAngle = fRot;

			return true;
		}
	}

	return false;
}

bool clsEnemyRobo::IsQuickBoostApproach(float& fPush, float& fAngle)//クイックブーストを使用し、ターゲットとの距離を詰める.
{
	if (m_pTarget)
	{
		float fDis = D3DXVec3Length(&(m_pTarget->GetPosition() - m_pBody->GetPosition()));

		if (fDis > 100.0f)
		{
			return true;
		}
	}

	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoid(float& fPush, float& fAngle)//クイックブーストによる回避.
{
	if (IsQuickBoostAvoidtoDamage(fPush,fAngle))
	{
		return true;
	}

	if (IsQuickBoostAvoidtoRockTime(fPush, fAngle))
	{
		return true;
	}

	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoidtoRockTime(float& fPush, float& fAngle)
{


	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoidtoDamage(float& fPush, float& fAngle)
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
			return m_pComMoveSwitch;
		}
	}

	else
	{
		if (IsBoostOn())
		{
			return m_pComMoveSwitch;
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

clsRoboCommand* clsEnemyRobo::QuickTurnOperation(float& fPower, float& fAngle)
{
	if (IsQuickTurn(fPower, fAngle))
	{
		return m_pComQuickTrun;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::QuickBoostOperation(float& fPower, float& fAngle)
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