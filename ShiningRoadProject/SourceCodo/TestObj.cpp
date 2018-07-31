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
	RoboInit(pPtrGroup);

	m_pInput = new clsInputRobo(pPtrGroup->GetDxInput(), pPtrGroup->GetXInput());

	m_pMesh->SetAnimSpeed(0.01);
}

void clsTestObj::Action(const clsDX9Mesh* pWall)
{
	float fPush = 0.0f;
	float fAngle = 0.0f;
	clsRoboCommand* pRoboCom;

	float fPushMin = 0.5f;

	Updata();

	pRoboCom = m_pInput->MoveSwitch();

	if (pRoboCom)
	{
		pRoboCom->PushBotton(this);
	}

	pRoboCom = m_pInput->BoostRising();

	if (pRoboCom)
	{
		pRoboCom->PushBotton(this);
	}

	pRoboCom = m_pInput->LSInput(fPush, fAngle);
	pRoboCom->Trigger(this, fPush, fAngle);

	if (abs(fPush) >= fPushMin)//LS押し込み.
	{
		pRoboCom = m_pInput->QuickBoost(fPush);//クイックブースト.
		if (pRoboCom)
		{
			pRoboCom->Trigger(this, fPush, fAngle);
			pRoboCom->PushBotton(this);
		}

		pRoboCom = m_pInput->RSHorInput(fPush, fAngle);//旋回.
		pRoboCom->Trigger(this, abs(fPush), fAngle);
	}

	else
	{
		pRoboCom = m_pInput->RSHorInput(fPush, fAngle);//旋回.
		pRoboCom->Trigger(this, abs(fPush), fAngle);

		if (abs(fPush) >= fPushMin)//RS押し込み.
		{
			pRoboCom = m_pInput->QuickTurn(fPush);

			if (pRoboCom)
			{
				pRoboCom->Trigger(this, fPush, fAngle);
				pRoboCom->PushBotton(this);
			}
		}

		else
		{
			pRoboCom = m_pInput->QuickBoost(fPush);

			if (pRoboCom)
			{
				pRoboCom->PushBotton(this);
			}
		}
	}
}

void clsTestObj::InhUpdate()
{
	m_pMesh->SetAnimSpeed(0.01);
}