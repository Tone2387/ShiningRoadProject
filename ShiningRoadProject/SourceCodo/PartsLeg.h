#pragma once


#include "PartsBase.h"


class clsPARTS_LEG : public clsPARTS_BASE
{
public:
	clsPARTS_LEG();
	~clsPARTS_LEG();

	//ステータス変数の添え字.
	enum enSTATUS : PARTS_STATUS_TYPE
	{
		HP = 0,		//体力.
		WALK_SPD,	//歩行速度.
		STABILITY,	//安定性能.
		JUMP_POWER,	//ジャンプ力.
						
		size
	};


private:
	//----- 各パーツごとの関数 -----//.
	void CreateProduct() final;//各シーンのCreate.
	void UpdateProduct() final;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.



};

