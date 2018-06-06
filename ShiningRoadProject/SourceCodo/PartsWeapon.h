#pragma once

#include "PartsBase.h"


class clsPARTS_WEAPON : public clsPARTS_BASE
{
public:
	clsPARTS_WEAPON();
	~clsPARTS_WEAPON();

	//Xe[^XฯฬYฆ.
	enum enSTATUS : PARTS_STATUS_TYPE
	{
		ATK = 0,			//Uอ.
		BULLET_SPD,			//eฌ.
		RANGE,				//ห๖ฃ.
		COST,				//ม๏GlM[.
		LOAD_TIME,			//Uิ.
		LOCK_SPD,			//bNฌx.
		LOCK_RANGE,			//bNฃ.

		REACTION,			//หฝฎ.
		ACCURACY,			//หธx.
		MAGAZINE_LOAD_TIME,	//}KWUิ.
		BULLETS_NUM,		//e.
						
		size
	};



private:
	//----- ep[cฒฦฬึ -----//.
	void CreateProduct() final;//eV[ฬCreate.
	void UpdateProduct() final;//eV[ฬUpdate.
	//----- ep[cฒฦฬึ -----//.


};

