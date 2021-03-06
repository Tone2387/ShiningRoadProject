#ifndef PARTS_ARM_R_H_
#define PARTS_ARM_R_H_


#include "PartsArmBase.h"


class clsPARTS_ARM_R : public clsPARTS_ARM_BASE
{
public:
	clsPARTS_ARM_R();
	~clsPARTS_ARM_R();

private:
	//----- 各パーツごとの関数 -----//.
	void InitProduct()	final{ clsPARTS_ARM_BASE::InitProduct(); };
	void UpdateProduct()final{ clsPARTS_ARM_BASE::UpdateProduct(); };
	//----- 各パーツごとの関数 -----//.

};

#endif//#ifndef PARTS_ARM_R_H_