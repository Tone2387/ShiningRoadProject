#ifndef PARTS_ARM_L_H_
#define PARTS_ARM_L_H_


#include "PartsArmBase.h"



class clsPARTS_ARM_L : public clsPARTS_ARM_BASE
{
public:
	clsPARTS_ARM_L();
	~clsPARTS_ARM_L();

private:
	//----- 各パーツごとの関数 -----//.
	void InitProduct()	final{ clsPARTS_ARM_BASE::InitProduct(); };
	void UpdateProduct()final{ clsPARTS_ARM_BASE::UpdateProduct(); };
	//----- 各パーツごとの関数 -----//.

};

#endif//#ifndef PARTS_ARM_L_H_