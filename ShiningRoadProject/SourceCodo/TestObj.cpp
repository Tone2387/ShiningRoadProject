#include"TestObject.h"

const bool g_bAction = true;

clsTestObj::clsTestObj()
	: m_pAI(nullptr)
{
	//ZeroMemory(this, sizeof(clsTestObj));
}

clsTestObj::~clsTestObj()
{

}

void clsTestObj::Init(clsPOINTER_GROUP* const pPtrGroup)
{
	RoboInit(pPtrGroup, pPtrGroup->GetRoboStatus());

	m_v_Spheres.resize(1);
	m_v_Spheres[0].vCenter = &m_vCenterPos;
	m_v_Spheres[0].fRadius = 0.1f;

	//m_HP = m_MaxHP = 5;
}

void clsTestObj::Init(clsPOINTER_GROUP* const pPtrGroup,
	LPSTR strEnemyFolderName)
{
	m_pAI = new clsEnemyRobo;
	m_pAI->Init("", this);

	Init(pPtrGroup);
}

void clsTestObj::ActionProduct()
{
	float fPush = 0.0f;
	float fAngle = 0.0f;
	clsRoboCommand* pRoboCom;

	float fPushMin = 0.5f;

	LockChara();

	m_pAI->SearchTarget(m_v_pEnemys);

	if (g_bAction)
	{
		pRoboCom = m_pAI->MoveOperation(fPush, fAngle);

		if (pRoboCom)
		{
			pRoboCom->Trigger(this, fPush, fAngle);
		}
	}

	pRoboCom = m_pAI->RotateOperation(fPush, fAngle);//旋回.

	if (pRoboCom)
	{
		pRoboCom->Trigger(this, abs(fPush), fAngle);
	}

	pRoboCom = m_pAI->LookOperation(fPush, fAngle);//上下視線移動.

	if (pRoboCom)
	{
		pRoboCom->Trigger(this, fPush, fAngle);
	}

	if (g_bAction)
	{
		pRoboCom = m_pAI->LShotOperation();

		if (pRoboCom)
		{
			pRoboCom->PushBotton(this);
		}

		pRoboCom = m_pAI->RShotOperation();

		if (pRoboCom)
		{
			pRoboCom->PushBotton(this);
		}
	

		pRoboCom = m_pAI->QuickTurnOperation(fPush, fAngle);

		if (pRoboCom)
		{
			if (m_bBoost)
			{
				MoveSwitch();//QuickTrunの為に強制的にブースターを切る.
			}

			pRoboCom->Trigger(this, fPush, fAngle);
			pRoboCom->PushBotton(this);
		}

		else
		{
			pRoboCom = m_pAI->MoveSwitchOperation();

			if (pRoboCom)
			{
				pRoboCom->PushBotton(this);
			}

			pRoboCom = m_pAI->BoostOperation();

			if (pRoboCom)
			{
				pRoboCom->PushBotton(this);
			}
		}
	}

	Updata();
}

void clsTestObj::InhUpdate()
{
	//m_pMesh->SetAnimSpeed(0.01);
}