#pragma once

#include "PartsLeg.h"
#include "PartsCore.h"
#include "PartsHead.h"
//#include "PartsArms.h"
#include "PartsArmL.h"
#include "PartsArmR.h"
#include "PartsWeapon.h"

//========== パーツの集合体 ==========//.
class clsPARTS_SET
{
public:
	clsPARTS_SET();
	~clsPARTS_SET();

private:

	clsPARTS_LEG*		m_pLeg;
	clsPARTS_CORE*		m_pCore;
	clsPARTS_HEAD*		m_pHead;
//	clsPARTS_ARMS*		m_pArms;
	clsPARTS_ARM_L*		m_pArmL;
	clsPARTS_ARM_R*		m_pArmR;
	clsPARTS_WEAPON*	m_pWeapon;

};

