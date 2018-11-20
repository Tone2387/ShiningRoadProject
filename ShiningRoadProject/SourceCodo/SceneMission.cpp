#include "SceneMission.h"

namespace
{
	const char strMissonFolderPath[] = "Data\\FileData\\Hiyoshi\\Misson";
};

//================================//
//========== ミッション中クラス ==========//
//================================//
clsSCENE_MISSION::clsSCENE_MISSION(clsPOINTER_GROUP* const ptrGroup) : clsSCENE_BASE(ptrGroup),
m_pPlayer(nullptr)
, m_bEnemyStop(false)
, m_bCamTarChange(false)
{
}

clsSCENE_MISSION::~clsSCENE_MISSION()
{
	//SAFE_DELETE( m_pTestChara );
	SAFE_DELETE(m_pPlayer);
}

//生成時に一度だけ通る処理.
void clsSCENE_MISSION::CreateProduct()
{
	CreateFriends();
	CreateEnemys();

	SetEnemys();

	CreateUI();

	m_pCamTar = m_pPlayer;

	//テストモデルの初期化.

	m_pStage = new clsStage( m_wpPtrGroup );

	m_fCamMoveSpeed = 0.01f;
}

void clsSCENE_MISSION::CreateUI()
{
	assert(!m_pLimitTime);
	char pText[STR_BUFF_MAX];
	D3DXVECTOR2 vPos = { 0.0f, 0.0f };

	//活動限界時間.

	m_pLimitTime = new clsUiText;
	m_pLimitTime->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 3.0f);
	m_pLimitTime->SetPos(vPos);

	sprintf_s(pText,"");

	m_pLimitTime->SetText(pText);

	//残弾数.
	assert(!m_pLBulletNum);
	vPos = { 250.0f, 0.0f };

	m_pLBulletNum = new clsUiText;
	m_pLBulletNum->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 3.0f);
	m_pLBulletNum->SetPos(vPos);

	sprintf_s(pText, "");

	m_pLBulletNum->SetText(pText);

	assert(!m_pRBulletNum);
	vPos = { WND_W / 2 + 250.0f, 0.0f };

	m_pRBulletNum = new clsUiText;
	m_pRBulletNum->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 3.0f);
	m_pRBulletNum->SetPos(vPos);

	sprintf_s(pText, "");

	m_pRBulletNum->SetText(pText);

	//HP.

	assert(!m_pHP);
	vPos = { WND_W / 2, 0.0f };

	m_pHP = new clsUiText;
	m_pHP->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 3.0f);
	m_pHP->SetPos(vPos);

	sprintf_s(pText, "");

	m_pHP->SetText(pText);

	SPRITE_STATE ss;

	assert(!m_pRaderWindowFront);
	ss.Disp = { 960.0f / 8, 640.0f / 8 };
	ss.Anim = { 1.0f, 1.0f };

	m_pRaderWindowFront = new clsSPRITE2D_CENTER;

	m_pRaderWindowFront->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\RadarWindowFront.png", ss);
	m_pRaderWindowFront->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	m_pRaderWindowFront->SetAlpha(1.0f);

	assert(!m_pRaderWindowBack);
	ss.Disp = { 960.0f / 8, 640.0f / 8 };
	ss.Anim = { 1.0f, 1.0f };

	m_fRaderSizeW = ss.Disp.w;
	m_fRaderSizeH = ss.Disp.h;

	m_pRaderWindowBack = new clsSPRITE2D_CENTER;

	m_pRaderWindowBack->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\RadarWindowBack.png", ss);
	m_pRaderWindowBack->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	m_pRaderWindowBack->SetAlpha(0.4f);

	ss.Disp = { 44.0f / 8, 44.0f / 8 };

	m_fRaderMarkSizeW = ss.Disp.w;
	m_fRaderMarkSizeH = ss.Disp.h;

	m_fRaderDis = 50.0f;

	m_v_pRaderEnemyMark.resize(m_v_pEnemys.size());

	for (unsigned int i = 0; i < m_v_pRaderEnemyMark.size(); i++)
	{
		m_v_pRaderEnemyMark[i] = new clsSPRITE2D_CENTER;

		m_v_pRaderEnemyMark[i]->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\RadarEnemyMark.png", ss);
		m_v_pRaderEnemyMark[i]->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });
	}

	assert(!m_pCursor);
	ss.Disp = { 1.0f, 1.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pCursor = new clsSPRITE2D_CENTER;

	m_pCursor->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\Lockon.png", ss);
	m_pCursor->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	m_pCursor->SetAlpha(0.4f);

	assert(!m_pCursorFrame);
	ss.Disp = { 512.0f, 512.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pCursorFrame = new clsSPRITE2D_CENTER;

	m_pCursorFrame->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\Lockon.png", ss);
	m_pCursorFrame->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	//EN.

	assert(!m_pEnelgy);

	ss.Disp = { 512.0f / 2, 64.0f / 4 };
	ss.Anim = { 2.0f, 1.0f };

	m_pEnelgy = new clsSPRITE2D_CENTER;

	m_pEnelgy->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\Gauge.png", ss);
	m_pEnelgy->SetPos({ WND_W / 2, m_pCursorFrame->GetPos().y - ((256.0f / 2) + (64.0f)), 0.0f });

	assert(!m_pEnelgyFrame);

	ss.Disp = { 512.0f / 2 + 5, 64.0f / 4 + 3 };
	ss.Anim = { 1.0f, 1.0f };

	m_pEnelgyFrame = new clsSPRITE2D_CENTER;

	m_pEnelgyFrame->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\GaugeWaku.png", ss);
	m_pEnelgyFrame->SetPos(m_pEnelgy->GetPos());//{ WND_W / 2, WND_H / 5, 0.0f }

	assert(!m_pLWeaponLockMark);
	ss.Disp = { 128.0f, 64.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pLWeaponLockMark = new clsSPRITE2D_CENTER;

	m_pLWeaponLockMark->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\LockMark.png", ss);
	m_pLWeaponLockMark->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	m_pLWeaponLockMark->SetAlpha(0.4f);

	assert(!m_pRWeaponLockMark);
	ss.Disp = { 128.0f, 64.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pRWeaponLockMark = new clsSPRITE2D_CENTER;

	m_pRWeaponLockMark->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\LockMark.png", ss);
	m_pRWeaponLockMark->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	m_pRWeaponLockMark->SetAlpha(0.4f);

	assert(!m_pHitMark);
	ss.Disp = { 128.0f, 64.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_fHitMarkRaderSizeW = ss.Disp.w;
	m_fHitMarkRaderSizeH = ss.Disp.h;

	m_pHitMark = new clsSPRITE2D_CENTER;

	m_pHitMark->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\HitMark.png", ss);
	m_pHitMark->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	m_pHitMark->SetAlpha(0.4f);

	assert(!m_pLockWindow);

	ss.Disp = { 100.0f, 100.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pLockWindow = new clsSPRITE2D_CENTER;

	m_pLockWindow->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\LockWindow.png", ss);
	m_pLockWindow->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });
}

//毎フレーム通る処理.
void clsSCENE_MISSION::UpdateProduct( enSCENE &enNextScene )
{
	//nullならassert.
	assert(m_pPlayer);
	//m_pPlayer->Action(m_pStage);

	if (GetAsyncKeyState('C') & 0x1)
	{
		if (!m_bCamTarChange)
		{
			m_bCamTarChange = true;
			m_pCamTar = m_v_pEnemys[0];
		}

		else
		{
			m_bCamTarChange = false;
			m_pCamTar = m_pPlayer;
		}
	}
	
	if (GetAsyncKeyState('S') & 0x1)
	{
		m_pTestObj->SwitchMove();
	}

	for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
	{
		m_v_pFriends[i]->Update(m_pStage);
	}
	
	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		m_v_pEnemys[i]->Update(m_pStage);
	}
	
	UpdateCamTargetPos(m_pCamTar);

	D3DXVECTOR3 vCamPosTmp;
	D3DXVECTOR3 vLookPosTmp;

	vCamPosTmp = m_vCamTargetPos;
	vLookPosTmp = m_vLookTargetPos;

	m_wpCamera->SetPos(vCamPosTmp, false);
	m_wpCamera->SetLookPos(vLookPosTmp);

	//エンディングに行く場合は以下のようにする.
	if (AllEnemyDead())
	{
		enNextScene = enSCENE::ENDING;
	}

	if (m_pPlayer->m_bDeadFlg || m_pPlayer->m_bTimeUp)
	{
		enNextScene = enSCENE::GAMEOVER;
	}
}

//描画.
void clsSCENE_MISSION::RenderProduct( const D3DXVECTOR3 &vCamPos )
{

	//m_pTestChara->Render(m_mView, m_mProj, m_vLight, vCamPos);

	for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
	{
		m_v_pFriends[i]->Render(m_mView, m_mProj, m_vLight, vCamPos);
	}

	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		m_v_pEnemys[i]->Render(m_mView, m_mProj, m_vLight, vCamPos);
	}

	Collison();

	m_pStage->Render(m_mView, m_mProj, m_vLight, vCamPos);
}

void clsSCENE_MISSION::RenderUi()
{
	SetDepth(false);	//Zテスト:OFF.
	//活動限界時間.
	int iTmp = m_pPlayer->m_iActivityLimitTime;
	int iMin = iTmp / 3600;
	int iSecond = (iTmp - (iMin * 3600)) / 60;
	int iN = iTmp % 60;
	D3DXVECTOR3 vPosTmp;

	char pText[STR_BUFF_MAX];

	sprintf_s(pText, "%2d:%2d:%2d", iMin, iSecond, iN);
	m_pLimitTime->SetText(pText);
	m_pLimitTime->Render();

	//武器残弾数.

	int iNowNum = m_pPlayer->m_v_pWeapons[clsRobo::enWeaponLHand]->GetNowBulletNum();
	int iMaxNum = m_pPlayer->m_v_pWeapons[clsRobo::enWeaponLHand]->GetMaxBulletNum();

	sprintf_s(pText, "%2d/%2d", iNowNum, iMaxNum);
	m_pLBulletNum->SetText(pText);
	m_pLBulletNum->Render();

	iNowNum = m_pPlayer->m_v_pWeapons[clsRobo::enWeaponRHand]->GetNowBulletNum();
	iMaxNum = m_pPlayer->m_v_pWeapons[clsRobo::enWeaponRHand]->GetMaxBulletNum();

	sprintf_s(pText, "%2d/%2d", iNowNum, iMaxNum);
	m_pRBulletNum->SetText(pText);
	m_pRBulletNum->Render();

	//HP.

	int iHP = m_pPlayer->m_iHP;

	sprintf_s(pText, "%d", iHP);
	m_pHP->SetText(pText);
	m_pHP->Render(clsUiText::enPOS::MIDDLE);

	//EN.

	float fNowEN = static_cast<float>(m_pPlayer->m_iEnelgy);
	float fENMax = static_cast<float>(m_pPlayer->m_iEnelgyMax);

	POINTFLOAT fEnelgyPar = { 1.0f - (fNowEN / fENMax), 1.0f };

	m_pEnelgyFrame->Render();

	m_pEnelgy->SetAnim(fEnelgyPar);
	m_pEnelgy->Render();

	//レーダー.
	
	D3DXVECTOR3 vPlayerPos = m_pPlayer->GetPosition();

	const float fRaderDis = m_pPlayer->GetRaderRange();//大きくするとレーダーの索敵範囲が広がる.

	m_pRaderWindowBack->Render();

	for (unsigned int i = 0; i < m_v_pRaderEnemyMark.size(); i++)
	{
		if (!m_v_pEnemys[i])continue;

		D3DXVECTOR3 vThisEnemyPos = m_v_pEnemys[i]->GetPosition();

		D3DXVECTOR3 vPos = (vThisEnemyPos - vPlayerPos) / fRaderDis;
		float fW = (vThisEnemyPos.x - vPlayerPos.x) / fRaderDis;
		float fH = (vPlayerPos.z - vThisEnemyPos.z) / fRaderDis;

		D3DXMATRIX mRot;
		float fYaw = -m_pPlayer->GetRotation().y;//-だと回転値に合った動きする理由という英知を授けてください.

		D3DXMatrixRotationY(&mRot, fYaw);

		D3DXVec3TransformCoord(&vPos, &vPos, &mRot);

		float fPosX = vPos.x;//fW; //+(fW * -abs(fXYaw));
		float fPosY = -vPos.z;//fH; //+(fH * -abs(fYaw));

		if (abs(fPosX) + (m_fRaderMarkSizeW / 2) < m_fRaderSizeW / 2 &&
			abs(fPosY) + (m_fRaderMarkSizeH / 2) < m_fRaderSizeH / 2)
		{
			fPosX = m_pRaderWindowFront->GetPos().x + fPosX;
			fPosY = m_pRaderWindowFront->GetPos().y + fPosY;

			m_v_pRaderEnemyMark[i]->SetPos({ fPosX, fPosY, 0.0f });
			m_v_pRaderEnemyMark[i]->Render();
		}
	}

	m_pRaderWindowFront->Render();


	//ロックオン関係.
	//ロックオンフレーム.
	vPosTmp = m_pPlayer->GetLookTargetPos();
	//float fYaw = m_pPlayer->GetLockYaw();

	vPosTmp = ConvDimPos(vPosTmp);

	m_pCursorFrame->SetPos(vPosTmp);
	//m_pCursorFrame->SetRot({ 0.0f, fYaw ,0.0f});
	m_pCursorFrame->Render();

	//ロックオンカーソル.
	float fTmp = m_pPlayer->GetLockCircleScale() / 2;

	m_pCursor->SetScale(fTmp);
	m_pCursor->SetPos(vPosTmp);
	//m_pCursor->SetRot({ 0.0f, fYaw, 0.0f });
	m_pCursor->Render();

	//ロックオンカーソル内に敵を入れている間の処理.
	if (m_pPlayer->GetTargetPos(vPosTmp))
	{
		//ロックオンウィンドウ描画.
		vPosTmp = m_pPlayer->m_vTargetScrPos;
		
		m_pLockWindow->SetPos(vPosTmp);
		m_pLockWindow->Render();

		//ヒットマーク描画.
		if (iHitDispTime > 0)
		{
			m_pHitMark->SetPos(vPosTmp);
			m_pHitMark->Render();
			iHitDispTime--;
		}

		if (m_pPlayer->IsLWeaponLock())
		{
			m_pLWeaponLockMark->SetPos(vPosTmp - D3DXVECTOR3{ m_fHitMarkRaderSizeW, 0.0f, 0.0f });
			m_pLWeaponLockMark->Render();
		}

		if (m_pPlayer->IsRWeaponLock())
		{
			m_pRWeaponLockMark->SetPos(vPosTmp + D3DXVECTOR3{ m_fHitMarkRaderSizeW, 0.0f, 0.0f });
			m_pRWeaponLockMark->Render();
		}
	}

	else
	{
		//ロックオンカーソル内に敵がいないなら、ヒットマークを出さない.
		if (iHitDispTime > 0)
		{
			iHitDispTime = 0;
		}
	}
	
	SetDepth(true);
}

bool clsSCENE_MISSION::AllEnemyDead()
{
	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		if (!m_v_pEnemys[i]->m_bDeadFlg)
		{
			return false;
		}
	}

	return true;
}

void clsSCENE_MISSION::CreateFriends()
{
	m_pPlayer = CreatePlayer();
	m_v_pFriends.push_back(m_pPlayer);

	/*clsFriendFactory clsFactory;

	m_v_pFriends = clsFactory.CreateFriend(m_wpPtrGroup, strMissonFolderPath, m_pPlayer);

	m_v_pFriends.shrink_to_fit();*/
}

void clsSCENE_MISSION::CreateEnemys()
{
	m_pTestObj = CreateEnemy();
	m_v_pEnemys.push_back(m_pTestObj);

	/*clsEnemyFactory clsFactory;
	
	m_v_pEnemys = clsFactory.CreateEnemy(m_wpPtrGroup, strMissonFolderPath);

	m_v_pEnemys.shrink_to_fit();*/
}

void clsSCENE_MISSION::Collison()
{
	if (ColFShottoEBody())
	{
		iHitDispTime = 30;// *static_cast<int>(g_fFPS);
	}
	ColEShottoFBody();
}

//同キャラのShotが同キャラのBodyに当たる判定を入れるかは処理の兼ね合いで入れる.
void clsSCENE_MISSION::ColFShottoFBody()
{
	for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
	{
		for (unsigned int j = 0; j < m_v_pFriends.size(); j++)
		{
			HitState Tmp = m_v_pFriends[i]->BulletHit(m_v_pFriends[j]->m_v_Spheres);
			Tmp.iDamage = 0;
			m_v_pFriends[j]->Damage(Tmp);
		}
	}
}

bool clsSCENE_MISSION::ColFShottoEBody()
{
	bool bResult = false;

	for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
	{
		for (unsigned int j = 0; j < m_v_pEnemys.size(); j++)
		{
			HitState Tmp = m_v_pFriends[i]->BulletHit(m_v_pEnemys[j]->m_v_Spheres);
			m_v_pEnemys[j]->Damage(Tmp);
			if (!bResult)bResult = Tmp.bHit;
		}
	}

	return bResult;
}

void clsSCENE_MISSION::ColEShottoFBody()
{
	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		for (unsigned int j = 0; j < m_v_pFriends.size(); j++)
		{
			HitState Tmp = m_v_pEnemys[i]->BulletHit(m_v_pFriends[j]->m_v_Spheres);
			m_v_pFriends[j]->Damage(Tmp);
		}
	}
}

void clsSCENE_MISSION::ColEShottoEBody()
{
	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		for (unsigned int j = 0; j < m_v_pEnemys.size(); j++)
		{
			HitState Tmp = m_v_pEnemys[i]->BulletHit(m_v_pEnemys[j]->m_v_Spheres);
			Tmp.iDamage = 0;
			m_v_pEnemys[j]->Damage(Tmp);
		}
	}
}

void ColFtoFDuplicate()
{

}

void ColFtoEDuplicate()
{

}

void ColEtoFDuplicate()
{

}

void ColEtoEDuplicate()
{

}

clsPlayer* clsSCENE_MISSION::CreatePlayer()
{
	clsPlayer* pPlayer = new clsPlayer;
	pPlayer->Init(m_wpPtrGroup);//4つ目の引数は効果音やエフェクトを出すために追加しました.

	D3DXVECTOR3 tmpVec3 = { 0.0f, 10.0f, 0.0f };
	pPlayer->SetPosition(tmpVec3);
	pPlayer->SetScale(0.01f);

	return pPlayer;
}

clsTestObj* clsSCENE_MISSION::CreateEnemy()
{
	clsTestObj* pEnemy = new clsTestObj;
	pEnemy->Init(m_wpPtrGroup, "Data\\FileData\\Hiyoshi\\Enemy");//4つ目の引数は効果音やエフェクトを出すために追加しました.

	D3DXVECTOR3 tmpVec3 = { 0.0f, 10.0f, 0.0f };
	pEnemy->SetPosition(tmpVec3);
	pEnemy->SetScale(0.01f);

	m_pTestObj = pEnemy;

	return pEnemy;
}

void clsSCENE_MISSION::SetEnemys()
{
	for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
	{
		m_v_pFriends[i]->SetEnemys(m_v_pEnemys);
	}

	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		m_v_pEnemys[i]->SetEnemys(m_v_pFriends);
	}
}

//============================ デバッグテキスト ===========================//
#if _DEBUG
void clsSCENE_MISSION::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 300;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.

	sprintf_s( strDbgTxt, 
		"EnemyEnelgy : [%d]",
		m_pTestObj->m_iEnelgy);
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s(strDbgTxt,
		"PlayerPos : x[%f], y[%f], z[%f]",
		m_v_pFriends[0]->GetPosition().x, m_v_pFriends[0]->GetPosition().y, m_v_pFriends[0]->GetPosition().z);
	m_upText->Render(strDbgTxt, 0, iTxtY += iOFFSET);

	sprintf_s( strDbgTxt, 
		"EnemyPos : x[%f], y[%f], z[%f]",
		m_v_pEnemys[0]->GetPosition().x, m_v_pEnemys[0]->GetPosition().y, m_v_pEnemys[0]->GetPosition().z);
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	/*sprintf_s(strDbgTxt,
		"CamPos : x[%f], y[%f], z[%f]",
		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z);
	m_upText->Render(strDbgTxt, 0, iTxtY += iOFFSET);

	sprintf_s(strDbgTxt,
		"Enelgy : [%d]",
		m_pPlayer->m_iEnelgy);
	m_upText->Render(strDbgTxt, 0, iTxtY += iOFFSET);
	
	if (m_pPlayer->m_bBoost)
	{
		sprintf_s(strDbgTxt,"ON");
		m_upText->Render(strDbgTxt, 0, iTxtY += iOFFSET);
	}

	else
	{
		sprintf_s(strDbgTxt,"OFF");
		m_upText->Render(strDbgTxt, 0, iTxtY += iOFFSET);
	}

	//dbgtxty += 10;
	//if( m_pBgm[0]->IsStopped() ){
	//	sprintf_s( strDbgTxt, "Stopped" );
	//	m_pText->Render( strDbgTxt, 0, dbgtxty );
	//}
	//if( m_pBgm[0]->IsPlaying() ){
	//	sprintf_s( strDbgTxt, "Playingn" );
	//	m_pText->Render( strDbgTxt, 0, dbgtxty );
	//}*/
}
#endif //#if _DEBUG

void clsSCENE_MISSION::UpdateCamTargetPos(clsCharactor* pChara)
{
	if (!pChara)
	{
		return;
	}

	m_vCamTargetPos = pChara->GetCamPos();
	m_vLookTargetPos = pChara->m_vLockPos;
}