#ifndef ROBO_STATUS_H_
#define ROBO_STATUS_H_


class clsFILE;
#include "Global.h"


//アセンブルシーンでの選択肢の型.
using ASSEMBLE_SCENE_SELECT_TYPE = short;


//ゲームクラスで作成、.
//アセンブルシーンで値を設定、.
//ミッショシーンで初期化の段階でロボに渡す.
class clsROBO_STATUS
{
public:
	clsROBO_STATUS();
	virtual ~clsROBO_STATUS();

	//色の棒.
	enum enCOLOR_GAGE : char
	{
		enCOLOR_GAGE_BASE_R = 0,
		enCOLOR_GAGE_BASE_G,
		enCOLOR_GAGE_BASE_B,

		enCOLOR_GAGE_ARMOR_R,
		enCOLOR_GAGE_ARMOR_G,
		enCOLOR_GAGE_ARMOR_B,

		enCOLOR_GAGE_size
	};
	 
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
		ACT_TIME,		//活動時間(ActivityTime).

		SEARCH,			//索敵性能.
		LOCK_ON_SPEED,	//ロック速度.
		LOCK_ON_RANGE,	//ロック距離.

		AIMING,			//照準精度( エイム ).
		QUICK_THRUST,	//クイック推力.
		QUICK_COST,		//クイック消費エネルギー.
		QUICK_TIME,		//クイック噴射時間.

		//隠しステータス.
		COL_SIZE_LEG,	//脚当たり判定サイズ.
		COL_SIZE_CORE,	//コア当たり判定サイズ.
		COL_SIZE_HEAD,	//頭当たり判定サイズ.
		COL_SIZE_ARMS,	//腕当たり判定サイズ.
		WALK_ANIM_SPEED,//脚歩行アニメ速度.

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
		LOCK_ON_TIME,		//ロック時間.
		SHOT_STABILITY,		//射撃安定.
		MAGAZINE_LOAD_TIME,	//マガジン装填時間.
		BULLETS_NUM,		//装弾数.

		//隠しステータス.
		COL_SIZE,	//弾の当たり判定サイズ.
		EFC_BULLET,	//弾本体のエフェクト.
		EFC_LOCUS,	//弾の軌跡のエフェクト.
		EFC_HIT,	//着弾のエフェクト.
		EFC_MUZZLE,//マズルエフェクト.
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


	//まっさらにする( アセンブルシーンでの初期化として使う ).
	void Clear();

	//データの受け取り.
	//これらの関数でデータを受け取る前にClear()しておくこと.
	//第一引数 : 攻撃力などのパラメータ群.
	//第二引数 : モデル番号.
	void ReceiveLeg(	const std::vector<int> &LegDatas,		const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveCore(	const std::vector<int> &CoreDatas,		const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveHead(	const std::vector<int> &HeadDatas,		const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveArms(	const std::vector<int> &ArmsDatas,		const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveWeaponL(const std::vector<int> &WeaponLDatas,	const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );
	void ReceiveWeaponR(const std::vector<int> &WeaponRDatas,	const ASSEMBLE_SCENE_SELECT_TYPE PartsNum );


	//ロボの情報を吐き出す.
	int GetRoboState( const enROBO_STATE enStateNum );
	//武器の情報を吐き出す.
	int GetWeaponState( const enWEAPON_NUM enArmLR, const enWEAPON_STATE enStateNum ) const;

	//パーツ番号を返す( いま装備しているパーツが何番か ).//#define SKIN_ENUM_TYPE UCHAR.
	//clsASSEMBLE_MODELクラスのAttachModelの第二引数として使います.
	UCHAR GetPartsNum( const enPARTS PartsType ) const;


	//色フラグのやり取り.
	void SetColorRank( const enCOLOR_GAGE enColorNum, const int iColorRate );
	int GetColorRank( const enCOLOR_GAGE enColorNum ) const;

protected:


	//継承先で外部からデータを読み込ませる.
	virtual void LoadFileData( const char* sFilePath ) = 0;

	//パーツのステータスデータを読み込む.
	void LoadPartsData( std::vector< std::unique_ptr< clsFILE > >& PartsFile );

	//読み込んだ番号のデータを取得する.
	void AttachData( const std::vector< std::unique_ptr< clsFILE > >& PartsFile );

	//ReceiveLeg()等の関数に格納するためのデータを作り、吐き出す.
	std::vector<int> CreateDataForReceive( 
		const std::vector< std::unique_ptr< clsFILE > >& PartsFile,
		const int PartsType,
		const int PartsNum );

protected:

	//シーンをまたいで必要なロボのデータ.
	struct ROBO_STATE_DATA
	{
		//パーツ番号の配列.
		UCHAR ucPartsModelNum[ static_cast<int>( enPARTS::MAX ) ];

		//色の段階.
		int iColorRank[ enCOLOR_GAGE_size ];
	}	m_RoboStateData;


	//継承クラスで使う.
	const int m_iFILE_VAR_ROW;
	const int m_iFILE_INDEX_COL_LEG;
	const int m_iFILE_INDEX_COL_CORE;
	const int m_iFILE_INDEX_COL_HEAD;
	const int m_iFILE_INDEX_COL_ARM_L;
	const int m_iFILE_INDEX_COL_ARM_R;
	const int m_iFILE_INDEX_COL_WEAPON_L;
	const int m_iFILE_INDEX_COL_WEAPON_R;
	const int m_iFILE_INDEX_COL_COLOR_ARMOR_R;
	const int m_iFILE_INDEX_COL_COLOR_ARMOR_G;
	const int m_iFILE_INDEX_COL_COLOR_ARMOR_B;
	const int m_iFILE_INDEX_COL_COLOR_BASE_R;
	const int m_iFILE_INDEX_COL_COLOR_BASE_G;
	const int m_iFILE_INDEX_COL_COLOR_BASE_B;



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
	int m_iRoboState[ enROBO_STATE_SIZE ];

	//武器のパラメータ.
	int m_iWeaponState[ enWEAPON_NUM_SIZE ][ enWEAPON_STATE_SIZE ];

	//ロボのHPを後で合算するための変数.
	int m_iRoboHp[ enHAVE_HP_PARTS_SIZE ];

};
#endif//#ifndef ROBO_STATUS_H_