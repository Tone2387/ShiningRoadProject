//#pragma once
#ifndef ROBOCOMMAND
#define ROBOCOMMAND

#include"Robo.h"
#include"CharactorCommand.h"

class clsRoboCommand
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
	virtual void Trigger(clsRobo* pRobo, const float fS, const float fAngle)
	{
		pRobo->SetDirQuickBoost(fAngle);
	}

	virtual void PushBotton(clsRobo* pRobo)
	{
		pRobo->QuickBoost();
	}
};

class clsCommandQuickTurn : public clsRoboCommand
{
public:
	virtual void Trigger(clsRobo* pRobo, const float fS, const float fAngle)
	{
		pRobo->SetDirQuickTurn(fAngle);
	}

	virtual void PushBotton(clsRobo* pRobo)
	{
		pRobo->QuickTurn();
	}
};

class clsCommandRoboRotate : public clsRoboCommand
{
public:
	virtual void Trigger(clsRobo* pRobo, const float fS, const float fAngle)
	{
		pRobo->Rotate(fAngle, fS);
	}

};

class clsCommandRoboMove : public clsRoboCommand
{
public:
	virtual void Trigger(clsRobo* pRobo, const float fS, const float fAngle)
	{
		pRobo->Move(fAngle, fS);
	}
};

#endif//#ifndef ROBOCOMMAND