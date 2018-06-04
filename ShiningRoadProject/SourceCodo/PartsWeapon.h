#pragma once

#include "PartsBase.h"


class clsPARTS_WEAPON : public clsPARTS_BASE
{
public:
	clsPARTS_WEAPON();
	~clsPARTS_WEAPON();

	//ステータス変数の添え字.
	enum enSTATUS : PARTS_STATUS_TYPE
	{
		ATK = 0,			//攻撃力.
		BULLET_SPD,			//弾速.
		RANGE,				//射程距離.
		COST,				//消費エネルギー.
		LOAD_TIME,			//装填時間.
		LOCK_SPD,			//ロック速度.
		LOCK_RANGE,			//ロック距離.

		REACTION,			//射撃反動.
		ACCURACY,			//射撃精度.
		MAGAZINE_LOAD_TIME,	//マガジン装填時間.
		BULLETS_NUM,		//装弾数.
						
		MAX
	};



private:
	//----- 各パーツごとの関数 -----//.
	void CreateProduct() final;//各シーンのCreate.
	void UpdateProduct() final;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.


};

