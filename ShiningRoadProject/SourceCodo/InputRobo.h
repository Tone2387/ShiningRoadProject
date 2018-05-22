#pragma once

#include"RoboCommand.h"
#include"DxInput.h"

class clsInputRobo
{
public:
	clsInputRobo::clsInputRobo()
	{
		m_pMoveSwitch = new clsCommandMoveSwitch;
		m_pQuickBoost = new	clsCommandQuickBoost;
		m_pBoostRising = new clsCommandBoostRising;

		m_pDxInput = new clsDxInput;

		m_pComLS = new clsCommandRoboMove;
		m_pComRS = new clsCommandRoboRotate;
	}

	clsInputRobo::~clsInputRobo()
	{
		delete m_pDxInput;
		delete m_pComLS;
		delete m_pComRS;

		delete m_pMoveSwitch;
		delete m_pQuickBoost;
		delete m_pBoostRising;
	}

	clsDxInput* m_pDxInput;

	clsRoboCommand* PlressInput()
	{
		m_pDxInput->UpdataInputState();

		return nullptr;
	}

	clsRoboCommand* LSInput(float& fPower, float& fAngle)
	{
		fPower = 0.0f;
		fAngle = 0.0f;

		fPower = m_pDxInput->GetLSPush();
		fAngle = m_pDxInput->GetLSDir();

		bool bNaname = false;

		if (GetAsyncKeyState('1') & 0x8000)
		{
			fPower = 1.0f;
			bNaname = true;
		}

		else if (GetAsyncKeyState('2') & 0x8000)
		{
			fPower = 1.0f;
			fAngle += -D3DX_PI;
			bNaname = true;
		}

		if (GetAsyncKeyState('3') & 0x8000)
		{
			fPower = 1.0f;
			int iDir;

			if (bNaname)
			{
				iDir = 4;
			}

			else
			{
				iDir = 2;
			}

			if (fAngle < 0.0f)
			{
				fAngle += D3DX_PI / iDir;
			}

			else
			{
				fAngle += -D3DX_PI / iDir;
			}
		}

		else if (GetAsyncKeyState('4') & 0x8000)
		{
			fPower = 1.0f;

			int iDir;

			if (bNaname)
			{
				iDir = 4;
			}

			else
			{
				iDir = 2;
			}

			if (fAngle < 0.0f)
			{
				fAngle += -D3DX_PI / iDir;
			}
			else
			{
				fAngle += D3DX_PI / iDir;
			}

		}

		return m_pComLS;
	};

	clsRoboCommand* RSInput(float& fPower, float& fAngle)
	{
		fPower = 0.0f;
		fAngle = 0.0f;

		fPower = m_pDxInput->GetRSPush();
		fAngle = m_pDxInput->GetRSDir();

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			fPower = 1.0f;
			fAngle = 0.1f;
		}

		else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			fPower = 1.0f;
			fAngle = -0.1f;
		}

		return m_pComRS;
	};

	clsRoboCommand* MoveSwitch()
	{
		if (m_pDxInput->IsPressKey(enPKey_03) ||
			GetAsyncKeyState(VK_F1) & 0x1)
		{
			return m_pMoveSwitch;
		}

		return nullptr;
	}

	clsRoboCommand* QuickBoost()
	{
		if (m_pDxInput->IsPressKey(enPKey_02) ||
			GetAsyncKeyState(VK_F2) & 0x8000)
		{
			return m_pQuickBoost;
		}

		return nullptr;
	}

	clsRoboCommand* BoostRising()
	{
		if (m_pDxInput->IsPressKey(enPKey_00) ||
			GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			return m_pBoostRising;
		}

		return nullptr;
	}

private:
	clsRoboCommand* m_pMoveSwitch;
	clsRoboCommand* m_pQuickBoost;
	clsRoboCommand* m_pBoostRising;

	clsRoboCommand* m_pComLS;
	clsRoboCommand* m_pComRS;
};

