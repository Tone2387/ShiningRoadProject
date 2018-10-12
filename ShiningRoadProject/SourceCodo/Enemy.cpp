#include"Enemy.h"

clsEnemyBase::clsEnemyBase(std::vector<clsCharactor*>& v_Enemys)
{
	m_vp_pEnemys = &v_Enemys;
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
	if (m_UpdateState.iProcessCnt < 0)
	{
		m_UpdateState.iProcessCnt--;
	}

	UpdateProduct();
}

void clsEnemyBase::SearchTarget(std::vector<clsCharactor*> v_pEnemys)
{
	m_pTarget = nullptr;

	std::vector<clsCharactor*> v_pEnemysTmp = *m_vp_pEnemys;

	for (unsigned int i = 0; i < v_pEnemysTmp.size(); i++)
	{
		if (!v_pEnemysTmp[i]->m_bDeadFlg)
		{
			m_pTarget = v_pEnemysTmp[i];
		}
	}

/*	for (int i = 0; i < m_visAreaData.iCategory; i++)
	{
		D3DXVECTOR3 vDisTmp = m_pTarget->GetPosition() - m_pTrans->vPos;
		float fDis = D3DXVec3Length(&vDisTmp);

		if (m_visAreaData.v_VisAreaState[i].iVisDistance > fDis)
		{
			if (!m_pTarget)
			{
				if (m_visAreaData.v_VisAreaState[i].iVisType == 0)
				{
					if (true)
					{
						m_pTarget = v_pEnemys[0];
						break;
					}
				}

				else
				{
					m_pTarget = v_pEnemys[0];
					break;
				}
			}

			else
			{
				if (D3DXVec3Length(&(m_pTarget->GetPosition() - m_pTrans->vPos)) > fDis)
				{
					if (m_visAreaData.v_VisAreaState[i].iVisType == 0)
					{
						if (true)
						{
							m_pTarget = v_pEnemys[0];
							break;
						}
					}

					else
					{
						m_pTarget = v_pEnemys[0];
						break;
					}
				}
			}
		}
	}*/

}

bool clsEnemyBase::SetMoveDir(float& fPush, float& fAngle)
{
	fPush = 0.0f;
	fAngle = 0.0f;

	/*switch (m_BaseData.iMoveCategoryVisType)
	{
	case 0:

		m_iMoveCategoryNo++;

		if (m_MoveData.iCategory > m_iMoveCategoryNo)
		{
			m_iMoveCategoryNo = 0;
		}

		break;

	case 1:

		m_iMoveCategoryNo = rand() % m_MoveData.iCategory;

		break;

	default:
		break;
	}*/

	if (m_pTarget)
	{
		if (m_UpdateState.iMoveUpdateCnt < 0)
		{
			MoveState MoveStatus = m_MoveData.v_MoveState[m_UpdateState.iMoveCategoryNo];

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
		const float fPushFull = 1.0f;

		const float fVecX = m_pTarget->GetPosition().x - m_pChara->GetPosition().x;
		const float fVecZ = m_pTarget->GetPosition().z - m_pChara->GetPosition().z;

		float fRot = atan2f(fVecX, fVecZ) - m_pChara->GetRotation().y;

		ObjRollOverGuard(&fRot);

		if (m_UpdateState.iProcessCnt < 0)
		{
			if (fRot > 0)
			{
				m_UpdateState.fRotDir = fPushFull;
			}

			else
			{
				m_UpdateState.fRotDir = -fPushFull;
			}
		}
		
		fPush = m_UpdateState.fRotDir;
		fAngle = fRot;

		return true;
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

		fRot = fRot - m_pChara->m_fLookUpDir;

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
		MoveState MoveStatus = m_MoveData.v_MoveState[m_UpdateState.iMoveCategoryNo];

		int iVerDestDis = MoveStatus.iVerDistance;

		int iRandMax = 0;
		iRandMax = MoveStatus.iVerDistRandMax;

		if (iRandMax != 0)//0‚¾‚ÆŽ~‚Ü‚é‚Ì‚Å–hŽ~.
		{
			iVerDestDis += (rand() % (iRandMax * 2)) - iRandMax;
		}

		m_UpdateState.fVerDis = iVerDestDis * g_fDistanceReference;

		if (m_pTarget->GetPosition().y < m_UpdateState.fVerDis)
		{
			float fDist = m_pTarget->GetPosition().y - m_pChara->GetPosition().y;
			
			if (fDist < m_UpdateState.fVerDis)
			{
				return true;
			}
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

		for (int i = 0; i < m_ShotData.iCategory; i++)
		{
			if (fDis <= m_ShotData.v_ShotState[i].iShotDisMax ||
				fDis >= m_ShotData.v_ShotState[i].iShotDisMin)
			{
				return true;
			}
		}
	}

	return false;
}

void clsEnemyBase::UpdateProduct(){}