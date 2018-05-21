#pragma once

#include"Robo.h"
#include"CharactorCommand.h"

class clsRoboCommand : public clsCharactorCommand
{
public:
	clsRoboCommand() {};
	~clsRoboCommand() {};

	//ボタン入力.
	virtual void PushBotton(clsRobo* pRobo)
	{

	};

	//ボタン入力.
	virtual void Trigger(clsRobo* pRobo, const float fS, const float fAngle)
	{

	};
};

class clsCommandBoostRising : public clsRoboCommand
{
public:
	virtual void PushBotton(clsRobo* pRobo)
	{
		pRobo->BoostRising();
	}
};

class clsCommandMoveSwitch : public clsRoboCommand
{
public:
	virtual void PushBotton(clsRobo* pRobo)
	{
		pRobo->MoveSwitch();
	}
};

class clsCommandQuickBoost : public clsRoboCommand
{
public:
	virtual void PushBotton(clsRobo* pRobo)
	{
		pRobo->QuickBoost();
	}
};