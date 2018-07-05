#pragma once

#include"CharactorCommand.h"
#include"DxInput.h"

class clsInputCharctor
{
public:
	clsInputCharctor::clsInputCharctor()
	{
		m_pDxInput = new clsDxInput;

		m_pComLS = new clsCommandLS;
		m_pComRS = new clsCommandRS;
		m_pComJump = new clsCommandJump;
		m_pComShot = new clsCommandShot;
	}

	virtual clsInputCharctor::~clsInputCharctor()
	{
		delete m_pDxInput;
		delete m_pComLS;
		delete m_pComRS;
		delete m_pComJump;
		delete m_pComShot;
	}

	clsDxInput* m_pDxInput;

	/*clsCharactorCommand* PlressInput()
	{
		m_pDxInput->UpdataInputState();

		return nullptr;
	}*/

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



		return m_pComLS;
	};

	clsCharactorCommand* RSInput(float& fPower, float& fAngle)
	{
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

	virtual clsCharactorCommand* Jump()
	{
		if (m_pDxInput->IsPressKey(enPKey_03) ||
			GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			return m_pComJump;
		}

		return nullptr;
	}

protected:
	clsCharactorCommand * m_pComJump;

private:
	clsCharactorCommand* m_pComLS;
	clsCharactorCommand* m_pComRS;

	clsCharactorCommand* m_pComShot;
};

