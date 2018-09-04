#include"Charactor.h"

void clsCharactor::CharactorUpdate()
{
	Move();
	Rotate();
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

	m_vMoveDir += (m_vAcceleDir - m_vMoveDir) / (m_iMoveReverseDirInertia);
}

void clsCharactor::MoveControl()
{
	if (abs(m_fMoveSpeed) < 0.0f)
	{
		m_bMoving = false;
		return;
	}
	
	m_bMoving = true;
	m_Trans.vPos += m_vMoveDir * abs(m_fMoveSpeed);
}

void clsCharactor::MoveAccele(const float fPower)//いらんかも.
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
	float fLookDir = m_fLookUpDir + fPush;

	Spin(m_fLookUpDir, fLookDir, m_fRotSpeedMax);

	const float fPitchMax = static_cast<float>D3DXToRadian(89);//90°になると視界がおかしくなるため防止.

	if (abs(m_fLookUpDir) > fPitchMax)
	{
		m_fLookUpDir = fPitchMax * (m_fLookUpDir / abs(m_fLookUpDir));
	}
}

void clsCharactor::Jump()
{
	if (m_bGround)
	{
		m_fFollPower = m_fJumpPower;
	}
}

//ﾀｰｹﾞｯﾄとする位置に対してﾚｲを飛ばして、遮蔽物がないか調べる.
bool clsCharactor::PointIntersect(
	const D3DXVECTOR3 StartPos,	//基準の位置.
	const D3DXVECTOR3 EndPos,		//標的の位置.
	const clsDX9Mesh* pTarget		//障害物の物体.
	)
{
	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//ﾚｲの開始終了位置をAttackerと合わせる.
	vecStart = StartPos;

	//標的の位置を終点に.
	vecEnd = EndPos;

	//対象が動いている物体でも、対象のworld行列の、
	//逆行列を用いれば、正しくﾚｲが当たる.
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(
		&matWorld,
		pTarget->m_Trans.vPos.x,
		pTarget->m_Trans.vPos.y,
		pTarget->m_Trans.vPos.z
		);

	//逆行列を求める.
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(
		&vecStart, &vecStart, &matWorld);

	D3DXVec3TransformCoord(
		&vecEnd, &vecEnd, &matWorld);

	//距離を求める.
	vecDistance = vecEnd - vecStart;

	D3DXVec3Normalize(&vecDistance, &vecDistance);

	//距離を求める.

	BOOL bHit = false;	//命中ﾌﾗｸﾞ.

	float fDis = 0xffffffff;//最大値を入れる.

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

bool clsCharactor::RecoLange(D3DXVECTOR3 CenterPos, D3DXVECTOR3 TargetPos, float Range)//円の認識範囲判定.
{
	if (pow(TargetPos.x - CenterPos.x, 2.0f) + pow(TargetPos.z - CenterPos.z, 2.0f) <= pow(Range, 2.0f))
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

bool clsCharactor::Reload()
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

void  clsCharactor::WeaponUpdate()
{
	for (int i = 0; i < m_v_pWeapons.size(); i++)
	{
		m_v_pWeapons[i]->Update();
	}
}

HitState clsCharactor::BulletHit(std::vector<clsObject::SPHERE> v_TargetSphere)
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

bool clsCharactor::Damage(HitState HitS)
{
	if (HitS.bHit)
	{
		if (m_HP < HitS.iDamage)
		{
			m_HP = 0;
			m_bDeadFlg = true;//仮.
		}

		else
		{
			m_HP -= HitS.iDamage;
		}

		return true;
	}

	return false;
}

void clsCharactor::LockChara()
{
	SetLockRangeDir();

	if (m_pTargetChara)
	{
		if (m_pTargetChara->m_bDeadFlg)//ターゲットが死亡.
		{
			LockOut();
			return;
		}

		if (IsInLockRange(m_pTargetChara->m_vCenterPos))
		{
			Lock();
		}

		else
		{
			LockOut();
			return;
		}
	}

	else
	{
		//新規にロックするターゲットを決定する.
		clsCharactor* pCharaTmp = nullptr;

		for (int i = 0; i < m_v_pEnemys.size(); i++)
		{
			if (m_v_pEnemys[i]->m_bDeadFlg)
			{
				continue;
			}

			if (IsInLockRange(m_v_pEnemys[i]->m_vCenterPos))
			{
				if (pCharaTmp)
				{
					float fPreviousDis = D3DXVec3Length(&(pCharaTmp->m_vCenterPos - m_vCenterPos));
					float fNowDis = D3DXVec3Length(&(m_v_pEnemys[i]->m_vCenterPos - m_vCenterPos));
				
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
		}
	}
}

bool clsCharactor::IsInLockRange(D3DXVECTOR3 vTargetPos)
{
	//開始位置.底面の方向.ターゲットの位置.距離.半径

	/*const float fhStantard = 50.0f;
	const float frStantard = 50.0f;

	float h = 1000.0f;//高さ.
	float fScale = h / fhStantard;
	float r = frStantard * fScale;*/

	D3DXVECTOR3 vC = m_vLockRangeDir * m_fLockRange;

	D3DXVECTOR3 H = vC - m_vLockRangePos;
	float fteimenDis = D3DXVec3Length(&H);
	D3DXVec3Normalize(&H, &H);

	D3DXVECTOR3 QP = vTargetPos - m_vLockRangePos;

	D3DXVECTOR3 A = { 0.0f, 0.0f, 0.0f };

	D3DXVec3Cross(&A, &H, &QP);

	float PA = D3DXVec3Length(&A);

	float L = D3DXVec3Dot(&H, &QP);
	float QA = abs(L);

	float Ar = QA / m_fLockRange * m_fLockCircleRadius;

	if ((m_fLockRange * PA) / (QA * m_fLockCircleRadius) > 1.0f)
	{
		return false;
	}

	if (0.0f > L || 
		m_fLockRange < L)
	{
		return false;
	}

	if (PA > Ar)
	{
		return false;
	}

	/*float QA = abs(L);

	float Ar = (QA) / m_fLockRange * m_fLockCircleRadius;

	if ((m_fLockRange * PA) / (QA * m_fLockCircleRadius) > 1.0f)
	{
		return false;
	}

	if (0.0f > L ||
		m_fLockRange < L)
	{
		return false;
	}

	if (PA > m_fLockCircleRadius)
	{
		return false;
	}*/

	return true;
}

void clsCharactor::Lock()
{
	if (m_pTargetChara)
	{
		for (int i = 0; i < m_v_pWeapons.size(); i++)
		{
			m_v_pWeapons[i]->Lock(m_pTargetChara);
		}
	}	
}

void clsCharactor::LockOut()
{
	//m_iTargetNo = -1;

	if (m_pTargetChara)
	{
		m_pTargetChara = nullptr;
	}

	for (int i = 0; i < m_v_pWeapons.size(); i++)
	{
		m_v_pWeapons[i]->LockOut();
	}
}

void clsCharactor::SetLockRangeDir()
{
	D3DXVECTOR3 vTmp = {0.0f,0.0f,0.0f};

	D3DXMATRIX mRot;

	//カメラ位置のための回転行列作成.
	D3DXMatrixRotationYawPitchRoll(
		&mRot,
		m_Trans.fYaw,
		-m_fLookUpDir,
		m_Trans.fRoll);

	D3DXVec3TransformCoord(&vTmp, &g_vDirForward, &mRot);

	m_vLockRangeDir = vTmp;
}

void clsCharactor::SetEnemys(std::vector<clsCharactor*> v_pEnemys)
{
	m_v_pEnemys = v_pEnemys;
}

clsCharactor::clsCharactor() :
	m_HP( 0 ),
	m_MaxHP( 0 ),
	m_bDeadFlg( false ),
	m_bMoving( false ),
	m_fMoveSpeedMax( 0.0f ),
	m_iTopMoveSpeedFrame( 0 ),
	m_fMoveAccele( 0.0f ),
	m_iMoveStopFrame( 0 ),
	m_fMoveDecele( 0.0f ),
	m_fRotSpeed( 0.0f ),
	m_fJumpPower( 0.0f ),
	m_fLockCircleRadius(0.0f)
{
//	ZeroMemory(this, sizeof(clsCharactor));
}

clsCharactor::~clsCharactor()
{
	if (m_pTargetChara)
	{
		m_pTargetChara = nullptr;
	}
}

void HitState::Clear()
{
	bHit = false;
	iDamage = 0;
	fInpuct = 0.0f;
	vInpuctDir = { 0.0f, 0.0f, 0.0f };
}