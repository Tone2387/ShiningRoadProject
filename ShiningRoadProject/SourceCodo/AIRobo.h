#pragma once

#include"Robo.h"
#include"AIRoboCommand.h"

class clsAIRobo final : public clsRobo
{
public:
	clsAIRobo();
	~clsAIRobo();

	void Init(clsPOINTER_GROUP* const pPtrGroup,
		unsigned int uiAINum);

	void Action(clsStage* const pStage) final;
	void SwitchMove();

private:
	clsEnemyRobo* m_pAI;
	bool m_bAct;
};