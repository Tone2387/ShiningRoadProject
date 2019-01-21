#include"Weapon.h"

const int g_iMOAReference = 1000;//ATK�̍�������u�������߂邽�߂̊�l.

void clsWeapon::Create(const WeaponState& State)
{
	m_State = State;

	m_ppBullet = new clsShot *[m_State.iBulletNumMax];

	for (int i = 0; i < m_State.iBulletNumMax; i++)
	{
		m_ppBullet[i] = new clsShot(m_pPtrGroup);
		m_ppBullet[i]->Init(m_State.BState);
	}

	m_bNeedReload = false;
	m_iRemainingBullet = m_State.iBulletNumMax;

	m_iReloadCnt = m_State.iReloadTime;

	LockOut();
}


void clsWeapon::Update(clsStage* const pStage)
{
	for (int i = 0; i < m_State.iBulletNumMax; i++)
	{
		m_ppBullet[i]->Update(pStage);
	}

	Reload();
	if (m_iRemainingBullet <= 0)
	{
		if (!m_bNeedReload)
		{
			m_iMagazineReloadCnt = m_State.iMagazineReloadTime;
			m_bNeedReload = true;
		}
	}

	m_iReloadCnt--;
}

int clsWeapon::Hit(std::vector<clsObject::SPHERE>& v_TargetSphere)
{
	int iResult = 0;

	for (int i = 0; i < m_State.iBulletNumMax; i++)
	{
		if (m_ppBullet[i]->Hit(v_TargetSphere))
		{
			iResult += m_State.iAtk;
		}
	}
	
	return iResult;
}

bool clsWeapon::Shot()
{
	//�c�e���m�F.
	if (m_iRemainingBullet > 0)
	{
		//�����[�h���Ԋm�F.
		if (m_iReloadCnt < 0)
		{
			D3DXVECTOR3 vPos = *m_State.SState.vShotStartPos;
			D3DXVECTOR3 vDir = *m_State.SState.vShotMoveDir;

			if (m_pTargetObj)//�^�[�Q�b�g����Ȃ�΍��ˌ�.
			{
				int iTime;
				float fDis, fVerDevia;
				D3DXVECTOR3 vHorDevia, vPrediction;

				fDis = D3DXVec3Length(&(vPos - m_pTargetObj->GetCenterPos()));//�^�[�Q�b�g�Ƃ̌��݋���.
				iTime = (int)(fDis / m_State.BState.fSpeed);//���B�܂ł̎���(���Ǝv������)

				fVerDevia = m_pTargetObj->GetFollPower() * iTime;//���������̗\������.

				vHorDevia = (m_pTargetObj->GetMoveDir()* m_pTargetObj->GetMoveSpeed()) * static_cast<float>(iTime);//���������ړ��x�N�g�� * ���B�\�z���� = ���������̗\�z����.
				vPrediction = m_pTargetObj->GetCenterPos();//�\���ʒu�ɂ܂��̓^�[�Q�b�g�̈ʒu������.

				vPrediction += vHorDevia;//�����̂ݗ\���ʒu.
				vPrediction.y += fVerDevia;//�\���ʒu.

				D3DXVECTOR3 vDirTmp = vPrediction - vPos;

				D3DXVec3Normalize(&vDirTmp, &vDirTmp);

				float fDirTmp = D3DXVec3Dot(&vDir, &vDirTmp);

				fDirTmp = acos(fDirTmp);

				if (fDirTmp > D3DX_PI / 2)
				{
					vDirTmp = -vDirTmp;
				}

				vDir = vDirTmp;
			}

			//�U���͂ɂ��u���Ƃ����}���鐔�l.
		/*	const float fMOA = float(m_State.iAtk) / float(g_iMOAReference);
			const float fDirErrorPar = m_State.iStablity * g_fPercentage - 1;

			float fRandMax = abs(fMOA * fDirErrorPar);

			if (fRandMax != 0.0f)//0���Ǝ~�܂�.
			{
				float fDirError;//�����덷.

				//fDirError = (float)(rand() % (iRandMax * 2) - iRandMax) * g_fDistanceReference;
				fDirError = fmodf(static_cast<float>(rand()), (fRandMax * 2)) - fRandMax;//�ϓ��l������ꍇ�A��������.
				vDir.x += fDirError;//x���̌덷.

				fDirError = fmodf(static_cast<float>(rand()), (fRandMax * 2)) - fRandMax;//�ϓ��l������ꍇ�A��������.
				vDir.y += fDirError;//y���̌덷.

				fDirError = fmodf(static_cast<float>(rand()), (fRandMax * 2)) - fRandMax;//�ϓ��l������ꍇ�A��������.
				vDir.z += fDirError;//z���̌덷.
			}*/

			D3DXVec3Normalize(&vDir, &vDir);

			for (int i = 0; i < m_State.iBulletNumMax; i++)
			{
				if (m_ppBullet[i]->Form(vPos, vDir))
				{
					m_iRemainingBullet--;
					m_iReloadCnt = m_State.iReloadTime;
					//�ˌ���.
					m_pPtrGroup->GetSound()->PlaySE(0);
					return true;
				}
			}
		}
	}
	
	return false;
}

bool clsWeapon::IsNeedReload()
{
	return m_bNeedReload;
}

void clsWeapon::Reload()
{
	if (!m_bNeedReload)
	{
		return;
	}

	if (m_iMagazineReloadCnt > 0)
	{
		--m_iMagazineReloadCnt;
		return;
	}

	m_bNeedReload = false;
	m_iRemainingBullet = m_State.iBulletNumMax;
	//�����[�h��.
	m_pPtrGroup->GetSound()->PlaySE(0);
}

int clsWeapon::GetNowBulletNum()
{
	return m_iRemainingBullet;
}

int clsWeapon::GetMaxBulletNum()
{
	return m_State.iBulletNumMax;
}

int clsWeapon::GetShotEN() const
{
	return m_State.iShotEN;
}

bool clsWeapon::IsLock()
{
	if (m_pTargetObj)
	{
		return true;
	}

	return false;
}

void clsWeapon::Lock(clsObject* pTargetObj)//���b�N�͈͂ɓ����Ă���.
{
	if (!pTargetObj)
	{
		return;
	}

	if (m_pTargetObj)
	{
		return;
	}

	m_iLockTime--;

	if (m_iLockTime < 0)
	{
		if (pTargetObj)
		{
			m_pTargetObj = pTargetObj;
		}
	}
}

void clsWeapon::LockOut()//���b�N�������.
{
	m_iLockTime = m_State.iLockSpeed * static_cast<int>(g_fFPS);
	m_pTargetObj = nullptr;
}

clsWeapon::clsWeapon(clsPOINTER_GROUP* pPtrGroup) 
	: m_pTargetObj(nullptr)
{
	m_pPtrGroup = pPtrGroup;
}

clsWeapon::~clsWeapon()
{
}