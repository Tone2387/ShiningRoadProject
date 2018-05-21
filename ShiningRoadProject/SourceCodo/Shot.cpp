#define _CRT_SECURE_NO_WARNINGS
#include"Shot.h"

clsShot::clsShot()
{
	ZeroMemory(this, sizeof(clsShot));
	m_pEffect = clsEffects::GetInstance();
}

clsShot::~clsShot()
{
}

HRESULT clsShot::Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11,SHOT_EFC ShotEffects,clsEffects::UseChar UseChar)
{
	ShotEfcTypes = ShotEffects;

	m_UseChar = UseChar;

	//HPUIInit(pDevice11, pContext11);
	SEInit(hWnd);
	//LockOnInit(pDevice11, pContext11);

	m_Trans.fScale = 0.1f;

	//当たり判定の大きさを決める.
	m_Sphere.fRadius = m_Trans.fScale;
	//確認用のｽﾌｨｱをﾚﾝﾀﾞﾘﾝｸﾞする.
	//当たり判定としては、ここ以降は不要.

#ifdef _DEBUG
	//m_pBodySphere = clsResource::GetInstance()->GetStaticModels(clsResource::enStaticModel_Shpere);
#endif


	//ﾎﾞｰﾝ名のｾｯﾄ.
	//体のﾎﾞｰﾝ.

	SEInit(hWnd);

	return S_OK;
}

void clsShot::SEInit(HWND hWnd)
{
	sound_data tmpSData[] =
	{
		{ "Data\\Sound\\SE\\Player\\Bash2.mp3", "Shoot" },
		{ "Data\\Sound\\SE\\SE01", "ShotHit" }
	};

	m_iSoundMaxNo = sizeof(tmpSData) / sizeof(tmpSData[0]);
	m_ppSE = new clsSound*[m_iSoundMaxNo];

	for (int i = 0; i < m_iSoundMaxNo; i++)
	{

		char No[STR_BUFF_MAX];
		sprintf(No, "%d", m_iThisShotIndex);

		strcat_s(tmpSData[i].sAlias, sizeof(tmpSData[i].sAlias), No);//ｴｲﾘｱｽを結合.

		m_ppSE[i] = new clsSound;
		//音声ﾌｧｲﾙを開く.
		m_ppSE[i]->Open(
			tmpSData[i].sPath,
			tmpSData[i].sAlias,
			hWnd);
	}
}

bool clsShot::Hit(SPHERE* ppTargetSphere,int iSphereMax)
{
	if (m_bShotExistFlg)
	{
		for (int i = 0; i < iSphereMax; i++)
		{
			if (Collision(m_Sphere, ppTargetSphere[i]))
			{
				m_pEffect->SetLocation(m_HitEfcH, m_Trans.vPos);
				m_HitEfcH = m_pEffect->Play(m_Trans.vPos, ShotEfcTypes.HitEfcType, m_UseChar);

				if (m_UseChar == clsEffects::enUse_Player)
				{
					m_pEffect->SetScale(m_HitEfcH, D3DXVECTOR3(0.5f, 0.5f, 0.5f));
				}

				else
				{
					m_pEffect->SetScale(m_HitEfcH, D3DXVECTOR3(0.2f, 0.2f, 0.2f));
				}

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

	m_Trans.vPos = m_vStartPos = m_Sphere.vCenter = vShotPos - m_vMoveAxis * SHOT_SPEED;

	m_bShotExistFlg = true;
	m_bExistFlg = true;

	return true;
}

void clsShot::Move()
{
	if (!m_bShotExistFlg)
	{
		if (!m_pEffect->PlayCheck(m_LineEfcH) && !m_pEffect->PlayCheck(m_HitEfcH))
		{
			m_bExistFlg = false;
		}

		m_pEffect->Stop(m_ShotEfcH);
		return;
	}

	if (D3DXVec3Length(&(m_Trans.vPos - m_vStartPos)) > 50.0f)
	{
		m_pEffect->Stop(m_ShotEfcH);
		m_bShotExistFlg = false;
	}

	if (!m_pEffect->PlayCheck(m_ShotEfcH))
	{
		m_ShotEfcH = m_pEffect->Play(m_Trans.vPos, ShotEfcTypes.ShotEfcType, m_UseChar);
		m_pEffect->SetScale(m_ShotEfcH, D3DXVECTOR3(0.5f, 0.5f, 0.5f));
		m_LineEfcH = m_pEffect->Play(m_Trans.vPos, ShotEfcTypes.LineEfcType, m_UseChar);
		m_pEffect->SetScale(m_LineEfcH, D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	}
	
	else
	{
		m_Trans.vPos += m_vMoveAxis  * SHOT_SPEED;
		m_Sphere.vCenter = m_Trans.vPos;
	}

	m_pEffect->SetLocation(m_ShotEfcH, m_Trans.vPos);
	m_pEffect->SetLocation(m_LineEfcH, m_Trans.vPos);

}

void clsShot::ReStart()
{
	m_bShotExistFlg = false;//弾の存在確認(falseなら弾が存在しない状態).
	m_bExistFlg = false;//弾,爆発,軌跡の存在確認(falseならそれら全てが存在しない状態)
}
