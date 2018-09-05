#include"Player.h"

clsPlayer::clsPlayer()
	: m_bCamPosXSwitch(false)//false:右.
	, m_pInput(nullptr)
{
	//ZeroMemory(this, sizeof(clsPlayer));
}

clsPlayer::~clsPlayer()
{

}

void clsPlayer::Init(clsPOINTER_GROUP* const pPtrGroup)
{
	RoboInit(pPtrGroup, pPtrGroup->GetRoboStatus());

	m_pInput = new clsInputRobo(pPtrGroup->GetDxInput(), pPtrGroup->GetXInput());

	clsObject::SPHERE Tmp;

	Tmp.vCenter = &m_vCenterPos;
	Tmp.fRadius = 0.1f;

	m_v_Spheres.push_back(Tmp);
	m_v_Spheres.shrink_to_fit();

	//m_pMesh->SetAnimSpeed(0.01);
}

void clsPlayer::ActionProduct()
{
	float fPush = 0.0f;
	float fAngle = 0.0f;
	clsRoboCommand* pRoboCom;

	LockChara();

	pRoboCom = m_pInput->MoveSwitch();

	if (pRoboCom)
	{
		pRoboCom->PushBotton(this);
	}

	pRoboCom = m_pInput->BoostRising();

	if (pRoboCom)
	{
		pRoboCom->PushBotton(this);
	}

	pRoboCom = m_pInput->LSInput(fPush, fAngle);

	if (pRoboCom)
	{
		pRoboCom->Trigger(this, fPush, fAngle);
	}

	pRoboCom = m_pInput->QuickBoost(fPush);//クイックブースト.
	if (pRoboCom)
	{
		pRoboCom->Trigger(this, fPush, fAngle);
		pRoboCom->PushBotton(this);

		pRoboCom = m_pInput->RSHorInput(fPush, fAngle);//旋回.

		if (pRoboCom)
		{
			pRoboCom->Trigger(this, abs(fPush), fAngle);
		}
	}

	else
	{
		pRoboCom = m_pInput->RSHorInput(fPush, fAngle);//旋回.

		if (pRoboCom)
		{
			pRoboCom->Trigger(this, abs(fPush), fAngle);
		}

		pRoboCom = m_pInput->QuickTurn(fPush);

		if (pRoboCom)
		{
			pRoboCom->Trigger(this, fPush, fAngle);
			pRoboCom->PushBotton(this);
		}

		else
		{
			float fPushFull = 1.0f;
			float fPushAngle = 0.0f;

			pRoboCom = m_pInput->QuickBoost(fPushFull);//クイックブースト.
			if (pRoboCom)
			{
				pRoboCom->Trigger(this, fPush, fPushAngle);
				pRoboCom->PushBotton(this);
			}
		}
	}

	pRoboCom = m_pInput->RSVerInput(fPush, fAngle);

	if (pRoboCom)
	{
		pRoboCom->Trigger(this, fPush, fAngle);
	}

	pRoboCom = m_pInput->LWeaponShot();

	if (pRoboCom)
	{
		pRoboCom->PushBotton(this);
	}

	pRoboCom = m_pInput->RWeaponShot();

	if (pRoboCom)
	{
		pRoboCom->PushBotton(this);
	}

	Updata();
	UpdateCamTargetPos();
}

D3DXVECTOR3 clsPlayer::GetCamTargetPos()
{
	return m_vCamTargetPos;
}


D3DXVECTOR3 clsPlayer::GetLookTargetPos()
{
	return m_vLookTargetPos;
}

D3DXVECTOR3 clsPlayer::GetLockCenterPos()
{
	return m_vLockCenterPos;
}


void clsPlayer::UpdateCamTargetPos()
{
	const float fCamMoveSpeed = 0.5f;
	const float fCamSpaceTmp = 4.0f;
	const float fCamPosX = 1.0f;

	D3DXMATRIX mRot;

	//カメラ位置のための回転行列作成.
	D3DXMatrixRotationYawPitchRoll(
		&mRot,
		m_Trans.fYaw,
		-m_fLookUpDir,
		m_Trans.fRoll);

	//軸ﾍﾞｸﾄﾙを用意.
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
		0.0f,
		m_vCenterPos.y - m_vLockRangePos.y,
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
	vCamPosTmp = m_vCamTargetPos;//現在位置を取得
	vCamPosTmp -= (vCamPosTmp - vCamTargetPos) * fCamMoveSpeed;

	m_vLookTargetPos = vCamPosTmp + m_vLockRangeDir * m_fLockRange;

	D3DXVec3Normalize(&vCamAxis, &vCamAxis);

	D3DXVECTOR3 vTmp = m_vCamTargetPos - m_vLockRangePos;

	D3DXVECTOR3 vForward;

	D3DXVec3TransformCoord(&vForward, &g_vDirForward, &mRot);

	D3DXVECTOR3 vPos =
	{
		-fCamAxisXTmp,
		m_vLockRangePos.y,
		m_vLockRangePos.z
	};

	//m_vLockCenterPos = m_vLockRangePos + m_vLockRangeDir * (m_fLockRange);
	//m_vLockCenterPos = m_vCamTargetPos + (vForward * m_fLockRange);

	m_vLockCenterPos = vPos + vForward * m_fLockRange;

	m_vCamTargetPos = vCamPosTmp;

	//CenterPosTest();
}

void clsPlayer::CenterPosTest()
{
	float fX = m_vLockRangePos.x - m_vCamTargetPos.x;
	float fZ = m_vLockRangePos.z - m_vCamTargetPos.z;

	float fYaw = atan2f(fX, fZ) / 2;

	D3DXMATRIX mCamRot;

	//カメラ位置のための回転行列作成.
	D3DXMatrixRotationYawPitchRoll(
		&mCamRot,
		fYaw,
		-m_fLookUpDir,
		m_Trans.fRoll);

	D3DXVECTOR3 vForward;

	D3DXVec3TransformCoord(&vForward, &g_vDirForward, &mCamRot);
	m_vLockCenterPos = m_vCamTargetPos + (vForward * m_fLockRange);
}

D3DXVECTOR3 clsPlayer::GetLockRangeTmp()
{
	SetLockRangeDir();
	D3DXVECTOR3 vX = g_vDirRight *  m_fLockCircleRadius;
	D3DXVECTOR3 vY = g_vDirDown *  m_fLockCircleRadius;
	D3DXVECTOR3 vZ = m_vLockRangeDir * m_fLockRange;

	D3DXVECTOR3 vTmp = vX + vY + vZ;
	return vTmp;
}

float clsPlayer::GetLockCircleScale()
{
	float fDis = 1 / m_fLockRange;
	float fTmp;

	float fScale = 0.0f;
	float fDiameter = 1.0f;

	float siya = 1.0f + static_cast<float>(D3DX_PI / 4.0);

	fScale = (2 * m_fLockRange) * tanf(siya / 2);
	fScale = fDiameter / fScale;

	fTmp = m_fLockCircleRadius * fScale;

	return fTmp;
}

bool clsPlayer::GetTargetPos(D3DXVECTOR3& vTmpPos)
{
	if (m_pTargetChara)
	{
		vTmpPos = m_pTargetChara->m_vCenterPos;
		return true;
	}

	return false;
}