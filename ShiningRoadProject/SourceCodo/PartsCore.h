#ifndef PARTS_CORE_H_
#define PARTS_CORE_H_



#include "PartsBase.h"



class clsPARTS_CORE : public clsPARTS_BASE
{
public:
	clsPARTS_CORE();
	~clsPARTS_CORE();


	//ステータス変数の添え字.
	enum enSTATUS : PARTS_STATUS_TYPE
	{
		HP = 0,			//体力.

		EN_CAPA,		//エネルギー容量 エネルギー : energy  容量 : capacity.
		EN_OUTPUT,		//エネルギー出力( 回復量 ).
		BOOST_THRUST_H,	//ブースター推力( 水平 ).Horizontal
		BOOST_COST_H,	//ブースター消費エネルギー( 水平 ).
		BOOST_THRUST_V,	//ブースター推力( 水直 ).vertical
		BOOST_COST_V,	//ブースター消費エネルギー( 水直 ).
		ACT_TIME,		//活動時間(ActivityTime).
						
		COL_SIZE,	//当たり判定サイズ.

		size
	};


private:
	//----- 各パーツごとの関数 -----//.
	void CreateProduct() final;//各シーンのCreate.
	void UpdateProduct() final;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.


};

#endif//#ifndef PARTS_CORE_H_