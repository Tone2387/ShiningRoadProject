#ifndef PARTS_ARM_L_H_
#define PARTS_ARM_L_H_


#include "PartsArmBase.h"



class clsPARTS_ARM_L : public clsPARTS_ARM_BASE
{
public:
	clsPARTS_ARM_L();
	~clsPARTS_ARM_L();

private:
	//----- �e�p�[�c���Ƃ̊֐� -----//.
	void InitProduct()	final{ clsPARTS_ARM_BASE::InitProduct(); };
	void UpdateProduct()final{ clsPARTS_ARM_BASE::UpdateProduct(); };
	//----- �e�p�[�c���Ƃ̊֐� -----//.

};

#endif//#ifndef PARTS_ARM_L_H_