#include"Charactor.h"

static const int g_iDupSENo = 11;

void clsCharactor::CharactorUpdate()
{
	Move();
	Rotate();

	UpdateLookOn();

	m_iDamage = 0;//毎フレーム初期化.
	m_bRadarWarning = false;//初期化.
}

void clsCharactor::SetMoveAcceleSpeed(float fMoveSpeedMax, int iTopSpeedFrame)//加速.
{
	m_fMoveSpeedMax = fMoveSpeedMax;
	m_iTopMoveSpeedFrame = iTopSpeedFrame;

	m_fMoveAccele = m_fMoveSpeedMax / m_iTopMoveSpeedFrame;
}

void clsCharactor::SetMoveDeceleSpeed(const int iMoveStopFrame)//減速.
{
	m_iMoveStopFrame = iMoveStopFrame;
	m_iMoveReverseDirInertia = m_iMoveStopFrame / 2;

	m_fMoveDecele = abs(m_fMoveSpeed) / m_iMoveStopFrame;
}

void clsCharactor::AddMoveAccele(const float fAngle, const float fPush)
{
	if (m_bStopComMove)
	{
		return;
	}

	if (!IsMoveControl())
	{
		return;//最高速を超えている.
	}

	SetMoveDir(fAngle);//移動方向決定.
	MoveAccele(fPush);//加速.

	m_bMoveAcceleOlder = true;//加速を行った.
}

void clsCharactor::Move()
{
	if (!IsMoveControl() || !m_bMoveAcceleOlder)
	{
		//移動速度が最大を超えているか、加速がされていない.
		MoveDecele();//減速.
	}

	MoveControl();//移動.

	m_bMoveAcceleOlder = false;//リセット.
}

bool clsCharactor::IsMoveing()
{
	if (m_fMoveSpeed == 0.00f)
	{
		return false;
	}

	return true;
}

bool clsCharactor::IsMoveControl()
{
	if (m_fMoveSpeed > m_fMoveSpeedMax || m_fMoveSpeed <= -m_fMoveSpeedMax)
	{
		return false;
	}

	return true;
}

void clsCharactor::SetMoveDir(const float fAngle)
{
	if (!IsMoveing())
	{
		m_vMoveDir = { 0.0f, 0.0f, 0.0f };
	}

	D3DXVECTOR3 vForward;

	//今向いている方向.
	vForward = GetVec3Dir(m_Trans.fYaw, g_vDirForward);

	//行きたい方向.
	m_vAcceleDir = GetVec3Dir(fAngle, vForward);

	m_vMoveDir += (m_vAcceleDir - m_vMoveDir) / static_cast<float>(m_iMoveReverseDirInertia);
}

void clsCharactor::MoveControl()
{
	if (abs(m_fMoveSpeed) < 0.0f)
	{
		m_bMoving = false;
		return;
	}
	
	m_bMoving = true;

	D3DXVECTOR3 vHorMoveDir = {0.0f,0.0f,0.0f};

	vHorMoveDir.x = m_vMoveDir.x * m_vWallHit.x;
	vHorMoveDir.z = m_vMoveDir.z * m_vWallHit.z;

	m_Trans.vPos += m_vMoveDir * abs(m_fMoveSpeed);
}

void clsCharactor::MoveAccele(const float fPower)
{
	if (m_fMoveSpeed <= m_fMoveSpeedMax && m_fMoveSpeed > -m_fMoveSpeedMax)
	{
		m_fMoveSpeed += m_fMoveAccele * fPower;

		if (m_fMoveSpeed > m_fMoveSpeedMax || m_fMoveSpeed <= -m_fMoveSpeedMax)
		{
			m_fMoveSpeed = m_fMoveSpeedMax * (m_fMoveSpeed / abs(m_fMoveSpeed));
		}
	}

	SetMoveDeceleSpeed(m_iMoveStopFrame);
}

void clsCharactor::MoveDecele()
{
	if (m_fMoveSpeed > m_fMoveDecele)
	{
		m_fMoveSpeed -= m_fMoveDecele;
	}

	else
	{
		m_fMoveSpeed = 0.00f;
		m_vMoveDir = GetVec3Dir(m_Trans.fYaw, g_vDirForward);
	}
}

//回転.

bool clsCharactor::IsRotate()
{
	return m_bRotation;
}

bool clsCharactor::IsRotControl()
{
	if (abs(m_fRotSpeed) > m_fRotSpeedMax)
	{
		m_bRotation = false;
		return false;
	}

	m_bRotation = true;
	return true;
}
void clsCharactor::RotAccele(const float fPower)
{
	m_fRotSpeed += m_fRotAccele * fPower;

	if (abs(m_fRotSpeed) > m_fRotSpeedMax)
	{
		m_fRotSpeed = m_fRotSpeedMax * (m_fRotSpeed / abs(m_fRotSpeed));
	}

	SetRotDeceleSpeed(m_iMoveStopFrame);
}

void clsCharactor::RotDecele()
{
	if (m_fRotSpeed > m_fRotDecele)
	{
		m_fRotSpeed -= m_fRotDecele;
	}

	else
	{
		m_fRotSpeed = 0.00f;
		m_bRotation = false;
	}
}

void clsCharactor::SetRotAcceleSpeed(const float fRotSpeedMax, const int iTopRotSpdFrame)
{
	m_fRotSpeedMax = fRotSpeedMax;
	m_iTopRotSpeedFrame = iTopRotSpdFrame;

	m_fRotAccele = m_fRotSpeedMax / m_iTopRotSpeedFrame;

	SetRotDeceleSpeed(m_iTopRotSpeedFrame);
}

void clsCharactor::SetRotDeceleSpeed(const int iRotStopFrame)
{
	m_iRotStopFrame = iRotStopFrame;

	m_fRotDecele = abs(m_fRotSpeed) / m_iRotStopFrame;
}

void clsCharactor::SetRotDir(float fAngle)
{
	m_fRotDir = m_Trans.fYaw + fAngle;
}

void clsCharactor::AddRotAccele(const float fAngle, const float fPush)
{
	if (m_bStopComRot)
	{
		return;
	}

	if (!IsRotControl())
	{
		return;//最高速を超えている.
	}

	float fPushPower = abs(fPush);

	SetRotDir(fAngle);
	RotAccele(fPush);

	m_bRotAcceleOlder = true;

}

void clsCharactor::Rotate()
{
	if (!IsRotControl() || !m_bRotAcceleOlder)
	{
		//最高速を超えているか、加速が行われていない.m_bRotAcceleOlder
		RotDecele();//減速.
	}

	Spin(m_Trans.fYaw, m_fRotDir, m_fRotSpeed);//回転.

	m_bRotAcceleOlder = false;//リセット.
}

void clsCharactor::LookUp(const float fAngle, const float fPush)
{
	float fLookDir = m_fVerLookDir + fPush;

	Spin(m_fVerLookDir, fLookDir, m_fRotSpeedMax);

	const float fPitchMax = static_cast<float>D3DXToRadian(75);//90°になると視界がおかしくなるため防止.

	if (abs(m_fVerLookDir) > fPitchMax)
	{
		m_fVerLookDir = fPitchMax * (m_fVerLookDir / abs(m_fVerLookDir));
	}
}

void clsCharactor::Jump()
{
	if (m_bGround)
	{
		m_fFollPower = m_fJumpPower;
	}
}

const bool clsCharactor::IsTargetWall(
	const D3DXVECTOR3& vEndPos,
	clsStage* const pStage)
{
	if (!pStage)return false;

	std::vector<clsDX9Mesh*> vvpMeshTmp;
	vvpMeshTmp = pStage->GetStageMeshArray();

	for (unsigned int i = 0; i < vvpMeshTmp.size(); i++)
	{
		clsDX9Mesh* pObjMesh = vvpMeshTmp[i];

		if (!pObjMesh)continue;
		pStage->SetStageObjTransform(i);

		if (IsPointIntersect(m_vLockStartingPos, vEndPos, pObjMesh))
		{
			return false;
		}
	}

	return true;
}

//ﾀｰｹﾞｯﾄとする位置に対してﾚｲを飛ばして、遮蔽物がないか調べる.
bool clsCharactor::IsPointIntersect(
	const D3DXVECTOR3& StartPos,	//基準の位置.
	const D3DXVECTOR3& EndPos,		//標的の位置.
	const clsDX9Mesh* pTarget		//障害物の物体.
	)
{
	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//ﾚｲの開始終了位置をAttackerと合わせる.
	vecStart = StartPos;

	//標的の位置を終点に.
	vecEnd = EndPos;

	//対象が動いてる物体でも、対象のworld行列の、逆並列を用いれば正しくﾚｲが当たる.
	D3DXMATRIX mWorld, mWorldInv, mTrans, mRotate, mScale;
	D3DXMatrixTranslation(&mTrans,
		pTarget->m_Trans.vPos.x,
		pTarget->m_Trans.vPos.y,
		pTarget->m_Trans.vPos.z);

	D3DXMatrixRotationYawPitchRoll(&mRotate,
		pTarget->m_Trans.fYaw,
		pTarget->m_Trans.fPitch,
		pTarget->m_Trans.fRoll);

	D3DXMatrixScaling(&mScale,
		pTarget->m_Trans.vScale.x,
		pTarget->m_Trans.vScale.y,
		pTarget->m_Trans.vScale.z);

	mWorld = mScale * mRotate * mTrans;//もともと.

	//逆並列を求める.
	D3DXMatrixInverse(&mWorldInv, NULL, &mWorld);
	D3DXVec3TransformCoord(&vecStart, &vecStart, &mWorldInv);
	D3DXVec3TransformCoord(&vecEnd, &vecEnd, &mWorldInv);

	//距離を求める.
	vecDistance = vecEnd - vecStart;
	D3DXVec3Normalize(&vecDistance, &vecDistance);

	BOOL bHit = false;	//命中ﾌﾗｸﾞ.

	float fDis = static_cast<float>(0xffffffff);//最大値を入れる.

	D3DXIntersect(
		pTarget->m_pMesh,	//対象ﾒｯｼｭ.
		&vecStart,			//開始位置.
		&vecDistance,		//ﾚｲの距離.
		&bHit,				//(out)判定結果.
		NULL,	//(out)bHitがTrue時、ﾚｲの始点に.
		//最も近くの面のｲﾝﾃﾞｯｸｽ値へのﾎﾟｲﾝﾀ.
		NULL, NULL,				//(out)重心ﾋｯﾄ座標.
		&fDis,			//ﾀｰｹﾞｯﾄとの距離.
		NULL, NULL);

	float VecLenght = D3DXVec3Length(&(vecEnd - vecStart));

	if (fDis < VecLenght)//ﾚｲの範囲内に何かあるか?.
	{
		return true;//何かがある.
	}

	return false;//何もない.
}

bool clsCharactor::IsCurcleLange(
	const D3DXVECTOR3& CenterPos, 
	const D3DXVECTOR3& TargetPos, 
	float Range)//円の認識範囲判定.
{
	if (pow(TargetPos.x - CenterPos.x, 2.0f) + pow(TargetPos.y - CenterPos.y, 2.0f) <= pow(Range, 2.0f))
	{
		return true;
	}

	return false;
}

void clsCharactor::Spin(float& fNowRot, const float fTargetRot, const float fTurnSpd)
{
	const int iDegHulf = 180;
	const int iDegFull = 360;

	float fTarget = fTargetRot;

	if (fTarget - fNowRot > static_cast<float>D3DXToRadian(iDegHulf))
	{
		fTarget -= static_cast<float>D3DXToRadian(iDegFull);
	}
	else if (fTarget - fNowRot < -static_cast<float>D3DXToRadian(iDegHulf))
	{
		fTarget += static_cast<float>D3DXToRadian(iDegFull);
	}

	if (fNowRot > static_cast<float>D3DXToRadian(iDegHulf))
	{
		fNowRot -= static_cast<float>D3DXToRadian(iDegFull);
	}
	else if (fNowRot < -static_cast<float>D3DXToRadian(iDegHulf))
	{
		fNowRot += static_cast<float>D3DXToRadian(iDegFull);
	}

	//角度が近づく.
	if (abs(fTarget - fNowRot) > fTurnSpd)
	{
		if (fNowRot < fTarget)
		{
			fNowRot += fTurnSpd;
		}
		else if (fNowRot > fTarget)
		{
			fNowRot -= fTurnSpd;
		}
	}
	else
	{
		fNowRot = fTarget;
	}

	ObjRollOverGuard(&fNowRot);
}

bool clsCharactor::Shot()
{
	return m_v_pWeapons[m_iWeaponNum]->Shot();
}

bool clsCharactor::IsNeedReload()
{
	return m_v_pWeapons[m_iWeaponNum]->IsNeedReload();
}

void clsCharactor::ShotSwich(const int iWeaponNum)
{
	m_iWeaponNum = iWeaponNum;

	if (m_iWeaponNum > m_iWeaponNumMax)
	{
		m_iWeaponNum = 0;
	}
}

void clsCharactor::WeaponInit(clsPOINTER_GROUP* pPrt, WeaponState* pWeapon, const int iWeaponMax)
{
	m_iWeaponNumMax = iWeaponMax;
	m_iWeaponNum = 0;

	m_v_pWeapons.resize(iWeaponMax);
	m_v_vMuzzlePos.resize(iWeaponMax);
	m_v_vShotDir.resize(iWeaponMax);

	for (int i = 0; i < m_iWeaponNumMax; i++)
	{
		pWeapon[i].SState.vShotStartPos = &m_v_vMuzzlePos[i];
		pWeapon[i].SState.vShotMoveDir = &m_v_vShotDir[i];

		m_v_pWeapons[i] = new clsWeapon(pPrt);
		m_v_pWeapons[i]->Create(pWeapon[i]);
	}

	m_v_pWeapons.shrink_to_fit();
}

void  clsCharactor::WeaponUpdate(clsStage* const pStage)
{
	for (unsigned int i = 0; i < m_v_pWeapons.size(); i++)
	{
		m_v_pWeapons[i]->Update(pStage);
	}
}

HitState clsCharactor::BulletHit(std::vector<clsObject::SPHERE>& v_TargetSphere)
{
	HitState HitS;
	HitS.Clear();

	for (int i = 0; i < m_iWeaponNumMax; i++)
	{
		HitS.iDamage += m_v_pWeapons[i]->Hit(v_TargetSphere);
	}

	if (HitS.iDamage > 0)
	{
		HitS.bHit = true;
	}

	return HitS;
}

bool clsCharactor::Damage(const HitState& HitS)
{
	if (HitS.bHit)
	{
		m_iDamage += HitS.iDamage;

		if (m_iHP < HitS.iDamage)
		{
			m_iHP = 0;
			ActStop();
		}

		else
		{
			m_iHP -= HitS.iDamage;
		}

		return true;
	}

	return false;
}

void clsCharactor::LockChara(clsStage* const pStage)
{
	if (m_pTargetChara)
	{
		if (m_pTargetChara->m_bDeadFlg)//ターゲットが死亡.
		{
			LockOut();
			return;
		}
		
		if (!IsTargetWall(m_pTargetChara->GetCenterPos(), pStage))
		{
			LockOut();
			return;
		}

		if (!IsInLockRange(m_pTargetChara->m_vCenterPos))
		{
			LockOut();
			return;
		}

		Lock();

		D3DXMATRIX mW;
		D3DXMatrixIdentity(&mW);

		D3DXVECTOR3 vTmp = m_pTargetChara->GetCenterPos();

		D3DXVec3Project(&m_vTargetScrPos,
			&vTmp,
			m_pViewPort, &m_mProj,
			&m_mThisCharaView,
			&mW);
	}

	else
	{
		//新規にロックするターゲットを決定する.
		clsCharactor* pCharaTmp = nullptr;

		for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
		{
			if (m_v_pEnemys[i]->m_bDeadFlg)
			{
				continue;
			}

			if (!IsTargetWall(m_v_pEnemys[i]->GetCenterPos(), pStage))
			{
				continue;
			}

			if (IsInLockRange(m_v_pEnemys[i]->m_vCenterPos))
			{
				if (pCharaTmp)
				{
					float fPreviousDis = D3DXVec3Length(&(pCharaTmp->m_vCenterPos - m_vLockStartingPos));
					float fNowDis = D3DXVec3Length(&(m_v_pEnemys[i]->m_vCenterPos - m_vLockStartingPos));
				
					if (fNowDis < fPreviousDis)
					{
						//ロックするターゲットを仮設定.
						pCharaTmp = m_v_pEnemys[i];
						//m_iTargetNo = i;
					}
				}

				else
				{
					//ロックするターゲットを仮設定.
					pCharaTmp = m_v_pEnemys[i];
					//m_iTargetNo = i;
				}
			}
		}

		if (pCharaTmp)
		{
			//ロックするターゲットを確定.
			m_pTargetChara = pCharaTmp;

			D3DXMATRIX mW;
			D3DXMatrixIdentity(&mW);

			D3DXVECTOR3 vTmp = m_pTargetChara->GetPosition();

			D3DXVec3Project(&vTmp,
				&m_vTargetScrPos,
				m_pViewPort, &m_mProj,
				&m_mThisCharaView,
				&mW);
		}
	}
}

bool clsCharactor::IsInLockRange(const D3DXVECTOR3& vTargetPos)
{
	//敵の方向が後ろ.
	if (!IsTargetDirBack(vTargetPos))
	{
		return false;
	}

	float fDis = D3DXVec3Length(&(m_vLockStartingPos - vTargetPos));

	if (fDis > m_fLockRange)
	{
		return false;
	}

	D3DXMATRIX mW;
	D3DXMatrixIdentity(&mW);

	D3DXVec3Project(&m_vLockCenterPos,
		&m_vLockPos,
		m_pViewPort, &m_mProj,
		&m_mThisCharaView,
		&mW);

	D3DXVECTOR3 vTarPosTmp;

	D3DXVec3Project(&vTarPosTmp,
		&vTargetPos,
		m_pViewPort, &m_mProj,
		&m_mThisCharaView,
		&mW);

	if (IsCurcleLange(m_vLockCenterPos, vTarPosTmp, m_fLockCircleRadius / 4))
	{
		return true;
	}

	return false;
}

bool clsCharactor::IsTargetDirBack(const D3DXVECTOR3& vTargetPos)
{
	D3DXVECTOR3 vForword = GetVec3Dir(m_Trans.fYaw, g_vDirForward);

	D3DXVECTOR3 vTarDir = vTargetPos - m_vLockStartingPos;
	vTarDir.y = 0.0f;
	D3DXVec3Normalize(&vTarDir, &vTarDir);

	float fDir = D3DXVec3Dot(&vTarDir, &vForword);
	fDir = acosf(fDir);

	const float fForwardRangefromacos = 1.0f;//acosで出てくる正面方向の範囲.

	if (abs(fDir) < fForwardRangefromacos)
	{
		return true;
	}

	return false;
}

void clsCharactor::Lock()
{
	if (m_pTargetChara)
	{
		for (unsigned int i = 0; i < m_v_pWeapons.size(); i++)
		{
			m_v_pWeapons[i]->Lock(m_pTargetChara);

			if (m_v_pWeapons[i]->IsLock())
			{
				m_pTargetChara->m_bRadarWarning = true;
			}
		}
	}	
}

void clsCharactor::LockOut()
{
	if (m_pTargetChara)
	{
		m_pTargetChara = nullptr;
	}

	for (unsigned int i = 0; i < m_v_pWeapons.size(); i++)
	{
		m_v_pWeapons[i]->LockOut();
	}
}

void clsCharactor::UpdateLookOn()
{
	UpdateLookStartingPos();
	
	UpdateCamPos();

	UpdateLookCenterPos();
}

void clsCharactor::UpdateLookStartingPos()
{
	m_vLockStartingPos = GetCenterPos();
}

void clsCharactor::UpdateLookCenterPos()
{
	D3DXVECTOR3 vTmp = { 0.0f, 0.0f, 0.0f };

	D3DXMATRIX mRot;

	//カメラ位置のための回転行列作成.
	D3DXMatrixRotationYawPitchRoll(
		&mRot,
		m_Trans.fYaw,
		-m_fVerLookDir,
		m_Trans.fRoll);

	D3DXVec3TransformCoord(&vTmp, &g_vDirForward, &mRot);

	m_vLockRangeDir = vTmp;

	m_vLockPos = m_vLockStartingPos + m_vLockRangeDir * m_fLockRange;
	m_vLockCenterPos = m_vLockPos;

	D3DXVECTOR3 vUp = { 0.0f, 1.0f, 0.0f };

	D3DXMatrixLookAtLH(&m_mThisCharaView, &m_vCamPos, &m_vLockPos, &vUp);
}

const float clsCharactor::GetLockYaw()
{
	D3DXVECTOR3 vLockDirForward = m_vLockStartingPos - GetVec3Dir(m_Trans.fYaw,g_vDirBack);

	D3DXVECTOR3 m_vCamPosDirTmp;
	D3DXVec3Normalize(&m_vCamPosDirTmp, &m_vCamPos);

	D3DXVECTOR3 vCamDir = m_vLockStartingPos - m_vCamPosDirTmp;

	D3DXVec3Normalize(&vLockDirForward, &vLockDirForward);
	D3DXVec3Normalize(&vCamDir, &vCamDir);

	float fDir = D3DXVec3Dot(&vLockDirForward, &vCamDir);

	fDir = acos(fDir);

	return fDir;
}

void clsCharactor::UpdateCamPos()
{
	const float fCamMoveSpeed = 0.5f;
	const float fCamSpaceTmp = 4.0f;
	const float fCamPosX = 1.0f;

	D3DXMATRIX mRot;

	//カメラ位置のための回転行列作成.
	D3DXMatrixRotationYawPitchRoll(
		&mRot,
		m_Trans.fYaw,
		-m_fVerLookDir,
		m_Trans.fRoll);

	float fCamAxisXTmp;

	if (m_bCamPosXSwitch)
	{
		fCamAxisXTmp = fCamPosX;
	}

	else
	{
		fCamAxisXTmp = -fCamPosX;
	}

	D3DXVECTOR3 vCamAxis =
	{
		fCamAxisXTmp,
		m_vCenterPos.y - m_vLockStartingPos.y,
		fCamSpaceTmp
	};

	D3DXVec3TransformCoord(&vCamAxis, &vCamAxis, &mRot);

	//ﾍﾞｸﾄﾙそのものを回転状態により変換する.

	D3DXVECTOR3 vCamPosTmp;

	//====================================================
	//ｶﾒﾗ追従処理 ここから.
	//====================================================
	//カメラが少し遅れてついてくるように.
	//カメラが現在目的としている位置を算出.
	const D3DXVECTOR3 vCamTargetPos = m_vCenterPos - vCamAxis;

	//現在位置を取得し、現在位置と目的の位置の差から移動量を計算する.
	vCamPosTmp = m_vLockStartingPos;//現在位置を取得
	vCamPosTmp -= (vCamPosTmp - vCamTargetPos) * fCamMoveSpeed;

	m_vCamPos = vCamPosTmp;
}

void clsCharactor::CharaDuplicate(clsCharactor* const pContactChara)
{
	if (ObjectCollision(pContactChara->m_v_Spheres))
	{
		D3DXVECTOR3 vDir = GetPosition() - pContactChara->GetPosition();
		D3DXVec3Normalize(&vDir, &vDir);

		vDir.y = 0.0f;

		float fSpeed = abs(m_fMoveSpeed - pContactChara->m_fMoveSpeed);
		D3DXVECTOR3 vPosTmp;

		if (m_fMoveSpeed > pContactChara->m_fMoveSpeed)
		{
			vPosTmp = pContactChara->GetPosition() - (vDir * fSpeed);
			pContactChara->SetPosition(vPosTmp);
		}

		else
		{
			vPosTmp = GetPosition() + (vDir * fSpeed);
			SetPosition(vPosTmp);
		}

		m_wpSound->PlaySE(g_iDupSENo);
	}
}

const D3DXVECTOR3 clsCharactor::GetCamPos() const
{
	return m_vCamPos;
}

void clsCharactor::SetEnemys(std::vector<clsCharactor*> v_pEnemys)
{
	m_v_pEnemys = v_pEnemys;
}

void clsCharactor::CharaInit(clsPOINTER_GROUP* pPointer)
{
	m_pViewPort = pPointer->GetViewPort10();
}

const float clsCharactor::GetRaderRange()const
{
	return m_fRaderRange;
}

void clsCharactor::UpdateProduct(clsStage* pStage)
{
	clsObject::UpdateProduct(pStage);

	if (m_iHP <= 0)
	{
		Down();
	}

	Move();
	Rotate();

	WeaponUpdate(pStage);
	UpdateLookOn();

	m_vAcceleDir = { 0.0f, 0.0f, 0.0f };//ブースターエフェクト発生に使っているので毎フレームの初期化が必要になる.
	m_iDamage = 0;//毎フレーム初期化.
	m_bRadarWarning = false;//初期化.
}

void clsCharactor::Down()
{

}

void clsCharactor::Dead()
{

}

clsCharactor::clsCharactor() :
	m_iHP( 0 ),
	m_iMaxHP( 0 ),
	m_bDeadFlg( false ),
	m_bMoving( false ),
	m_bStopComMove(false),
	m_bStopComRot(false),
	m_bStopComShot(false),
	m_bPlayer(false),
	m_fMoveSpeedMax( 0.0f ),
	m_iTopMoveSpeedFrame( 0 ),
	m_fMoveAccele( 0.0f ),
	m_iMoveStopFrame( 0 ),
	m_fMoveDecele( 0.0f ),
	m_fRotSpeed( 0.0f ),
	m_fJumpPower( 0.0f ),
	m_fLockCircleRadius(0.0f),
	m_wpResource(nullptr),
	m_wpEffects(nullptr),
	m_wpSound(nullptr)
{

}

clsCharactor::~clsCharactor()
{
	if (m_pTargetChara)
	{
		m_pTargetChara = nullptr;
	}

#ifdef Tahara
	//消すときdeleteしないでnullしてね.
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
#endif//#ifdef Tahara
}

void HitState::Clear()
{
	bHit = false;
	iDamage = 0;
}