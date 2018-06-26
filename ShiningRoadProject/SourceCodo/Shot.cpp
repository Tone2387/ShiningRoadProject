#define _CRT_SECURE_NO_WARNINGS
#include"Shot.h"
#include"Resource.h"

clsShot::clsShot()
{
	ZeroMemory(this, sizeof(clsShot));
}

clsShot::~clsShot()
{
}

HRESULT clsShot::Init(const float fScale)
{
	//当たり判定の大きさを決める.
	m_Sphere.vCenter = &m_Trans.vPos;
	m_Sphere.fRadius = fScale;
	//確認用のｽﾌｨｱをﾚﾝﾀﾞﾘﾝｸﾞする.
	//当たり判定としては、ここ以降は不要.

#ifdef _DEBUG
	//m_pBodySphere = clsResource::GetInstance()->GetStaticModels(clsResource::enStaticModel_Shpere);
#endif

	return S_OK;
}

bool clsShot::Hit(SPHERE* ppTargetSphere,int iSphereMax)
{
	if (m_bShotExistFlg)
	{
		for (int i = 0; i < iSphereMax; i++)
		{
			if (Collision(m_Sphere, ppTargetSphere[i]))
			{
				//m_pEffect->SetLocation(m_HitEfcH, m_Trans.vPos);
				m_HitEfcH = //m_pEffect->Play(clsEffects::enEFFECTS_ARBIA_ATK,m_Trans.vPos);

				//m_pEffect->SetScale(m_HitEfcH, D3DXVECTOR3(1.0f,1.0f,1.0f));

				m_bShotExistFlg = false;
				return true;
			}
		}
	}

	return false;
}

bool clsShot::Form(D3DXVECTOR3 vShotPos, D3DXVECTOR3 vTarget)
{
	if (m_bExistFlg)
	{
		return false;
	}

	D3DXMATRIX mInv;

	D3DXMatrixTranslation(
		&mInv,
		vTarget.x,
		vTarget.y,
		vTarget.z
		);

	D3DXMatrixInverse(&mInv, NULL, &mInv);

	D3DXVECTOR3 vStart,vTar;

	D3DXVec3TransformCoord(&vStart, &vShotPos, &mInv);
	D3DXVec3TransformCoord(&vTar, &vTarget, &mInv);

	m_vMoveAxis = vTar - vStart;
	D3DXVec3Normalize(&m_vMoveAxis, &m_vMoveAxis);

	

	m_Trans.vPos = m_vStartPos = vShotPos - m_vMoveAxis * m_fMoveSpeed;

	m_bShotExistFlg = true;
	m_bExistFlg = true;

	return true;
}

void clsShot::Move()
{
	if (!m_bShotExistFlg)
	{
		/*if (!//m_pEffect->PlayCheck(m_LineEfcH) && !//m_pEffect->PlayCheck(m_HitEfcH))
		{
			m_bExistFlg = false;
		}

		//m_pEffect->Stop(m_ShotEfcH);*/
		return;
	}

	if (D3DXVec3Length(&(m_Trans.vPos - m_vStartPos)) > 50.0f)
	{
		//m_pEffect->Stop(m_ShotEfcH);
		m_bShotExistFlg = false;
	}

	/*if (!//m_pEffect->PlayCheck(m_ShotEfcH))
	{
		/*m_ShotEfcH = //m_pEffect->Play(m_Trans.vPos, ShotEfcTypes.ShotEfcType, m_UseChar);
		//m_pEffect->SetScale(m_ShotEfcH, D3DXVECTOR3(0.5f, 0.5f, 0.5f));
		m_LineEfcH = //m_pEffect->Play(m_Trans.vPos, ShotEfcTypes.LineEfcType, m_UseChar);
		//m_pEffect->SetScale(m_LineEfcH, D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	}

	else
	{
		m_Trans.vPos += m_vMoveAxis  * m_fMoveSpeed;
	}

	//m_pEffect->SetLocation(m_ShotEfcH, m_Trans.vPos);
	//m_pEffect->SetLocation(m_LineEfcH, m_Trans.vPos);*/

}

void clsShot::ReStart()
{
	m_bShotExistFlg = false;//弾の存在確認(falseなら弾が存在しない状態).
	m_bExistFlg = false;//弾,爆発,軌跡の存在確認(falseならそれら全てが存在しない状態)
}
