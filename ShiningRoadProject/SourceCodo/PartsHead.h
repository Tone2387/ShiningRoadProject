#ifndef PARTS_HEAD_H_
#define PARTS_HEAD_H_


#include "PartsBase.h"

//----- 頭 -----//.
#define sBONE_NAME_HEAD_CENTER "Center"


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

	enum enHEAD_BONE_POSITIONS
	{
		enHEAD_BONE_POSITIONS_CENTER = 0,
			   
		enHEAD_BONE_POSITIONS_size
	};

	//直前のフレームのボーンの座標を持ってくる.
	D3DXVECTOR3 GetBonePosPreviosFrame( 
		const int enBoneName, 
		int iVecNum = 0 ) override;
	//↑で使うためにボーン座標を記録する( Renderの直後に使う ).
	void UpdateBonePosPreviosFrame() override{
		m_BonePositions.vCenter	= this->GetBonePos( sBONE_NAME_HEAD_CENTER );
	};

private:
	//----- 各パーツごとの関数 -----//.
	void InitProduct() final;//各シーンのInit.
	void UpdateProduct() final;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.

	struct HEAD_BONE_POSITIONS
	{
		D3DXVECTOR3 vCenter;
	}	m_BonePositions;

};

#endif//#ifndef PARTS_HEAD_H_