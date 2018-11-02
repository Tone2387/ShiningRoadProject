#ifndef PARTS_WEAPON_H_
#define PARTS_WEAPON_H_



#include "PartsBase.h"

//----- 武器 -----//.
#define sBONE_NAME_WEAPON_MUZZLE_ROOT			"MuzzleRoot"
#define sBONE_NAME_WEAPON_MUZZLE_END			"MuzzleEnd"
#define sBONE_NAME_WEAPON_CARTRIDGE_INJECTION	"CartridgeInjection"




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
		LOCK_ON_TIME,		//ロック時間.
		STABILITY,			//射撃安定.
		MAGAZINE_LOAD_TIME,	//マガジン装填時間.
		BULLETS_NUM,		//装弾数.

		COL_SIZE,	//弾の当たり判定サイズ.
		EFC_BULLET,	//弾本体のエフェクト.
		EFC_LOCUS,	//弾の軌跡のエフェクト.
		EFC_HIT,	//着弾のエフェクト.
		EFC_CARTRIDGE,//薬莢エフェクト.
		SE_FIER,	//発射SE.
		SE_HIT,		//着弾のSE.

		size
	};

	enum enWEAPON_BONE_POSITIONS
	{
		enWEAPON_BONE_POSITIONS_MUZZLE_ROOT,
		enWEAPON_BONE_POSITIONS_MUZZLE_END,
		enWEAPON_BONE_POSITIONS_CARTRIDGE_INJECTION,
				 
		enWEAPON_BONE_POSITIONS_size
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

	struct WEAPON_BONE_POSITIONS
	{
		D3DXVECTOR3 vMuzzleRoot;
		D3DXVECTOR3 vMuzzleEnd;
		D3DXVECTOR3 vCartridgeInjection;
	}	m_BonePositions;


};

#endif//PARTS_WEAPON_H_