#ifndef PARTS_ARM_BASE_H_
#define PARTS_ARM_BASE_H_

#include "PartsBase.h"


//----- 腕 -----//.
//ブースター( 00 ～ xx ).
#define sBONE_NAME_ARM_BOOSTER_FRONT_ROOT	"BoosterFrontRoot"
#define sBONE_NAME_ARM_BOOSTER_FRONT_END	"BoosterFrontEnd"
#define sBONE_NAME_ARM_BOOSTER_BACK_ROOT	"BoosterBackRoot"
#define sBONE_NAME_ARM_BOOSTER_BACK_END		"BoosterBackEnd"
#define sBONE_NAME_ARM_BOOSTER_SIDE_ROOT	"BoosterSideRoot"
#define sBONE_NAME_ARM_BOOSTER_SIDE_END		"BoosterSideEnd"
//連結部分のボーン名.
#define sBONE_NAME_ARM_TO_WEAPON			"JunctionWeapon"
//武器の回転情報確定に使う.
#define sBONE_NAME_ARM_WEAPON_VEC_ROOT		sBONE_NAME_ARM_TO_WEAPON	
#define sBONE_NAME_ARM_WEAPON_VEC_END		"WeaponVec"		

//関節( 00 ～ xx ).
#define sBONE_NAME_ARM_JOINT				"Joint"


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


	enum enARMS_BONE_POSITIONS
	{
		enARMS_BONE_POSITIONS_BOOSTER_FRONT_ROOT = 0,
		enARMS_BONE_POSITIONS_BOOSTER_FRONT_END,
		enARMS_BONE_POSITIONS_BOOSTER_BACK_ROOT,
		enARMS_BONE_POSITIONS_BOOSTER_BACK_END,
		enARMS_BONE_POSITIONS_BOOSTER_SIDE_ROOT,
		enARMS_BONE_POSITIONS_BOOSTER_SIDE_END,
			   
		enARMS_BONE_POSITIONS_JOINT,
		enARMS_BONE_POSITIONS_JUNCTION_WEAPON,
		enARMS_BONE_POSITIONS_WEAPON_VEC,
			   
		enARMS_BONE_POSITIONS_size
	};


	//直前のフレームのボーンの座標を持ってくる.
	D3DXVECTOR3 GetBonePosPreviosFrame( 
		const int enBoneName, 
		int iVecNum = 0 ) override;
	//↑で使うためにボーン座標を記録する( Renderの直後に使う ).
	void UpdateBonePosPreviosFrame() override;

protected:


	struct ARMS_BONE_POSITIONS
	{
		std::vector<D3DXVECTOR3> vecvBoosterFontRoot;
		std::vector<D3DXVECTOR3> vecvBoosterFontEnd;
		std::vector<D3DXVECTOR3> vecvBoosterBackRoot;
		std::vector<D3DXVECTOR3> vecvBoosterBackEnd;
		std::vector<D3DXVECTOR3> vecvBoosterSideRoot;
		std::vector<D3DXVECTOR3> vecvBoosterSideEnd;
		std::vector<D3DXVECTOR3> vecvJoint;
		D3DXVECTOR3				 vJunctionWeapon;
		D3DXVECTOR3				 vWeaponVec;
	}	m_BonePositions;


private:
	//----- 各パーツごとの関数 -----//.
	virtual void InitProduct() override;//各シーンのInit.
	virtual void UpdateProduct() override;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.


};

#endif//#ifndef PARTS_ARM_BASE_H_