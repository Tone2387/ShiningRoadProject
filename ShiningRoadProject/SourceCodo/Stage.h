#pragma once

#include"DX9Mesh.h"
#include"PtrGroup.h"
#include"Object.h"

class clsStage
{
public:
	clsStage();
	~clsStage();

	clsDX9Mesh* m_pStageGround;
	clsDX9Mesh** m_ppStageObject;

	bool WallJudge(clsObject* const pObj)
	{
		bool bHit = false;

		//StageGround‚Æ‚Ì“–‚½‚è”»’è.
		clsDX9Mesh* pGround = m_pStageGround;

		if (pObj->WallForward(pGround))if (!bHit)bHit = true;
		if (pObj->WallBack(pGround))if (!bHit)bHit = true;
		if (pObj->WallLeft(pGround))if (!bHit)bHit = true;
		if (pObj->WallRight(pGround))if (!bHit)bHit = true;

		pObj->m_bGround = pObj->WallUnder(pGround);
		if (pObj->m_bGround)if (!bHit)bHit = true;
		if (pObj->WallUp(pGround))if (!bHit)bHit = true;

		for (int i = 0; i < 1; i++)
		{
			clsDX9Mesh* pObjMesh = m_ppStageObject[i];

			//StageObject‚Æ‚Ì“–‚½‚è”»’è.
			if (pObj->WallForward(pObjMesh))if (!bHit)bHit = true;
			if (pObj->WallBack(pObjMesh))if (!bHit)bHit = true;
			if (pObj->WallLeft(pObjMesh))if (!bHit)bHit = true;
			if (pObj->WallRight(pObjMesh))if (!bHit)bHit = true;

			if (pObj->WallUnder(pObjMesh))
			{
				pObj->m_bGround = true;
				if (!bHit)bHit = true;

				D3DXVECTOR3 vMoveDir = pObj->GetPosition();

				vMoveDir += {0.0f, 0.0f, 0.0f};

				pObj->SetPosition(vMoveDir);
			}

			if (pObj->WallUp(pObjMesh))if (!bHit)bHit = true;
		}

		

		return bHit;
	}

private:

};

clsStage::clsStage()
{
}

clsStage::~clsStage()
{
}