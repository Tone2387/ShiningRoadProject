#include "SceneMission.h"
#ifdef Tahara
#include "MenuWindowMissionPause.h"
#include "SceneMissionInformation.h"
#endif//#ifdef Tahara

using namespace std;

static const int g_iStartCnt = 180;

static const int g_iGameStartSENo = 17;
static const int g_iLockOnSENo = 16;

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
, m_bStartFlg(false)
, m_bLockStart(false)
{
}

clsSCENE_MISSION::~clsSCENE_MISSION()
{
	m_pCamTar = nullptr;
	m_pPlayer = nullptr;

	SAFE_DELETE(m_pStage);

	SAFE_DELETE(m_pWindowScrFilter);
	SAFE_DELETE(m_pWindowScr);
	SAFE_DELETE(m_pHPTargetChara);
	SAFE_DELETE(m_pStartText);
	SAFE_DELETE(m_pLockWindow);
	SAFE_DELETE(m_pHitMark);
	SAFE_DELETE(m_pRWeaponLockMark);
	SAFE_DELETE(m_pLWeaponLockMark);
	SAFE_DELETE(m_pRWeaponLockWindow);
	SAFE_DELETE(m_pLWeaponLockWindow);

	for (unsigned int i = 0; i < m_v_pRaderEnemyMark.size(); i++)
	{
		SAFE_DELETE(m_v_pRaderEnemyMark[i]);
	}

	SAFE_DELETE(m_pRaderPlayerMark);
	SAFE_DELETE(m_pRaderWindowBack);
	SAFE_DELETE(m_pRaderWindowFront);
	SAFE_DELETE(m_pRBulletMark);
	SAFE_DELETE(m_pRBulletNum);
	SAFE_DELETE(m_pLBulletMark);
	SAFE_DELETE(m_pLBulletNum);
	SAFE_DELETE(m_pEnelgy);
	SAFE_DELETE(m_pBoostOn);
	SAFE_DELETE(m_pEnelgyFrame);
	SAFE_DELETE(m_pHP);
	SAFE_DELETE(m_pCursorFrame);
	SAFE_DELETE(m_pCursor);
	SAFE_DELETE(m_pLimitTime);

	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		SAFE_DELETE(m_v_pEnemys[i]);
	}

	m_v_pEnemys.clear();

	for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
	{
		SAFE_DELETE(m_v_pFriends[i]);
	}

	m_v_pFriends.clear();
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

#ifdef Tahara
	m_pStage->SetColorRed();
#endif//#ifdef Tahara

	m_fCamMoveSpeed = 0.01f;

	if (m_wpSound)
	{
		m_wpSound->PlayBGM(0);
	}

#ifdef Tahara
	//メニュー用日本語.
	const char* sFONT_TEXT_PATH_MISSION = "Data\\Font\\Text\\TextMission.csv";
	m_wpFont->Create( sFONT_TEXT_PATH_MISSION );
	//メニューの為のデータ取得&作成.
	clsMENU_WINDOW_MISSION_BASE::CreateInformation( &m_vecuiInformationDataArray, enINFORMATION_INDEX_size );

	//メニューを開いた時の暗転画像.
	m_upPauseDisplayBlack = make_unique< clsSprite2D >();
	const char* sPAUSE_DISPLAY_BLACK_PATH = "Data\\Image\\BlackScreen.png";
	SPRITE_STATE ss;
	ss.Disp = { 1.0f, 1.0f };
	m_upPauseDisplayBlack->Create(
		m_wpDevice, m_wpContext,
		sPAUSE_DISPLAY_BLACK_PATH,
		ss );
	m_upPauseDisplayBlack->SetPos( { 0.0f, 0.0f, 0.0f } );
	const float fBLACK_ALPHA = 0.5f;
	m_upPauseDisplayBlack->SetAlpha( fBLACK_ALPHA );
	const D3DXVECTOR3 vBLACK_SCALE = { WND_W, WND_H, 0.0f };
	m_upPauseDisplayBlack->SetScale( vBLACK_SCALE );
#endif//#ifdef Tahara

}

void clsSCENE_MISSION::CreateUI()
{
	assert(!m_pLimitTime);
	char pText[STR_BUFF_MAX];
	D3DXVECTOR2 vPos = { 0.0f, 0.0f };

	//活動限界時間.

	m_pLimitTime = new clsUiText;
	m_pLimitTime->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 5.0f);
	m_pLimitTime->SetPos(vPos);

	sprintf_s(pText,"");

	m_pLimitTime->SetText(pText);

	SPRITE_STATE ss,ss_CursorFrame;

	//カーソル.
	assert(!m_pCursor);
	ss.Disp = { 1.0f, 1.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pCursor = new clsSPRITE2D_CENTER;

	m_pCursor->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\Lockon.png", ss);
	m_pCursor->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	m_pCursor->SetAlpha(0.4f);

	assert(!m_pCursorFrame);
	ss.Disp = { 837.0f, 512.0f };
	ss.Anim = { 1.0f, 1.0f };

	ss_CursorFrame.Disp = { 512.0f, 512.0f };
	ss_CursorFrame.Anim = { 1.0f, 1.0f };

	m_pCursorFrame = new clsSPRITE2D_CENTER;

	m_pCursorFrame->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\LockonFrame.png", ss);
	m_pCursorFrame->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	//HP.

	assert(!m_pHP);
	vPos = { WND_W / 2, (WND_H / 2) - (ss_CursorFrame.Disp.h / 2) - 75 };

	m_pHP = new clsUiText;
	m_pHP->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 5.0f);
	m_pHP->SetPos(vPos);

	sprintf_s(pText, "");

	m_pHP->SetText(pText);

	//EN.

	//フレーム.
	assert(!m_pEnelgyFrame);

	ss.Disp = { 355.0f, 74.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pEnelgyFrame = new clsSPRITE2D_CENTER;

	m_pEnelgyFrame->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\ENFrame.png", ss);
	m_pEnelgyFrame->SetPos({ WND_W / 2, (WND_H / 2) + (ss_CursorFrame.Disp.h / 2) + (74.0f / 2), 0.0f });

	//ブースト表示
	assert(!m_pBoostOn);
	vPos = { WND_W / 2, m_pEnelgyFrame->GetPos().y + 30};
	m_pBoostOn = new clsUiText;
	m_pBoostOn->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 3.0f);
	m_pBoostOn->SetPos(vPos);
	m_pBoostOn->SetText("Boost");

	//ENバー本体.
	assert(!m_pEnelgy);

	ss.Disp = { 646.0f / 2, 36.0f};
	ss.Anim = { 2.0f, 1.0f };

	m_pEnelgy = new clsSPRITE2D_CENTER;

	m_pEnelgy->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\ENbar.png", ss);
	m_pEnelgy->SetPos({ m_pEnelgyFrame->GetPos().x, m_pEnelgyFrame->GetPos().y - 15.0f, 0.0f });

	//残弾数.
	//左武器.
	assert(!m_pLBulletNum);
	vPos = { (WND_W / 2) - (ss_CursorFrame.Disp.w / 2), WND_H / 2 - 50 - 65 };

	m_pLBulletNum = new clsUiText;
	m_pLBulletNum->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 5.0f);
	m_pLBulletNum->SetPos(vPos);

	sprintf_s(pText, "");

	m_pLBulletNum->SetText(pText);

	assert(!m_pLBulletMark);

	ss.Disp = { 48.0f, 26.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pLBulletMark = new clsSPRITE2D_CENTER;

	m_pLBulletMark->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\LW.png", ss);
	m_pLBulletMark->SetPos({ vPos.x - (ss.Disp.w / 2), vPos.y - (ss.Disp.h / 2), 0.0f });

	//右武器.
	assert(!m_pRBulletNum);
	vPos = { (WND_W / 2) + (ss_CursorFrame.Disp.w / 2), WND_H / 2 - 50 - 65 };

	m_pRBulletNum = new clsUiText;
	m_pRBulletNum->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 5.0f);
	m_pRBulletNum->SetPos(vPos);

	sprintf_s(pText, "");

	m_pRBulletNum->SetText(pText);

	assert(!m_pRBulletMark);

	ss.Disp = { 68.0f, 32.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pRBulletMark = new clsSPRITE2D_CENTER;

	m_pRBulletMark->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\RW.png", ss);
	m_pRBulletMark->SetPos({ vPos.x + (ss.Disp.w / 2), vPos.y - (ss.Disp.h / 2), 0.0f });

	//右上のレーダー
	assert(!m_pRaderWindowFront);
	ss.Disp = { 120.0f, 80.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pRaderWindowFront = new clsSPRITE2D_CENTER;

	m_pRaderWindowFront->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\RadarWindowFront.png", ss);
	m_pRaderWindowFront->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	m_pRaderWindowFront->SetAlpha(1.0f);

	assert(!m_pRaderWindowBack);
	ss.Disp = { 120.0f, 80.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_fRaderSizeW = ss.Disp.w;
	m_fRaderSizeH = ss.Disp.h;

	m_pRaderWindowBack = new clsSPRITE2D_CENTER;

	m_pRaderWindowBack->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\RadarWindowBack.png", ss);
	m_pRaderWindowBack->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

//	m_pRaderWindowBack->SetAlpha(0.4f);

	ss.Disp = { 5.0f, 5.0f };

	m_fRaderMarkSizeW = ss.Disp.w;
	m_fRaderMarkSizeH = ss.Disp.h;

	m_pRaderPlayerMark = new clsSPRITE2D_CENTER;

	m_pRaderPlayerMark->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\RadarPlayerMark.png", ss);
	m_pRaderPlayerMark->SetPos(m_pRaderWindowBack->GetPos());

	m_v_pRaderEnemyMark.resize(m_v_pEnemys.size());

	for (unsigned int i = 0; i < m_v_pRaderEnemyMark.size(); i++)
	{
		m_v_pRaderEnemyMark[i] = new clsSPRITE2D_CENTER;

		m_v_pRaderEnemyMark[i]->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\RadarEnemyMark.png", ss);
		m_v_pRaderEnemyMark[i]->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });
	}


	//左武器ロック.
	assert(!m_pLWeaponLockWindow);
	ss.Disp = { 92.0f, 106.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pLWeaponLockWindow = new clsSPRITE2D_CENTER;

	m_pLWeaponLockWindow->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\UILockL.png", ss);
	m_pLWeaponLockWindow->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	//m_pLWeaponLockMark->SetAlpha(0.4f);

	//右武器ロック.
	assert(!m_pRWeaponLockWindow);

	m_pRWeaponLockWindow = new clsSPRITE2D_CENTER;

	m_pRWeaponLockWindow->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\UILockR.png", ss);
	m_pRWeaponLockWindow->SetPos({ WND_W - (ss.Disp.w / 2), (ss.Disp.h / 2), 0.0f });

	//左ロックマーク
	assert(!m_pLWeaponLockMark);
	ss.Disp = { 68.0f, 32.0f };
	ss.Anim = { 1.0f, 1.0f };

	m_pLWeaponLockMark = new clsSPRITE2D_CENTER;

	m_pLWeaponLockMark->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\UILock.png", ss);
	m_pLWeaponLockMark->SetPos({ (WND_W / 2) - (ss_CursorFrame.Disp.w / 2) - (ss.Disp.w / 2) - 20, WND_H / 2, 0.0f });

	//右ロックマーク
	assert(!m_pRWeaponLockMark);

	m_pRWeaponLockMark = new clsSPRITE2D_CENTER;

	m_pRWeaponLockMark->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\UILock.png", ss);
	m_pRWeaponLockMark->SetPos({ (WND_W / 2) + (ss_CursorFrame.Disp.w / 2) + (ss.Disp.w / 2) + 20, WND_H / 2, 0.0f });

	//m_pRWeaponLockMark->SetAlpha(0.4f);

	assert(!m_pHitMark);
	ss.Disp = { 128.0f, 64.0f };
	ss.Anim = { 1.0f, 1.0f };

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

	vPos = { WND_W / 2, WND_H / 2 };

	m_iStartCnt = g_iStartCnt;

	m_pStartText = new clsUiText;
	m_pStartText->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H,5.0f);
	m_pStartText->SetPos(vPos);

	m_pHPTargetChara = new clsUiText;
	m_pHPTargetChara->Create(m_wpPtrGroup->GetContext(), WND_W, WND_H, 3.0f);
	m_pHPTargetChara->SetPos(vPos);

	ss.Disp = { WND_W, WND_H };
	ss.Anim = { 1.0f, 1.0f };

	m_pWindowScr = new clsSPRITE2D_CENTER;
	m_pWindowScr->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\Screen.png", ss);
	m_pWindowScr->SetPos({ WND_W / 2, (WND_H / 2), 0.0f });

	m_pWindowScrFilter = new clsSPRITE2D_CENTER;
	m_pWindowScrFilter->Create(m_wpPtrGroup->GetDevice(), m_wpPtrGroup->GetContext(), "Data\\Image\\MissonUI\\ScreenBack.png", ss);
	m_pWindowScrFilter->SetPos({ WND_W / 2, (WND_H / 2), 0.0f });

}

//毎フレーム通る処理.
void clsSCENE_MISSION::UpdateProduct( enSCENE &enNextScene )
{
#ifdef Tahara
	if( m_upMenu ){
		MenuUpdate( enNextScene );
		return;
	}
#endif//#ifdef Tahara


	//nullならassert.
	assert(m_pPlayer);
	//m_pPlayer->Action(m_pStage);

#ifdef _DEBUG

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
		for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
		{
			m_v_pEnemys[i]->m_iHP = 0;
		}
	}

#endif

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
		enNextScene = enSCENE::MOVIE_CLEAR;
	}

	else if (m_pPlayer->m_bDeadFlg || m_pPlayer->m_bTimeUp)
	{
		enNextScene = enSCENE::GAMEOVER;
	}
#ifdef Tahara
	//シーン移動暗転中はポーズさせないためのelse.
	else if( !m_upMenu ){
		//開始しない内にポーズはさせない.
		if( !m_bStartFlg ){
			return;
		}
		//スタートボタンで.
		if( isPressStart() ){
			//メニューを開く.
			m_upMenu = make_unique< clsMENU_WINDOW_MISSION_PAUSE >(
				m_hWnd, m_wpPtrGroup, nullptr, &m_vecuiInformationDataArray );
			//エフェクトの一時停止.
			m_wpEffects->PausedAll( true );
			//========== アニメーションの一時停止はここ ==========//.
			for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
			{
				m_v_pFriends[i]->AnimPause();
			}

			for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
			{
				m_v_pEnemys[i]->AnimPause();
			}
		}
	}
#endif//#ifdef Tahara
}

//描画.
void clsSCENE_MISSION::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	Duplicate();
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

#ifdef Tahara
	//ビルの裏側描画.
	SetDepth( true );	//Zテスト:OFF.
	m_pStage->RenderInside( m_mView, m_mProj, m_vLight, m_wpCamera->GetPos() );
	SetDepth( false );	//Zテスト:OFF.
#endif//#ifdef Tahara

	m_pWindowScrFilter->Render();
	m_pWindowScr->Render();

	//活動限界時間.
	int iTmp = m_pPlayer->m_iActivityLimitTime;
	int iMin = iTmp / 3600;
	int iSecond = (iTmp - (iMin * 3600)) / 60;
	int iN = iTmp % 60;
	D3DXVECTOR3 vPosTmp;

	char pText[STR_BUFF_MAX];

	sprintf_s(pText, "%02d:%02d:%02d", iMin, iSecond, iN);
	m_pLimitTime->SetText(pText);
	m_pLimitTime->Render();

	//武器残弾数.

	int iNowNum = m_pPlayer->m_v_pWeapons[clsRobo::enWeaponLHand]->GetNowBulletNum();
	int iMaxNum = m_pPlayer->m_v_pWeapons[clsRobo::enWeaponLHand]->GetMaxBulletNum();

	sprintf_s(pText, "%2d/%2d", iNowNum, iMaxNum);
	m_pLBulletNum->SetText(pText);
	m_pLBulletNum->Render(clsUiText::enPOS::RIGHT);

	m_pLBulletMark->Render();

	iNowNum = m_pPlayer->m_v_pWeapons[clsRobo::enWeaponRHand]->GetNowBulletNum();
	iMaxNum = m_pPlayer->m_v_pWeapons[clsRobo::enWeaponRHand]->GetMaxBulletNum();

	sprintf_s(pText, "%2d/%2d", iNowNum, iMaxNum);
	m_pRBulletNum->SetText(pText);
	m_pRBulletNum->Render();

	m_pRBulletMark->Render();

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

	//ブースター点灯表示.
	if (m_pPlayer->m_bBoost)
	{
		m_pBoostOn->Render(clsUiText::enPOS::MIDDLE);
	}

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

	m_pRaderPlayerMark->Render();

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
		if (!m_bLockStart)
		{
			m_wpSound->PlaySE(g_iLockOnSENo);
			m_bLockStart = true;
		}

		//ロックオンウィンドウ描画.
		vPosTmp = m_pPlayer->m_vTargetScrPos;
		
		m_pLockWindow->SetPos(vPosTmp);
		m_pLockWindow->Render();

		int iTargetHP = m_pPlayer->GetTargetChara()->m_iHP;

		sprintf_s(pText, "%d", iTargetHP);
		m_pHPTargetChara->SetText(pText);
		m_pHPTargetChara->Render(clsUiText::enPOS::MIDDLE);

		//ヒットマーク描画.
		if (iHitDispTime > 0)
		{
			m_pHitMark->SetPos(vPosTmp);
			m_pHitMark->Render();
			iHitDispTime--;
		}

		if (m_pPlayer->IsLWeaponLock())
		{
			m_pLWeaponLockWindow->SetPos(vPosTmp);
			m_pLWeaponLockWindow->Render();

			m_pLWeaponLockMark->Render();
		}

		if (m_pPlayer->IsRWeaponLock())
		{
			m_pRWeaponLockWindow->SetPos(vPosTmp);
			m_pRWeaponLockWindow->Render();

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

		m_bLockStart = false;
	}

	if (!m_bStartFlg)
	{
		if (m_iStartCnt < g_fFPS)
		{
			sprintf_s(pText, "Misson Start");
			m_pStartText->SetText(pText);
			m_pStartText->SetColor({1.0f,0.0f,0.0f,1.0f});
			m_pStartText->Render(clsUiText::enPOS::MIDDLE);
		}

		if (m_iStartCnt < 0)
		{
			GameStart();
			m_wpSound->PlaySE(g_iGameStartSENo);
			m_bStartFlg = true;
		}

		m_iStartCnt--;
		
	}
	

#ifdef Tahara
	if( m_upMenu ){
		if( m_upPauseDisplayBlack ){
			m_upPauseDisplayBlack->Render();
		}
		m_upMenu->Render();
	}
#endif//#ifdef Tahara

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
	clsFriendFactory clsFactory;

	m_pPlayer = new clsPlayer;

	m_v_pFriends = clsFactory.CreateFriend(m_wpPtrGroup, strMissonFolderPath, m_pPlayer);

	m_v_pFriends.shrink_to_fit();
}

void clsSCENE_MISSION::CreateEnemys()
{
	clsEnemyFactory clsFactory;
	
	m_v_pEnemys = clsFactory.CreateEnemy(m_wpPtrGroup, strMissonFolderPath);

	m_v_pEnemys.shrink_to_fit();
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
			HitState Tmp = m_v_pFriends[i]->BulletHit(m_v_pFriends[j]->GetColSpheres());
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
			HitState Tmp = m_v_pFriends[i]->BulletHit(m_v_pEnemys[j]->GetColSpheres());
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
			HitState Tmp = m_v_pEnemys[i]->BulletHit(m_v_pFriends[j]->GetColSpheres());
			m_v_pFriends[j]->Damage(Tmp);

			if (m_v_pFriends[j]->IsPlayerChara())
			{
				if (Tmp.bHit)
				{
#ifdef Tahara
#ifdef RENDER_SCREEN_TEXTURE_
					//ノイズエフェクト.
					PlayNoise(Tmp);
#endif//#ifdef RENDER_SCREEN_TEXTURE_
#endif//#ifdef Tahara
				}
			}
		}
	}
}



void clsSCENE_MISSION::ColEShottoEBody()
{
	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		for (unsigned int j = 0; j < m_v_pEnemys.size(); j++)
		{
			HitState Tmp = m_v_pEnemys[i]->BulletHit(m_v_pEnemys[j]->GetColSpheres());
			Tmp.iDamage = 0;
			m_v_pEnemys[j]->Damage(Tmp);
		}
	}
}

void clsSCENE_MISSION::Duplicate()
{
	ColFtoFDuplicate();
	ColFtoEDuplicate();
	ColEtoFDuplicate();
}

void clsSCENE_MISSION::ColFtoFDuplicate()
{
	for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
	{
		for (unsigned int j = 0; j < m_v_pFriends.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			m_v_pFriends[i]->CharaDuplicate(m_v_pFriends[j]);

		}
	}
}

void clsSCENE_MISSION::ColFtoEDuplicate()
{
	for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
	{
		for (unsigned int j = 0; j < m_v_pEnemys.size(); j++)
		{
			m_v_pFriends[i]->CharaDuplicate(m_v_pEnemys[j]);
		}
	}
}

void clsSCENE_MISSION::ColEtoFDuplicate()
{
	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		for (unsigned int j = 0; j < m_v_pEnemys.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			m_v_pEnemys[i]->CharaDuplicate(m_v_pEnemys[j]);

		}
	}
}

/*clsPlayer* clsSCENE_MISSION::CreatePlayer()
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
}*/

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
#ifdef _DEBUG
void clsSCENE_MISSION::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 300;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.

	/*sprintf_s( strDbgTxt, 
		"EnemyEnelgy : [%d]",
		m_pTestObj->m_iEnelgy);
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );*/

	sprintf_s(strDbgTxt,
		"PlayerPos : x[%f], y[%f], z[%f]",
		m_v_pFriends[0]->GetPosition().x, m_v_pFriends[0]->GetPosition().y, m_v_pFriends[0]->GetPosition().z);
	m_upText->Render(strDbgTxt, 0, iTxtY += iOFFSET);

	sprintf_s( strDbgTxt, 
		"EnemyPos : x[%f], y[%f], z[%f]",
		m_v_pEnemys[0]->GetPosition().x, m_v_pEnemys[0]->GetPosition().y, m_v_pEnemys[0]->GetPosition().z);
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
}
#endif //#ifdef _DEBUG

void clsSCENE_MISSION::UpdateCamTargetPos(clsCharactor* pChara)
{
	if (!pChara)
	{
		return;
	}

	m_vCamTargetPos = pChara->GetCamPos();
	m_vLookTargetPos = pChara->m_vLockPos;
}

void clsSCENE_MISSION::GameStart()
{
	for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
	{
		m_v_pFriends[i]->ActStart();
	}

	for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
	{
		m_v_pEnemys[i]->ActStart();
	}
}







#ifdef Tahara
//メニューの動き.
void clsSCENE_MISSION::MenuUpdate( enSCENE &enNextScene )
{
	m_upMenu->Update();
	//メニューが何か返してくる.
	unsigned int uiReceiveInformation = m_upMenu->GetInformation();
	if( uiReceiveInformation )
	{
		char cInformationIndex = -1;
		for( char i=0; i<enINFORMATION_INDEX_size; i++ ){
			//有用な情報と合致したなら.
			if( uiReceiveInformation == m_vecuiInformationDataArray[i] ){
				cInformationIndex = i;
			}
		}
		switch( cInformationIndex )
		{
		case enINFORMATION_INDEX_MISSION_FAILED:
			enNextScene = enSCENE::GAMEOVER;
			break;

		case enINFORMATION_INDEX_WINDOW_CLOSE:
			m_upMenu->Close();
			break;

		default:
			m_upMenu->Close();
			assert( !"不正な情報が返されました" );
			break;
		}
	}

	//( 見た目が )消えたら( メモリからも )消える.
	if( m_upMenu->isDeletePermission() ){
		m_upMenu.reset( nullptr );
		//消し終わったら.
		if( !m_upMenu ){
			//エフェクトの一時停止の解除.
			m_wpEffects->PausedAll( false );
			//========== モデルのアニメーションの一時停止の解除 ==========//.
			for (unsigned int i = 0; i < m_v_pFriends.size(); i++)
			{
				m_v_pFriends[i]->AnimPlay();
			}

			for (unsigned int i = 0; i < m_v_pEnemys.size(); i++)
			{
				m_v_pEnemys[i]->AnimPlay();
			}
		}
	}
}


#ifdef RENDER_SCREEN_TEXTURE_
//ノイズエフェクト.
void clsSCENE_MISSION::PlayNoise( const HitState& Hitstate )
{
	if( Hitstate.bHit ){
		//ダメージ量のによるノイズの種類の境界線.
		const int iNOISE_STRONG_BORDER = 1000;
		if( Hitstate.iDamage >= iNOISE_STRONG_BORDER ){
			if( NoiseStrong() ){
				m_wpXInput->SetVibPowerL( XINPUT_VIBRATION_MAX, Hitstate.iDamage );
				m_wpXInput->SetVibPowerR( XINPUT_VIBRATION_MAX, Hitstate.iDamage );
			}
		}
		else{
			if( NoiseWeak() ){
				m_wpXInput->SetVibPowerR( XINPUT_VIBRATION_MAX, Hitstate.iDamage );
			}
		}
	}
}
#endif//#ifdef RENDER_SCREEN_TEXTURE_


#endif//#ifdef Tahara
