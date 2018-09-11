#include"Charactor.h"

void clsCharactor::CharactorUpdate()
{
	Move();
	Rotate();

	m_iDamage = 0;
}

void clsCharactor::SetMoveAcceleSpeed(float fMoveSpeedMax, int iTopSpeedFrame)//����.
{
	m_fMoveSpeedMax = fMoveSpeedMax;
	m_iTopMoveSpeedFrame = iTopSpeedFrame;

	m_fMoveAccele = m_fMoveSpeedMax / m_iTopMoveSpeedFrame;
}

void clsCharactor::SetMoveDeceleSpeed(const int iMoveStopFrame)//����.
{
	m_iMoveStopFrame = iMoveStopFrame;
	m_iMoveReverseDirInertia = m_iMoveStopFrame / 2;

	m_fMoveDecele = abs(m_fMoveSpeed) / m_iMoveStopFrame;
}

void clsCharactor::AddMoveAccele(const float fAngle, const float fPush)
{
	if (!IsMoveControl())
	{
		return;//�ō����𒴂��Ă���.
	}

	SetMoveDir(fAngle);//�ړ���������.
	MoveAccele(fPush);//����.

	m_bMoveAcceleOlder = true;//�������s����.
}

void clsCharactor::Move()
{
	if (!IsMoveControl() || !m_bMoveAcceleOlder)
	{
		//�ړ����x���ő�𒴂��Ă��邩�A����������Ă��Ȃ�.
		MoveDecele();//����.
	}

	MoveControl();//�ړ�.

	m_bMoveAcceleOlder = false;//���Z�b�g.
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

	//�������Ă������.
	vForward = GetVec3Dir(m_Trans.fYaw, g_vDirForward);

	//�s����������.
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

void clsCharactor::MoveAccele(const float fPower)//����񂩂�.
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

//��].

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
		return;//�ō����𒴂��Ă���.
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
		//�ō����𒴂��Ă��邩�A�������s���Ă��Ȃ�.m_bRotAcceleOlder
		RotDecele();//����.
	}

	Spin(m_Trans.fYaw, m_fRotDir, m_fRotSpeed);//��].

	m_bRotAcceleOlder = false;//���Z�b�g.
}

void clsCharactor::LookUp(const float fAngle, const float fPush)
{
	float fLookDir = m_fLookUpDir + fPush;

	Spin(m_fLookUpDir, fLookDir, m_fRotSpeedMax);

	const float fPitchMax = static_cast<float>D3DXToRadian(89);//90���ɂȂ�Ǝ��E�����������Ȃ邽�ߖh�~.

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

//���ޯĂƂ���ʒu�ɑ΂���ڲ���΂��āA�Օ������Ȃ������ׂ�.
bool clsCharactor::PointIntersect(
	const D3DXVECTOR3 StartPos,	//��̈ʒu.
	const D3DXVECTOR3 EndPos,		//�W�I�̈ʒu.
	const clsDX9Mesh* pTarget		//��Q���̕���.
	)
{
	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//ڲ�̊J�n�I���ʒu��Attacker�ƍ��킹��.
	vecStart = StartPos;

	//�W�I�̈ʒu���I�_��.
	vecEnd = EndPos;

	//�Ώۂ������Ă��镨�̂ł��A�Ώۂ�world�s��́A
	//�t�s���p����΁A������ڲ��������.
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(
		&matWorld,
		pTarget->m_Trans.vPos.x,
		pTarget->m_Trans.vPos.y,
		pTarget->m_Trans.vPos.z
		);

	//�t�s������߂�.
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(
		&vecStart, &vecStart, &matWorld);

	D3DXVec3TransformCoord(
		&vecEnd, &vecEnd, &matWorld);

	//���������߂�.
	vecDistance = vecEnd - vecStart;

	D3DXVec3Normalize(&vecDistance, &vecDistance);

	//���������߂�.

	BOOL bHit = false;	//�����׸�.

	float fDis = 0xffffffff;//�ő�l������.

	D3DXIntersect(
		pTarget->m_pMesh,	//�Ώ�ү��.
		&vecStart,			//�J�n�ʒu.
		&vecDistance,		//ڲ�̋���.
		&bHit,				//(out)���茋��.
		NULL,	//(out)bHit��True���Aڲ�̎n�_��.
		//�ł��߂��̖ʂ̲��ޯ���l�ւ��߲��.
		NULL, NULL,				//(out)�d�S˯č��W.
		&fDis,			//���ޯĂƂ̋���.
		NULL, NULL);

	float VecLenght = D3DXVec3Length(&(vecEnd - vecStart));


	if (fDis < VecLenght)//ڲ�͈͓̔��ɉ������邩?.
	{

		return true;//����������.
	}


	return false;//�����Ȃ�.
}

bool clsCharactor::IsCurcleLange(D3DXVECTOR3 CenterPos, D3DXVECTOR3 TargetPos, float Range)//�~�̔F���͈͔���.
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

	//�p�x���߂Â�.
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
		m_iDamage = HitS.iDamage;
		if (m_HP < HitS.iDamage)
		{
			m_HP = 0;
			m_bDeadFlg = true;//��.
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
		if (m_pTargetChara->m_bDeadFlg)//�^�[�Q�b�g�����S.
		{
			LockOut();
			return;
		}

		if (IsInLockRange(m_pTargetChara->m_vCenterPos))
		{
			Lock();

			D3DXMATRIX mW;
			D3DXMatrixIdentity(&mW);

			D3DXVECTOR3 vTmp = m_pTargetChara->GetPosition();

			D3DXVec3Project(&m_vTargetScrPos,
				&vTmp,
				m_pViewPort, &m_mProj,
				&m_mThisCharaView,
				&mW);
		}

		else
		{
			LockOut();
			return;
		}
	}

	else
	{
		//�V�K�Ƀ��b�N����^�[�Q�b�g�����肷��.
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
						//���b�N����^�[�Q�b�g�����ݒ�.
						pCharaTmp = m_v_pEnemys[i];
						//m_iTargetNo = i;
					}
				}

				else
				{
					//���b�N����^�[�Q�b�g�����ݒ�.
					pCharaTmp = m_v_pEnemys[i];
					//m_iTargetNo = i;
				}
			}
		}

		if (pCharaTmp)
		{
			//���b�N����^�[�Q�b�g���m��.
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

bool clsCharactor::IsInLockRange(D3DXVECTOR3 vTargetPos)
{
	//�J�n�ʒu.��ʂ̕���.�^�[�Q�b�g�̈ʒu.����.���a
	if (IsTargetDirBack())
	{
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

		if (IsCurcleLange(m_vLockCenterPos, vTarPosTmp, m_fLockCircleRadius))
		{
			return true;
		}
	}
	

	/*const float fhStantard = 50.0f;
	const float frStantard = 50.0f;

	float h = 1000.0f;//����.
	float fScale = h / fhStantard;
	float r = frStantard * fScale;*/
	


	/*D3DXVECTOR3 vC = m_vLockRangeDir * m_fLockRange;

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

	float Ar = (QA) / m����_fLockRange * m_fLockCircleRadius;

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

	return false;
}

bool clsCharactor::IsTargetDirBack()
{
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
	const float fCamMoveSpeed = 0.5f;
	const float fCamSpaceTmp = 4.0f;
	const float fCamPosX = 1.0f;

	D3DXVECTOR3 vTmp = {0.0f,0.0f,0.0f};

	D3DXMATRIX mRot;

	//�J�����ʒu�̂��߂̉�]�s��쐬.
	D3DXMatrixRotationYawPitchRoll(
		&mRot,
		m_Trans.fYaw,
		-m_fLookUpDir,
		m_Trans.fRoll);

	D3DXVec3TransformCoord(&vTmp, &g_vDirForward, &mRot);

	m_vLockRangeDir = vTmp;

	m_vLockPos = m_vLockRangePos + m_vLockRangeDir * m_fLockRange;

	//���޸�ق�p��.
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

	//�޸�ق��̂��̂���]��Ԃɂ��ϊ�����.

	D3DXVECTOR3 vCamPosTmp;

	//====================================================
	//��גǏ]���� ��������.
	//====================================================
	//�J�����������x��Ă��Ă���悤��.
	//�J���������ݖړI�Ƃ��Ă���ʒu���Z�o.
	const D3DXVECTOR3 vCamTargetPos = m_vCenterPos - vCamAxis;

	//���݈ʒu���擾���A���݈ʒu�ƖړI�̈ʒu�̍�����ړ��ʂ��v�Z����.
	vCamPosTmp = m_vLockRangePos;//���݈ʒu���擾
	vCamPosTmp -= (vCamPosTmp - vCamTargetPos) * fCamMoveSpeed;

	m_vLockRangePos = vCamPosTmp;

	D3DXVECTOR3 vUp = { 0.0f, 1.0f, 0.0f };

	D3DXMatrixLookAtLH(&m_mThisCharaView, &m_vLockRangePos, &m_vLockPos, &vUp);
}

void clsCharactor::SetEnemys(std::vector<clsCharactor*> v_pEnemys)
{
	m_v_pEnemys = v_pEnemys;
}

void clsCharactor::CharaInit(clsPOINTER_GROUP* pPointer)
{
	m_pViewPort = pPointer->GetViewPort10();
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