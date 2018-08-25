#include"Enemy.h"

clsEnemyBase::clsEnemyBase()
{
}

clsEnemyBase::~clsEnemyBase()
{
}

clsCharactor* clsEnemyBase::SearchTarget()
{
	m_pTarget = nullptr;

	for (int i = 0; i < m_v_pEnemys.size(); i++)
	{
		if (!m_v_pEnemys[i]->m_bDeadFlg)
		{
			m_pTarget = m_v_pEnemys[i];
		}
	}

	return m_pTarget;

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
						m_pTarget = m_v_pEnemys[0];
						break;
					}
				}

				else
				{
					m_pTarget = m_v_pEnemys[0];
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
							m_pTarget = m_v_pEnemys[0];
							break;
						}
					}

					else
					{
						m_pTarget = m_v_pEnemys[0];
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
		fPush = 1.0f;

		m_UpdateState.vHorMoveDir = { 0.0f, 0.0f, 0.0f };
		m_UpdateState.fVerDis = 0.0f;

		D3DXVECTOR3 vTmp;

		float fRandMax = 0.0f;

		float fHorDestDis = 0.0f;//Destination:ñ⁄ìIín.
		fHorDestDis = m_MoveData.v_MoveState[m_iMoveCategoryNo].iHorDistance * g_fDistanceReference;
		fRandMax = m_MoveData.v_MoveState[m_iMoveCategoryNo].iHorDisRandMax* g_fDistanceReference;

		if (fRandMax != 0.0f)//0ÇæÇ∆é~Ç‹ÇÈÇÃÇ≈ñhé~.
		{
			fHorDestDis += fmodf(static_cast<float>(rand()), (fRandMax));//ïœìÆílÇ™Ç†ÇÈèÍçáÅAóêêîê∂ê¨.
		}

		int iRandMax = 0;

		m_UpdateState.iHorDirResult = m_MoveData.v_MoveState[m_iMoveCategoryNo].iMoveDir;
		iRandMax = m_MoveData.v_MoveState[m_iMoveCategoryNo].iMoveDirRandMax;

		if (iRandMax != 0)//0ÇæÇ∆é~Ç‹ÇÈÇÃÇ≈ñhé~.
		{
			m_UpdateState.iHorDirResult += (rand() % (iRandMax * 2)) - iRandMax;
		}

		vTmp = m_pChara->GetPosition() - m_pTarget->GetPosition();
		vTmp.y = 0.0f;//êÇíºï˚å¸ÇÕä÷åWÇ»Ç¢ÇÃÇ≈0Ç…Ç∑ÇÈ.
		D3DXVec3Normalize(&vTmp, &vTmp);//É^Å[ÉQÉbÉgÇ©ÇÁé©ï™Ç÷ÇÃï˚å¸ÉxÉNÉgÉãÇéÊìæ.

		m_UpdateState.vHorMoveDir = m_pTarget->GetPosition() + vTmp * fHorDestDis;//ñ⁄ìIín.

		m_UpdateState.vHorMoveDir.y = 0.0f;//êÇíºï˚å¸ÇÕä÷åWÇ»Ç¢ÇÃÇ≈0Ç…Ç∑ÇÈ.

		//_MoveState.iNowUpdateCnt = m_MoveState.iUpdateInterval;
	}

	else
	{
		return false;
	}

	D3DXVECTOR3 vTarPosDir;
	D3DXVec3Normalize(&vTarPosDir, &m_UpdateState.vHorMoveDir);//ñ⁄ìIínÇ÷ÇÃï˚å¸ÉxÉNÉgÉã.

	D3DXVECTOR3 vForward;//åªç›ÇÃâÒì]Ç…çáÇÌÇπÇƒêiÇﬁï˚å¸ÇåàÇﬂÇÈ.
	vForward = GetVec3Dir(m_pChara->GetRotation().y, g_vDirForward);//ê≥ñ ï˚å¸ÉxÉNÉgÉã.

	float fTmp = D3DXVec3Dot(&vTarPosDir, &vForward);
	fTmp = cos(fTmp);

	fTmp += static_cast<float>(D3DXToRadian(m_UpdateState.iHorDirResult));

	fAngle = fTmp;

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

		if (fRot > 0)
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

	return false;
}

bool clsEnemyBase::SetLook(float& fPush, float& fAngle)
{
	fPush = 0.0f;
	fAngle = 0.0f;

	if (m_pTarget)
	{
		const float fPushFull = 1.0f;

		const float fHulf = 2;

		const float fVecY = m_pTarget->GetPosition().y - m_pChara->GetPosition().y;

		float fRot = (atanf(fVecY) / fHulf) - m_pChara->m_fLookUpDir;

		ObjRollOverGuard(&fRot);

		if (fRot > 0)
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
	
	return false;
}

bool clsEnemyBase::IsJump()
{
	if (m_pTarget)
	{
		int iVerDestDis = m_MoveData.v_MoveState[m_iMoveCategoryNo].iVerDistance;

		int iRandMax = 0;
		iRandMax = m_MoveData.v_MoveState[m_iMoveCategoryNo].iVerDistRandMax;

		if (iRandMax != 0)//0ÇæÇ∆é~Ç‹ÇÈÇÃÇ≈ñhé~.
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