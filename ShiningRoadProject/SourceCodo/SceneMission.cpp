#include "SceneMission.h"


//================================//
//========== ミッション中クラス ==========//
//================================//
clsSCENE_MISSION::clsSCENE_MISSION( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup ),
	m_pTestRobo( nullptr ),
	m_pTestChara( nullptr )
{
}

clsSCENE_MISSION::~clsSCENE_MISSION()
{
	SAFE_DELETE( m_pTestChara );
	SAFE_DELETE( m_pTestRobo );
	SAFE_DELETE( m_pCam );
}

//生成時に一度だけ通る処理.
void clsSCENE_MISSION::CreateProduct()
{
	m_pTestRobo = new clsTestObj;
	m_pTestRobo->Init(nullptr, nullptr, nullptr, m_wpDxInput, m_wpPtrGroup);//4つ目の引数は効果音やエフェクトを出すために追加しました.

	D3DXVECTOR3 tmpVec3 = { 0.0f, 10.0f, 0.0f };
	m_pTestRobo->SetPosition(tmpVec3);
	m_pTestRobo->SetScale(0.01f);

	//テストモデルの初期化.
	m_pTestChara = new clsCharaStatic;
	m_pTestChara->AttachModel( 
		m_wpResource->GetStaticModels( 
			clsResource::enSTATIC_MODEL::enStaticModel_Enemy ) );
	m_pTestChara->Init();
	m_pTestChara->SetPosition( D3DXVECTOR3( -2.0f, 0.0f, 0.0f ) );
	m_pTestChara->SetRotation( D3DXVECTOR3( 0.0f, 0.0f, (float)D3DX_PI*0.5 ) );

	m_pStage = m_wpResource->GetStaticModels(clsResource::enStaticModel_Ground);
	m_pStage->m_Trans.vScale = { 10.0f, 10.0f, 10.0f };
	m_pStage->m_Trans.fYaw = 0.0f;
	m_pStage->m_Trans.fRoll = 0.0f;
	m_pStage->m_Trans.fPitch = 0.0f;
	m_pStage->m_Trans.vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pCam = new clsCAMERA_MISSION;
}

//毎フレーム通る処理.
void clsSCENE_MISSION::UpdateProduct( enSCENE &enNextScene )
{
	//nullならassert.
	ASSERT_IF_NULL( m_pTestRobo );
	m_pTestRobo->Action(m_pStage);

	D3DXVECTOR3 vLookPosTmp = m_pTestRobo->m_Trans.vPos;
	vLookPosTmp.y += 0.5f;
	//m_wpCamera->SetPos(vTmp);

	D3DXVECTOR3 vCamPosTmp = m_pTestRobo->m_Trans.vPos + (GetVec3Dir(m_pTestRobo->m_Trans.fYaw, g_vDirBack) * 2);
	vCamPosTmp.y += 0.5f;

	m_pCam->Update(vCamPosTmp, vLookPosTmp);

	m_wpCamera = m_pCam;
	//m_wpCamera->SetPos(vTmp, false);

	//エンディングに行く場合は以下のようにする.
	if( !"クリア条件を満たすとここを通る" ){
		enNextScene = enSCENE::ENDING;
	}
}

//描画.
void clsSCENE_MISSION::RenderProduct( const D3DXVECTOR3 &vCamPos )
{

	m_pTestChara->Render(m_mView, m_mProj, m_vLight, vCamPos);
	m_pTestRobo->Render(m_mView, m_mProj, m_vLight, vCamPos);
	m_pStage->Render(m_mView, m_mProj, m_vLight, vCamPos);
}


//============================ デバッグテキスト ===========================//
#if _DEBUG
void clsSCENE_MISSION::RenderDebugText()
{
	//NULLチェック.
	ASSERT_IF_NULL( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.

	sprintf_s( strDbgTxt, 
		"CameraPos : x[%f], y[%f], z[%f]",
		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamLokPos : x[%f], y[%f], z[%f]",
		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	D3DXVECTOR3 vTmp = m_pTestRobo->m_pMesh->GetBonePos("");

	sprintf_s(strDbgTxt,
		"BonePos : x[%f], y[%f], z[%f]",
		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z);
	m_upText->Render(strDbgTxt, 0, iTxtY += iOFFSET);

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
