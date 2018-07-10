#pragma once

#include "PartsBase.h"



class clsPARTS_HEAD : public clsPARTS_BASE
{
public:
	clsPARTS_HEAD();
	~clsPARTS_HEAD();

	enum enSTATUS : PARTS_STATUS_TYPE
	{
		HP = 0,	//体力.
		SEARCH,	//索敵性能.

		COL_SIZE,	//当たり判定サイズ.

		size
	};



private:
	//----- 各パーツごとの関数 -----//.
	void CreateProduct() final;//各シーンのCreate.
	void UpdateProduct() final;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.


};

