#pragma once

#include"RoboCommand.h"
#include"DxInput.h"
#include"CXInput.h"

const float g_fStickPushMin = 0.5f;

//ˆê‰žXInput‚ð—Dæ‚³‚¹‚Ü‚·.

class clsInputRobo
{
public:
	clsInputRobo::clsInputRobo(clsDxInput* pDxInput,clsXInput* pXInput)
	{
		if (pXInput)
		{
			m_pXInput = pXInput;
		}

		if (pDxInput)
		{
			m_pDxInput = pDxInput;
		}

		m_bChangeSwitch = false;

		m_pMoveSwitch = new clsCommandMoveSwitch;
		m_pQuickBoost = new	clsCommandQuickBoost;
		m_pQuickTurn = new clsCommandQuickTurn;
		m_pBoostRising = new clsCommandBoostRising;

		m_pComLShot = new clsCommandLShot;
		m_pComRShot = new clsCommandRShot;

		m_pComLS = new clsCommandMove;
		m_pComLSHor = new clsCommandMove;
		m_pComLSVer = new clsCommandMove;

		m_pComRS = new clsCommandRotate;
		m_pComRSHor = new clsCommandRotate;
		m_pComRSVer = new clsCommandLookVerMove;

	}

	clsInputRobo::~clsInputRobo()
	{
		m_pDxInput = nullptr;
		m_pXInput = nullptr;
		delete m_pComLS;
		delete m_pComRS;

		delete m_pMoveSwitch;
		delete m_pQuickBoost;
		delete m_pBoostRising;
	}

	void InputTest()
	{
		m_pDxInput->IsLSUpEnter();
		m_pDxInput->IsLSDownEnter();
		m_pDxInput->IsLSLeftEnter();
		m_pDxInput->IsLSRightEnter();

		m_pDxInput->IsLSUpStay();
		m_pDxInput->IsLSDownStay();
		m_pDxInput->IsLSLeftStay();
		m_pDxInput->IsLSRightStay();

		m_pDxInput->IsRSUpEnter();
		m_pDxInput->IsRSDownEnter();
		m_pDxInput->IsRSLeftEnter();
		m_pDxInput->IsRSRightEnter();

		m_pDxInput->IsRSUpStay();
		m_pDxInput->IsRSDownStay();
		m_pDxInput->IsRSLeftStay();
		m_pDxInput->IsRSRightStay();
	}

	clsRoboCommand* LSInput(float& fPower, float& fAngle)
	{
		fPower = 0.0f;
		fAngle = 0.0f;

		if (m_pXInput->isConnected())
		{
			fPower = m_pXInput->GetLStickSlope();
			fAngle = m_pXInput->GetLStickTheta();
		}

		else if (m_pDxInput)
		{
			fPower = m_pDxInput->GetLSPush();
			fAngle = m_pDxInput->GetLSDir();
		}

		bool bNaname = false;

		if (abs(fPower) < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComLS;
	};

	clsRoboCommand* LSHorInput(float& fPower, float& fAngle)
	{
		fPower = 0.0f;
		fAngle = 0.0f;

		if (m_pXInput->isConnected())
		{
			fPower = m_pXInput->GetLStickX();
			fAngle = m_pXInput->GetLStickTheta();
		}

		else if (m_pDxInput)
		{
			fPower = m_pDxInput->GetHorLSPush();
			fAngle = m_pDxInput->GetLSDir();
		}

		if (abs(fPower) < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComLSHor;
	}

	clsRoboCommand* LSVerInput(float& fPower, float& fAngle)
	{
		fPower = 0.0f;
		fAngle = 0.0f;

		if (m_pXInput->isConnected())
		{
			fPower = m_pXInput->GetLStickY();
			fAngle = m_pXInput->GetLStickTheta();
		}

		else if (m_pDxInput)
		{
			fPower = m_pDxInput->GetVerLSPush();
			fAngle = m_pDxInput->GetLSDir();
		}

		if (abs(fPower) < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComLSVer;
	}

	clsRoboCommand* RSInput(float& fPower, float& fAngle)
	{
		fPower = 0.0f;
		fAngle = 0.0f;

		if (m_pXInput->isConnected())
		{
			fPower = m_pXInput->GetRStickSlope();
			fAngle = m_pXInput->GetRStickTheta();
		}

		else if (m_pDxInput)
		{

			fPower = m_pDxInput->GetRSPush();
			fAngle = m_pDxInput->GetRSDir();
		}

		if (abs(fPower) < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComRS;
	};

	clsRoboCommand* RSHorInput(float& fPower, float& fAngle)
	{
		fPower = 0.0f;
		fAngle = 0.0f;

		if (m_pXInput->isConnected())
		{
			fPower = m_pXInput->GetRStickX();
			fAngle = m_pXInput->GetRStickTheta();
		}

		else if (m_pDxInput)
		{
			fPower = m_pDxInput->GetHorRSPush();
			fAngle = m_pDxInput->GetRSDir();
		}

		if (abs(fPower) < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComRSHor;
	}

	clsRoboCommand* RSVerInput(float& fPower, float& fAngle)
	{
		fPower = 0.0f;
		fAngle = 0.0f;

		if (m_pXInput->isConnected())
		{
			fPower = m_pXInput->GetRStickY();
			fAngle = m_pXInput->GetRStickTheta();
		}

		else if (m_pDxInput)
		{
			fPower = m_pDxInput->GetVerRSPush();
			fAngle = m_pDxInput->GetRSDir();
		}

		if (abs(fPower) < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComRSVer;
	}

	clsRoboCommand* MoveSwitch()
	{
		if (m_pDxInput->IsPressKey(enPKey_00))
		{
			if (!m_bChangeSwitch)
			{
				m_bChangeSwitch = true;
				return m_pMoveSwitch;
			}
		}

		else
		{
			m_bChangeSwitch = false;
		}
		
		return nullptr;
	}

	clsRoboCommand* QuickBoost(float fPower)
	{
		if (abs(fPower) > g_fStickPushMin)
		{
			if (m_pXInput->isConnected())
			{
				if (m_pXInput->isLTriggerEnter())
				{
					return m_pQuickBoost;
				}
			}

			else if (m_pDxInput)
			{
				if (m_pDxInput->IsPressKey(enPKey_02))
				{
					return m_pQuickBoost;
				}
			}
		}

		return nullptr;
	}

	clsRoboCommand* QuickTurn(float fPower)
	{
		if (abs(fPower) > g_fStickPushMin)
		{
			if (m_pXInput->isConnected())
			{
				if (m_pXInput->isLTriggerEnter())
				{
					return m_pQuickTurn;
				}
			}

			else if (m_pDxInput)
			{
				if (m_pDxInput->IsPressKey(enPKey_02))
				{
					return m_pQuickTurn;
				}
			}
		}

		return nullptr;
	}

	clsRoboCommand* BoostRising()
	{
		if (m_pXInput->isConnected())
		{
			if (m_pXInput->isRTriggerStay())
			{
				return m_pBoostRising;
			}
		}

		else if (m_pDxInput)
		{
			if (m_pDxInput->IsPressKey(enPKey_00))
			{
				return m_pBoostRising;
			}
		}

		return nullptr;
	}

	clsRoboCommand* LWeaponShot()
	{
		if (m_pXInput->isConnected())
		{
			if (m_pXInput->isPressStay(XINPUT_LB))
			{
				return m_pComLShot;
			}
		}

		else if(m_pDxInput)
		{
			if (m_pDxInput->IsPressKey(enPKey_05))
			{
				return m_pComLShot;
			}
		}

		return nullptr;
	}

	clsRoboCommand* RWeaponShot()
	{
		if (m_pXInput->isConnected())
		{
			if (m_pXInput->isPressStay(XINPUT_RB))
			{
				return m_pComRShot;
			}
		}

		else if (m_pDxInput)
		{
			if (m_pDxInput->IsPressKey(enPKey_06))
			{
				return m_pComRShot;
			}
		}

		return nullptr;
	}

	clsDxInput* m_pDxInput;
	clsXInput* m_pXInput;

private:
	bool m_bChangeSwitch;

	clsRoboCommand* m_pMoveSwitch;
	clsRoboCommand* m_pQuickBoost;
	clsRoboCommand* m_pQuickTurn;
	clsRoboCommand* m_pBoostRising;

	clsRoboCommand* m_pComLShot;
	clsRoboCommand* m_pComRShot;

	clsRoboCommand* m_pComLS;
	clsRoboCommand* m_pComLSHor;
	clsRoboCommand* m_pComLSVer;

	clsRoboCommand* m_pComRS;
	clsRoboCommand* m_pComRSHor;
	clsRoboCommand* m_pComRSVer;
};

