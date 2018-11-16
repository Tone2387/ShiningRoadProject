#include"Player.h"

clsPlayer::clsPlayer()
	: m_pInput(nullptr)
{
	//ZeroMemory(this, sizeof(clsPlayer));
}

clsPlayer::~clsPlayer()
{

}

void clsPlayer::Init(clsPOINTER_GROUP* const pPtrGroup)
{
	RoboInit(pPtrGroup, pPtrGroup->GetRoboStatus());

	m_pInput = new clsInputRobo(pPtrGroup->GetDxInput(), pPtrGroup->GetXInput());

	/*clsObject::SPHERE Tmp;

	Tmp.vCenter = &m_vCenterPos;
	Tmp.fRadius = 0.1f;

	m_v_Spheres.push_back(Tmp);
	m_v_Spheres.shrink_to_fit();*/

	m_vCamPosDivia = m_Trans.vPos - m_pMesh->GetBonePos(enPARTS::CORE, "Jenerator");
}

void clsPlayer::Action()
{
	float fPush = 0.0f;
	float fAngle = 0.0f;
	clsRoboCommand* pRoboCom;

	LockChara();

	m_pInput->InputTest();

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

	pRoboCom = m_pInput->RSVerInput(fPush, fAngle);

	if (pRoboCom)
	{
		pRoboCom->Trigger(this, fPush, fAngle);
	}

	pRoboCom = m_pInput->LWeaponShot();

	if (pRoboCom)
	{
		pRoboCom->PushBotton(this);
	}

	pRoboCom = m_pInput->RWeaponShot();

	if (pRoboCom)
	{
		pRoboCom->PushBotton(this);
	}
}

D3DXVECTOR3 clsPlayer::GetCamTargetPos()
{
	return m_vLockStartingPos;
}


D3DXVECTOR3 clsPlayer::GetLookTargetPos()
{
	return m_vLockPos;
}

D3DXVECTOR3 clsPlayer::GetLockCenterPos()
{
	return m_vLockCenterPos;
}

float clsPlayer::GetLockCircleScale()
{
	return m_fLockCircleRadius;
}

bool clsPlayer::GetTargetPos(D3DXVECTOR3& vTmpPos)
{
	if (m_pTargetChara)
	{
		vTmpPos = m_pTargetChara->m_vCenterPos;
		return true;
	}

	return false;
}