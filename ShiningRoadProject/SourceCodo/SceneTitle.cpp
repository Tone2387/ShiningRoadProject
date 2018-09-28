#include "SceneTitle.h"
#include "WindowBox.h"

using namespace std;


const float fROBO_SCALE = 0.5f;
const float fBACK_SCALE = 1.0f;

//ロゴ.
const char* sFILE_PATH_LOGO = "Data\\Image\\TitleUi\\TitleLogo.png";
const WHSIZE_FLOAT INIT_LOGO_SIZE = { 512.0f, 128.0f };
const D3DXVECTOR3 vINIT_LOGO_POS = { 128.0f, 75.0f, 0.0f };

//ロボ.
const float fROBO_YAW = static_cast<float>( M_PI_4 ) * 0.75f;

//フラッシュ.
const char* sFLASH_PATH = "Data\\Image\\TitleUi\\Flash.png";
const float fFLASH_DOWN = -0.05f;


////日本語UI.
const char* sFONT_TEXT_PATH_TITLE = "Data\\Font\\Text\\TextTitle.csv";

//ボタンを押してね.
const float fPLESS_START_SCALE = 2.0f;
const char* sPLESS_START_TEXT = "Press B Button";
const D3DXVECTOR2 vPLESS_START_POS = {
	vINIT_LOGO_POS.x + INIT_LOGO_SIZE.w * 0.5f, 
	static_cast<float>( WND_H ) * 0.75f 
};



//================================//
//========== タイトルクラス ==========//
//================================//
clsSCENE_TITLE::clsSCENE_TITLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_pRoboModel( nullptr )
	,m_isDispPlessStart( false )
{
}

clsSCENE_TITLE::~clsSCENE_TITLE()
{
	SAFE_DELETE( m_pRoboModel );
}


void clsSCENE_TITLE::CreateProduct()
{
	m_wpSound->PlaySE( enSE_BOMBER );

	//モデルさん作成.
	assert( !m_pRoboModel );
	m_pRoboModel = new clsASSEMBLE_MODEL;
	m_pRoboModel->Create( m_wpResource, m_wpRoboStatus, true );
	m_pRoboModel->SetRot( { 0.0f, fROBO_YAW, 0.0f } );
	m_pRoboModel->SetScale( fROBO_SCALE );

	//ロゴ.
	assert( !m_upLogo );
	m_upLogo = make_unique< clsSprite2D >();
	SPRITE_STATE ss;
	ss.Disp = INIT_LOGO_SIZE;
	m_upLogo->Create( m_wpDevice, m_wpContext, sFILE_PATH_LOGO, ss );
	m_upLogo->SetPos( vINIT_LOGO_POS );
	m_upLogo->SetAlpha( 0.0f );

	//背景.
	assert( !m_upBack );
	m_upBack  = make_unique< clsCharaStatic >();
	m_upBack->AttachModel( 
		m_wpResource->GetStaticModels( clsResource::enStaticModel_StageBase ) );
	m_upBack->SetPosition( m_pRoboModel->GetPos() );
	m_upBack->SetScale( fBACK_SCALE );

	//ごまかしフラッシュ.
	assert( !m_upFlash );
	ss.Disp = { WND_W, WND_H };
	m_upFlash = make_unique< clsSprite2D >();
	m_upFlash->Create( m_wpDevice, m_wpContext, sFLASH_PATH, ss );
	m_upFlash->SetAlpha( 0.0f );



	m_wpFont->Create( sFONT_TEXT_PATH_TITLE );
	assert( !m_upPlessStart );
	m_upPlessStart = make_unique< clsUiText >();
	m_upPlessStart->Create( m_wpContext, WND_W, WND_H, fPLESS_START_SCALE );
	m_upPlessStart->SetText( sPLESS_START_TEXT );
	m_upPlessStart->SetPos( vPLESS_START_POS );


	//箱.
	assert( !m_upBox );
	m_upBox = make_unique< clsWINDOW_BOX >( m_wpDevice, m_wpContext );
	m_upBox->SetSize( 0.0f );
	m_upBox->SetAlpha( 0.5f );

//	//カメラ.
//	assert( m_wpCamera );
//	m_wpCamera->SetPos( { 0.0f, 100.0f, -80.0f } );
//	m_wpCamera->SetLookPos( { 0.0f, 45.0f, 0.0f } );

}

void clsSCENE_TITLE::UpdateProduct( enSCENE &enNextScene )
{
	if( m_wpXInput->isSlopeExit( XINPUT_UP ) ){
		m_wpSound->PlaySE( enSE_ENTER );
	}
	if( m_wpXInput->isSlopeExit( XINPUT_LEFT ) ){
		m_wpSound->PlaySE( enSE_EXIT );
	}
	if( m_wpXInput->isSlopeExit( XINPUT_DOWN ) ){
		m_wpSound->PlaySE( enSE_BOMBER );
	}
	if( m_wpXInput->isSlopeExit( XINPUT_RIGHT ) ){
		m_wpSound->PlaySE( enSE_GUITAR );
	}

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
		m_wpSound->PlaySE( enSE_ENTER );
	}


#ifdef _DEBUG
	if( GetAsyncKeyState( 'Z' ) & 0x1 ){
		m_upBox->SetSizeTarget( { 200.0f, 200.0f, 0.0f } );
	}
	if( GetAsyncKeyState( 'X' ) & 0x1 ){
		m_upBox->SetSizeTarget( { 300.0f, 40.0f, 0.0f } );
	}
	if( GetAsyncKeyState( 'C' ) & 0x1 ){
		m_upBox->SetSizeTarget( { 400.0f, 200.0f, 0.0f } );
	}
	if( GetAsyncKeyState( 'V' ) & 0x1 ){
		m_upBox->SetSizeTarget( { 0.0f, 0.0f, 0.0f } );
	}

	if( GetAsyncKeyState( 'A' ) & 0x1 ){
		m_upBox->AddChangeData( 
			50.0f, 50.0f, clsLINE_BOX::encBEFOR_CHANGE::WIDTH );
	}
	if( GetAsyncKeyState( 'S' ) & 0x1 ){
		m_upBox->AddChangeData( 
			50.0f, 50.0f, clsLINE_BOX::encBEFOR_CHANGE::HEIGHT );
	}
	if( GetAsyncKeyState( 'D' ) & 0x1 ){
		m_upBox->AddChangeData( 
			50.0f, 50.0f, clsLINE_BOX::encBEFOR_CHANGE::BOTH );
	}
	if( GetAsyncKeyState( 'F' ) & 0x1 ){
		m_upBox->AddChangeData( 
			100.0f, 100.0f, clsLINE_BOX::encBEFOR_CHANGE::BOTH );
	}
#endif//#ifdef _DEBUG


	//薄くする.
	m_upFlash->AddAlpha( fFLASH_DOWN );
	//消えたら見えなくする.
	if( m_upFlash->GetAlpha() <= 0.0f ){
		m_upFlash->SetScale( 0.0f );
	}



	m_wpCamera->Update();
	m_upBox->Update();

	//フラッシュする瞬間.
	clsCAMERA_TITLE* pCam = (clsCAMERA_TITLE*)m_wpCamera;//ゴリ押しでごめんなさい.
	if( pCam->isFlash() ){
		m_wpSound->PlayBGM( enBGM_MAOU3 );
		m_upFlash->SetAlpha( 1.0f );
		m_upFlash->SetScale( { WND_W, WND_H, 0.0f } );
		m_upLogo->SetAlpha( 1.0f );
		m_isDispPlessStart = true;
	}

}

void clsSCENE_TITLE::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	assert( m_pRoboModel );
	m_pRoboModel->Render( m_mView, m_mProj, m_vLight, vCamPos );

	assert( m_upBack );
	m_upBack->Render( m_mView, m_mProj, m_vLight, vCamPos );


}
void clsSCENE_TITLE::RenderUi()
{
	assert( m_upLogo );
	m_upLogo->Render();

	m_wpFont->SetPos( { 0, 0, 0 } );
	m_wpFont->SetScale( 20 );
	m_wpFont->Render( 4, 100 );
	if( m_isDispPlessStart ){
		assert( m_upPlessStart );
		m_upPlessStart->Render( clsUiText::enPOS::MIDDLE );
	}

	assert( m_upBox );
	m_upBox->Render();

	assert( m_upFlash );
	m_upFlash->Render();
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
		"LTheta : x[%f]",
		m_wpXInput->GetLStickTheta() );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

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
