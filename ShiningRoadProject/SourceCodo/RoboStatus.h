#pragma once 

#include "Global.h"

#include <vector>

//武器情報( 左右別々なので面倒なのでまとめた ).
struct WERPON_STATUS
{
	int iAtk;			//攻撃力.
	int iBulletSpd;		//弾速.
	int iRange;			//射程距離.
	int iCost;			//消費エネルギー.
	int iLoadTIme;		//装填時間.
	int iLockSpd;		//ロック速度.
	int iLockRange;		//ロック距離.
		
	int iReaction;		//射撃反動.
	int iAccuracy;		//射撃精度.
	int iMagazineLoadTime;//マガジン装填時間.
	int iBulletNum;		//装弾数.

	void Clear(){
		iAtk = 0;
		iBulletSpd = 0;
		iRange = 0;
		iCost = 0;
		iLoadTIme = 0;
		iLockSpd = 0;
		iLockRange = 0;
		
		iReaction = 0;
		iAccuracy = 0;
		iMagazineLoadTime = 0;
		iBulletNum = 0;
	}
};


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
	void ReceiveLeg( std::vector<int> LegDatas );
	void ReceiveCore( std::vector<int> CoreDatas );
	void ReceiveHead( std::vector<int> HeadDatas );
	void ReceiveArms( std::vector<int> ArmsDatas );
	void ReceiveWeaponL( std::vector<int> WeaponLDatas );
	void ReceiveWeaponR( std::vector<int> WeaponRDatas );

private:
	
	int m_iHp;			//四つのパーツに影響される.
	int m_iWalkSpd;		//歩行速度.
	int m_iStability;	//安定性能.
	int m_iSpn;			//旋回性能.
	int m_iJumpPower;	//ジャンプ力.
	int m_iSearch;		//索敵性能.
	int m_iEnCapa;		//エネルギー容量 エネルギー : energy  容量 : capacity.
	int m_iEnOutPut;	//エネルギー出力( 回復量 ).
	int m_iBoostThrust;	//ブースター推力.
	int m_iBoostCost;	//ブースター消費エネルギー.
	int m_iQuickThrust;	//クイック推力.
	int m_iQuickCost;	//クイック消費エネルギー.
	int m_iActTime;		//活動時間(ActivityTime).
	int m_iQuickTime;	//クイック噴射時間.
	int m_iAiming;		//照準精度( エイム ).

	WERPON_STATUS m_WeaponL;
	WERPON_STATUS m_WeaponR;

};
