#pragma once

#include"Object.h"
#include"Charactor.h"
#include"Robo.h"

class clsObjFactory
{
public:
	clsObjFactory();
	~clsObjFactory();

	enum enObj
	{
		enObjObject = 0,
		enObjCharactor,
		enObjRobo,

		enObjMax
	};

	clsObject* Create(enObj enObjType);

private:

};

clsObjFactory::clsObjFactory()
{
}

clsObjFactory::~clsObjFactory()
{
}