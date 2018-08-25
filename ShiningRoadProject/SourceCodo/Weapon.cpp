#include"Weapon.h"

void clsWeapon::Create(WeaponState State)
{
	m_State = State;

	m_ppBullet = new clsShot *[m_State.iBulletNumMax];

	for (int i = 0; i < m_State.iBulletNumMax; i++)
	{
		m_ppBullet[i] = new clsShot(m_pPtrGroup);
		m_ppBullet[i]->Init(m_State.BState);
	}

	Reload();
}


void clsWeapon::Update()
{
	for (int i = 0; i < m_State.iBulletNumMax; i++)
	{
		m_ppBullet[i]->Move();
	}
}

int clsWeapon::Hit(std::vector<clsObject::SPHERE> v_TargetSphere)
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

bool clsWeapon::Shot(clsObject* pTargetObj)
{
	if (m_iRemainingBullet > 0 || 
		m_iReloadCnt < 0)
	{
		D3DXVECTOR3 vPos = *m_State.SState.vShotStartPos;
		D3DXVECTOR3 vDir = *m_State.SState.vShotMoveDir;

		int iRandMax = m_State.iStablity;

		if (pTargetObj)//ターゲットいるなら偏差射撃.
		{
			int iTime;
			float fDis,fVerDevia;
			D3DXVECTOR3 vHorDevia, vPrediction;

			fDis = D3DXVec3Length(&(vPos - pTargetObj->GetPosition()));//ターゲットとの現在距離.
			iTime = (int)(fDis / m_State.BState.fSpeed);//到達までの時間(だと思いたい)

			fVerDevia = pTargetObj->m_fFollPower * iTime;//垂直方向の予測距離.

			vHorDevia = (pTargetObj->m_vMoveDir * pTargetObj->m_fMoveSpeed) * iTime;//水平方向移動ベクトル x 到達予想時間 = 水平方向の予想距離.
			vPrediction = pTargetObj->GetPosition();//予測位置にまずはターゲットの位置を入れる.

			vPrediction += vHorDevia;//水平のみ予測位置.
			vPrediction.y += fVerDevia;//予測位置.

			D3DXVec3Normalize(&vDir, &vPrediction);
		}

		float fDirError;//方向誤差.
		fDirError = (float)(rand() % (iRandMax * 2) - iRandMax) * g_fDistanceReference;
		vDir.x += fDirError;//x軸の誤差.

		fDirError = (float)(rand() % (iRandMax * 2) - iRandMax) * g_fDistanceReference;
		vDir.y += fDirError;//y軸の誤差.

		fDirError = (float)(rand() % (iRandMax * 2) - iRandMax) * g_fDistanceReference;
		vDir.z += fDirError;//z軸の誤差.

		for (int i = 0; i < m_State.iBulletNumMax; i++)
		{
			if (m_ppBullet[i]->Form(vPos, vDir))
			{
				m_iRemainingBullet--;
				m_iReloadCnt = m_State.iReloadTime;
				return true;
			}
		}
	}

	m_iReloadCnt--;

	return false;
}

bool clsWeapon::IsNeedReload()
{
	if (m_iRemainingBullet > 0)
	{
		//射撃可能.
		return false;
	}

	Reload();

	return true;
}

void clsWeapon::Reload()
{
	m_iRemainingBullet = m_State.iBulletNumMax;
}

int clsWeapon::GetNowBulletNum()
{
	return m_iRemainingBullet;
}

int clsWeapon::GetMaxBulletNum()
{
	return m_State.iBulletNumMax;
}

clsWeapon::clsWeapon(clsPOINTER_GROUP* pPtrGroup)
{
	m_pPtrGroup = pPtrGroup;
}

clsWeapon::~clsWeapon()
{
}