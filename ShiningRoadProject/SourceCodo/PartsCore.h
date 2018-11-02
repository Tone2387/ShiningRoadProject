#ifndef PARTS_CORE_H_
#define PARTS_CORE_H_



#include "PartsBase.h"

//----- コア -----//.
#define sBONE_NAME_CORE_NULL		"null"
//ブースター( 00 ～ xx ).
#define sBONE_NAME_CORE_BOOSTER_ROOT "BoosterRoot"
#define sBONE_NAME_CORE_BOOSTER_END	 "BoosterEnd"
//モデルの中心.
#define sBONE_NAME_CORE_JENERATOR	"Jenerator"
//連結部分のボーン名.
#define sBONE_NAME_CORE_TO_HEAD		"JunctionHead"
#define sBONE_NAME_CORE_TO_ARM_L	"JunctionArmL"
#define sBONE_NAME_CORE_TO_ARM_R	"JunctionArmR"


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


	enum enCORE_BONE_POSITIONS : int
	{
		enCORE_BONE_POSITIONS_BOOSTER_ROOT = 0,
		enCORE_BONE_POSITIONS_BOOSTER_END,
		enCORE_BONE_POSITIONS_JENERATOR,
		enCORE_BONE_POSITIONS_JUNCTION_HEAD,
		enCORE_BONE_POSITIONS_JUNCTION_ARM_L,
		enCORE_BONE_POSITIONS_JUNCTION_ARM_R,
		
		enCORE_STATUS_size
	};

	//直前のフレームのボーンの座標を持ってくる.
	D3DXVECTOR3 GetBonePosPreviosFrame( 
		const int enBoneName, 
		int iVecNum = 0 ) override;
	//↑で使うためにボーン座標を記録する( Renderの直後に使う ).
	void UpdateBonePosPreviosFrame() override;

private:
	//----- 各パーツごとの関数 -----//.
	void InitProduct() final;//各シーンのInit.
	void UpdateProduct() final;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.

	struct CORE_BONE_POSITIONS
	{
		std::vector<D3DXVECTOR3> vecvBoosterRoot;
		std::vector<D3DXVECTOR3> vecvBoosterEnd;
		D3DXVECTOR3				 vJenerator;
		D3DXVECTOR3				 vJunctionHead;
		D3DXVECTOR3				 vJunctionArmL;
		D3DXVECTOR3				 vJunctionArmR;
	}	m_BonePositions;


};

#endif//#ifndef PARTS_CORE_H_