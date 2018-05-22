#include"Robo.h"

void clsRobo::RoboInit(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	clsPOINTER_GROUP* const pPtrGroup)
{
#ifdef Tahara
	m_wpResource = pPtrGroup->GetResource();
	m_wpEffects = pPtrGroup->GetEffects();
	m_wpSound = pPtrGroup->GetSound();
#endif//#ifdef Tahara

	m_pMesh = new clsSkinMesh;

	m_pMesh->AttachModel(m_wpResource->GetSkinModels(clsResource::enSkinModel_Player));
	m_pMesh->SetAnimSpeed(0.1);

	SetScale(0.005f);

	m_Trans.vPos.y = 10.0f;

	m_fWalktMoveSpeedMax = 0.25f;
	m_fWalkTopSpeedFrame = 5;

	m_fBoostMoveSpeedMax = 0.5;
	m_iBoostTopSpeedFrame = 60;

	m_fBoostRisingSpeedMax = 0.5f;//スピードの最大値.
	m_iBoostRisingTopSpeedFrame = 20;//↑に達するまでのフレーム値.
	m_fBoostRisingAccele = m_fBoostRisingSpeedMax / m_iBoostRisingTopSpeedFrame;// = m_fMoveSpeedMax / m_fTopSpeedFrame;

	SetMoveAcceleSpeed(m_fWalktMoveSpeedMax, m_fWalkTopSpeedFrame);

	SetRotationSpeed(0.1f);
	SetJumpPower(0.5f);
}

void clsRobo::Walk()
{
	SetMoveAcceleSpeed(m_fWalktMoveSpeedMax, m_fWalkTopSpeedFrame);
	m_bBoost = false;
}

void clsRobo::Boost()
{
	SetMoveAcceleSpeed(m_fBoostMoveSpeedMax, m_iBoostTopSpeedFrame);
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
		Jump();
	}
}

void clsRobo::SetDirQuickBoost(const float fAngle)
{
	if (IsMoveControl())
	{
		D3DXVECTOR3 vForward;

		//今向いている方向.
		vForward = GetVec3Dir(m_Trans.fYaw, vDirForward);

		//行きたい方向.
		m_vMoveDir = GetVec3Dir(fAngle, vForward);
	}
}

void clsRobo::QuickBoost()
{
	if (IsMoveControl())
	{
		m_fMoveSpeed = m_fBoostMoveSpeedMax * 5.0f;
		SetMoveDecelerationSpeed(m_iBoostTopSpeedFrame * 2);
	}
}


clsRobo::clsRobo() :
m_pMesh(NULL),
m_bBoost(false),
m_fWalktMoveSpeedMax(0.0f),
m_fWalkTopSpeedFrame(0.0f),
m_fBoostMoveSpeedMax(0.0f),
m_iBoostTopSpeedFrame(0),
m_fBoostRisingSpeedMax(0.0f),
m_iBoostRisingTopSpeedFrame(0),
m_fBoostRisingAccele(0.0f),

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