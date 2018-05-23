#pragma once

#include "PartsArmL.h"
#include "PartsArmR.h"


class clsPARTS_ARMS
{
public:
	clsPARTS_ARMS();
	~clsPARTS_ARMS();

private:
	clsPARTS_ARM_L* m_pL;
	clsPARTS_ARM_R* m_pR;


};

