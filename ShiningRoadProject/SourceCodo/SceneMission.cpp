#include "SceneMission.h"


//================================//
//========== ミッション中クラス ==========//
//================================//
clsSCENE_MISSION::clsSCENE_MISSION( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup ),
	m_pPlayer( nullptr )
	//m_pTestChara( nullptr )
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

	//テストモデルの初期化.

	m_pStage = new clsStage(m_wpResource);

	m_fCamMoveSpeed = 0.01f;
}

//毎フレーム通る処理.
void clsSCENE_MISSION::UpdateProduct( enSCENE &enNextScene )
{
	//nullならassert.
	assert(m_pPlayer);
	//m_pPlayer->Action(m_pStage);

	for (int i = 0; i < m_v_pFriends.size(); i++)
	{
		m_v_pFriends[i]->Action(m_pStage);
	}

	for (int i = 0; i < m_v_pEnemys.size(); i++)
	{
		m_v_pEnemys[i]->Action(m_pStage);
	}

	D3DXVECTOR3 vCamPosTmp = m_pPlayer->GetCamTargetPos();
	D3DXVECTOR3 vLookPosTmp = m_pPlayer->GetLookTargetPos();

	m_wpCamera->SetPos(vCamPosTmp, false);
	m_wpCamera->SetLookPos(vLookPosTmp);

	//エンディングに行く場合は以下のようにする.
	if (AllEnemyDead()){
		enNextScene = enSCENE::ENDING;
	}

	if (m_pPlayer->m_bDeadFlg){
		enNextScene = enSCENE::ENDING;
	}
}

//描画.
void clsSCENE_MISSION::RenderProduct( const D3DXVECTOR3 &vCamPos )
{

	//m_pTestChara->Render(m_mView, m_mProj, m_vLight, vCamPos);

	for (int i = 0; i < m_v_pFriends.size(); i++)
	{
		m_v_pFriends[i]->Render(m_mView, m_mProj, m_vLight, vCamPos);
	}

	for (int i = 0; i < m_v_pEnemys.size(); i++)
	{
		m_v_pEnemys[i]->Render(m_mView, m_mProj, m_vLight, vCamPos);
	}

	Collison();

	m_pStage->Render(m_mView, m_mProj, m_vLight, vCamPos);
}

void clsSCENE_MISSION::RenderUi()
{
}

bool clsSCENE_MISSION::AllEnemyDead()
{
	for (int i = 0; i < m_v_pEnemys.size(); i++)
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

	m_v_pFriends.shrink_to_fit();
}

void clsSCENE_MISSION::CreateEnemys()
{
	for (int i = 0; i < 1; i++)
	{
		clsCharactor* pChara;
		
		pChara = CreateEnemy();

		m_v_pEnemys.push_back(pChara);
	}

	m_v_pEnemys.shrink_to_fit();
}

void clsSCENE_MISSION::Collison()
{
	ColFShottoEBody();
	ColEShottoFBody();
}

//同キャラのShotが同キャラのBodyに当たる判定を入れるかは処理の兼ね合いで入れる.
void clsSCENE_MISSION::ColFShottoFBody()
{
	for (int i = 0; i < m_v_pFriends.size(); i++)
	{
		for (int j = 0; j < m_v_pFriends.size(); j++)
		{
			HitState Tmp = m_v_pFriends[i]->BulletHit(m_v_pFriends[j]->m_v_Spheres);
			Tmp.iDamage = 0;
			m_v_pFriends[j]->Damage(Tmp);
		}
	}
}

void clsSCENE_MISSION::ColFShottoEBody()
{
	for (int i = 0; i < m_v_pFriends.size(); i++)
	{
		for (int j = 0; j < m_v_pEnemys.size(); j++)
		{
			HitState Tmp = m_v_pFriends[i]->BulletHit(m_v_pEnemys[j]->m_v_Spheres);
			m_v_pEnemys[j]->Damage(Tmp);
		}
	}
}

void clsSCENE_MISSION::ColEShottoFBody()
{
	for (int i = 0; i < m_v_pEnemys.size(); i++)
	{
		for (int j = 0; j < m_v_pFriends.size(); j++)
		{
			HitState Tmp = m_v_pEnemys[i]->BulletHit(m_v_pFriends[j]->m_v_Spheres);
			m_v_pFriends[j]->Damage(Tmp);
		}
	}
}

void clsSCENE_MISSION::ColEShottoEBody()
{
	for (int i = 0; i < m_v_pEnemys.size(); i++)
	{
		for (int j = 0; j < m_v_pEnemys.size(); j++)
		{
			HitState Tmp = m_v_pEnemys[i]->BulletHit(m_v_pEnemys[j]->m_v_Spheres);
			Tmp.iDamage = 0;
			m_v_pEnemys[j]->Damage(Tmp);
		}
	}
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
	pEnemy->Init(m_wpPtrGroup);//4つ目の引数は効果音やエフェクトを出すために追加しました.

	D3DXVECTOR3 tmpVec3 = { 0.0f, 10.0f, 0.0f };
	pEnemy->SetPosition(tmpVec3);
	pEnemy->SetScale(0.01f);

	return pEnemy;
}

//============================ デバッグテキスト ===========================//
#if _DEBUG
void clsSCENE_MISSION::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.

	sprintf_s( strDbgTxt, 
		"PlayerPos : x[%f], y[%f], z[%f]",
		m_pPlayer->GetPosition().x,
		m_pPlayer->GetPosition().y, 
		m_pPlayer->GetPosition().z);
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamLokPos : x[%f], y[%f], z[%f]",
		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s(strDbgTxt,
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
	//}
}
#endif //#if _DEBUG
