#pragma once 

#include "Global.h"

#include <vector>



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
	void ReceiveLeg( const std::vector<int> &LegDatas );
	void ReceiveCore( const std::vector<int> &CoreDatas );
	void ReceiveHead( const std::vector<int> &HeadDatas );
	void ReceiveArms( const std::vector<int> &ArmsDatas );
	void ReceiveWeaponL( const std::vector<int> &WeaponLDatas );
	void ReceiveWeaponR( const std::vector<int> &WeaponRDatas );
	 
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
		BOOST_THRUST,	//ブースター推力.
		BOOST_COST,		//ブースター消費エネルギー.
		QUICK_THRUST,	//クイック推力.
		QUICK_COST,		//クイック消費エネルギー.
		ACT_TIME,		//活動時間(ActivityTime).
		QUIC_KTIME,		//クイック噴射時間.

		SEARCH,	//索敵性能.

		AIMING,	//照準精度( エイム ).

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

		REACTION,			//射撃反動.
		ACCURACY,			//射撃精度.
		MAGAZINE_LOAD_TIME,	//マガジン装填時間.
		BULLETS_NUM,		//装弾数.

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

	int m_iRoboHp[enHAVE_HP_PARTS_SIZE];

};
