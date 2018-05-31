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

void clsEnemyBase::SearchTarget(clsObject* pObj)
{
	m_pTarget = nullptr;

	for (int i = 0; i < m_visAreaData.iCategory; i++)
	{
		float fDis = D3DXVec3Length(&(pObj->GetPosition() - m_pTrans->vPos));

		if (m_visAreaData.VisAreaState[i]->iVisDistance > fDis)
		{
			if (!m_pTarget)
			{
				if (m_visAreaData.VisAreaState[i]->iVisType == 0)
				{
					if (true)
					{
						m_pTarget = pObj;
						break;
					}
				}

				else
				{
					m_pTarget = pObj;
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
							m_pTarget = pObj;
							break;
						}
					}

					else
					{
						m_pTarget = pObj;
						break;
					}
				}
			}
		}
	}
}

void clsEnemyBase::SetMoveDir()
{
	m_vMoveDir = {0.0f,0.0f,0.0f};

	

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
		float fHorDestDis;//Destination:–Ú“I’n.
		fHorDestDis = m_MoveData.MoveState[m_iMoveCategoryNo]->iHorDistance * g_fDisStandard;
		fHorDestDis += m_MoveData.MoveState[m_iMoveCategoryNo]->iHorDisRandMax * g_fDisStandard;

		
	}
}

bool clsEnemyBase::IsShot()
{
	return false;
}