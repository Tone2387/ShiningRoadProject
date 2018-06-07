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
	m_iHp = 0;
	m_iWalkSpd = 0;
	m_iStability = 0;
	m_iSpn = 0;
	m_iJumpPower = 0;
	m_iSearch = 0;
	m_iEnCapa = 0;
	m_iEnOutPut = 0;
	m_iBoostThrust = 0;
	m_iBoostCost = 0;
	m_iQuickThrust = 0;
	m_iQuickCost = 0;
	m_iActTime = 0;
	m_iQuickTime = 0;
	m_iAiming = 0;
	m_WeaponL.Clear();
	m_WeaponR.Clear();
}


//データの受け取り.
void clsROBO_STATUS::ReceiveLeg( vector<int> LegDatas )
{
	if( LegDatas.size() > clsPARTS_LEG::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iHp		+= LegDatas[clsPARTS_LEG::HP];			//四つのパーツに影響される.
	m_iWalkSpd	 = LegDatas[clsPARTS_LEG::WALK_SPD];	//歩行速度.
	m_iStability = LegDatas[clsPARTS_LEG::STABILITY];	//安定性能.
	m_iSpn		 = LegDatas[clsPARTS_LEG::SPIN];		//旋回性能.
	m_iJumpPower = LegDatas[clsPARTS_LEG::JUMP_POWER];	//ジャンプ力.
}

void clsROBO_STATUS::ReceiveCore( vector<int> CoreDatas )
{
	if( CoreDatas.size() > clsPARTS_CORE::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iHp			+= CoreDatas[clsPARTS_CORE::HP];			//四つのパーツに影響される.
	m_iEnCapa		 = CoreDatas[clsPARTS_CORE::EN_CAPA];		//エネルギー容量 エネルギー : energy  容量 : capacity.
	m_iEnOutPut		 = CoreDatas[clsPARTS_CORE::EN_OUTPUT];		//エネルギー出力( 回復量 ).
	m_iBoostThrust	 = CoreDatas[clsPARTS_CORE::BOOST_THRUST];	//ブースター推力.
	m_iBoostCost	 = CoreDatas[clsPARTS_CORE::BOOST_COST];	//ブースター消費エネルギー.
	m_iQuickThrust	 = CoreDatas[clsPARTS_CORE::QUICK_THRUST];	//クイック推力.
	m_iQuickCost	 = CoreDatas[clsPARTS_CORE::QUICK_COST];	//クイック消費エネルギー.
	m_iActTime		 = CoreDatas[clsPARTS_CORE::ACT_TIME];		//活動時間(ActivityTime).
	m_iQuickTime	 = CoreDatas[clsPARTS_CORE::QUIC_KTIME];	//クイック噴射時間.
}

void clsROBO_STATUS::ReceiveHead( vector<int> HeadDatas )
{
	if( HeadDatas.size() > clsPARTS_HEAD::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iHp		+= HeadDatas[clsPARTS_HEAD::HP];	//四つのパーツに影響される.
	m_iSearch	 = HeadDatas[clsPARTS_HEAD::SEARCH];//索敵性能.
}

void clsROBO_STATUS::ReceiveArms( vector<int> ArmsDatas )
{
	if( ArmsDatas.size() > clsPARTS_ARM_BASE::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_iHp		+= ArmsDatas[clsPARTS_ARM_BASE::HP];	//四つのパーツに影響される.
	m_iAiming	 = ArmsDatas[clsPARTS_ARM_BASE::AIMING];//照準精度( エイム ).
}

void clsROBO_STATUS::ReceiveWeaponL( vector<int> WeaponLDatas )
{
	if( WeaponLDatas.size() > clsPARTS_WEAPON::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_WeaponL.iAtk				= WeaponLDatas[clsPARTS_WEAPON::ATK];				//攻撃力.
	m_WeaponL.iBulletSpd		= WeaponLDatas[clsPARTS_WEAPON::BULLET_SPD];		//弾速.
	m_WeaponL.iRange			= WeaponLDatas[clsPARTS_WEAPON::RANGE];				//射程距離.
	m_WeaponL.iCost				= WeaponLDatas[clsPARTS_WEAPON::COST];				//消費エネルギー.
	m_WeaponL.iLoadTIme			= WeaponLDatas[clsPARTS_WEAPON::LOAD_TIME];			//装填時間.
	m_WeaponL.iLockSpd			= WeaponLDatas[clsPARTS_WEAPON::LOCK_SPD];			//ロック速度.
	m_WeaponL.iLockRange		= WeaponLDatas[clsPARTS_WEAPON::LOCK_RANGE];		//ロック距離.
	
	m_WeaponL.iReaction			= WeaponLDatas[clsPARTS_WEAPON::REACTION];			//射撃反動.
	m_WeaponL.iAccuracy			= WeaponLDatas[clsPARTS_WEAPON::ACCURACY];			//射撃精度.
	m_WeaponL.iMagazineLoadTime	= WeaponLDatas[clsPARTS_WEAPON::MAGAZINE_LOAD_TIME];//マガジン装填時間.
	m_WeaponL.iBulletNum		= WeaponLDatas[clsPARTS_WEAPON::BULLETS_NUM];		//装弾数.
}

void clsROBO_STATUS::ReceiveWeaponR( vector<int> WeaponRDatas )
{
	if( WeaponRDatas.size() > clsPARTS_WEAPON::size ){
		assert( !"受け取ったvectorデータのsizeが大きすぎます" );
	}

	m_WeaponR.iAtk				= WeaponRDatas[clsPARTS_WEAPON::ATK];				//攻撃力.
	m_WeaponR.iBulletSpd		= WeaponRDatas[clsPARTS_WEAPON::BULLET_SPD];		//弾速.
	m_WeaponR.iRange			= WeaponRDatas[clsPARTS_WEAPON::RANGE];				//射程距離.
	m_WeaponR.iCost				= WeaponRDatas[clsPARTS_WEAPON::COST];				//消費エネルギー.
	m_WeaponR.iLoadTIme			= WeaponRDatas[clsPARTS_WEAPON::LOAD_TIME];			//装填時間.
	m_WeaponR.iLockSpd			= WeaponRDatas[clsPARTS_WEAPON::LOCK_SPD];			//ロック速度.
	m_WeaponR.iLockRange		= WeaponRDatas[clsPARTS_WEAPON::LOCK_RANGE];		//ロック距離.

	m_WeaponR.iReaction			= WeaponRDatas[clsPARTS_WEAPON::REACTION];			//射撃反動.
	m_WeaponR.iAccuracy			= WeaponRDatas[clsPARTS_WEAPON::ACCURACY];			//射撃精度.
	m_WeaponR.iMagazineLoadTime	= WeaponRDatas[clsPARTS_WEAPON::MAGAZINE_LOAD_TIME];//マガジン装填時間.
	m_WeaponR.iBulletNum		= WeaponRDatas[clsPARTS_WEAPON::BULLETS_NUM];		//装弾数.
}


