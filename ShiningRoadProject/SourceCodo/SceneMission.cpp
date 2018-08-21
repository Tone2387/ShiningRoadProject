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
	m_pPlayer = new clsPlayer;
	m_pPlayer->Init(m_wpPtrGroup);//4つ目の引数は効果音やエフェクトを出すために追加しました.

	D3DXVECTOR3 tmpVec3 = { 0.0f, 10.0f, 0.0f };
	m_pPlayer->SetPosition(tmpVec3);
	m_pPlayer->SetScale(0.01f);

	//テストモデルの初期化.
	/*m_pTestChara = new clsCharaStatic;
	m_pTestChara->AttachModel( 
		m_wpResource->GetStaticModels( 
			clsResource::enSTATIC_MODEL::enStaticModel_Enemy ) );
	m_pTestChara->Init();
	m_pTestChara->SetPosition( D3DXVECTOR3( -2.0f, 0.0f, 0.0f ) );
	m_pTestChara->SetRotation( D3DXVECTOR3( 0.0f, 0.0f, (float)D3DX_PI*0.5 ) );*/

	m_pStage = new clsStage(m_wpResource);

	m_fCamMoveSpeed = 0.01f;
}

//毎フレーム通る処理.
void clsSCENE_MISSION::UpdateProduct( enSCENE &enNextScene )
{
	//nullならassert.
	assert(m_pPlayer);
	m_pPlayer->Action(m_pStage);

	D3DXVECTOR3 vCamPosTmp = m_pPlayer->GetCamTargetPos();

	D3DXVECTOR3 vLookPosTmp = m_pPlayer->GetLookTargetPos();
	//vLookPosTmp.y += 0.5f;
	//m_wpCamera->SetPos(vTmp);

	m_wpCamera->SetPos(vCamPosTmp,false);
	m_wpCamera->SetLookPos(vLookPosTmp);

	//エンディングに行く場合は以下のようにする.
	if( !"クリア条件を満たすとここを通る" ){
		enNextScene = enSCENE::ENDING;
	}
}

//描画.
void clsSCENE_MISSION::RenderProduct( const D3DXVECTOR3 &vCamPos )
{

	//m_pTestChara->Render(m_mView, m_mProj, m_vLight, vCamPos);
	m_pPlayer->Render(m_mView, m_mProj, m_vLight, vCamPos);
	m_pStage->Render(m_mView, m_mProj, m_vLight, vCamPos);
}

void clsSCENE_MISSION::CreateFriends()
{
	m_pPlayer = new clsPlayer;
	m_pPlayer->Init(m_wpPtrGroup);//4つ目の引数は効果音やエフェクトを出すために追加しました.

	D3DXVECTOR3 tmpVec3 = { 0.0f, 10.0f, 0.0f };
	m_pPlayer->SetPosition(tmpVec3);
	m_pPlayer->SetScale(0.01f);

	m_v_pFriends.push_back(m_pPlayer);

	m_v_pFriends.shrink_to_fit();
}

void clsSCENE_MISSION::CreateEnemys()
{

}

void clsSCENE_MISSION::Collison()
{

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
