#pragma once

#include"Charactor.h"

class clsCharactorCommand
{
public:
	clsCharactorCommand(){};
	~clsCharactorCommand(){};

	//ボタン入力.
	virtual void PushBotton(clsCharactor* pChara)
	{

	};

	//ボタン入力.
	virtual void Trigger(clsCharactor* pChara, const float fS, const float fAngle)
	{

	};
};

class clsCommandRotation : public clsCharactorCommand
{
public:
	virtual void Trigger(clsCharactor* pChara, const float fS, const float fAngle)
	{
		pChara->AddRotAccele(fAngle, fS);
	}

};

class clsCommandMove : public clsCharactorCommand
{
public:
	virtual void Trigger(clsCharactor* pChara, const float fS, const float fAngle)
	{
		pChara->AddMoveAccele(fAngle, fS);
	}
};

class clsCommandLookUp : public clsCharactorCommand
{
public:
	virtual void Trigger(clsCharactor* pChara, const float fS, const float fAngle)
	{
		pChara->LookUp(fAngle, fS);
	}
};

class clsCommandJump : public clsCharactorCommand
{
public:
	virtual void PushBotton(clsCharactor* pChara)
	{
		pChara->Jump();
	}
};

class clsCommandShot : public clsCharactorCommand
{
public:
	virtual void PushBotton(clsCharactor* pChara)
	{
		pChara->Shot();
	}
};
