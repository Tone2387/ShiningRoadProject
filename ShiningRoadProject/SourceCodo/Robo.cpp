#include"Robo.h"
#include "OperationString.h"

//ジョイントボーンの数字の桁数.
const char cBONE_NAME_NUM_DIGIT_JOINT = 2;

void clsRobo::RoboInit(
	clsPOINTER_GROUP* const pPtrGroup,
	clsROBO_STATUS* const pRobo)
{
#ifdef Tahara
	m_wpResource = pPtrGroup->GetResource();
	m_wpEffects = pPtrGroup->GetEffects();
	m_wpSound = pPtrGroup->GetSound();
#endif//#ifdef Tahara

	m_pMesh = new clsMISSION_MODEL;

	m_pMesh->Create(m_wpResource, pRobo);

	m_Trans.vPos.y = 10.0f;

	m_fWalktMoveSpeedMax = 0.25f;
	m_iWalkTopSpeedFrame = 5;

	m_fBoostMoveSpeedMax = 0.5;
	m_iBoostTopSpeedFrame = 60;

	m_fQuickBoostSpeedMax = m_fBoostMoveSpeedMax * 3.0f;
	m_iQuickBoostTopSpeedTime = 1 * static_cast<int>(g_fFPS);

	m_fQuickTrunSpeedMax = (float)D3DX_PI / g_iQuickTurnFrame;
	m_iQuickTrunTopSpeedTime = 15;

	m_fBoostRisingSpeedMax = 0.5f;//スピードの最大値.
	m_iBoostRisingTopSpeedFrame = 20;//↑に達するまでのフレーム値.
	m_fBoostRisingAccele = m_fBoostRisingSpeedMax / m_iBoostRisingTopSpeedFrame;// = m_fMoveSpeedMax / m_fTopSpeedFrame;
	m_fBoostFollRes = m_fBoostRisingSpeedMax / 10;

	SetMoveAcceleSpeed(m_fWalktMoveSpeedMax, m_iWalkTopSpeedFrame);
	SetMoveDeceleSpeed(m_iTopMoveSpeedFrame);

	m_iQuickBoostEnelgyCost = 1000;
	m_iQuickTrunEnelgyCost = 1000;
	m_iBoostRisingyCost = 100;

	m_iEnelgyMax = 10000;
	m_iEnelgy = m_iEnelgyMax;
	m_iEnelgyOutput = 1500 / static_cast<int>(g_fFPS);
	m_iBoostFloatRecovery = m_iEnelgyOutput / 2;

	SetRotAcceleSpeed(0.01f, 30);
	SetJumpPower(0.5f);

	m_iActivityLimitTime = 300 * static_cast<int>(g_fFPS);

	m_MaxHP = 5000;
	m_HP = m_MaxHP;

	WeaponState WS[enWeaponTypeSize];

	for (int i = 0; i < enWeaponTypeSize; i++)
	{
		//float型はintで入ってきたステータスにあれこれして計算する.

		WS[i].iAtk = 5;
		WS[i].iBulletNumMax = 10;
		WS[i].iLockSpeed = 10;
		WS[i].iShotEN = 0;
		WS[i].iReloadTime = 10;
		WS[i].iStablity = 0;
		WS[i].MagazineReloadTime = 60;

		WS[i].BState.fRangeMax = 1.5f;
		WS[i].BState.fScale = 0.5f;
		WS[i].BState.fSpeed = 5.0f;
		WS[i].BState.iHitEfcNum = 0;
		WS[i].BState.iLineEfcNum = 3;
		WS[i].BState.iSEHitNum = 0;
		WS[i].BState.iSEShotNum = 0;
		WS[i].BState.iShotEfcNum = 3;
	}

	WeaponInit(pPtrGroup, WS, enWeaponTypeSize);

	SetBoostEffect();

	m_fLockRange = 500.0f;//ロックオン距離.
	m_fLockCircleRadius = 500.0f;//ロックオン判定の半径.

	m_pViewPort = pPtrGroup->GetViewPort10();
}

void clsRobo::Walk()
{
	SetMoveAcceleSpeed(m_fWalktMoveSpeedMax, m_iWalkTopSpeedFrame);
	m_iMoveStopFrame = m_iWalkTopSpeedFrame;
	
	m_bBoost = false;
}

void clsRobo::Boost()
{
	SetMoveAcceleSpeed(m_fBoostMoveSpeedMax, m_iBoostTopSpeedFrame);
	m_iMoveStopFrame = m_iBoostTopSpeedFrame;

	m_bBoost = true;
}

void clsRobo::MoveSwitch()
{
	if (IsMoveControl())
	{
		if (m_bBoost)
		{
			Walk();
		}

		else
		{
			Boost();
		}
	}
}

void clsRobo::BoostRising()
{
	if (!m_bGround || IsMoveing())
	{
		if (EnelgyConsumption(m_iBoostRisingyCost))
		{
			if (m_fFollPower < m_fBoostMoveSpeedMax)
			{
				m_fFollPower += m_fBoostRisingAccele;
			}

			else
			{
				m_fFollPower = m_fBoostMoveSpeedMax;
			}

			if (!m_bBoost)
			{
				Boost();
			}
		}

		else
		{
			Walk();
		}
	}

	else
	{
		Jump();
	}
}

void clsRobo::SetDirQuickBoost(const float fAngle)
{
	//クイックブーストするため、強制的に移動方向を決定.
	if (IsMoveControl())
	{
		D3DXVECTOR3 vForward;

		//今向いている方向.
		vForward = GetVec3Dir(m_Trans.fYaw, g_vDirForward);

		//行きたい方向.
		m_vMoveDir = GetVec3Dir(fAngle, vForward);
	}
}

void clsRobo::QuickBoost()
{
	if (IsMoveControl())
	{
		if (EnelgyConsumption(m_iQuickBoostEnelgyCost))
		{
			if (m_iQuickInterbal < 0)
			{
				m_iQuickInterbal = g_iQuickInterbal;
				m_fMoveSpeed = m_fQuickBoostSpeedMax;
				m_iQuickBoostDecStartTime = m_iQuickBoostTopSpeedTime;
				SetMoveDeceleSpeed(m_iQuickInterbal);
			}
		}
	}
}

void clsRobo::SetDirQuickTurn(const float fAngle)
{
	if (!m_bBoost)
	{
		if (IsRotControl())
		{
			float fTmp = (int)D3DX_PI * (fAngle / abs(fAngle));
			SetRotDir(fTmp);
		}
	}
}

void clsRobo::QuickTurn()
{
	if (!m_bBoost)
	{
		if (EnelgyConsumption(m_iQuickTrunEnelgyCost))
		{
			if (IsRotControl())
			{
				if (m_iQuickInterbal < 0)
				{
					m_iQuickInterbal = g_iQuickInterbal;
					m_fRotSpeed = m_fQuickTrunSpeedMax;
					m_iQuickTrunDecStartTime = m_iQuickTrunTopSpeedTime;
					SetRotDeceleSpeed(m_iQuickInterbal);
				}
			}
		}
	}
}

void clsRobo::Updata()
{
	

	PlayBoostEfc();
	CharactorUpdate();
	m_vAcceleDir = { 0.0f, 0.0f, 0.0f };//ブースターエフェクト発生に使っているので毎フレームの初期化が必要になる.

	if (m_iQuickBoostDecStartTime > 0)//クイックブースト.
	{
		m_fMoveSpeed = m_fQuickBoostSpeedMax;
		m_iQuickBoostDecStartTime--;
	}

	if (m_iQuickTrunDecStartTime > 0)//クイックターン.
	{
		m_fRotSpeed = m_fQuickTrunSpeedMax;
		m_iQuickTrunDecStartTime--;
	}

	if (IsMoveControl())
	{
		if (!m_bBoost)
		{
			Walk();
		}

		else
		{
			Boost();
		}
	}

	if (m_bBoost)
	{
		if (m_fFollPower < -m_fBoostFollRes)
		{
			m_fFollPower += m_fBoostFollRes;

			if (m_fFollPower > -m_fBoostFollRes)
			{
				m_fFollPower = -m_fBoostFollRes;
			}
		}
	}

	if (IsRotControl())
	{
		SetRotDeceleSpeed(m_iRotStopFrame);
	}

	if (m_iQuickInterbal >= 0)
	{
		m_iQuickInterbal--;
	}

	WeaponUpdate();

	EnelgyRecovery();

	UpdataLimitTime();

	
}

void clsRobo::UpdataLimitTime()
{
	if (m_iActivityLimitTime < 0)
	{
		m_bTimeUp = true;
	}

	else
	{
		m_iActivityLimitTime--;
	}
}

void clsRobo::EnelgyRecovery()
{
	SetEnelgyRecoveryAmount();

	m_iEnelgy += m_iEnelgyRecoveryPoint;

	if (m_iEnelgy > m_iEnelgyMax)
	{
		m_iEnelgy = m_iEnelgyMax;
	}
}

void clsRobo::SetEnelgyRecoveryAmount()
{
	m_iEnelgyRecoveryPoint = m_iEnelgyOutput;

	if (m_bBoost)
	{
		if (!m_bGround)
		{
			m_iEnelgyRecoveryPoint -= m_iBoostFloatRecovery;
		}

		m_iEnelgyRecoveryPoint -= m_iBoostMoveCost;
	}

	if (false)//射撃準備完了.
	{
		m_iEnelgyRecoveryPoint -= (m_iEnelgyOutput);
	}
}

bool clsRobo::EnelgyConsumption(const int iConsumption)
{
	if (m_iEnelgy >= iConsumption)
	{
		m_iEnelgy -= iConsumption;
		return true;
	}

	return false;
}

void clsRobo::UpdatePosfromBone()
{
	m_vCenterPos = m_pMesh->GetBonePos(enPARTS::CORE, "Jenerator");

	m_vLockRangePos = m_pMesh->GetBonePos(enPARTS::HEAD, "Center");

	m_v_vMuzzlePos[enWeaponLHand] = m_pMesh->GetBonePos(enPARTS::WEAPON_L, "MuzzleEnd");
	m_v_vShotDir[enWeaponLHand] = m_v_vMuzzlePos[enWeaponLHand] - m_pMesh->GetBonePos(enPARTS::WEAPON_L, "MuzzleRoot");

	m_v_vMuzzlePos[enWeaponRHand] = m_pMesh->GetBonePos(enPARTS::WEAPON_R, "MuzzleEnd");
	m_v_vShotDir[enWeaponRHand] = m_v_vMuzzlePos[enWeaponRHand] - m_pMesh->GetBonePos(enPARTS::WEAPON_R, "MuzzleRoot");

	for (int i = 0; i < enWeaponTypeSize; i++)
	{
		D3DXVec3Normalize(&m_v_vShotDir[i], &m_v_vShotDir[i]);
	}
}

void clsRobo::ShotLWeapon()
{
	ShotSwich(enWeaponLHand);
	if (Shot())
	{
		EnelgyConsumption(m_v_pWeapons[enWeaponLHand]->GetShotEN());
		//射撃アニメ処理.

		
	}

	else
	{
		if (Reload())
		{
			//リロードアニメ処理.
		}
	}
}

void clsRobo::ShotRWeapon()
{
	ShotSwich(enWeaponRHand);
	if (Shot())
	{
		EnelgyConsumption(m_v_pWeapons[enWeaponLHand]->GetShotEN());
		//射撃アニメ処理.
	}

	else
	{
		if (Reload())
		{
			//リロードアニメ処理.
		}
	}
}

void clsRobo::SetBoostEffect()
{
	SetRHandFrontBoostEffect();
	SetRHandSideBoostEffect();
	SetRHandBackBoostEffect();

	SetLHandFrontBoostEffect();
	SetLHandSideBoostEffect();
	SetLHandBackBoostEffect();

	SetCoreBoostEffect();

	SetLegBoostEffect();
}

void clsRobo::SetRHandFrontBoostEffect()
{
	enPARTS PartsNum = enPARTS::ARM_R;

	std::string strBoostRootBoneName = "BoosterFront";

	int iEfcNum = GetBoostEfcNum(PartsNum, strBoostRootBoneName.c_str());

	m_v_RHandFrontBoostEfc.resize(iEfcNum);

	for (unsigned int i = 0; i < m_v_RHandFrontBoostEfc.size(); i++)
	{
		m_v_RHandFrontBoostEfc[i] = 0;
	}
}

void clsRobo::SetRHandSideBoostEffect()
{
	enPARTS PartsNum = enPARTS::ARM_R;

	std::string strBoostRootBoneName = "BoosterSide";

	int iEfcNum = GetBoostEfcNum(PartsNum, strBoostRootBoneName.c_str());

	m_v_RHandSideBoostEfc.resize(iEfcNum);

	for (unsigned int i = 0; i < m_v_RHandSideBoostEfc.size(); i++)
	{
		m_v_RHandSideBoostEfc[i] = 0;
	}
}

void clsRobo::SetRHandBackBoostEffect()
{
	enPARTS PartsNum = enPARTS::ARM_R;

	std::string strBoostRootBoneName = "BoosterBack";

	int iEfcNum = GetBoostEfcNum(PartsNum, strBoostRootBoneName.c_str());

	m_v_RHandBackBoostEfc.resize(iEfcNum);

	for (unsigned int i = 0; i < m_v_RHandBackBoostEfc.size(); i++)
	{
		m_v_RHandBackBoostEfc[i] = 0;
	}
}

void clsRobo::SetLHandFrontBoostEffect()
{
	enPARTS PartsNum = enPARTS::ARM_L;

	std::string strBoostRootBoneName = "BoosterFront";

	int iEfcNum = GetBoostEfcNum(PartsNum, strBoostRootBoneName.c_str());

	m_v_LHandFrontBoostEfc.resize(iEfcNum);

	for (unsigned int i = 0; i < m_v_LHandFrontBoostEfc.size(); i++)
	{
		m_v_LHandFrontBoostEfc[i] = 0;
	}
}

void clsRobo::SetLHandSideBoostEffect()
{
	enPARTS PartsNum = enPARTS::ARM_L;

	std::string strBoostRootBoneName = "BoosterSide";

	int iEfcNum = GetBoostEfcNum(PartsNum, strBoostRootBoneName.c_str());

	m_v_LHandSideBoostEfc.resize(iEfcNum);

	for (unsigned int i = 0; i < m_v_LHandSideBoostEfc.size(); i++)
	{
		m_v_LHandSideBoostEfc[i] = 0;
	}
}

void clsRobo::SetLHandBackBoostEffect()
{
	enPARTS PartsNum = enPARTS::ARM_L;

	std::string strBoostRootBoneName = "BoosterBack";

	int iEfcNum = GetBoostEfcNum(PartsNum, strBoostRootBoneName.c_str());

	m_v_LHandBackBoostEfc.resize(iEfcNum);

	for (unsigned int i = 0; i < m_v_LHandBackBoostEfc.size(); i++)
	{
		m_v_LHandBackBoostEfc[i] = 0;
	}
}

void clsRobo::SetCoreBoostEffect()
{
	enPARTS PartsNum = enPARTS::CORE;

	std::string strBoostRootBoneName = "Booster";

	int iEfcNum = GetBoostEfcNum(PartsNum, strBoostRootBoneName.c_str());

	m_v_CoreBoostEfc.resize(iEfcNum);

	for (unsigned int i = 0; i < m_v_CoreBoostEfc.size(); i++)
	{
		m_v_CoreBoostEfc[i] = 0;
	}
}

void clsRobo::SetLegBoostEffect()
{
	enPARTS PartsNum = enPARTS::LEG;

	std::string strBoostRootBoneName = "Booster";

	int iEfcNum = GetBoostEfcNum(PartsNum, strBoostRootBoneName.c_str());

	m_v_LegBoostEfc.resize(iEfcNum);

	for (unsigned int i = 0; i < m_v_LegBoostEfc.size(); i++)
	{
		m_v_LegBoostEfc[i] = 0;
	}
}

int clsRobo::GetBoostEfcNum(enPARTS PartsNum, const char* strBoostPosition)
{
	std::string strBoostRootBoneName = strBoostPosition;
	strBoostRootBoneName += "Root";

	std::string strBoostEndBoneName = strBoostPosition;
	strBoostEndBoneName += "End";

	int iRootNum = m_pMesh->GetSimilarityNameBoneNum(PartsNum, strBoostRootBoneName.c_str());
	int iEndNum = m_pMesh->GetSimilarityNameBoneNum(PartsNum, strBoostEndBoneName.c_str());

	int iResult = 0;
	if (iRootNum > iEndNum)
	{
		iResult = iEndNum;
	}

	else
	{
		iResult = iRootNum;
	}

	return iResult;
}

bool clsRobo::IsLWeaponLock()
{
	if (m_v_pWeapons[enWeaponLHand]->IsLock())
	{
		return true;
	}

	return false;
}

bool clsRobo::IsRWeaponLock()
{
	if (m_v_pWeapons[enWeaponRHand]->IsLock())
	{
		return true;
	}

	return false;
}

void clsRobo::PlayBoostEfc()
{
	m_vMoveDirforBoost = GetVec3Dir(-m_Trans.fYaw, m_vAcceleDir);
	m_vMoveDirforBoost.z = +m_vMoveDirforBoost.z;

	PlayFrontBoostEfc();
	PlayRightBoostEfc();
	PlayLeftBoostEfc();
	PlayBackBoostEfc();

	PlayLegBoostEfc();
}

void clsRobo::PlayFrontBoostEfc()
{
	clsOPERATION_STRING OprtStr;//ボーン名と番号を繋げる役割.

	if (m_vMoveDirforBoost.z < -0.1f)
	{
		std::string strBoostRootName = "BoosterFrontRoot";
		std::string strBoostEndName = "BoosterFrontEnd";
		std::string strBoostRootNameTmp = "";
		std::string strBoostEndNameTmp = "";

		D3DXVECTOR3 vPosRotTmp = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 vPosEndTmp = { 0.0f, 0.0f, 0.0f };

		for (unsigned int i = 0; i < m_v_LHandFrontBoostEfc.size(); i++)
		{
			//付け根の名前を生成.
			strBoostRootNameTmp = strBoostRootName;
			strBoostRootNameTmp = OprtStr.ConsolidatedNumber(strBoostRootNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			strBoostEndNameTmp = strBoostEndName;
			strBoostEndNameTmp = OprtStr.ConsolidatedNumber(strBoostEndNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			vPosRotTmp = m_pMesh->GetDirfromBone(enPARTS::ARM_L, strBoostRootName.c_str(), strBoostEndName.c_str());
			vPosEndTmp = m_pMesh->GetBonePos(enPARTS::ARM_L, strBoostEndNameTmp.c_str());

			if (!m_wpEffects->isPlay(m_v_LHandFrontBoostEfc[i]))
			{
				m_v_LHandFrontBoostEfc[i] = m_wpEffects->Play(3, vPosEndTmp);
			}

			else
			{
				m_wpEffects->SetPosition(m_v_LHandFrontBoostEfc[i], vPosEndTmp);
			}

			m_wpEffects->SetRotation(m_v_LHandFrontBoostEfc[i], vPosRotTmp);
		}

		for (unsigned int i = 0; i < m_v_RHandFrontBoostEfc.size(); i++)
		{
			//付け根の名前を生成.
			strBoostRootNameTmp = strBoostRootName;
			strBoostRootNameTmp = OprtStr.ConsolidatedNumber(strBoostRootNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			strBoostEndNameTmp = strBoostEndName;
			strBoostEndNameTmp = OprtStr.ConsolidatedNumber(strBoostEndNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			vPosRotTmp = m_pMesh->GetDirfromBone(enPARTS::ARM_R, strBoostRootName.c_str(), strBoostEndName.c_str());

			vPosEndTmp = m_pMesh->GetBonePos(enPARTS::ARM_R, strBoostEndNameTmp.c_str());
			if (!m_wpEffects->isPlay(m_v_RHandFrontBoostEfc[i]))
			{
				m_v_RHandFrontBoostEfc[i] = m_wpEffects->Play(3, vPosEndTmp);
			}

			else
			{
				m_wpEffects->SetPosition(m_v_RHandFrontBoostEfc[i], vPosEndTmp);
			}
			m_wpEffects->SetRotation(m_v_RHandFrontBoostEfc[i], vPosRotTmp);
			
		}
	}

	else
	{
		for (unsigned int i = 0; i < m_v_LHandFrontBoostEfc.size(); i++)
		{
			if (m_wpEffects->isPlay(m_v_LHandFrontBoostEfc[i]))
			{
				m_wpEffects->Stop(m_v_LHandFrontBoostEfc[i]);
			}
		}

		for (unsigned int i = 0; i < m_v_RHandFrontBoostEfc.size(); i++)
		{
			if (m_wpEffects->isPlay(m_v_RHandFrontBoostEfc[i]))
			{
				m_wpEffects->Stop(m_v_RHandFrontBoostEfc[i]);
			}
		}
	}
}

void clsRobo::PlayRightBoostEfc()
{
	clsOPERATION_STRING OprtStr;//ボーン名と番号を繋げる役割.

	if (m_vMoveDirforBoost.x < -0.1f)
	{
		std::string strBoostRootName = "BoosterSideRoot";
		std::string strBoostEndName = "BoosterSideEnd";
		std::string strBoostRootNameTmp = "";
		std::string strBoostEndNameTmp = "";

		D3DXVECTOR3 vPosRotTmp = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 vPosEndTmp = { 0.0f, 0.0f, 0.0f };

		for (unsigned int i = 0; i < m_v_RHandSideBoostEfc.size(); i++)
		{
			//付け根の名前を生成.
			strBoostRootNameTmp = strBoostRootName;
			strBoostRootNameTmp = OprtStr.ConsolidatedNumber(strBoostRootNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			strBoostEndNameTmp = strBoostEndName;
			strBoostEndNameTmp = OprtStr.ConsolidatedNumber(strBoostEndNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			vPosRotTmp = m_pMesh->GetDirfromBone(enPARTS::ARM_R, strBoostRootName.c_str(), strBoostEndName.c_str());

			vPosEndTmp = m_pMesh->GetBonePos(enPARTS::ARM_R, strBoostEndNameTmp.c_str());
			if (!m_wpEffects->isPlay(m_v_RHandSideBoostEfc[i]))
			{
				m_v_RHandSideBoostEfc[i] = m_wpEffects->Play(3, vPosEndTmp);
			}

			else
			{
				m_wpEffects->SetPosition(m_v_RHandSideBoostEfc[i], vPosEndTmp);
			}

			m_wpEffects->SetRotation(m_v_RHandSideBoostEfc[i], vPosRotTmp);
			
		}
	}

	else
	{
		for (unsigned int i = 0; i < m_v_RHandSideBoostEfc.size(); i++)
		{
			if (m_wpEffects->isPlay(m_v_RHandSideBoostEfc[i]))
			{
				m_wpEffects->Stop(m_v_RHandSideBoostEfc[i]);
			}
		}
	}
}

void clsRobo::PlayLeftBoostEfc()
{
	clsOPERATION_STRING OprtStr;//ボーン名と番号を繋げる役割.

	if (m_vMoveDirforBoost.x > 0.1f)
	{
		std::string strBoostRootName = "BoosterSideRoot";
		std::string strBoostEndName = "BoosterSideEnd";
		std::string strBoostRootNameTmp = "";
		std::string strBoostEndNameTmp = "";

		D3DXVECTOR3 vPosRotTmp = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 vPosEndTmp = { 0.0f, 0.0f, 0.0f };

		for (unsigned int i = 0; i < m_v_LHandSideBoostEfc.size(); i++)
		{
			//付け根の名前を生成.
			strBoostRootNameTmp = strBoostRootName;
			strBoostRootNameTmp = OprtStr.ConsolidatedNumber(strBoostRootNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			strBoostEndNameTmp = strBoostEndName;
			strBoostEndNameTmp = OprtStr.ConsolidatedNumber(strBoostEndNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			vPosRotTmp = m_pMesh->GetDirfromBone(enPARTS::ARM_L, strBoostRootName.c_str(), strBoostEndName.c_str());

			vPosEndTmp = m_pMesh->GetBonePos(enPARTS::ARM_L, strBoostEndNameTmp.c_str());

			if (!m_wpEffects->isPlay(m_v_LHandSideBoostEfc[i]))
			{
				m_v_LHandSideBoostEfc[i] = m_wpEffects->Play(3, vPosEndTmp);
			}

			else
			{
				m_wpEffects->SetPosition(m_v_LHandSideBoostEfc[i], vPosEndTmp);
			}

			m_wpEffects->SetRotation(m_v_LHandSideBoostEfc[i], vPosRotTmp);
		}
	}

	else
	{
		for (unsigned int i = 0; i < m_v_LHandSideBoostEfc.size(); i++)
		{
			if (m_wpEffects->isPlay(m_v_LHandSideBoostEfc[i]))
			{
				m_wpEffects->Stop(m_v_LHandSideBoostEfc[i]);
			}
		}
	}
}

void clsRobo::PlayBackBoostEfc()
{
	clsOPERATION_STRING OprtStr;//ボーン名と番号を繋げる役割.

	if (m_vMoveDirforBoost.z > 0.1f)
	{
		std::string strBoostRootName = "BoosterBackRoot";
		std::string strBoostEndName = "BoosterBackEnd";
		std::string strBoostRootNameTmp = "";
		std::string strBoostEndNameTmp = "";

		D3DXVECTOR3 vPosRotTmp = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 vPosEndTmp = { 0.0f, 0.0f, 0.0f };

		for (unsigned int i = 0; i < m_v_LHandBackBoostEfc.size(); i++)
		{
			//付け根の名前を生成.
			strBoostRootNameTmp = strBoostRootName;
			strBoostRootNameTmp = OprtStr.ConsolidatedNumber(strBoostRootNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			strBoostEndNameTmp = strBoostEndName;
			strBoostEndNameTmp = OprtStr.ConsolidatedNumber(strBoostEndNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			vPosRotTmp = m_pMesh->GetDirfromBone(enPARTS::ARM_L, strBoostRootName.c_str(), strBoostEndName.c_str());

			vPosEndTmp = m_pMesh->GetBonePos(enPARTS::ARM_L, strBoostEndNameTmp.c_str());

			if (!m_wpEffects->isPlay(m_v_LHandBackBoostEfc[i]))
			{
				m_v_LHandBackBoostEfc[i] = m_wpEffects->Play(3, vPosEndTmp);
			}

			else
			{
				m_wpEffects->SetPosition(m_v_LHandBackBoostEfc[i], vPosEndTmp);
			}

			m_wpEffects->SetRotation(m_v_LHandBackBoostEfc[i], vPosRotTmp);
		}

		for (unsigned int i = 0; i < m_v_RHandBackBoostEfc.size(); i++)
		{
			//付け根の名前を生成.
			strBoostRootNameTmp = strBoostRootName;
			strBoostRootNameTmp = OprtStr.ConsolidatedNumber(strBoostRootNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			strBoostEndNameTmp = strBoostEndName;
			strBoostEndNameTmp = OprtStr.ConsolidatedNumber(strBoostEndNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			vPosRotTmp = m_pMesh->GetDirfromBone(enPARTS::ARM_R, strBoostRootName.c_str(), strBoostEndName.c_str());

			vPosEndTmp = m_pMesh->GetBonePos(enPARTS::ARM_R, strBoostEndNameTmp.c_str());

			if (!m_wpEffects->isPlay(m_v_RHandBackBoostEfc[i]))
			{
				m_v_RHandBackBoostEfc[i] = m_wpEffects->Play(3, vPosEndTmp);
			}

			else
			{
				m_wpEffects->SetPosition(m_v_RHandBackBoostEfc[i], vPosEndTmp);
			}
			m_wpEffects->SetRotation(m_v_RHandBackBoostEfc[i], vPosRotTmp);
		}

		strBoostRootName = "BoosterRoot";
		strBoostEndName = "BoosterEnd";

		for (unsigned int i = 0; i < m_v_CoreBoostEfc.size(); i++)
		{
			//付け根の名前を生成.
			strBoostRootNameTmp = strBoostRootName;
			strBoostRootNameTmp = OprtStr.ConsolidatedNumber(strBoostRootNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			strBoostEndNameTmp = strBoostEndName;
			strBoostEndNameTmp = OprtStr.ConsolidatedNumber(strBoostEndNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			vPosRotTmp = m_pMesh->GetDirfromBone(enPARTS::CORE, strBoostRootName.c_str(), strBoostEndName.c_str());

			vPosEndTmp = m_pMesh->GetBonePos(enPARTS::CORE, strBoostEndNameTmp.c_str());
			if (!m_wpEffects->isPlay(m_v_CoreBoostEfc[i]))
			{
				m_v_CoreBoostEfc[i] = m_wpEffects->Play(3, vPosEndTmp);
			}

			else
			{
				m_wpEffects->SetPosition(m_v_CoreBoostEfc[i], vPosEndTmp);
			}
			m_wpEffects->SetRotation(m_v_CoreBoostEfc[i], vPosRotTmp);
			m_wpEffects->SetScale(m_v_CoreBoostEfc[i], 2.0f);

		}
	}

	else
	{
		for (unsigned int i = 0; i < m_v_LHandBackBoostEfc.size(); i++)
		{
			if (m_wpEffects->isPlay(m_v_LHandBackBoostEfc[i]))
			{
				m_wpEffects->Stop(m_v_LHandBackBoostEfc[i]);
			}
		}

		for (unsigned int i = 0; i < m_v_RHandBackBoostEfc.size(); i++)
		{
			if (m_wpEffects->isPlay(m_v_RHandBackBoostEfc[i]))
			{
				m_wpEffects->Stop(m_v_RHandBackBoostEfc[i]);
			}
		}

		for (unsigned int i = 0; i < m_v_CoreBoostEfc.size(); i++)
		{
			if (m_wpEffects->isPlay(m_v_CoreBoostEfc[i]))
			{
				m_wpEffects->Stop(m_v_CoreBoostEfc[i]);
			}
		}
	}
}

void clsRobo::PlayLegBoostEfc()
{
	clsOPERATION_STRING OprtStr;//ボーン名と番号を繋げる役割.

	if (m_bBoost)
	{
		std::string strBoostRootName = "BoosterRoot";
		std::string strBoostEndName = "BoosterEnd";
		std::string strBoostRootNameTmp = "";
		std::string strBoostEndNameTmp = "";

		D3DXVECTOR3 vPosRotTmp = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 vPosEndTmp = { 0.0f, 0.0f, 0.0f };

		for (unsigned int i = 0; i < m_v_LegBoostEfc.size(); i++)
		{
			//付け根の名前を生成.
			strBoostRootNameTmp = strBoostRootName;
			strBoostRootNameTmp = OprtStr.ConsolidatedNumber(strBoostRootNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			strBoostEndNameTmp = strBoostEndName;
			strBoostEndNameTmp = OprtStr.ConsolidatedNumber(strBoostEndNameTmp, i, cBONE_NAME_NUM_DIGIT_JOINT);

			vPosRotTmp = m_pMesh->GetDirfromBone(enPARTS::LEG, strBoostRootName.c_str(), strBoostEndName.c_str());

			vPosEndTmp = m_pMesh->GetBonePos(enPARTS::LEG, strBoostEndNameTmp.c_str());

			if (!m_wpEffects->isPlay(m_v_LegBoostEfc[i]))
			{
				m_v_LegBoostEfc[i] = m_wpEffects->Play(3, vPosEndTmp);
			}

			else
			{
				m_wpEffects->SetPosition(m_v_LegBoostEfc[i], vPosEndTmp);
			}
				
			m_wpEffects->SetRotation(m_v_LegBoostEfc[i], vPosRotTmp);
		}
	}

	else
	{
		for (unsigned int i = 0; i < m_v_LegBoostEfc.size(); i++)
		{
			if (m_wpEffects->isPlay(m_v_LegBoostEfc[i]))
			{
				m_wpEffects->Stop(m_v_LegBoostEfc[i]);
			}
		}
	}
}

clsRobo::clsRobo() :
m_pMesh(NULL),
m_bBoost(false),
m_fWalktMoveSpeedMax(0.0f),
m_iWalkTopSpeedFrame(0),
m_fBoostMoveSpeedMax(0.0f),
m_iBoostTopSpeedFrame(0),
m_fBoostRisingSpeedMax(0.0f),
m_iBoostRisingTopSpeedFrame(0),
m_fBoostRisingAccele(0.0f),
m_iQuickInterbal(0),
m_wpResource(nullptr),
m_wpEffects(nullptr),
m_wpSound(nullptr)
{
	
}

clsRobo::~clsRobo()
{
	if (m_pMesh != NULL)
	{
		delete m_pMesh;
		m_pMesh = NULL;
	}

#ifdef Tahara
	//消すときdeleteしないでnullしてね.
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
#endif//#ifdef Tahara
}