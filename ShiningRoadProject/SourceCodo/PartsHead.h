#ifndef PARTS_HEAD_H_
#define PARTS_HEAD_H_


#include "PartsBase.h"



class clsPARTS_HEAD : public clsPARTS_BASE
{
public:
	clsPARTS_HEAD();
	~clsPARTS_HEAD();

	enum enSTATUS : PARTS_STATUS_TYPE
	{
		HP = 0,	//体力.

		SEARCH,		//索敵性能.
		LOCK_ON_SPEED,	//ロック速度.
		LOCK_ON_RANGE,	//ロック距離.

		COL_SIZE,	//当たり判定サイズ.

		size
	};



private:
	//----- 各パーツごとの関数 -----//.
	void CreateProduct() final;//各シーンのCreate.
	void UpdateProduct() final;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.


};

#endif//#ifndef PARTS_HEAD_H_