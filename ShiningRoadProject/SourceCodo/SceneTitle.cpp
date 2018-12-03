#include "SceneTitle.h"
#include "SceneTitleInformation.h"
#include "CameraTitle.h"
#include "AssembleModel.h"
#include "Stage.h"
#include "WindowBox.h"
#include "MenuWindowTitleStartOrEnd.h"
#include "File.h"


using namespace std;


namespace{

	const float fROBO_SCALE = 0.5f;
	const float fBACK_SCALE = 1.0f;

	//ロゴ.
	const char* sFILE_PATH_LOGO = "Data\\Image\\TitleUi\\TitleLogo.png";
	const WHSIZE_FLOAT INIT_LOGO_SIZE = { 960.0f, 640.0f };
	const D3DXVECTOR3 vINIT_LOGO_POS = { 53.0f, 75.0f, 0.0f };
	const float fINIT_LOGO_SCALE = 0.682451844f;
	//ロボ.
	const float fROBO_YAW = static_cast<float>( M_PI_4 ) * 0.75f;

	//フラッシュ.
	const char* sFLASH_PATH = "Data\\Image\\TitleUi\\Flash.png";
	const float fFLASH_DOWN = -0.05f;


	////日本語UI.
	const char* sFONT_TEXT_PATH_TITLE = "Data\\Font\\Text\\TextTitle.csv";

	//ボタンを押してね.
	const float fPLESS_START_SCALE = 16.0f;
	const D3DXVECTOR3 vPLESS_START_POS = {
		300, 
		static_cast<float>( WND_H ) * 0.75f ,
		0.0f
	};
	const int iPLESS_START_MESSAGE_INDEX = 0;

}

//================================//
//========== タイトルクラス ==========//
//================================//
clsSCENE_TITLE::clsSCENE_TITLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_fTextAlpha( 0.0f )
	,m_iTextAlphaStopFrame( 0 )
	,m_encTextAlphaMode( encTEXT_ALPHA_MODE::NEXT_MINUS )
{
}

clsSCENE_TITLE::~clsSCENE_TITLE()
{
}


void clsSCENE_TITLE::CreateProduct()
{
	m_wpFont->Create( sFONT_TEXT_PATH_TITLE );

	m_wpSound->PlaySE( enSE_BOMBER );



	//textUI.
	assert( m_wpFont );
	m_wpFont->SetPos( vPLESS_START_POS );
	m_wpFont->SetAlpha( 0.0f );
	m_wpFont->SetScale( fPLESS_START_SCALE );


	//最後にクリアした状態にする.
	//ロボモデルを作る前にクリアしたロボの情報を得る.
	m_wpRoboStatus->LodeHeroData();
	//モデルさん作成.
	assert( !m_upRoboModel );
	m_upRoboModel = make_unique< clsASSEMBLE_MODEL >();
	m_upRoboModel->Create( m_wpResource, m_wpRoboStatus );
	m_upRoboModel->SetRot( { 0.0f, fROBO_YAW, 0.0f } );
	m_upRoboModel->SetScale( fROBO_SCALE );


	//ロゴ.
	assert( !m_upLogo );
	m_upLogo = make_unique< clsSprite2D >();
	SPRITE_STATE ss;
	ss.Disp = INIT_LOGO_SIZE;
	m_upLogo->Create( m_wpDevice, m_wpContext, sFILE_PATH_LOGO, ss );
	m_upLogo->SetPos( vINIT_LOGO_POS );
	m_upLogo->SetScale( fINIT_LOGO_SCALE );
	m_upLogo->SetAlpha( 0.0f );

	//背景.
	assert( !m_upBack );
	m_upBack = make_unique< clsStage >( m_wpPtrGroup );

	//ごまかしフラッシュ.
	assert( !m_upFlash );
	ss.Disp = { WND_W, WND_H };
	m_upFlash = make_unique< clsSprite2D >();
	m_upFlash->Create( m_wpDevice, m_wpContext, sFLASH_PATH, ss );
	m_upFlash->SetAlpha( 0.0f );


	//照合用情報の作成の為のファイルデータ取得.
	const char sTITLE_INFORMATION_DATA_PATH[] = "Data\\FileData\\Tahara\\TitleMenuInformation.csv";
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	upFile->Open( sTITLE_INFORMATION_DATA_PATH );
	//照合用情報の作成.
	m_vecuiInformationDataArray.resize( enINFORMATION_INDEX_size );
	for( char i=0; i<enINFORMATION_INDEX_size; i++ ){
		const int iCOL = 0;
		assert( static_cast<unsigned int>( i ) < upFile->GetSizeRow() );
		m_vecuiInformationDataArray[i] = static_cast<unsigned int>( upFile->GetDataInt( i, iCOL ) );
	}
	upFile.reset();

//	//箱.
//	assert( !m_upMenuBox );
//	m_upMenuBox = make_unique< clsMENU_WINDOW_TITLE_START_OR_END >( 
//		m_wpPtrGroup, nullptr );

//	//カメラ.
//	assert( m_wpCamera );
//	m_wpCamera->SetPos( { 0.0f, 100.0f, -80.0f } );
//	m_wpCamera->SetLookPos( { 0.0f, 45.0f, 0.0f } );

}

void clsSCENE_TITLE::UpdateProduct( enSCENE &enNextScene )
{
#ifdef _DEBUG

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
		m_wpSound->PlaySE( enSE_WIN_APP );
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

	const float fPOS_CHANGE = 7.5f;
	if( GetAsyncKeyState( 'A' ) & 0x8000 ){
		m_upLogo->AddPos( { -fPOS_CHANGE, 0.0f, 0.0f } );
	}
	if( GetAsyncKeyState( 'D' ) & 0x8000 ){
		m_upLogo->AddPos( { fPOS_CHANGE, 0.0f, 0.0f } );
	}
	if( GetAsyncKeyState( 'S' ) & 0x8000 ){
		m_upLogo->AddPos( { 0.0f, fPOS_CHANGE, 0.0f } );
	}
	if( GetAsyncKeyState( 'W' ) & 0x8000 ){
		m_upLogo->AddPos( { 0.0f, -fPOS_CHANGE, 0.0f } );
	}

	const float fSCALE_CHANGE = 0.99f;
	if( GetAsyncKeyState( VK_UP ) & 0x8000 ){
		m_upLogo->AddScale( 1.0f - fSCALE_CHANGE + 1.0f );
	}
	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 ){
		m_upLogo->AddScale( fSCALE_CHANGE );
	}



//	if( GetAsyncKeyState( 'Z' ) & 0x1 ){
//		m_upBox->SetSizeTarget( { 200.0f, 200.0f, 0.0f } );
//	}
//	if( GetAsyncKeyState( 'X' ) & 0x1 ){
//		m_upBox->SetSizeTarget( { 300.0f, 40.0f, 0.0f } );
//	}
//	if( GetAsyncKeyState( 'C' ) & 0x1 ){
//		m_upBox->SetSizeTarget( { 400.0f, 200.0f, 0.0f } );
//	}
//	if( GetAsyncKeyState( 'V' ) & 0x1 ){
//		m_upBox->SetSizeTarget( { 0.0f, 0.0f, 0.0f } );
//	}
//
//	if( GetAsyncKeyState( 'A' ) & 0x1 ){
//		m_upBox->AddChangeData( 
//			50.0f, 50.0f, clsLINE_BOX::encBEFOR_CHANGE::WIDTH );
//	}
//	if( GetAsyncKeyState( 'S' ) & 0x1 ){
//		m_upBox->AddChangeData( 
//			50.0f, 50.0f, clsLINE_BOX::encBEFOR_CHANGE::HEIGHT );
//	}
//	if( GetAsyncKeyState( 'D' ) & 0x1 ){
//		m_upBox->AddChangeData( 
//			50.0f, 50.0f, clsLINE_BOX::encBEFOR_CHANGE::BOTH );
//	}
//	if( GetAsyncKeyState( 'F' ) & 0x1 ){
//		m_upBox->AddChangeData( 
//			100.0f, 100.0f, clsLINE_BOX::encBEFOR_CHANGE::BOTH );
//	}
#endif//#ifdef _DEBUG

	//暗転中は操作不能.
	bool isCanControl = true;
	if( m_wpBlackScreen->GetAlpha() ){
		isCanControl = false;
	}

	//薄くする.
	m_upFlash->AddAlpha( fFLASH_DOWN );
	//消えたら見えなくする.
	if( m_upFlash->GetAlpha() <= 0.0f ){
		m_upFlash->SetScale( 0.0f );
	}

	m_wpCamera->Update();

	//フラッシュする瞬間.
	if( static_cast<clsCAMERA_TITLE*>( m_wpCamera )->isFlash() ){
		const float fNEW_ALPHA = 1.0f;
		m_wpSound->PlayBGM( enBGM_MAOU3 );
		m_upFlash->SetScale( { WND_W, WND_H, 0.0f } );
		m_upFlash->SetAlpha( fNEW_ALPHA );
		m_upLogo->SetAlpha( fNEW_ALPHA );
		m_wpFont->SetAlpha( fNEW_ALPHA );
	}


	//メニューが開いているなら.
	if( m_upMenuBox ){
		MenuUpdate( enNextScene );
		return;
	}

	if( isCanControl ){

		TextAlphaUpdate();

		//音声とシーン移動.
		if( isPressEnter() ){
			//メニューウィンドウ作成.
			assert( !m_upMenuBox );
			m_upMenuBox = make_unique< clsMENU_WINDOW_TITLE_START_OR_END >( 
				m_wpPtrGroup, nullptr,
				&m_vecuiInformationDataArray );
			const D3DXVECTOR3 vMENU_POS = { 400.0f, 570.0f, 0.0f };
			m_upMenuBox->SetPos( vMENU_POS );
		}
	}



}

//テキストの明滅.
void clsSCENE_TITLE::TextAlphaUpdate()
{
	const float fADD_ALPHA = 0.125f * 0.125f;
	const float fALPHA_MIN = 0.25f;
	const float fALPHA_MAX = 1.0f;
	const int iTEXT_ALPHA_STOP_FRAME_PLUS = 60;
	const int iTEXT_ALPHA_STOP_FRAME_MINUS = 0;

	switch( m_encTextAlphaMode )
	{
	case encTEXT_ALPHA_MODE::PLUS:
		m_fTextAlpha += fADD_ALPHA;
		if( m_fTextAlpha > fALPHA_MAX ){
			m_fTextAlpha = fALPHA_MAX;
			m_encTextAlphaMode = encTEXT_ALPHA_MODE::NEXT_MINUS;
		}
		break;
	case encTEXT_ALPHA_MODE::NEXT_MINUS:
		m_iTextAlphaStopFrame ++;
		if( m_iTextAlphaStopFrame >= iTEXT_ALPHA_STOP_FRAME_PLUS ){
			m_encTextAlphaMode = encTEXT_ALPHA_MODE::MINUS;
			m_iTextAlphaStopFrame = 0;
		}
		break;
	case encTEXT_ALPHA_MODE::MINUS:
			m_fTextAlpha -= fADD_ALPHA;
			if( m_fTextAlpha < fALPHA_MIN ){
				m_fTextAlpha = fALPHA_MIN;
				m_encTextAlphaMode = encTEXT_ALPHA_MODE::NEXT_PLUS;
			}
		break;
	case encTEXT_ALPHA_MODE::NEXT_PLUS:
		m_iTextAlphaStopFrame ++;
		if( m_iTextAlphaStopFrame >= iTEXT_ALPHA_STOP_FRAME_MINUS ){
			m_encTextAlphaMode = encTEXT_ALPHA_MODE::PLUS;
			m_iTextAlphaStopFrame = 0;
		}
		break;
	}
}


//メニューの動き.
void clsSCENE_TITLE::MenuUpdate( enSCENE &enNextScene )
{
	m_upMenuBox->Update();
	//メニューが何か返してくる.
	unsigned int uiReceiveInformation = m_upMenuBox->GetInformation();
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
		case enINFORMATION_INDEX_GAME_END:
//			exit( true );
			enNextScene = enSCENE::EXIT_APP;
			break;

		case enINFORMATION_INDEX_NEXT_SCENE:
			enNextScene = enSCENE::ASSEMBLE;
			break;

		case enINFORMATION_INDEX_CLOSE_MENU:
			m_upMenuBox->Close();
			break;

		default:
			assert( !"不正な情報が返されました" );
			break;
		}
	}

	//( 見た目が )消えたら( メモリからも )消える.
	if( m_upMenuBox->isDeletePermission() ){
		m_upMenuBox.reset( nullptr );
	}

}


void clsSCENE_TITLE::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	assert( m_upRoboModel );
	m_upRoboModel->Render( m_mView, m_mProj, m_vLight, vCamPos );

	assert( m_upBack );
	m_upBack->Render( m_mView, m_mProj, m_vLight, vCamPos );


}
void clsSCENE_TITLE::RenderUi()
{
	assert( m_upLogo );
	m_upLogo->Render();


	if( m_upMenuBox ){
		m_upMenuBox->Render();
	}
	else{
		assert( m_wpFont );
		const D3DXVECTOR4 vTEXT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_wpFont->SetPos( vPLESS_START_POS );
		m_wpFont->SetScale( fPLESS_START_SCALE );
		m_wpFont->SetColor( vTEXT_COLOR );
		m_wpFont->SetAlpha( m_fTextAlpha );
		m_wpFont->Render( iPLESS_START_MESSAGE_INDEX );
	}

#ifdef _DEBUG
	m_wpFont->SetPos( { 0, 0, 0 } );
	m_wpFont->SetScale( 20 );
	m_wpFont->Render( 4, 100 );
#endif//#ifdef _DEBUG

	assert( m_upFlash );
	m_upFlash->Render();
}


//============================ デバッグテキスト ===========================//
#ifdef _DEBUG
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

//	sprintf_s( strDbgTxt, 
//		"CameraPos : x[%f], y[%f], z[%f]",
//		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"CamLokPos : x[%f], y[%f], z[%f]",
//		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamRot : x[%f], y[%f], z[%f]",
		m_wpCamera->GetRot().x, m_wpCamera->GetRot().y, m_wpCamera->GetRot().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	//移動力.
	clsCAMERA_TITLE* pCam;
	pCam = (clsCAMERA_TITLE*)m_wpCamera;
	sprintf_s( strDbgTxt, 
		"CamSpd : x[%f], y[%f], z[%f]",
		pCam->GetSpd().x, pCam->GetSpd().y, pCam->GetSpd().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamAcc : x[%f], y[%f], z[%f]",
		pCam->GetAcc().x, pCam->GetAcc().y, pCam->GetAcc().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
	pCam = nullptr;

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
#endif //#ifdef _DEBUG
