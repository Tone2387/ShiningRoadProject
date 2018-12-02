#pragma once

#include"Charactor.h"
#include"SkinMesh.h"

//太原の書き足した分.
#ifdef Tahara
#include "PtrGroup.h"
#include "MissionModel.h"

//2018/05/16( 水 ).
	//関数「RoboInit」に「clsPOINTER_GROUP」型の引数を追加し、.
	//追加したメンバ変数( m_wpResource、m_wpEffects、m_wpSound ).
	//にアドレスを割り当てる処理を追加しました.
	
	//装飾子の都合上このクラスの「Render」関数と.
	//「clsSkinMesh」クラスの「ModelRender」関数と.
	//「clsD3DXSKINMESH」クラスの「Render」関数の.
	//引数にconstを付けました.
	
	//Zeromemoryはポインタが大暴れするので初期化を一つ一つ行いました.
	//↑「clsCharactor」と「clsObject」「clsSkinMesh」も.

#endif//#ifdef Tahara

class clsRobo : public clsCharactor
{
public:
	enum enWeaponType
	{
		enWeaponLHand,
		enWeaponRHand,

		enWeaponTypeSize
	};

	clsMISSION_MODEL* m_pMesh;

	void RoboInit(clsPOINTER_GROUP* const pPtrGroup, 
		clsROBO_STATUS* const pRobo);

	void ModelUpdate()
	{
		D3DXVECTOR3 vTmp;

		vTmp = GetRotation();
		
		SetRotateHeadParts();
		SetRotateArmParts();
		SetRotateCoreParts();
		SetRotateLegParts();

		vTmp = GetPosition();
		m_pMesh->SetPos(vTmp);

		m_pMesh->SetScale(m_Trans.vScale.x);

		m_pMesh->UpDate();
	}

	void SetRotateHeadParts();
	void SetRotateArmParts();
	void SetRotateCoreParts();
	void SetRotateLegParts();

	void Render( 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye) override
	{
		m_mProj = mProj;
		ModelUpdate();
		m_pMesh->Render(mView, mProj, vLight, vEye);
		UpdatePosfromBone();
		m_pMesh->UpdateColPos();
	}

	void UpdateCollsion();

	bool m_bBoost;//true:ブースター展開/false:非展開.

	//Headパーツから数値を取得する変数と関連する変数//

	//Coreパーツから数値を取得する変数と関連する変数//
	int m_iEnelgyMax;//エネルギー最大容量.
	int m_iEnelgy;//現在エネルギー残量.
	int m_iEnelgyOutput;//エネルギー出力(回復量)
	int m_iEnelgyRecoveryPoint;//現在のエネルギー回復量.
	int m_iBoostFloatRecovery;//空中でブースト展開中のエネルギー使用量.

	void EnelgyRecovery();
	void SetEnelgyRecoveryAmount();
	bool IsEnelgyRamaining(const int iConsumption);//エネルギーが消費量を超えているか?true:超えている.
	bool EnelgyConsumption(const int iConsumption);//エネルギー消費はここから.

	float m_fBoostMoveSpeedMax;
	int m_iBoostMoveCost;

	float m_fQuickBoostSpeedMax;
	int m_iQuickBoostEnelgyCost;
	int m_iQuickBoostTopSpeedTime;//最高速を保つフレーム値.
	int m_iQuickBoostDecStartTime;//残クイック噴射時間.

	float m_fQuickTrunSpeedMax;
	float m_fQuickTrunDir;
	int m_iQuickTrunEnelgyCost;
	int m_iQuickTrunTopSpeedTime;//最高速を保つフレーム値.
	int m_iQuickTrunDecStartTime;//残クイック噴射時間.

	int m_iQuickInterbal;//クイックブーストの再噴射までのフレーム.

	float m_fBoostRisingSpeedMax;//ブースター垂直移動の最高速.
	int m_iBoostRisingTopSpeedFrame;//↑に達するまでのフレーム値.
	float m_fBoostRisingAccele;// = m_fMoveSpeedMax / m_fTopSpeedFrame;
	int m_iBoostRisingyCost;
	float m_fBoostFollRes;//ブースター展開時に落ちる速度.

	int m_iActivityLimitTime;//活動限界時間.
	bool m_bTimeUp;//活動限界.

	//Armパーツから数値を取得する変数と関連する変数//

	//Legパーツから数値を取得する変数と関連する変数//
	float m_fWalktMoveSpeedMax;//Legパーツから取得する歩行速度.
	int m_iWalkTopSpeedFrame;//安定性能から計算.
	
	int m_iBoostTopSpeedFrame;//安定性能から計算.

	void Walk();
	void Boost();
	void MoveSwitch();
	void BoostRising();
	void QuickBoost();
	void SetDirQuickBoost(const float fAngle);
	void QuickTurn();
	void SetDirQuickTurn(const float fAngle);

	void ShotLWeapon();
	void ShotRWeapon();
	bool IsLWeaponLock();
	bool IsRWeaponLock();

	virtual void UpdateProduct(clsStage* pStage) override;
	void UpdataQuick();
	void UpdataLimitTime();
	void UpdataBoost();

	void UpdatePosfromBone();

	clsRobo();
	~clsRobo();

	

private:
	void Down() final;

	bool m_bStopComShotL;
	bool m_bStopComShotR;

	D3DXVECTOR3 m_vMoveDirforBoost;

	//右腕ブースターエフェクト.
	std::vector<::Effekseer::Handle> m_v_RHandFrontBoostEfc;
	std::vector<::Effekseer::Handle> m_v_RHandSideBoostEfc;
	std::vector<::Effekseer::Handle> m_v_RHandBackBoostEfc;

	//左腕ブースターエフェクト.
	std::vector<::Effekseer::Handle> m_v_LHandFrontBoostEfc;
	std::vector<::Effekseer::Handle> m_v_LHandSideBoostEfc;
	std::vector<::Effekseer::Handle> m_v_LHandBackBoostEfc;

	//コアブースターエフェクト.
	std::vector<::Effekseer::Handle> m_v_CoreBoostEfc;

	//脚ブースターエフェクト.
	std::vector<::Effekseer::Handle> m_v_LegBoostEfc;

	void SetBoostEffect();
	
	void SetRHandFrontBoostEffect();
	void SetRHandSideBoostEffect();
	void SetRHandBackBoostEffect();

	void SetLHandFrontBoostEffect();
	void SetLHandSideBoostEffect();
	void SetLHandBackBoostEffect();

	void SetCoreBoostEffect();

	void SetLegBoostEffect();

	void PlayBoostEfc();
	
	void PlayFrontBoostEfc();
	void PlayRightBoostEfc();
	void PlayLeftBoostEfc();
	void PlayBackBoostEfc();

	void PlayCoreBoostEfc();

	void PlayLegBoostEfc();

	int GetBoostEfcNum(enPARTS PartsNum,const char* strBoostPosition);

	virtual void UpdateLookStartingPos() override;

	enum enAnimNoLeg
	{
		enAnimNoLegWait = 0,
		enAnimNoLegWalkStart,
		enAnimNoLegWalkRight,
		enAnimNoLegWalkLeft,
		enAnimNoLegWalkEndRight,
		enAnimNoLegWalkEndLeft,
		enAnimNoLegBoostStart,
		enAnimNoLegBoost,
		enAnimNoLegBoostEnd,
		enAnimNoLegJumpStart,
		enAnimNoLegJumpUp,
		enAnimNoLegJump,
		enAnimNoLegJumpDown,
		enAnimNoLegJumpEnd,
		enAnimNoLegDown
	};

	enum enAnimNoArm
	{
		enAnimNoArmWait = 0,
		enAnimNoArmWeaponHoldStart,
		enAnimNoArmWeaponHoldAct,
		enAnimNoArmWeaponShot,
		enAnimNoArmWeaponHoldEnd,
		enAnimNoArmWeaponReloadStart,
		enAnimNoArmWeaponReload,
		enAnimNoArmWeaponReloadEnd,
		enAnimNoArmDown,
	};

	enum enAnimNoCore
	{
		enAnimNoCoreWait = 0,
		enAnimNoCoreDown,
		enAnimNoCoreRadiatorOpen,
		enAnimNoCoreRadiatorAct,
		enAnimNoCoreRadiatorClose
	};

	enum enAnimNoHead
	{
		enAnimNoHeadWait = 0,
		enAnimNoHeadDown
	};

	enum enAnimNoWeapon
	{
		enAnimNoWeaponWait = 0,
		enAnimNoWeaponShot,
		enAnimNoWeaponReload
	};

	void AnimUpdate();

	void AnimUpdateLeg();
	void AnimUpdateCore();
	void AnimUpdateHead();
	void AnimUpdateArmL();
	void AnimUpdateArmR();
	void AnimUpdateWeaponL();
	void AnimUpdateWeaponR();

	const bool IsLegPartsAnimBoost();//脚パーツのアニメーションがブースター関係ならtrue.

	const bool IsArmLPartsAnimReload();//左腕パーツのアニメーションがリロード関係ならtrue.
	const bool IsArmRPartsAnimReload();//右腕パーツのアニメーションがリロード関係ならtrue.

	void AnimChangeLeg(enAnimNoLeg enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeCore(enAnimNoCore enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeHead(enAnimNoHead enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeArmL(enAnimNoArm enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeArmR(enAnimNoArm enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeWeaponL(enAnimNoWeapon enChangeAnimNo, double dAnimTime = 0.0);
	void AnimChangeWeaponR(enAnimNoWeapon enChangeAnimNo, double dAnimTime = 0.0);

//太原の書き足した分.
#ifdef Tahara
	//消すときdeleteしないでnullしてね.
	clsResource*		m_wpResource;
	clsEffects*			m_wpEffects;
	clsSOUND_MANAGER_BASE*	m_wpSound;

	//当たり判定のポインタ.
//	std::shared_ptr< std::vector< D3DXVECTOR3 > > m_spColPoss;

	//ロボモデル.
//	std::unique_ptr< clsMISSION_MODEL > m_upMissModel;

#endif//#ifdef Tahara


};


