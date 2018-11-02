#ifndef PARTS_LEG_H_
#define PARTS_LEG_H_



#include "PartsBase.h"

//----- 足 -----//.
#define sBONE_NAME_LEG_NULL			"null"
//基点のボーン名.
#define sBONE_NAME_LEG_POSITION_BASE "PositionBase"
//連結部分のボーン名.
#define sBONE_NAME_LEG_TO_CORE		"JunctionCore"
//足の裏( 0 ～ x ).
#define sBONE_NAME_LEG_SOLE			"Sole"
//連結部分のボーン名.
#define sBONE_NAME_LEG_TO_CORE		"JunctionCore"

//ブースター( 00 ～ xx ).
#define sBONE_NAME_LEG_BOOSTER_ROOT	"BoosterRoot"
#define sBONE_NAME_LEG_BOOSTER_END	"BoosterEnd"

//関節( 00 ～ xx ).
#define sBONE_NAME_LEG_JOINT		"Joint"


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
		TURN,		//旋回性能.
		JUMP_POWER,	//ジャンプ力.

		COL_SIZE,	//当たり判定サイズ.
						
		size
	};

	enum enLEG_BONE_POSITIONS : int
	{
		enLEG_BONE_POSITIONS_BOOSTER_ROOT = 0,
		enLEG_BONE_POSITIONS_BOOSTER_END,
		enLEG_BONE_POSITIONS_JOINT,
		enLEG_BONE_POSITIONS_JUNCTION_CORE,
		enLEG_BONE_POSITIONS_SOLE,
		enLEG_BONE_POSITIONS_POSITION_BASE,
		enLEG_BONE_POSITIONS_NULL,
			  
		enLEG_BONE_POSITIONS_size
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

	struct LEG_BONE_POSITIONS
	{
		std::vector<D3DXVECTOR3> vecvBoosterRoot;
		std::vector<D3DXVECTOR3> vecvBoosterEnd;
		std::vector<D3DXVECTOR3> vecvJoint;
		D3DXVECTOR3				 vJunctionCore;
		std::vector<D3DXVECTOR3> vecvSole;
		D3DXVECTOR3				 vPositionBase;
		D3DXVECTOR3				 vNull;
	}	m_BonePositions;


};

#endif//#ifndef PARTS_LEG_H_