#include "RoboStatus.h"

#include "PartsLeg.h"
#include "PartsCore.h"
#include "PartsHead.h"
#include "PartsArmL.h"
#include "PartsArmR.h"
#include "PartsWeapon.h"


using namespace std;


clsROBO_STATUS::clsROBO_STATUS()
{
	Clear();
}

clsROBO_STATUS::~clsROBO_STATUS()
{
	Clear();
}




//まっさらにする.
void clsROBO_STATUS::Clear()
{
	for( UCHAR i=0; i<enROBO_STATE_SIZE; i++ ){
		m_iRoboState[i] = 0;
	}

	for( UCHAR i=0; i<enWEAPON_STATE_SIZE; i++ ){
		m_iWeaponState[enWEAPON_NUM::LEFT][i] = 0;
		m_iWeaponState[enWEAPON_NUM::RIGHT][i] = 0;
	}
	for( UCHAR i=0; i<enHAVE_HP_PARTS_SIZE; i++ ){
		m_iRoboHp[i] = 0;
	}
}


//データの受け取り.
void clsROBO_STATUS::ReceiveLeg( const vector<int> &LegDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( LegDatas.size() > clsPARTS_LEG::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::LEG]		  = LegDatas[clsPARTS_LEG::HP];			//四つのパーツに影響される.
	m_iRoboState[enROBO_STATE::WALK_SPD]  = LegDatas[clsPARTS_LEG::WALK_SPD];	//歩行速度.
	m_iRoboState[enROBO_STATE::STABILITY] = LegDatas[clsPARTS_LEG::STABILITY];	//安定性能.
	m_iRoboState[enROBO_STATE::TURN]	  = LegDatas[clsPARTS_LEG::TURN];		//旋回性能.
	m_iRoboState[enROBO_STATE::JUMP_POWER]= LegDatas[clsPARTS_LEG::JUMP_POWER];	//ジャンプ力.

	m_ucPartsModelNum[ static_cast<int>( enPARTS::LEG ) ] = PartsNum;
}

void clsROBO_STATUS::ReceiveCore( const vector<int> &CoreDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( CoreDatas.size() > clsPARTS_CORE::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::CORE]	    = CoreDatas[clsPARTS_CORE::HP];				//四つのパーツに影響される.
	m_iRoboState[enROBO_STATE::EN_CAPA]		= CoreDatas[clsPARTS_CORE::EN_CAPA];		//エネルギー容量 エネルギー : energy  容量 : capacity.
	m_iRoboState[enROBO_STATE::EN_OUTPUT]	= CoreDatas[clsPARTS_CORE::EN_OUTPUT];		//エネルギー出力( 回復量 ).
	m_iRoboState[enROBO_STATE::BOOST_THRUST]= CoreDatas[clsPARTS_CORE::BOOST_THRUST];	//ブースター推力.
	m_iRoboState[enROBO_STATE::BOOST_COST]	= CoreDatas[clsPARTS_CORE::BOOST_COST];		//ブースター消費エネルギー.
	m_iRoboState[enROBO_STATE::QUICK_THRUST]= CoreDatas[clsPARTS_CORE::QUICK_THRUST];	//クイック推力.
	m_iRoboState[enROBO_STATE::QUICK_COST]  = CoreDatas[clsPARTS_CORE::QUICK_COST];		//クイック消費エネルギー.
	m_iRoboState[enROBO_STATE::ACT_TIME]	= CoreDatas[clsPARTS_CORE::ACT_TIME];		//活動時間(ActivityTime).
	m_iRoboState[enROBO_STATE::QUICK_TIME]	= CoreDatas[clsPARTS_CORE::QUICK_TIME];		//クイック噴射時間.

	m_ucPartsModelNum[ static_cast<int>( enPARTS::CORE ) ] = PartsNum;
}

void clsROBO_STATUS::ReceiveHead( const vector<int> &HeadDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( HeadDatas.size() > clsPARTS_HEAD::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::HEAD]  = HeadDatas[clsPARTS_HEAD::HP];		//四つのパーツに影響される.
	m_iRoboState[enROBO_STATE::SEARCH]= HeadDatas[clsPARTS_HEAD::SEARCH];	//索敵性能.

	m_ucPartsModelNum[ static_cast<int>( enPARTS::HEAD ) ] = PartsNum;
}

void clsROBO_STATUS::ReceiveArms( const vector<int> &ArmsDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( ArmsDatas.size() > clsPARTS_ARM_BASE::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::ARMS]  = ArmsDatas[clsPARTS_ARM_BASE::HP];		//四つのパーツに影響される.
	m_iRoboState[enROBO_STATE::AIMING]= ArmsDatas[clsPARTS_ARM_BASE::AIMING]; //照準精度( エイム ).

	m_ucPartsModelNum[ static_cast<int>( enPARTS::ARM_L ) ] = PartsNum;
	m_ucPartsModelNum[ static_cast<int>( enPARTS::ARM_R ) ] = PartsNum;
}

void clsROBO_STATUS::ReceiveWeaponL( const vector<int> &WeaponLDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( WeaponLDatas.size() > clsPARTS_WEAPON::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::ATK]			= WeaponLDatas[clsPARTS_WEAPON::ATK];		//攻撃力.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::BULLET_SPD]	= WeaponLDatas[clsPARTS_WEAPON::BULLET_SPD];//弾速.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::RANGE]		= WeaponLDatas[clsPARTS_WEAPON::RANGE];		//射程距離.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::COST]		= WeaponLDatas[clsPARTS_WEAPON::COST];		//消費エネルギー.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::LOAD_TIME]	= WeaponLDatas[clsPARTS_WEAPON::LOAD_TIME];	//装填時間.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::LOCK_SPD]	= WeaponLDatas[clsPARTS_WEAPON::LOCK_SPD];	//ロック速度.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::LOCK_RANGE]	= WeaponLDatas[clsPARTS_WEAPON::LOCK_RANGE];//ロック距離.

	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::REACTION]	= WeaponLDatas[clsPARTS_WEAPON::REACTION];						//射撃反動.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::ACCURACY]	= WeaponLDatas[clsPARTS_WEAPON::ACCURACY];						//射撃精度.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::MAGAZINE_LOAD_TIME]	= WeaponLDatas[clsPARTS_WEAPON::MAGAZINE_LOAD_TIME];	//マガジン装填時間.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::BULLETS_NUM]	= WeaponLDatas[clsPARTS_WEAPON::BULLETS_NUM];					//装弾数.

	m_ucPartsModelNum[ static_cast<int>( enPARTS::WEAPON_L ) ] = PartsNum;
}

void clsROBO_STATUS::ReceiveWeaponR( const vector<int> &WeaponRDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( WeaponRDatas.size() > clsPARTS_WEAPON::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::ATK]		= WeaponRDatas[clsPARTS_WEAPON::ATK];		//攻撃力.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::BULLET_SPD]	= WeaponRDatas[clsPARTS_WEAPON::BULLET_SPD];//弾速.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::RANGE]		= WeaponRDatas[clsPARTS_WEAPON::RANGE];		//射程距離.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::COST]		= WeaponRDatas[clsPARTS_WEAPON::COST];		//消費エネルギー.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::LOAD_TIME]	= WeaponRDatas[clsPARTS_WEAPON::LOAD_TIME];	//装填時間.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::LOCK_SPD]	= WeaponRDatas[clsPARTS_WEAPON::LOCK_SPD];	//ロック速度.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::LOCK_RANGE]	= WeaponRDatas[clsPARTS_WEAPON::LOCK_RANGE];//ロック距離.
								 
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::REACTION]	= WeaponRDatas[clsPARTS_WEAPON::REACTION];						//射撃反動.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::ACCURACY]	= WeaponRDatas[clsPARTS_WEAPON::ACCURACY];						//射撃精度.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::MAGAZINE_LOAD_TIME]	= WeaponRDatas[clsPARTS_WEAPON::MAGAZINE_LOAD_TIME];	//マガジン装填時間.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::BULLETS_NUM]	= WeaponRDatas[clsPARTS_WEAPON::BULLETS_NUM];				//装弾数.

	m_ucPartsModelNum[ static_cast<int>( enPARTS::WEAPON_R ) ] = PartsNum;
}


//ロボの情報を吐き出す.
int clsROBO_STATUS::GetRoboState( const enROBO_STATE enStateNum )
{
	if( enStateNum >= enROBO_STATE_SIZE ){
		assert( !"不正な情報を取得しようとされました" );
		return 0;
	}
	//HP合算.
	if( enStateNum == enROBO_STATE::HP ){
		m_iRoboState[enStateNum] = 0;
		for( UCHAR i=0; i<enHAVE_HP_PARTS_SIZE; i++ ){
			m_iRoboState[enStateNum] += m_iRoboHp[i];
		}
	}

	return m_iRoboState[enStateNum];
}
//武器の情報を吐き出す.
int clsROBO_STATUS::GetWeaponState( 
	const enWEAPON_NUM enArmLR, const enWEAPON_STATE enStateNum ) const
{
	if( enArmLR >= enWEAPON_NUM_SIZE ){
		assert( !"不正な数の武器が指定されました" );
		return 0;
	}

	if( enStateNum >= enWEAPON_STATE_SIZE ){
		assert( !"不正な武器情報を取得しようとされました" );
		return 0;
	}

	return m_iWeaponState[enArmLR][enStateNum];
}


//パーツ番号を返す.
UCHAR clsROBO_STATUS::GetPartsNum( const enPARTS PartsType )
{
	return m_ucPartsModelNum[ static_cast<int>( PartsType ) ];
}
