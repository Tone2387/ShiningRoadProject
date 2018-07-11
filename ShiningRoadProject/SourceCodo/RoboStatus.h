#pragma once 

#include "Global.h"

#include <vector>

//アセンブルシーンでの選択肢の型.
//#define ASSEMBLE_SCENE_SELECT_TYPE short 
using ASSEMBLE_SCENE_SELECT_TYPE = short;


//ゲームクラスで作成、.
//アセンブルシーンで値を設定、.
//ミッショシーンで初期化の段階でロボに渡す.
class clsROBO_STATUS
{
public:
	clsROBO_STATUS();
	~clsROBO_STATUS();

	//まっさらにする.
	void Clear();

	//データの受け取り.
	//これらの関数でデータを受け取る前にClear()しておくこと.
	//第一引数 : 攻撃力などのパラメータ群.
	//第二引数 : モデル番号.
	void ReceiveLeg( const std::vector<int> &LegDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveCore( const std::vector<int> &CoreDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveHead( const std::vector<int> &HeadDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveArms( const std::vector<int> &ArmsDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveWeaponL( const std::vector<int> &WeaponLDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveWeaponR( const std::vector<int> &WeaponRDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	 
	//ロボのステータス配列の引数.
	enum enROBO_STATE : UCHAR
	{
		HP = 0,		//体力.
		WALK_SPD,	//歩行速度.
		STABILITY,	//安定性能.
		TURN,		//旋回性能.
		JUMP_POWER,	//ジャンプ力.

		EN_CAPA,		//エネルギー容量 エネルギー : energy  容量 : capacity.
		EN_OUTPUT,		//エネルギー出力( 回復量 ).
		BOOST_THRUST_H,	//ブースター推力( 水平 ).
		BOOST_COST_H,	//ブースター消費エネルギー( 水平 ).
		BOOST_THRUST_V,	//ブースター推力( 水直 ).
		BOOST_COST_V,	//ブースター消費エネルギー( 水直 ).
		QUICK_THRUST,	//クイック推力.
		QUICK_COST,		//クイック消費エネルギー.
		QUICK_TIME,		//クイック噴射時間.
		ACT_TIME,		//活動時間(ActivityTime).

		SEARCH,	//索敵性能.

		AIMING,	//照準精度( エイム ).

		COL_SIZE_LEG,	//脚当たり判定サイズ.
		COL_SIZE_CORE,	//コア当たり判定サイズ.
		COL_SIZE_HEAD,	//頭当たり判定サイズ.
		COL_SIZE_ARMS,	//腕当たり判定サイズ.

		enROBO_STATE_SIZE
	};

	//武器のステータス配列の引数.
	enum enWEAPON_STATE : UCHAR
	{
		ATK = 0,			//攻撃力.
		BULLET_SPD,			//弾速.
		RANGE,				//射程距離.
		COST,				//消費エネルギー.
		LOAD_TIME,			//装填時間.
		LOCK_SPD,			//ロック速度.
		LOCK_RANGE,			//ロック距離.
		SHOT_STABILITY,		//射撃安定.
		MAGAZINE_LOAD_TIME,	//マガジン装填時間.
		BULLETS_NUM,		//装弾数.

		COL_SIZE,	//弾の当たり判定サイズ.
		EFC_BULLET,	//弾本体のエフェクト.
		EFC_LOCUS,	//弾の軌跡のエフェクト.
		EFC_HIT,	//着弾のエフェクト.
		SE_FIER,	//発射SE.
		SE_HIT,		//着弾のSE.

		enWEAPON_STATE_SIZE
	};

	//武器のステータス左右の引数.
	enum enWEAPON_NUM : UCHAR
	{
		LEFT = 0,
		RIGHT= 1,

		enWEAPON_NUM_SIZE
	};


	//ロボの情報を吐き出す.
	int GetRoboState( const enROBO_STATE enStateNum );
	//武器の情報を吐き出す.
	int GetWeaponState( const enWEAPON_NUM enArmLR, const enWEAPON_STATE enStateNum ) const;

	//パーツ番号を返す( いま装備しているパーツが何番か ).//#define SKIN_ENUM_TYPE UCHAR.
	UCHAR GetPartsNum( const enPARTS PartsType );

private:

	//HPを持つパーツ.
	enum enHAVE_HP_PARTS
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,

		enHAVE_HP_PARTS_SIZE
	};

	//ロボット本体のパラメータ.
	int m_iRoboState[enROBO_STATE_SIZE];

	//武器のパラメータ.
	int m_iWeaponState[enWEAPON_NUM_SIZE][enWEAPON_STATE_SIZE];

	//ロボのHPを後で合算するための変数.
	int m_iRoboHp[enHAVE_HP_PARTS_SIZE];

	//パーツ番号の配列.//#define SKIN_ENUM_TYPE UCHAR.
	UCHAR m_ucPartsModelNum[ static_cast<int>( enPARTS::MAX ) ];
};
