#include"Player.h"

clsPlayer::clsPlayer()
{
	ZeroMemory(this, sizeof(clsPlayer));
}

clsPlayer::~clsPlayer()
{

}

void clsPlayer::Init(clsPOINTER_GROUP* const pPtrGroup)
{
	RoboInit(pPtrGroup);

	m_pInput = new clsInputRobo(pPtrGroup->GetDxInput(), pPtrGroup->GetXInput());

	m_pMesh->SetAnimSpeed(0.01);
}

void clsPlayer::Action(clsStage* const pWall)
{
	float fPush = 0.0f;
	float fAngle = 0.0f;
	clsRoboCommand* pRoboCom;

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

	if (pRoboCom)
	{
		pRoboCom->Trigger(this, fPush, fAngle);
	}

	pRoboCom = m_pInput->QuickBoost(fPush);//クイックブースト.
	if (pRoboCom)
	{
		pRoboCom->Trigger(this, fPush, fAngle);
		pRoboCom->PushBotton(this);

		pRoboCom = m_pInput->RSHorInput(fPush, fAngle);//旋回.

		if (pRoboCom)
		{
			pRoboCom->Trigger(this, abs(fPush), fAngle);
		}
	}

	else
	{
		pRoboCom = m_pInput->RSHorInput(fPush, fAngle);//旋回.

		if (pRoboCom)
		{
			pRoboCom->Trigger(this, abs(fPush), fAngle);
		}

		pRoboCom = m_pInput->QuickTurn(fPush);

		if (pRoboCom)
		{
			pRoboCom->Trigger(this, fPush, fAngle);
			pRoboCom->PushBotton(this);
		}

		else
		{
			float fPushFull = 1.0f;
			float fPushAngle = 0.0f;

			pRoboCom = m_pInput->QuickBoost(fPushFull);//クイックブースト.
			if (pRoboCom)
			{
				pRoboCom->Trigger(this, fPush, fPushAngle);
				pRoboCom->PushBotton(this);
			}
		}
	}

	Updata();

	WallJudge(pWall);
}

void clsPlayer::InhUpdate()
{
	m_pMesh->SetAnimSpeed(0.01);
}