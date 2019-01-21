#include"AI.h"
#include"File.h"

/*clsEnemyBase::clsEnemyBase(std::vector<clsCharactor*>& v_Enemys)
{
	m_vp_pEnemys = &v_Enemys;
}*/

clsEnemyBase::clsEnemyBase()
{
	
}

clsEnemyBase::~clsEnemyBase()
{
	if (m_pTarget)
	{
		m_pTarget = nullptr;
	}

	if (m_vp_pEnemys)
	{
		m_vp_pEnemys = nullptr;
	}
}

void clsEnemyBase::Update()
{
	if (m_UpdateState.iMoveUpdateCnt >= 0)
	{
		m_UpdateState.iMoveUpdateCnt--;
	}

	if (m_UpdateState.iProcessCnt >= 0)
	{
		m_UpdateState.iProcessCnt--;
	}

	UpdateProduct();
}

void clsEnemyBase::SearchTarget(std::vector<clsCharactor*> v_pEnemys)
{
	m_pTarget = nullptr;

	std::vector<clsCharactor*> v_pEnemysTmp = v_pEnemys;//*m_vp_pEnemys;

	for (int i = 0; i < v_pEnemysTmp.size(); i++)
	{
		if (v_pEnemysTmp[i]->m_bDeadFlg)
		{
			continue;
		}

		const D3DXVECTOR3 vEnemyPos = v_pEnemysTmp[i]->GetCenterPos();

		for (int j = 0; j < m_v_VisAreaState.size(); j++)
		{
			switch (m_v_VisAreaState[i].iVisType)
			{
			case clsEnemyBase::enVisibilityTypeNormal:
				if (!IsVisibilityArea(vEnemyPos, m_v_VisAreaState[j]))
				{
					continue;//”FŽ¯ƒ^ƒCƒv‚Ì”FŽ¯ðŒ‚ð–ž‚½‚µ‚Ä‚È‚¢ê‡‚Í”²‚¯‚é.
				}
				break;
			case clsEnemyBase::enVisibilityTypeOutlook:
				if (!IsVisibilityArea(vEnemyPos, m_v_VisAreaState[j]))
				{
					continue;//”FŽ¯ƒ^ƒCƒv‚Ì”FŽ¯ðŒ‚ð–ž‚½‚µ‚Ä‚È‚¢ê‡‚Í”²‚¯‚é.
				}
				break;
			}

			if (m_pTarget)
			{
				switch (m_BaseState.iTargetEvaluationType)
				{
				case clsEnemyBase::enTargetEvaluationTypeNear:

					if (!IsTargetNear(vEnemyPos))
					{
						continue;
					}

					break;

				default:
					continue;
					break;
				}
			}

			m_pTarget = v_pEnemysTmp[i];
		}
	}
}

bool clsEnemyBase::IsVisibilityArea(const D3DXVECTOR3 vEnemyPos, const VisibilityAreaState VisAreaState)
{
	D3DXVECTOR3 vDisTmp = vEnemyPos - m_pChara->m_vLockStartingPos;
	float fDis = D3DXVec3Length(&vDisTmp);

	if (fDis < VisAreaState.iVisDistance)
	{
		//”FŽ¯”ÍˆÍ“à.
		return true;
	}

	return false;
}

bool clsEnemyBase::IsTargetNear(const D3DXVECTOR3 vEnemyPos)
{
	if (!m_pTarget)
	{
		//ƒ^[ƒQƒbƒg‚ª‚È‚¢ê‡.
		return true;
	}

	D3DXVECTOR3 vDisTarget = m_pTarget->GetCenterPos() - m_pChara->GetCenterPos();
	D3DXVECTOR3 vDisEnemy = vEnemyPos - m_pChara->GetCenterPos();

	float fDisTarget = D3DXVec3Length(&vDisTarget);
	float fDisEnemy = D3DXVec3Length(&vDisEnemy);

	if (fDisEnemy < fDisTarget)
	{
		return true;
	}

	return false;
}

void clsEnemyBase::SwitchMoveState()
{
	switch (m_BaseState.iMoveSwitchType)
	{
	case clsEnemyBase::enMoveSwitchTypeOrder:
		m_UpdateState.iMoveCategoryNo++;
		break;
	case clsEnemyBase::enMoveSwitchTypeRandam:
		m_UpdateState.iMoveCategoryNo = rand() % m_v_MoveState.size();
		break;
	}

	if (m_UpdateState.iMoveCategoryNo >= m_v_MoveState.size())
	{
		m_UpdateState.iMoveCategoryNo = 0;
	}
}

bool clsEnemyBase::SetMoveDir(float& fPush, float& fAngle)
{
	fPush = 0.0f;
	fAngle = 0.0f;

	SwitchMoveState();

	if (m_pTarget)
	{
		fPush = 1.0f;

		if (m_UpdateState.iMoveUpdateCnt < 0)
		{
			MoveState MoveStatus = m_v_MoveState[m_UpdateState.iMoveCategoryNo];

			m_UpdateState.vHorMovePos = { 0.0f, 0.0f, 0.0f };
			m_UpdateState.fVerDis = 0.0f;

			D3DXVECTOR3 vTmp;

			float fRandMax = 0.0f;

			float fHorDestDis = 0.0f;//Destination:–Ú“I’n.
			fHorDestDis = MoveStatus.iHorDistance * g_fDistanceReference;
			fRandMax = MoveStatus.iHorDisRandMax* g_fDistanceReference;

			if (fRandMax != 0.0f)//0‚¾‚ÆŽ~‚Ü‚é‚Ì‚Å–hŽ~.
			{
				fHorDestDis += fmodf(static_cast<float>(rand()), (fRandMax));//•Ï“®’l‚ª‚ ‚éê‡A—”¶¬.
			}

			int iRandMax = 0;

			m_UpdateState.iHorDirResult = MoveStatus.iMoveDir;

			iRandMax = MoveStatus.iMoveDirRandMax;

			if (iRandMax != 0)//0‚¾‚ÆŽ~‚Ü‚é‚Ì‚Å–hŽ~.
			{
				m_UpdateState.iHorDirResult += (rand() % (iRandMax * 2)) - iRandMax;
			}

			vTmp = m_pChara->GetPosition() - m_pTarget->GetPosition();
			vTmp.y = 0.0f;//‚’¼•ûŒü‚ÍŠÖŒW‚È‚¢‚Ì‚Å0‚É‚·‚é.
			D3DXVec3Normalize(&vTmp, &vTmp);//ƒ^[ƒQƒbƒg‚©‚çŽ©•ª‚Ö‚Ì•ûŒüƒxƒNƒgƒ‹‚ðŽæ“¾.

			m_UpdateState.vHorMovePos = m_pTarget->GetPosition() + vTmp * fHorDestDis;//–Ú“I’n.

			m_UpdateState.vHorMovePos.y = 0.0f;//‚’¼•ûŒü‚ÍŠÖŒW‚È‚¢‚Ì‚Å0‚É‚·‚é.

			const float fVecX = m_UpdateState.vHorMovePos.x - m_pChara->GetPosition().x;
			const float fVecZ = m_UpdateState.vHorMovePos.z - m_pChara->GetPosition().z;

			m_UpdateState.iHorDirResult = atan2f(fVecX, fVecZ) - m_pChara->GetRotation().y;

			m_UpdateState.iMoveUpdateCnt = MoveStatus.iMoveUpdateInterval;
		}
	}

	else
	{
		return false;
	}

	const float fVecX = m_UpdateState.vHorMovePos.x - m_pChara->GetPosition().x;
	const float fVecZ = m_UpdateState.vHorMovePos.z - m_pChara->GetPosition().z;

	float fRot = atan2f(fVecX, fVecZ) - m_pChara->GetRotation().y;

	fRot += static_cast<float>(D3DXToRadian(m_UpdateState.iHorDirResult));

	ObjRollOverGuard(&fRot);

	fAngle = fRot;

	return true;

}

bool clsEnemyBase::SetRotate(float& fPush, float& fAngle)
{
	fPush = 0.0f;
	fAngle = 0.0f;

	if (m_pTarget)
	{
		if (m_UpdateState.iProcessCnt < 0)
		{
			const float fPushFull = 1.0f;

			const float fVecX = m_pTarget->GetPosition().x - m_pChara->GetPosition().x;
			const float fVecZ = m_pTarget->GetPosition().z - m_pChara->GetPosition().z;

			float fRot = atan2f(fVecX, fVecZ) - m_pChara->GetRotation().y;

			ObjRollOverGuard(&fRot);
			if (fRot > 0)
			{
				m_UpdateState.fRotDir = fPushFull;
			}

			else
			{
				m_UpdateState.fRotDir = -fPushFull;
			}

			fPush = m_UpdateState.fRotDir;
			fAngle = fRot;

			return true;
		}
	}

	return false;
}

bool clsEnemyBase::SetLook(float& fPush, float& fAngle)
{
	fPush = 0.0f;
	fAngle = 0.0f;

	if (m_pTarget)
	{
		const float fPushFull = 1.0f;

		const float fVecX = m_pTarget->GetPosition().x - m_pChara->GetPosition().x;
		const float fVecZ = m_pTarget->GetPosition().z - m_pChara->GetPosition().z;
		const float fVecY = m_pTarget->GetPosition().y - m_pChara->GetPosition().y;

		const float fVecXZ = abs(fVecX) + abs(fVecZ);

		float fRot = atan2f(fVecY, fVecXZ);

		fRot = fRot - m_pChara->m_fVerLookDir;

		ObjRollOverGuard(&fRot);

		if (m_UpdateState.iProcessCnt < 0)
		{
			if (fRot > 0)
			{
				m_UpdateState.fVerLookDir = fPushFull;
			}

			else
			{
				m_UpdateState.fVerLookDir = -fPushFull;
			}
		}

		fPush = m_UpdateState.fVerLookDir;
		fAngle = fRot;

		return true;
	}
	
	return false;
}

bool clsEnemyBase::IsJump()
{
	if (m_pTarget)
	{
		MoveState MoveStatus = m_v_MoveState[m_UpdateState.iMoveCategoryNo];

		int iVerDestDis = MoveStatus.iVerDistance;

		int iRandMax = 0;
		iRandMax = MoveStatus.iVerDistRandMax;

		if (iRandMax != 0)//0‚¾‚ÆŽ~‚Ü‚é‚Ì‚Å–hŽ~.
		{
			iVerDestDis += (rand() % (iRandMax * 2)) - iRandMax;
		}

		m_UpdateState.fVerDis = iVerDestDis * g_fDistanceReference;

		float fDist = m_pTarget->GetPosition().y - m_pChara->GetPosition().y;
			
		if (fDist < m_UpdateState.fVerDis)
		{
			return true;
		}
	}

	return false;
}

bool clsEnemyBase::IsShot()
{
	if (m_pTarget)
	{
		float fDis;
		fDis = D3DXVec3Length(&(m_pTarget->GetPosition() - m_pChara->GetPosition()));

		for (int i = 0; i < m_v_ShotState.size(); i++)
		{
			if (fDis <= m_v_ShotState[i].iShotDisMax ||
				fDis >= m_v_ShotState[i].iShotDisMin)
			{
				return true;
			}
		}
	}

	return false;
}

void clsEnemyBase::SetData(std::string strEnemyFolderName)
{
	SetBaseData(strEnemyFolderName);
	SetMoveData();
	SetVisibilityData();
	SetShotData();

	SetDataProduct();
}

void clsEnemyBase::SetBaseData(std::string strEnemyFolderName)
{
	m_BaseState.strEnemyFolderName = strEnemyFolderName;

	std::string strBaseDataName = m_BaseState.strEnemyFolderName + "\\Base.csv";

	clsFILE File;
	if (File.Open(strBaseDataName))
	{
		m_BaseState.iMoveSwitchType = File.GetDataInt(0,enBaseStateMoveSwichType);
		m_BaseState.iProcFrame = File.GetDataInt(0, enBaseStateProcFrame);
		m_BaseState.iTargetEvaluationType = File.GetDataInt(0, enBaseStateTargetEvaluationType);

		File.Close();
	}
}

void clsEnemyBase::SetMoveData()
{
	std::string strMoveDataName = m_BaseState.strEnemyFolderName + "\\Move.csv";

	clsFILE File;
	if (File.Open(strMoveDataName))
	{
		m_v_MoveState.resize(File.GetSizeRow());

		for (int i = 0; i < m_v_MoveState.size(); i++)
		{
			m_v_MoveState[i].iMoveUpdateInterval = File.GetDataInt(i, enMoveStateMoveUpdateInterval);

			m_v_MoveState[i].iHorDistance = File.GetDataInt(i, enMoveStateHorDistance);
			m_v_MoveState[i].iHorDisRandMax = File.GetDataInt(i, enMoveStateHorDisRandMax);
			m_v_MoveState[i].iMoveDir = File.GetDataInt(i, enMoveStateMoveDir);
			m_v_MoveState[i].iMoveDirRandMax = File.GetDataInt(i, enMoveStateMoveDirRandMax);

			m_v_MoveState[i].iVerDistance = File.GetDataInt(i, enMoveStateVerDistance);
			m_v_MoveState[i].iVerDistRandMax = File.GetDataInt(i, enMoveStateVerDistRandMax);
		}

		File.Close();
	}
}

void clsEnemyBase::SetVisibilityData()
{
	std::string strVisibilityDataName = m_BaseState.strEnemyFolderName + "\\Visibility.csv";

	clsFILE File;
	if (File.Open(strVisibilityDataName))
	{
		m_v_VisAreaState.resize(File.GetSizeRow());

		for (int i = 0; i < m_v_VisAreaState.size(); i++)
		{
			m_v_VisAreaState[i].iVisType = File.GetDataInt(i, enVisibilityAreaVisType);

			m_v_VisAreaState[i].iVisDistance = File.GetDataInt(i, enVisibilityAreaVisDistance);
			m_v_VisAreaState[i].iVisAngle = File.GetDataInt(i, enVisibilityAreaVisAngle);
		}

		File.Close();
	}
}

void clsEnemyBase::SetShotData()
{
	std::string strShotDataName = m_BaseState.strEnemyFolderName + "\\Shot.csv";

	clsFILE File;
	if (File.Open(strShotDataName))
	{
		m_v_ShotState.resize(File.GetSizeRow());

		for (int i = 0; i < m_v_ShotState.size(); i++)
		{
			m_v_ShotState[i].iShotDisMax = File.GetDataInt(i, enShotStateShotDisMax);
			m_v_ShotState[i].iShotDisMin = File.GetDataInt(i, enShotStateShotDisMin);

			m_v_ShotState[i].iShotENLimitParcent = File.GetDataInt(i, enShotStateShotENLimitParcent);
		}

		File.Close();
	}
}


void clsEnemyBase::SetDataProduct(){}

void clsEnemyBase::UpdateProduct(){}