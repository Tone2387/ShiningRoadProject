#ifndef PARTS_ARM_BASE_H_
#define PARTS_ARM_BASE_H_

#include "PartsBase.h"



class clsPARTS_ARM_BASE : public clsPARTS_BASE
{
public:
	clsPARTS_ARM_BASE();
	virtual ~clsPARTS_ARM_BASE();


	//ステータス変数の添え字.
	enum enSTATUS : PARTS_STATUS_TYPE
	{
		HP = 0,	//体力.

		AIMING,	//照準精度( エイム ).
		QUICK_THRUST,	//クイック推力.
		QUICK_COST,		//クイック消費エネルギー.
		QUICK_TIME,		//クイック噴射時間.
						
		COL_SIZE,	//当たり判定サイズ.

		size
	};


private:
	//----- 各パーツごとの関数 -----//.
	virtual void CreateProduct() override;//各シーンのCreate.
	virtual void UpdateProduct() override;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.


};

#endif//#ifndef PARTS_ARM_BASE_H_