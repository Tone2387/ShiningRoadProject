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
	for (int i = 0; i < m_visAreaData.iCategory; i++)
	{
		float fDis = D3DXVec3Length(&(pObj->GetPosition() - m_pTrans->vPos));

		if (m_visAreaData.VisAreaState[i]->iVisDistance < fDis)
		{
			switch (m_visAreaData.VisAreaState[i]->iVisType)
			{
			case 0:
				if (true)
				{
					m_visAreaData.VisAreaState[i]->iVisDistance;
				}
				break;
			case 1:
				m_visAreaData.VisAreaState[i]->iVisDistance;
				break;
			default:
				break;
			}
		}

		
		
		
		
	}
}

void clsEnemyBase::SetMoveDir()
{

}

bool clsEnemyBase::IsShot()
{
	return false;
}