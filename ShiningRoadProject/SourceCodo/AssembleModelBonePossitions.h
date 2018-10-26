#ifndef ASSEMBLE_MODEL_BONE_POSITIONS_H_
#define ASSEMBLE_MODEL_BONE_POSITIONS_H_

#include "Global.h"


//----- 足 -----//.
//基点のボーン名.
#define sBONE_NAME_LEG_POSITION_BASE "PositionBase"
//連結部分のボーン名.
#define sBONE_NAME_LEG_TO_CORE		"JunctionCore"
//足の裏( 0 ～ x ).
#define sBONE_NAME_LEG_SOLE			"Sole"
//連結部分のボーン名.
#define sBONE_NAME_LEG_TO_CORE		"JunctionCore"

//----- コア -----//.
//モデルの中心.
#define sBONE_NAME_CORE_JENERATOR	"Jenerator"
//連結部分のボーン名.
#define sBONE_NAME_CORE_TO_HEAD		"JunctionHead"
#define sBONE_NAME_CORE_TO_ARM_L	"JunctionArmL"
#define sBONE_NAME_CORE_TO_ARM_R	"JunctionArmR"

//----- 頭 -----//.
#define sBONE_NAME_HEAD_CENTER		"Center"

//----- 腕 -----//.
//ブースター( 00 ～ xx ).
#define sBONE_NAME_FRONT_ROOT		"BoosterFrontRoot"
#define sBONE_NAME_FRONT_END		"BoosterFrontEnd"
#define sBONE_NAME_BACK_ROOT		"BoosterBackRoot"
#define sBONE_NAME_BACK_END			"BoosterBackEnd"
#define sBONE_NAME_SIDE_ROOT		"BoosterSideRoot"
#define sBONE_NAME_SIDE_END			"BoosterSideEnd"
//連結部分のボーン名.
#define sBONE_NAME_ARM_TO_WEAPON	"JunctionWeapon"
//武器の回転情報確定に使う.
#define sBONE_NAME_ARM_WEAPON_VEC_ROOT	 sBONE_NAME_ARM_TO_WEAPON	
#define sBONE_NAME_ARM_WEAPON_VEC_END	 "WeaponVec"		

//----- 武器 -----//.
#define sBONE_NAME_WEAPON_MUZZLE_ROOT	"MuzzleRoot"
#define sBONE_NAME_WEAPON_MUZZLE_END	"MuzzleEnd"
#define sBONE_NAME_WEAPON_CARTRIDGE_INJECTION "CartridgeInjection"



//----- 全パーツ共通 -----//.
#define sBONE_NAME_NULL			"null"


//----- 脚とコア共通 -----//.
//ブースター( 00 ～ xx ).
#define sBONE_NAME_BOOSTER_ROOT	"BoosterRoot"
#define sBONE_NAME_BOOSTER_END	"BoosterEnd"

//----- 脚と腕共通 -----//.
//関節( 00 ～ xx ).
#define sBONE_NAME_JOINT		"Joint"


class clsASSEMBLE_MODEL_BONE_POSITIONS
{
public:
	clsASSEMBLE_MODEL_BONE_POSITIONS();
	~clsASSEMBLE_MODEL_BONE_POSITIONS();

	enum enPARTS_INDEX : int
	{
		enPARTS_INDEX_LEG = 0,
		enPARTS_INDEX_CORE,
		enPARTS_INDEX_HEAD,
		enPARTS_INDEX_ARM_L,
		enPARTS_INDEX_ARM_R,
		enPARTS_INDEX_WEAPON_L,
		enPARTS_INDEX_WEAPON_R,

		enPARTS_INDEX_size
	};


private:

};

#endif//#ifndef ASSEMBLE_MODEL_BONE_POSITIONS_H_