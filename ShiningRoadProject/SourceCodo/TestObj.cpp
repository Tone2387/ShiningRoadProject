#include"TestObject.h"

clsTestObj::clsTestObj()
{
	ZeroMemory(this, sizeof(clsTestObj));
}

clsTestObj::~clsTestObj()
{

}

void clsTestObj::Init(clsPOINTER_GROUP* const pPtrGroup )
{
	RoboInit(pPtrGroup, pPtrGroup->GetRoboStatus());

	m_v_Spheres.resize(1);
	m_v_Spheres[0].vCenter = &m_vCenterPos;
	m_v_Spheres[0].fRadius = 0.1f;

	//m_pMesh->SetAnimSpeed(0.01);
}

void clsTestObj::Init(clsPOINTER_GROUP* const pPtrGroup,
	LPSTR strEnemyFolderName,
	std::vector<clsCharactor*> v_pEnemys)
{
	Init(pPtrGroup);

	m_pAI = new clsEnemyRobo;
	m_pAI->Init("", this, v_pEnemys);
}

void clsTestObj::ActionProduct()
{
	float fPush = 0.0f;
	float fAngle = 0.0f;
	clsRoboCommand* pRoboCom;

	float fPushMin = 0.5f;

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

	pRoboCom = m_pAI->MoveOperation(fPush, fAngle);

	if (pRoboCom)
	{
		pRoboCom->Trigger(this, fPush, fAngle);
	}

	pRoboCom = m_pAI->RotateOperation(fPush, fAngle);//ù‰ñ.

	if (pRoboCom)
	{
		pRoboCom->Trigger(this, abs(fPush), fAngle);
	}

	pRoboCom = m_pAI->LookOperation(fPush, fAngle);

	if (pRoboCom)
	{
		pRoboCom->Trigger(this, fPush, fAngle);
	}

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

	Move();
	Rotate();

	Updata();
}

void clsTestObj::InhUpdate()
{
	//m_pMesh->SetAnimSpeed(0.01);
}