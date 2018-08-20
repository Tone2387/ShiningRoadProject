#include"Player.h"

clsPlayer::clsPlayer()
{
	ZeroMemory(this, sizeof(clsPlayer));
}

clsPlayer::~clsPlayer()
{

}

void clsPlayer::Init(clsPOINTER_GROUP* const pPtrGroup)
{
	RoboInit(pPtrGroup);

	m_pInput = new clsInputRobo(pPtrGroup->GetDxInput(), pPtrGroup->GetXInput());

	//m_pMesh->SetAnimSpeed(0.01);
}

void clsPlayer::Action(clsStage* const pWall)
{
	float fPush = 0.0f;
	float fAngle = 0.0f;
	clsRoboCommand* pRoboCom;

	float fPushMin = 0.5f;

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

	Move();
	Rotate();

	Updata();
	UpdateCamTargetPos();

	WallJudge(pWall);
}

void clsPlayer::InhUpdate()
{
	//m_pMesh->SetAnimSpeed(0.01);
}

D3DXVECTOR3 clsPlayer::GetCamTargetPos()
{
	return m_vCamTargetPos;
}


D3DXVECTOR3 clsPlayer::GetLookTargetPos()
{
	return m_vLookTargetPos;
}


void clsPlayer::UpdateCamTargetPos()
{
	const float fCamMoveSpeed = 0.5f;
	const float fLookPosSpace = 50.0f;
	const float fCamSpaceTmp = 2.0f;
	const float fCamPosX = 0.5f;

	D3DXMATRIX mRot;

	//カメラ位置のための回転行列作成.
	D3DXMatrixRotationYawPitchRoll(
		&mRot,
		m_Trans.fYaw,
		m_fLookUpDir,
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
		fCamAxisXTmp,
		0.0f,
		fCamSpaceTmp
	};

	D3DXVECTOR3 vLookAxis;

	//ﾍﾞｸﾄﾙそのものを回転状態により変換する.
	D3DXVec3TransformCoord(&vCamAxis, &vCamAxis, &mRot);
	D3DXVec3TransformCoord(&vLookAxis, &g_vDirForward, &mRot);

	D3DXVECTOR3 vCamPosTmp;

	//====================================================
	//ｶﾒﾗ追従処理 ここから.
	//====================================================
	//カメラが少し遅れてついてくるように.
	//カメラが現在目的としている位置を算出.
	const D3DXVECTOR3 vCamTargetPos = m_vCenterPos - vCamAxis * fCamSpaceTmp;

	//現在位置を取得し、現在位置と目的の位置の差から移動量を計算する.
	vCamPosTmp = m_vCamTargetPos;//現在位置を取得
	vCamPosTmp -= (vCamPosTmp - vCamTargetPos) * fCamMoveSpeed;

	m_vLookTargetPos = vCamPosTmp + vLookAxis * fLookPosSpace;

	m_vCamTargetPos = vCamPosTmp;
}