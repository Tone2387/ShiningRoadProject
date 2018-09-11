#include "SceneTitle.h"
#include "WindowBox.h"

using namespace std;

const float fROBO_SCALE = 0.5f;
const float fBACK_SCALE = 1.0f;

//ロゴ.
const char* sFILE_PATH_LOGO = "Data\\Image\\TitleUi\\TitleLogo.png";
const WHSIZE_FLOAT INIT_LOGO_SIZE = { 512.0f, 128.0f };
const D3DXVECTOR3 vINIT_LOGO_POS = { WND_W*0.5f, 75.0f, 0.0f };



//日本語UI.
const char* sFONT_TEXT_PATH_TITLE = "Data\\Font\\Text\\TextTitle.csv";


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
	m_wpFont->Create( sFONT_TEXT_PATH_TITLE );

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
	m_upBack->SetScale( fBACK_SCALE );

	//箱.
	assert( !m_upBox );
	m_upBox = make_unique< clsWINDOW_BOX >( m_wpDevice, m_wpContext );
	m_upBox->SetSize( 0.0f );

//	//カメラ.
//	assert( m_wpCamera );
//	m_wpCamera->SetPos( { 0.0f, 100.0f, -80.0f } );
//	m_wpCamera->SetLookPos( { 0.0f, 45.0f, 0.0f } );

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

	if( m_upKey->isEnter( 'Z' ) ){
		m_upBox->SetSizeTarget( { 20.0f, 20.0f, 0.0f } );
	}
	if( m_upKey->isEnter( 'X' ) ){
		m_upBox->SetSizeTarget( { 10.0f, 4.0f, 0.0f } );
	}
	if( m_upKey->isEnter( 'C' ) ){
		m_upBox->SetSizeTarget( { 4.0f, 10.0f, 0.0f } );
	}
	if( m_upKey->isEnter( 'V' ) ){
		m_upBox->SetSizeTarget( { 0.0f, 0.0f, 0.0f } );
	}

	if( m_upKey->isEnter( 'A' ) ){
		m_upBox->AddChangeData( 
			1.0f, 1.0f, clsLINE_BOX::encBEFOR_CHANGE::WIDTH );
	}
	if( m_upKey->isEnter( 'S' ) ){
		m_upBox->AddChangeData( 
			1.0f, 1.0f, clsLINE_BOX::encBEFOR_CHANGE::HEIGHT );
	}
	if( m_upKey->isEnter( 'D' ) ){
		m_upBox->AddChangeData( 
			1.0f, 1.0f, clsLINE_BOX::encBEFOR_CHANGE::BOTH );
	}
	if( m_upKey->isEnter( 'F' ) ){
		m_upBox->AddChangeData( 
			100.0f, 100.0f, clsLINE_BOX::encBEFOR_CHANGE::BOTH );
	}




//	clsCAMERA_TITLE pct;
//	pct.SetPos( m_wpCamera->GetPos() );
//	pct.SetLookPos( m_wpCamera->GetLookPos() );
//	pct.SetRot( m_wpCamera->GetRot() );
//	pct.Update();
//	*m_wpCamera = pct;
	m_wpCamera->Update();
	m_upBox->Update();

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

	assert( m_upBox );
	m_upBox->Render();

	SetDepth( true );
}
void clsSCENE_TITLE::RenderUi()
{
	m_wpFont->Render( 0, 100 );

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

	//移動力.
	clsCAMERA_TITLE* pct;
	pct = (clsCAMERA_TITLE*)m_wpCamera;
	sprintf_s( strDbgTxt, 
		"CamSpd : x[%f], y[%f], z[%f]",
		pct->GetSpd().x, pct->GetSpd().y, pct->GetSpd().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamAcc : x[%f], y[%f], z[%f]",
		pct->GetAcc().x, pct->GetAcc().y, pct->GetAcc().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	pct = nullptr;

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
