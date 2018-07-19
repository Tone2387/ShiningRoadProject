#include "SceneTitle.h"

using namespace std;

const float fROBO_SCALE = 0.5f;

//ロゴ.
const char* sFILE_PATH_LOGO = "Data\\Image\\TitleUi\\TitleLogo.png";
const WHSIZE_FLOAT INIT_LOGO_SIZE = { 512.0f, 128.0f };
const D3DXVECTOR3 vINIT_LOGO_POS = { WND_W*0.5f, 150.0f, 0.0f };


//================================//
//========== タイトルクラス ==========//
//================================//
clsSCENE_TITLE::clsSCENE_TITLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_pRoboModel( nullptr )
{
}

clsSCENE_TITLE::~clsSCENE_TITLE()
{
	SAFE_DELETE( m_pRoboModel );
}


void clsSCENE_TITLE::CreateProduct()
{

	//モデルさん作成.
	assert( !m_pRoboModel );
	m_pRoboModel = new clsASSEMBLE_MODEL;
	m_pRoboModel->Create( m_wpResource, m_wpRoboStatus );
	m_pRoboModel->SetScale( fROBO_SCALE );

	//ロゴ.
	assert( !m_upLogo );
	m_upLogo = make_unique< clsSPRITE2D_CENTER >();
	SPRITE_STATE ss;
	ss.Disp = INIT_LOGO_SIZE;
	m_upLogo->Create( m_wpDevice, m_wpContext, sFILE_PATH_LOGO, ss );
	m_upLogo->SetPos( vINIT_LOGO_POS );

	//背景.
	assert( !m_upBack );
	m_upBack  = make_unique< clsCharaStatic >();
	m_upBack->AttachModel( 
		m_wpResource->GetStaticModels( clsResource::enStaticModel_StageBase ) );
	m_upBack->SetPosition( m_pRoboModel->GetPos() );
	m_upBack->SetScale( 16.0f );

	//カメラ.
	assert( m_wpCamera );
	m_wpCamera->SetPos( { 0.0f, 100.0f, -80.0f } );
	m_wpCamera->SetLookPos( { 0.0f, 45.0f, 0.0f } );

}

void clsSCENE_TITLE::UpdateProduct( enSCENE &enNextScene )
{
	//エフェクトの使い方.
	if( GetAsyncKeyState( VK_SPACE ) & 0x1 ){
		//						Excelの行番号	座標.
		m_ehHibana = m_wpEffects->Play( 2, { 0.0f, 20.0f, 0.0f } );

		//大きくする.
		m_wpEffects->SetScale( m_ehHibana, 50.0f );
		//座標.
		m_wpEffects->SetPosition( m_ehHibana, { 0.0f, 10.0f, 0.0f } );
		//回転.
		static float fff = 0.0f;
		m_wpEffects->SetRotation( m_ehHibana, { 0.0f, 0.0f, fff } );
		fff += 0.1f;
	}

	//音声とシーン移動.
	if( isPressEnter() ){
		enNextScene = enSCENE::ASSEMBLE;
		//			Excelの行番号.
		assert( m_wpSound );
		m_wpSound->StopBGM( 0 );
		m_wpSound->PlaySE( 0 );
	}


	clsCAMERA_TITLE pct;
	pct.SetPos( m_wpCamera->GetPos() );
	pct.SetLookPos( m_wpCamera->GetLookPos() );
	pct.SetRot( m_wpCamera->GetRot() );
	pct.Update();
	*m_wpCamera = pct;


}

void clsSCENE_TITLE::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	assert( m_pRoboModel );
	m_pRoboModel->Render( m_mView, m_mProj, m_vLight, vCamPos );

	assert( m_upBack );
	m_upBack->Render( m_mView, m_mProj, m_vLight, vCamPos );


	SetDepth( false );

	assert( m_upLogo );
	m_upLogo->Render();

	SetDepth( true );
}


//============================ デバッグテキスト ===========================//
#if _DEBUG
void clsSCENE_TITLE::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

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

	sprintf_s( strDbgTxt, 
		"CamRot : x[%f], y[%f], z[%f]",
		m_wpCamera->GetRot().x, m_wpCamera->GetRot().y, m_wpCamera->GetRot().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );


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
