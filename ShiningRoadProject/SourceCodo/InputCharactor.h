#pragma once

#include"CharactorCommand.h"
#include"DxInput.h"

const float g_fStickPushMin = 0.5f;

class clsInputCharctor
{
public:
	clsInputCharctor::clsInputCharctor()
	{
		m_pDxInput = new clsDxInput;

		m_pComLS = new clsCommandMove;
		m_pComRSHor = new clsCommandRotation;
		m_pComRSVer = new clsCommandLookUp;
		m_pComJump = new clsCommandJump;
		m_pComShot = new clsCommandShot;
	}

	virtual clsInputCharctor::~clsInputCharctor()
	{
		delete m_pDxInput;
		delete m_pComLS;
		delete m_pComRSHor;
		delete m_pComRSVer;
		delete m_pComJump;
		delete m_pComShot;
	}

	clsDxInput* m_pDxInput;

	clsCharactorCommand* LSInput(float& fPower, float& fAngle)
	{
		fPower = m_pDxInput->GetLSPush();
		fAngle = m_pDxInput->GetLSDir();

		bool bNaname = false;

		if (GetAsyncKeyState('W') & 0x8000)
		{
			fPower = 1.0f;
			bNaname = true;
		}

		else if (GetAsyncKeyState('S') & 0x8000)
		{
			fPower = 1.0f;
			fAngle += -D3DX_PI;
			bNaname = true;
		}

		if (GetAsyncKeyState('A') & 0x8000)
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

		else if (GetAsyncKeyState('D') & 0x8000)
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

		if (fPower < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComLS;
	};

	clsCharactorCommand* LSHorInput(float& fPower, float& fAngle)
	{
		fPower = m_pDxInput->GetHorLSPush();
		fAngle = m_pDxInput->GetLSDir();

		if (fPower < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComLS;
	}

	clsCharactorCommand* LSVerInput(float& fPower, float& fAngle)
	{
		fPower = m_pDxInput->GetVerLSPush();
		fAngle = m_pDxInput->GetLSDir();

		if (fPower < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComLS;
	}

	clsCharactorCommand* RSInput(float& fPower, float& fAngle)
	{
		fPower = m_pDxInput->GetRSPush();
		fAngle = m_pDxInput->GetRSDir();

		if (fPower < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComRSHor;
	};

	clsCharactorCommand* RSHorInput(float& fPower, float& fAngle)
	{
		fPower = m_pDxInput->GetHorRSPush();
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

		if (fPower < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComRSHor;
	};

	clsCharactorCommand* RSVerInput(float& fPower, float& fAngle)
	{
		fPower = m_pDxInput->GetVerRSPush();
		fAngle = m_pDxInput->GetRSDir();

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			fPower = 1.0f;
			fAngle = 0.1f;
		}

		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			fPower = -1.0f;
			fAngle = -0.1f;
		}

		if (fPower < g_fStickPushMin)
		{
			return nullptr;
		}

		return m_pComRSVer;
	};

	virtual clsCharactorCommand* Jump()
	{
		if (m_pDxInput->IsPressKey(enPKey_03) ||
			GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			return m_pComJump;
		}

		return nullptr;
	}

private:
	clsCharactorCommand* m_pComLS;
	clsCharactorCommand* m_pComRSHor;
	clsCharactorCommand* m_pComRSVer;

	clsCharactorCommand * m_pComJump;

	clsCharactorCommand* m_pComShot;
};

