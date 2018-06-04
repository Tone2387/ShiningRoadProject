#pragma once


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
		BOOST_THRUST,	//ブースター推力.
		BOOST_COST,		//ブースター消費エネルギー.
		QUICK_THRUST,	//クイック推力.
		QUICK_COST,		//クイック消費エネルギー.
		ACT_TIME,		//活動時間(ActivityTime).
		QUICKTIME,		//クイック噴射時間.
						
		MAX
	};


private:
	//----- 各パーツごとの関数 -----//.
	void CreateProduct() final;//各シーンのCreate.
	void UpdateProduct() final;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.


};

