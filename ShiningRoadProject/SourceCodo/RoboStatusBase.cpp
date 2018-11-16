#include "RoboStatusBase.h"

#include "PartsLeg.h"
#include "PartsCore.h"
#include "PartsHead.h"
#include "PartsArmL.h"
#include "PartsArmR.h"
#include "PartsWeapon.h"


using namespace std;

namespace{

	const int iCOLOR_RANK_INIT = 16;

}



clsROBO_STATUS::clsROBO_STATUS()
	:m_iFILE_VAR_ROW( 0 )//一行しか存在しない.
	,m_iFILE_INDEX_COL_LEG( 0 )
	,m_iFILE_INDEX_COL_CORE( 1 )
	,m_iFILE_INDEX_COL_HEAD( 2 )
	,m_iFILE_INDEX_COL_ARM_L( 3 )
	,m_iFILE_INDEX_COL_ARM_R( 4 )
	,m_iFILE_INDEX_COL_WEAPON_L( 5 )
	,m_iFILE_INDEX_COL_WEAPON_R( 6 )
	,m_iFILE_INDEX_COL_COLOR_ARMOR_R( 7 )
	,m_iFILE_INDEX_COL_COLOR_ARMOR_G( 8 )
	,m_iFILE_INDEX_COL_COLOR_ARMOR_B( 9 )
	,m_iFILE_INDEX_COL_COLOR_BASE_R( 10 )
	,m_iFILE_INDEX_COL_COLOR_BASE_G( 11 )
	,m_iFILE_INDEX_COL_COLOR_BASE_B( 12 )
{	 
	Clear();


	UCHAR tmpSize = sizeof( m_RoboStateData.ucPartsModelNum ) / sizeof( m_RoboStateData.ucPartsModelNum[0] );
	for( UCHAR i=0; i<tmpSize; i++ ){
		m_RoboStateData.ucPartsModelNum[i] = 0;
	}

	for( char i=0; i<enCOLOR_GAGE_size; i++ ){
		m_RoboStateData.iColorRank[i] = iCOLOR_RANK_INIT;
	}

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


//パーツ番号を返す( いま装備しているパーツが何番か ).
UCHAR clsROBO_STATUS::GetPartsNum( const enPARTS PartsType ) const
{
	return m_RoboStateData.ucPartsModelNum[ static_cast<int>( PartsType ) ];
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

	m_iRoboState[enROBO_STATE::COL_SIZE_LEG]	= LegDatas[clsPARTS_LEG::COL_SIZE];//当たり判定サイズ.
	m_iRoboState[enROBO_STATE::WALK_ANIM_SPEED] = LegDatas[clsPARTS_LEG::WALK_ANIM_SPEED];//歩行アニメ速度

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::LEG ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveCore( const vector<int> &CoreDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( CoreDatas.size() > clsPARTS_CORE::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::CORE]			= CoreDatas[clsPARTS_CORE::HP];				//四つのパーツに影響される.
	
	m_iRoboState[enROBO_STATE::EN_CAPA]			= CoreDatas[clsPARTS_CORE::EN_CAPA];		//エネルギー容量 エネルギー : energy  容量 : capacity.
	m_iRoboState[enROBO_STATE::EN_OUTPUT]		= CoreDatas[clsPARTS_CORE::EN_OUTPUT];		//エネルギー出力( 回復量 ).
	m_iRoboState[enROBO_STATE::BOOST_THRUST_H]	= CoreDatas[clsPARTS_CORE::BOOST_THRUST_H];	//ブースター推力( 水平 ).
	m_iRoboState[enROBO_STATE::BOOST_COST_H]	= CoreDatas[clsPARTS_CORE::BOOST_COST_H];	//ブースター消費エネルギー( 水平 ).
	m_iRoboState[enROBO_STATE::BOOST_THRUST_V]	= CoreDatas[clsPARTS_CORE::BOOST_THRUST_V];	//ブースター推力( 水直 ).
	m_iRoboState[enROBO_STATE::BOOST_COST_V]	= CoreDatas[clsPARTS_CORE::BOOST_COST_V];	//ブースター消費エネルギー( 水直 ).
	m_iRoboState[enROBO_STATE::ACT_TIME]		= CoreDatas[clsPARTS_CORE::ACT_TIME];		//活動時間(ActivityTime).

	m_iRoboState[enROBO_STATE::COL_SIZE_CORE]	= CoreDatas[clsPARTS_CORE::COL_SIZE];	//当たり判定サイズ.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::CORE ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveHead( const vector<int> &HeadDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( HeadDatas.size() > clsPARTS_HEAD::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::HEAD]  = HeadDatas[clsPARTS_HEAD::HP];		//四つのパーツに影響される.
	
	m_iRoboState[enROBO_STATE::SEARCH]			= HeadDatas[clsPARTS_HEAD::SEARCH];		//索敵性能.
	m_iRoboState[enROBO_STATE::LOCK_ON_SPEED]	= HeadDatas[clsPARTS_HEAD::LOCK_ON_SPEED];	//ロック速度.
	m_iRoboState[enROBO_STATE::LOCK_ON_RANGE]	= HeadDatas[clsPARTS_HEAD::LOCK_ON_RANGE];	//ロック距離.

	m_iRoboState[enROBO_STATE::COL_SIZE_HEAD]= HeadDatas[clsPARTS_HEAD::COL_SIZE];	//当たり判定サイズ.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::HEAD ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveArms( const vector<int> &ArmsDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( ArmsDatas.size() > clsPARTS_ARM_BASE::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iRoboHp[enHAVE_HP_PARTS::ARMS]		= ArmsDatas[clsPARTS_ARM_BASE::HP];		//四つのパーツに影響される.
	
	m_iRoboState[enROBO_STATE::AIMING]		= ArmsDatas[clsPARTS_ARM_BASE::AIMING];			//照準精度( エイム ).
	m_iRoboState[enROBO_STATE::QUICK_THRUST]= ArmsDatas[clsPARTS_ARM_BASE::QUICK_THRUST];	//クイック推力.
	m_iRoboState[enROBO_STATE::QUICK_COST]	= ArmsDatas[clsPARTS_ARM_BASE::QUICK_COST];		//クイック消費エネルギー.
	m_iRoboState[enROBO_STATE::QUICK_TIME]	= ArmsDatas[clsPARTS_ARM_BASE::QUICK_TIME];		//クイック噴射時間.

	m_iRoboState[enROBO_STATE::COL_SIZE_ARMS] = ArmsDatas[clsPARTS_ARM_BASE::COL_SIZE]; //当たり判定サイズ.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::ARM_L ) ] = static_cast<UCHAR>( PartsNum );
	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::ARM_R ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveWeaponL( const vector<int> &WeaponLDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( WeaponLDatas.size() > clsPARTS_WEAPON::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::ATK]					= WeaponLDatas[clsPARTS_WEAPON::ATK];				//攻撃力.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::BULLET_SPD]			= WeaponLDatas[clsPARTS_WEAPON::BULLET_SPD];		//弾速.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::RANGE]				= WeaponLDatas[clsPARTS_WEAPON::RANGE];				//射程距離.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::COST]				= WeaponLDatas[clsPARTS_WEAPON::COST];				//消費エネルギー.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::LOAD_TIME]			= WeaponLDatas[clsPARTS_WEAPON::LOAD_TIME];			//装填時間.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::LOCK_ON_TIME]		= WeaponLDatas[clsPARTS_WEAPON::LOCK_ON_TIME];		//ロック時間.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::SHOT_STABILITY]		= WeaponLDatas[clsPARTS_WEAPON::STABILITY];			//射撃安定.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::MAGAZINE_LOAD_TIME]	= WeaponLDatas[clsPARTS_WEAPON::MAGAZINE_LOAD_TIME];//マガジン装填時間.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::BULLETS_NUM]			= WeaponLDatas[clsPARTS_WEAPON::BULLETS_NUM];		//装弾数.

	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::COL_SIZE]	= WeaponLDatas[clsPARTS_WEAPON::COL_SIZE];	//弾の当たり判定サイズ.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::EFC_BULLET]	= WeaponLDatas[clsPARTS_WEAPON::EFC_BULLET];//弾本体のエフェクト.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::EFC_LOCUS]	= WeaponLDatas[clsPARTS_WEAPON::EFC_LOCUS];	//弾の軌跡のエフェクト.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::EFC_HIT]		= WeaponLDatas[clsPARTS_WEAPON::EFC_HIT];	//着弾のエフェクト.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::EFC_CARTRIDGE]=WeaponLDatas[clsPARTS_WEAPON::EFC_HIT];	//着弾のエフェクト.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::SE_FIER]		= WeaponLDatas[clsPARTS_WEAPON::SE_FIER];	//発射SE.
	m_iWeaponState[enWEAPON_NUM::LEFT][enWEAPON_STATE::SE_HIT]		= WeaponLDatas[clsPARTS_WEAPON::SE_HIT];	//着弾のSE.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::WEAPON_L ) ] = static_cast<UCHAR>( PartsNum );
}

void clsROBO_STATUS::ReceiveWeaponR( const vector<int> &WeaponRDatas, const ASSEMBLE_SCENE_SELECT_TYPE PartsNum )
{
	if( WeaponRDatas.size() > clsPARTS_WEAPON::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::ATK]				= WeaponRDatas[clsPARTS_WEAPON::ATK];				//攻撃力.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::BULLET_SPD]			= WeaponRDatas[clsPARTS_WEAPON::BULLET_SPD];		//弾速.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::RANGE]				= WeaponRDatas[clsPARTS_WEAPON::RANGE];				//射程距離.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::COST]				= WeaponRDatas[clsPARTS_WEAPON::COST];				//消費エネルギー.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::LOAD_TIME]			= WeaponRDatas[clsPARTS_WEAPON::LOAD_TIME];			//装填時間.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::LOCK_ON_TIME]		= WeaponRDatas[clsPARTS_WEAPON::LOCK_ON_TIME];		//ロック時間.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::SHOT_STABILITY]		= WeaponRDatas[clsPARTS_WEAPON::STABILITY];			//射撃安定.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::MAGAZINE_LOAD_TIME]	= WeaponRDatas[clsPARTS_WEAPON::MAGAZINE_LOAD_TIME];//マガジン装填時間.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::BULLETS_NUM]		= WeaponRDatas[clsPARTS_WEAPON::BULLETS_NUM];		//装弾数.

	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::COL_SIZE]	= WeaponRDatas[clsPARTS_WEAPON::COL_SIZE];	//弾の当たり判定サイズ.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::EFC_BULLET]	= WeaponRDatas[clsPARTS_WEAPON::EFC_BULLET];//弾本体のエフェクト.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::EFC_LOCUS]	= WeaponRDatas[clsPARTS_WEAPON::EFC_LOCUS];	//弾の軌跡のエフェクト.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::EFC_HIT]	= WeaponRDatas[clsPARTS_WEAPON::EFC_HIT];	//着弾のエフェクト.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::EFC_CARTRIDGE]=WeaponRDatas[clsPARTS_WEAPON::EFC_HIT];	//着弾のエフェクト.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::SE_FIER]	= WeaponRDatas[clsPARTS_WEAPON::SE_FIER];	//発射SE.
	m_iWeaponState[enWEAPON_NUM::RIGHT][enWEAPON_STATE::SE_HIT]		= WeaponRDatas[clsPARTS_WEAPON::SE_HIT];	//着弾のSE.

	m_RoboStateData.ucPartsModelNum[ static_cast<int>( enPARTS::WEAPON_R ) ] = static_cast<UCHAR>( PartsNum );
}


//色フラグのやり取り.
void clsROBO_STATUS::SetColorRank( 
	const enCOLOR_GAGE enColorNum, 
	const int iColorRate )
{
	assert( enColorNum >= 0  );
	assert( enColorNum < enCOLOR_GAGE_size );
	m_RoboStateData.iColorRank[ enColorNum ] = iColorRate;
}


int clsROBO_STATUS::GetColorRank( const enCOLOR_GAGE enColorNum ) const
{
	assert( enColorNum >= 0  );
	assert( enColorNum < enCOLOR_GAGE_size );
	return m_RoboStateData.iColorRank[ enColorNum ];
}
