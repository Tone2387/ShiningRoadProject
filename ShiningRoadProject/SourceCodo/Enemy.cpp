#include"Enemy.h"

clsEnemyBase::clsEnemyBase()
{
}

clsEnemyBase::~clsEnemyBase()
{
}

void clsEnemyBase::Init(LPSTR strEnemyFolderName)
{

}

void clsEnemyBase::SearchTarget(clsCharactor* pChara)
{
	m_pTarget = nullptr;

	for (int i = 0; i < m_visAreaData.iCategory; i++)
	{
		D3DXVECTOR3 vDisTmp = pChara->GetPosition() - m_pTrans->vPos;
		float fDis = D3DXVec3Length(&vDisTmp);

		if (m_visAreaData.VisAreaState[i]->iVisDistance > fDis)
		{
			if (!m_pTarget)
			{
				if (m_visAreaData.VisAreaState[i]->iVisType == 0)
				{
					if (true)
					{
						m_pTarget = pChara;
						break;
					}
				}

				else
				{
					m_pTarget = pChara;
					break;
				}
			}

			else
			{
				if (D3DXVec3Length(&(m_pTarget->GetPosition() - m_pTrans->vPos)) > fDis)
				{
					if (m_visAreaData.VisAreaState[i]->iVisType == 0)
					{
						if (true)
						{
							m_pTarget = pChara;
							break;
						}
					}

					else
					{
						m_pTarget = pChara;
						break;
					}
				}
			}
		}
	}
}

void clsEnemyBase::SetMoveDir()
{
	m_vMovePlansDir = { 0.0f, 0.0f, 0.0f };

	m_UpdateState.vHorMoveDir = { 0.0f, 0.0f, 0.0f };
	m_UpdateState.fVerDis = 0.0f;

	switch (m_BaseData.iMoveCategoryVisType)
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
	}

	if (m_pTarget)
	{
		int iRandMax;

		float fHorDestDis = 0.0f;//Destination:目的地.
		fHorDestDis = m_MoveData.MoveState[m_iMoveCategoryNo]->iHorDistance * g_fDisStandard;
		iRandMax = m_MoveData.MoveState[m_iMoveCategoryNo]->iHorDisRandMax;
		fHorDestDis += (rand() % (iRandMax * 2) - iRandMax) * g_fDisStandard;
		
		int iMoveDir = 0.0f;
		iMoveDir = m_MoveData.MoveState[m_iMoveCategoryNo]->iMoveDir;
		iRandMax = m_MoveData.MoveState[m_iMoveCategoryNo]->iMoveDirRandMax;
		iMoveDir += rand() % (iRandMax * 2) - iRandMax;

		D3DXVECTOR3 vTmp;

		vTmp = m_pTarget->m_Trans.vPos - m_pTrans->vPos;
		D3DXVec3Normalize(&vTmp, &vTmp);//ターゲットから自分への方向ベクトルを取得.

		D3DXVECTOR3 vDestination;
		//移動角度()、方向ベクトル、目的距離の計算で目的地を決定.
		vDestination = GetVec3Dir((float)D3DXToRadian(iMoveDir), vTmp) * fHorDestDis;

		D3DXVec3Normalize(&m_UpdateState.vHorMoveDir,&vDestination);//移動方向ベクトル決定.

		float fVerDestDis = 0.0f;//Destination:目的地.
		fVerDestDis = m_MoveData.MoveState[m_iMoveCategoryNo]->iVerDistance * g_fDisStandard;
		iRandMax = m_MoveData.MoveState[m_iMoveCategoryNo]->iVerDistRandMax;
		fVerDestDis += (rand() % (iRandMax * 2) - iRandMax) * g_fDisStandard;

		float fTmp;
		fTmp = m_pTarget->GetPosition().y + fVerDestDis;
		m_UpdateState.fVerDis = fTmp - m_pTrans->vPos.y;

	}
}

bool clsEnemyBase::IsShot()
{

	float fDis;
	fDis = D3DXVec3Length(&(m_pTarget->GetPosition() - m_pTrans->vPos));

	for (int i = 0; i < m_ShotData.iCategory; i++)
	{
		if (fDis <= m_ShotData.ShotState[i]->iShotDisMax ||
			fDis >= m_ShotData.ShotState[i]->iShotDisMin)
		{
			return true;
		}
	}

	return false;
}