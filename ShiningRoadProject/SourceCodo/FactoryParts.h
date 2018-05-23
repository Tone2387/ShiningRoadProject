#pragma once


#include "PartsLeg.h"
#include "PartsCore.h"
#include "PartsHead.h"
//#include "PartsArms.h"
#include "PartsArmL.h"
#include "PartsArmR.h"
#include "PartsWeapon.h"

//フラグに応じてパーツを吐き出すクラス.
class clsFACTORY_PARTS
{
public:
//	clsFACTORY_PARTS();
//	~clsFACTORY_PARTS();

	//パーツを吐き出す.
	clsPARTS_BASE* Create( const enPARTS enParts );

};

