#include "Enemy.h"

clsEnemy::clsEnemy()
{
	ZeroMemory(this, sizeof(clsEnemy));
	m_pEffect = clsEffects::GetInstance();
}

clsEnemy::~clsEnemy()
{

}

HRESULT clsEnemy::Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	m_pDX9Mesh = clsResource::GetInstance()->GetStaticModels(clsResource::enStaticModel_Enemy);

	//HPUIInit(pDevice11, pContext11);
	SEInit(hWnd);
	//LockOnInit(pDevice11, pContext11);

	m_Trans.vScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	m_pDX9Mesh->m_Trans.fScale = 0.5f;

	m_iDownEfcCnt = 0;

	m_iShotMax = 32;

	m_ppShot = new clsShot *[m_iShotMax];
	
	SHOT_EFC ShotEfcTypes;
	ShotEfcTypes.ShotEfcType = clsEffects::enEfcType_Shot;
	ShotEfcTypes.LineEfcType = clsEffects::enEfcType_Line;
	ShotEfcTypes.HitEfcType = clsEffects::enEfcType_Hit;

	for (int i = 0; i < m_iShotMax; i++)
	{
		m_ppShot[i] = new clsShot;
		m_ppShot[i]->Init(hWnd, pDevice11, pContext11, ShotEfcTypes,clsEffects::UseChar::enUse_Enemy);
	}

	ReStart();

	//当たり判定の大きさを決める.
	m_BodySphere.fRadius = m_pDX9Mesh->m_Trans.fScale;
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

void clsEnemy::SEInit(HWND hWnd)
{
	sound_data tmpSData[] =
	{
		{ "Data\\Sound\\SE\\Enemy\\EShot.mp3", "EShot" },
		{ "Data\\Sound\\SE\\Enemy\\EHit.mp3", "EHit" },
		{ "Data\\Sound\\SE\\Down.mp3", "EDown" },
		{ "Data\\Sound\\SE\\Enemy\\EDead.mp3", "EDead" },
	};

	m_iSoundMaxNo = sizeof(tmpSData) / sizeof(tmpSData[0]);
	m_pSE = new clsSound*[m_iSoundMaxNo];

	for (int i = 0; i < m_iSoundMaxNo; i++)
	{

		char No[STR_BUFF_MAX];
		sprintf(No, "%d", m_iThisEnemyIndex);

		strcat_s(tmpSData[i].sAlias, sizeof(tmpSData[i].sAlias), No);//ｴｲﾘｱｽを結合.

		m_pSE[i] = new clsSound;
		//音声ﾌｧｲﾙを開く.
		m_pSE[i]->Open(
			tmpSData[i].sPath,
			tmpSData[i].sAlias,
			hWnd);
	}
}

void clsEnemy::GameMove(D3DXVECTOR3 PlayerPos, clsDX9Mesh* Ground)
{
	for (int i = 0; i < m_iShotMax; i++)
	{
		m_ppShot[i]->Move();
	}

	if (m_bDeadFlg || m_bStopFlg)
	{
		return;
	}

	if (m_bDownFlg)
	{
		if (m_iDownEfcCnt >= E_DOWNBOM)
		{
			if (!m_pEffect->PlayCheck(m_DownEfc) && !m_pEffect->PlayCheck(m_DeadEfc))
			{
				m_pEffect->SetLocation(m_DeadEfc, m_Trans.vPos);
				m_DeadEfc = m_pEffect->Play(m_Trans.vPos, clsEffects::enEfcType_Dead, clsEffects::enUse_Enemy);
				m_pEffect->SetScale(m_DeadEfc, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				
				SoundDistanceVolume(m_pSE[3], m_vCamPos, m_Trans.vPos, SE_VOLUME, SE_VOLUME_DISTANCEDECAY);
				m_pSE[3]->SeekToStart();
				m_pSE[3]->Play(true);

				m_bDeadFlg = true;

				return;
			}
		}

		if (!m_pEffect->PlayCheck(m_DownEfc))
		{
			m_pEffect->SetLocation(m_DownEfc, m_Trans.vPos);
			m_DownEfc = m_pEffect->Play(m_Trans.vPos, clsEffects::enEfcType_Down, clsEffects::enUse_Enemy);
			m_pEffect->SetScale(m_DownEfc, D3DXVECTOR3(0.5f, 0.5f, 0.5f));

			SoundDistanceVolume(m_pSE[2], m_vCamPos, m_Trans.vPos, SE_VOLUME, SE_VOLUME_DISTANCEDECAY);
			m_pSE[2]->SeekToStart();
			m_pSE[2]->Play(true);

			m_iDownEfcCnt++;
		}

		return;
	}

	D3DXVECTOR3 vPlayer = PlayerPos;
	vPlayer.y += 0.5f;

	m_vOldPos = m_Trans.vPos;

	TrackingMove(vPlayer, Ground);

	if (m_iShotCnt < 0)
	{
		for (int i = 0; i < m_iShotMax; i++)
		{
			if (m_ppShot[i]->Form(m_Trans.vPos, vPlayer))
			{
				m_iShotCnt = E_SHOTCNT + (int)(rand() % 30);

				SoundDistanceVolume(m_pSE[0], m_vCamPos, m_Trans.vPos, SE_VOLUME - 200, SE_VOLUME_DISTANCEDECAY);
				m_pSE[0]->SeekToStart();
				m_pSE[0]->Play(true);

				break;
			}
		}
	}

	m_iShotCnt--;
}



void clsEnemy::Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye, LPD3DXANIMATIONCONTROLLER pAC)
{
	
	if (m_pDX9Mesh == NULL || m_bDeadFlg)return;

	Update();

	m_pDX9Mesh->Render(mView, mProj, vLight, vEye);
}

void clsEnemy::Update()
{
	if (m_pDX9Mesh == NULL)
	{
		return;
	}
	
	//m_Trans.vPos.y = 2.0f;

	m_pDX9Mesh->m_Trans.vPos = m_Trans.vPos;
	m_pDX9Mesh->m_Trans.fYaw = m_Trans.fYaw;
	m_pDX9Mesh->m_Trans.fPitch = m_Trans.fPitch;
	m_pDX9Mesh->m_Trans.fRoll = m_Trans.fRoll;
	m_pDX9Mesh->m_Trans.fScale = m_Trans.vScale.x;

	UpdateCollisionPos();//ﾎﾞｰﾝ位置移動.
}


void clsEnemy::Hit(int Damage)
{
	HP -= Damage;

	if (HP <= 0)
	{
		m_bDownFlg = true;
	}
}

void clsEnemy::VectorInput(FILE *fp, int No)
{
	char s[100];//ｶﾝﾏ区切りに対応しつつ、最初の一行に入れたのを吐き出すため.
	int nlin;//現在見ているのは何番目か？

	FILELOAD Tmp;

	rewind(fp);
	nlin = 0;

	while (fgets(s, 80, fp) != NULL)
	{
		if (++nlin <= No)continue;
		fscanf(fp, "%[^,],%f,%f,%f,%d", &s, &Tmp.vOrizinPos.x, &Tmp.vOrizinPos.y, &Tmp.vOrizinPos.z, &Tmp.MovePattern);
		break;
	}

	m_Trans.vPos = m_vOldPos = m_vOriginPos = Tmp.vOrizinPos;
	m_iMovePattern = Tmp.MovePattern;
}

void clsEnemy::ReStart()
{
	m_Trans.vPos = m_vOldPos = m_vOriginPos;//初期地点移動.
	UpdateCollisionPos();//ﾎﾞｰﾝ位置移動.

	RaySpece = 1.0f + E_MOVE_SPEED * 2;

	m_MaxHP = E_HP;//最大HP設定.
	HP = m_MaxHP;//HPを最大HPに.

	m_iDownEfcCnt = 0;

	m_bDeadFlg = false;//死亡ﾌﾗｸﾞ.
	m_bDownFlg = false;

	Update();

	m_iShotCnt = E_SHOTCNT;
	m_bStopFlg = false;

	for (int i = 0; i < m_iShotMax; i++)
	{
		m_ppShot[i]->ReStart();
	}
}

void clsEnemy::UpdateCollisionPos()
{
	//体の当たり判定.
	//ﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱの中心をﾎﾞｰﾝの位置に移動.
	m_BodySphere.vCenter = m_Trans.vPos;
}

void clsEnemy::TrackingMove(D3DXVECTOR3 PlayerPos, clsDX9Mesh* Ground)
{
	if (m_iMovePattern != enMPattaern_Normal)
	{
		return;
	}

	RaySpece = 1.0f + E_MOVE_SPEED * 2;

	D3DXMATRIX mYaw;
	D3DXMatrixRotationY(&mYaw, m_Trans.fYaw);//Y軸回転.

	//Z軸ﾍﾞｸﾄﾙを用意.
	vecAxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vecAxisX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	//Z軸ﾍﾞｸﾄﾙそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ,	//(out)
		&vecAxisZ,
		&mYaw);	//Y軸回転行列.

	//Z軸ﾍﾞｸﾄﾙそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisX,	//(out)
		&vecAxisX,
		&mYaw);	//Y軸回転行列.

	D3DXVECTOR3 RayStartPos = m_Trans.vPos;
	RayStartPos.y += 0.5f;

	float Dis = D3DXVec3Length(&(m_Trans.vPos - PlayerPos));

	if (Dis > 10.0f)
	{
		m_Trans.vPos -= vecAxisZ * E_MOVE_SPEED;
	}

	else
	{
		m_Trans.vPos += vecAxisX * E_MOVE_SPEED / 2;
	}

	D3DXVECTOR3 RayEndPos = PlayerPos;
	RayEndPos.y = 0.5f;

	Spin(m_Trans.fYaw, atan2f((m_Trans.vPos.x - PlayerPos.x), (m_Trans.vPos.z - PlayerPos.z)), 0.1f, 0.1f);

	D3DXVECTOR3 Vec3 = m_Trans.vPos;

	WallJudge(Ground);

	Vec3 = m_Trans.vPos;
}

bool clsEnemy::ShotCollision(SPHERE* pTargrt, int iSphereMax)
{
	for (int i = 0; i < m_iShotMax; i++)
	{
		if (m_ppShot[i]->Hit(pTargrt,iSphereMax))
		{
			SoundDistanceVolume(m_pSE[1], m_vCamPos, pTargrt->vCenter, SE_VOLUME, SE_VOLUME_DISTANCEDECAY);
			m_pSE[1]->SeekToStart();
			m_pSE[1]->Play(true);

			return true;
		}
	}
	
	return false;
}

bool clsEnemy::BodyCollision(SPHERE* pTargrt, int iSphereMax)
{
	for (int i = 0; i < iSphereMax; i++)
	{
		if (Collision(m_BodySphere, pTargrt[i]))
		{
			return true;
		}
	}

	return false;
}