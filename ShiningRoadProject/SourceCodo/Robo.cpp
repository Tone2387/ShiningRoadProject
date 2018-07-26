#include"Robo.h"

void clsRobo::RoboInit(clsPOINTER_GROUP* const pPtrGroup)
{
#ifdef Tahara
	m_wpResource = pPtrGroup->GetResource();
	m_wpEffects = pPtrGroup->GetEffects();
	m_wpSound = pPtrGroup->GetSound();
#endif//#ifdef Tahara

	m_pMesh = new clsSkinMesh;

	m_pMesh->AttachModel(
		m_wpResource->GetSkinModels(
				clsResource::enSkinModel_Leg ) );
	//m_wpResource->GetPartsModels( enPARTS::LEG, 3 ));
	m_pMesh->SetAnimSpeed(0.1);

	SetScale(0.005f);

	m_Trans.vPos.y = 10.0f;

	m_fWalktMoveSpeedMax = 0.25f;
	m_iWalkTopSpeedFrame = 5;

	m_fBoostMoveSpeedMax = 0.5;
	m_iBoostTopSpeedFrame = 60;

	m_fQuickBoostSpeedMax = m_fBoostMoveSpeedMax * 3.0f;
	m_iQuickBoostTopSpeedTime = 1 * g_fFPS;

	m_fQuickTrunSpeedMax = (float)D3DX_PI / g_iQuickTurnFrame;
	m_iQuickTrunTopSpeedTime = 15;

	m_fBoostRisingSpeedMax = 0.5f;//スピードの最大値.
	m_iBoostRisingTopSpeedFrame = 20;//↑に達するまでのフレーム値.
	m_fBoostRisingAccele = m_fBoostRisingSpeedMax / m_iBoostRisingTopSpeedFrame;// = m_fMoveSpeedMax / m_fTopSpeedFrame;
	m_fBoostFollRes = m_fBoostRisingSpeedMax / 10;

	SetMoveAcceleSpeed(m_fWalktMoveSpeedMax, m_iWalkTopSpeedFrame);
	SetMoveDeceleSpeed(m_iTopMoveSpeedFrame);

	m_iQuickBoostEnelgyCost = 1000;
	m_iQuickTrunEnelgyCost = 1000;
	m_iBoostRisingyCost = 100;

	m_iEnelgyMax = 10000;
	m_iEnelgy = m_iEnelgyMax;
	m_iEnelgyOutput = 1500 / g_fFPS;
	m_iBoostFloatRecovery = m_iEnelgyOutput / 2;

	SetRotAcceleSpeed(0.01f, 30);
	SetJumpPower(0.5f);
}

void clsRobo::Walk()
{
	SetMoveAcceleSpeed(m_fWalktMoveSpeedMax, m_iWalkTopSpeedFrame);
	m_iMoveStopFrame = m_iWalkTopSpeedFrame;
	
	m_bBoost = false;
}

void clsRobo::Boost()
{
	SetMoveAcceleSpeed(m_fBoostMoveSpeedMax, m_iBoostTopSpeedFrame);
	m_iMoveStopFrame = m_iBoostTopSpeedFrame;

	m_bBoost = true;
}

void clsRobo::MoveSwitch()
{
	if (IsMoveControl())
	{
		if (m_bBoost)
		{
			Walk();
		}

		else
		{
			Boost();
		}
	}
}

void clsRobo::BoostRising()
{
	if (!m_bGround || IsMoveing())
	{
		if (EnelgyConsumption(m_iBoostRisingyCost))
		{
			if (m_fFollPower < m_fBoostMoveSpeedMax)
			{
				m_fFollPower += m_fBoostRisingAccele;
			}

			else
			{
				m_fFollPower = m_fBoostMoveSpeedMax;
			}

			if (!m_bBoost)
			{
				Boost();
			}
		}

		else
		{
			Walk();
		}
	}

	else
	{
		Jump();
	}
}

void clsRobo::SetDirQuickBoost(const float fAngle)
{
	if (IsMoveControl())
	{
		D3DXVECTOR3 vForward;

		//今向いている方向.
		vForward = GetVec3Dir(m_Trans.fYaw, g_vDirForward);

		//行きたい方向.
		m_vMoveDir = GetVec3Dir(fAngle, vForward);
	}
}

void clsRobo::QuickBoost()
{
	if (IsMoveControl())
	{
		if (EnelgyConsumption(m_iQuickBoostEnelgyCost))
		{
			if (m_iQuickInterbal < 0)
			{
				m_iQuickInterbal = g_iQuickInterbal;
				m_fMoveSpeed = m_fQuickBoostSpeedMax;
				m_iQuickBoostDecStartTime = m_iQuickBoostTopSpeedTime;
				SetMoveDeceleSpeed(m_iQuickInterbal);
			}
		}
	}
}

void clsRobo::SetDirQuickTurn(const float fAngle)
{
	if (!m_bBoost)
	{
		if (IsRotControl())
		{
			float fTmp = (int)D3DX_PI * (fAngle / abs(fAngle));
			SetRotDir(fTmp);
		}
	}
}

void clsRobo::QuickTurn()
{
	if (!m_bBoost)
	{
		if (EnelgyConsumption(m_iQuickTrunEnelgyCost))
		{
			if (IsRotControl())
			{
				if (m_iQuickInterbal < 0)
				{
					m_iQuickInterbal = g_iQuickInterbal;
					m_fRotSpeed = m_fQuickTrunSpeedMax;
					m_iQuickTrunDecStartTime = m_iQuickTrunTopSpeedTime;
					SetRotDeceleSpeed(m_iQuickInterbal);
				}
			}
		}
	}
}

void clsRobo::Updata()
{
	if (m_iQuickBoostDecStartTime > 0)//クイックブースト.
	{
		m_fMoveSpeed = m_fQuickBoostSpeedMax;
		m_iQuickBoostDecStartTime--;
	}

	if (m_iQuickTrunDecStartTime > 0)//クイックターン.
	{
		m_fRotSpeed = m_fQuickTrunSpeedMax;
		m_iQuickTrunDecStartTime--;
	}

	if (IsMoveControl())
	{
		if (!m_bBoost)
		{
			Walk();
		}

		else
		{
			Boost();
		}
	}

	if (m_bBoost)
	{
		if (m_fFollPower < -m_fBoostFollRes)
		{
			m_fFollPower += m_fBoostFollRes;

			if (m_fFollPower > -m_fBoostFollRes)
			{
				m_fFollPower = -m_fBoostFollRes;
			}
		}
	}

	if (IsRotControl())
	{
		SetRotDeceleSpeed(m_iRotStopFrame);
	}

	if (m_iQuickInterbal >= 0)
	{
		m_iQuickInterbal--;
	}

	EnelgyRecovery();
}

void clsRobo::UpdataLimitTime()
{
	m_iActivityLimitTime--;

	if (m_iActivityLimitTime < 0)
	{
		m_bTimeUp = true;
	}
}

void clsRobo::EnelgyRecovery()
{
	SetEnelgyRecoveryAmount();

	m_iEnelgy += m_iEnelgyRecoveryPoint;

	if (m_iEnelgy > m_iEnelgyMax)
	{
		m_iEnelgy = m_iEnelgyMax;
	}
}

void clsRobo::SetEnelgyRecoveryAmount()
{
	m_iEnelgyRecoveryPoint = m_iEnelgyOutput;

	if (m_bBoost)
	{
		if (!m_bGround)
		{
			m_iEnelgyRecoveryPoint -= m_iBoostFloatRecovery;
		}

		m_iEnelgyRecoveryPoint -= m_iBoostMoveCost;
	}

	if (false)//射撃準備完了.
	{
		m_iEnelgyRecoveryPoint - (m_iEnelgyOutput);
	}
}

bool clsRobo::EnelgyConsumption(const int iConsumption)
{
	if (m_iEnelgy >= iConsumption)
	{
		m_iEnelgy -= iConsumption;
		return true;
	}

	return false;
}

void clsRobo::ShotLWeapon()
{
	ShotSwich(enWeaponLHand);
	//if(m_pLHandWeapon->Shot()){}
}

void clsRobo::ShotRWeapon()
{
	ShotSwich(enWeaponRHand);
	//if(m_pRHandWeapon->Shot()){}
}

clsRobo::clsRobo() :
m_pMesh(NULL),
m_bBoost(false),
m_fWalktMoveSpeedMax(0.0f),
m_iWalkTopSpeedFrame(0),
m_fBoostMoveSpeedMax(0.0f),
m_iBoostTopSpeedFrame(0),
m_fBoostRisingSpeedMax(0.0f),
m_iBoostRisingTopSpeedFrame(0),
m_fBoostRisingAccele(0.0f),
m_iQuickInterbal(0),
m_wpResource(nullptr),
m_wpEffects(nullptr),
m_wpSound(nullptr)
{
	//	ZeroMemory(this, sizeof(clsRobo));
}

clsRobo::~clsRobo()
{
	if (m_pMesh != NULL)
	{
		delete m_pMesh;
		m_pMesh = NULL;
	}

#ifdef Tahara
	//消すときdeleteしないでnullしてね.
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
#endif//#ifdef Tahara
}