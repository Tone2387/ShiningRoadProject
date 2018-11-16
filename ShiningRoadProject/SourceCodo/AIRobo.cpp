#include"AIRobo.h"

clsAIRobo::clsAIRobo()
	: m_pAI(nullptr)
	, m_bAct(false)
{
}

clsAIRobo::~clsAIRobo()
{

}

void clsAIRobo::Init(clsPOINTER_GROUP* const pPtrGroup,
	std::string strEnemyFolderName)
{
	RoboInit(pPtrGroup, pPtrGroup->GetRoboStatus());

	m_pAI = new clsEnemyRobo;
	m_pAI->Init("Data\\FileData\\Hiyoshi\\Enemy1", this);
}

void clsAIRobo::Action()
{
	float fPush = 0.0f;
	float fAngle = 0.0f;
	clsRoboCommand* pRoboCom;

	float fPushMin = 0.5f;

	LockChara();

	m_pAI->Update();

	m_pAI->SearchTarget(m_v_pEnemys);

	if (m_bAct)
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

	if (m_bAct)
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

		pRoboCom = m_pAI->QuickBoostOperation(fPush, fAngle);

		if (pRoboCom)//クイックブーストの使用.
		{
			pRoboCom->Trigger(this, fPush, fAngle);
			pRoboCom->PushBotton(this);

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

		else
		{
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

		pRoboCom = m_pAI->QuickTurnOperation(fPush, fAngle);
	}
}

void clsAIRobo::SwitchMove()
{
	if (m_bAct)
	{
		m_bAct = false;
		return;
	}

	m_bAct = true;
}
