#include"AIRobo.h"
#include"OperationString.h"
#include"RoboStatusEnemy.h"

namespace
{
	const char strRoboAIFolderPath[] = "Data\\FileData\\Hiyoshi\\RoboAI\\Robo";
};

clsAIRobo::clsAIRobo()
	: m_pAI(nullptr)
	, m_bAct(true)
{
}

clsAIRobo::~clsAIRobo()
{
	SAFE_DELETE(m_pAI);
}

void clsAIRobo::Init(clsPOINTER_GROUP* const pPtrGroup,
	unsigned int uiAINum)
{
	clsROBO_STATUS_ENEMY EnemyState(uiAINum);

	RoboInit(pPtrGroup, &EnemyState);

	clsOPERATION_STRING strOtr;
	std::string strFolderName = strRoboAIFolderPath;

	strFolderName = strOtr.ConsolidatedNumber(strFolderName, uiAINum, strOtr.GetNumDigit(uiAINum));

	m_pAI = new clsEnemyRobo;
	m_pAI->Init(strFolderName, this);
}

void clsAIRobo::Action(clsStage* const pStage)
{
	if (m_iHP <= 0)
	{
		return;
	}

	float fPush = 0.0f;
	float fAngle = 0.0f;
	clsRoboCommand* pRoboCom;

	float fPushMin = 0.5f;

	LockChara(pStage);

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
