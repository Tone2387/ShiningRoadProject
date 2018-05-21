#pragma once

#include"InputCharactor.h"
#include"RoboCommand.h"



class clsInputRobo : public clsInputCharctor
{
public:
	clsInputRobo::clsInputRobo()
	{
		m_pMoveSwitch = new clsCommandMoveSwitch;
		m_pQuickBoost = new	clsCommandQuickBoost;
		m_pBoostRising = new clsCommandBoostRising;
	}

	clsInputRobo::~clsInputRobo()
	{
		delete m_pBoostRising;
		delete m_pQuickBoost;
		delete m_pMoveSwitch;
	}	

	clsRoboCommand* MoveSwitch()
	{
		if (m_pDxInput->IsPressKey(enPKey_00) ||
			GetAsyncKeyState(VK_F1) & 0x8000)
		{
			return m_pMoveSwitch;
		}

		return nullptr;
	}

	clsRoboCommand* QuickBoost()
	{
		if (m_pDxInput->IsPressKey(enPKey_01) ||
			GetAsyncKeyState(VK_F2) & 0x8000)
		{
			return m_pQuickBoost;
		}

		return nullptr;
	}

	clsRoboCommand* BoostRising()
	{
		clsCharactorCommand* pCharaCom = Jump();
		if(pCharaCom)
		{
			return m_pBoostRising;
		}

		return nullptr;
	}

private:
	clsCommandMoveSwitch* m_pMoveSwitch;
	clsCommandQuickBoost* m_pQuickBoost;
	clsCommandBoostRising* m_pBoostRising;
};

