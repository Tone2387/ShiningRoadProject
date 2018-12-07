#include "SceneBase.h"
#include "ScreenTexture.h"


using namespace std;

#define XINPUT_ENTER ( XINPUT_START | XINPUT_B )
#define XINPUT_EXIT  ( XINPUT_A )

namespace{

	//ライト方向.
	const D3DXVECTOR3 vLIGHT_DIR = { 0.005f, 0.01f, -0.01f };
	//カメラのより具合.
	const float fZOOM = static_cast<float>( D3DX_PI / 4.0 );
	//描画限界距離.
	const float fRENDER_LIMIT = 760.0f;//640.0f;.//150.0f.



	//ボタンのホールドフレーム.
	const int iHOLD_FREAM = 30;
	const int iHOLD_FREAM_FIRST = 6;
	const int iHOLD_FREAM_FIRST_STEP = 1;

	const int iNOISE_FRAME_RANGE_RATE = 4;
	const int iNOISE_DOWN_RATE = 2;
	const float fNOISE_BLOCK_RATE = 512.0f;
	const float fNOISE_PULSE_RATE = 0.25f;

	//何を累乗するか & 減衰率.
	const float fNOISE_ORIGINAL = 2.0f;
	

#ifdef _DEBUG
	const D3DXVECTOR4 vDEBUG_TEXT_COLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	const float fDEBUG_TEXT_SIZE = 50.0f;
#endif//#ifdef _DEBUG
}


//================================//
//========== 基底クラス ==========//
//================================//
clsSCENE_BASE::clsSCENE_BASE( clsPOINTER_GROUP* const ptrGroup )
	:m_wpPtrGroup(				ptrGroup )
	,m_wpDevice(				m_wpPtrGroup->GetDevice() )
	,m_wpContext(				m_wpPtrGroup->GetContext() )
	,m_wpViewPort10(			m_wpPtrGroup->GetViewPort10() )
	,m_wpViewPort11(			m_wpPtrGroup->GetViewPort11() )
	,m_wpDxInput(				m_wpPtrGroup->GetDxInput() )
	,m_wpXInput(				m_wpPtrGroup->GetXInput() )
	,m_wpResource(				m_wpPtrGroup->GetResource() )
	,m_wpEffects(				m_wpPtrGroup->GetEffects() )
	,m_wpSound(					m_wpPtrGroup->GetSound() )
	,m_wpCamera(				m_wpPtrGroup->GetCamera() )
	,m_wpRoboStatus(			m_wpPtrGroup->GetRoboStatus() )
	,m_wpBlackScreen(			m_wpPtrGroup->GetBlackScreen() )
	,m_wpFont(					m_wpPtrGroup->GetFont() )
	,m_enNextScene(				enSCENE::NOTHING )
	,m_encNoise(				encNOISE::NOTHING )
	,m_wpViewPortUsing(			m_wpViewPort11 )
	,m_pDepthStencilStateOn(	nullptr )
	,m_pDepthStencilStateOff(	nullptr )
	,m_iNoiseFrame(				0 )
	,m_fBlock(					0.0f )
	,m_fPulse(					0.0f )
	,m_bStopNoiseSe(			false )
	,m_fRenderLimit(			fRENDER_LIMIT )
	,m_fZoom(					fZOOM )
{
}

clsSCENE_BASE::~clsSCENE_BASE()
{
	if( m_wpFont ) m_wpFont->Release();

	//次のシーンに余計なエフェクトを持ち込まない.
	if( m_wpEffects ) m_wpEffects->StopAll();

	SAFE_RELEASE( m_pDepthStencilStateOff );
	SAFE_RELEASE( m_pDepthStencilStateOn );


	m_enNextScene = enSCENE::NOTHING;

	m_wpFont = nullptr;
	m_wpBlackScreen = nullptr;
	m_wpRoboStatus = nullptr;
	m_wpCamera = nullptr;
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
	m_wpDxInput = nullptr;
	m_wpPtrGroup = nullptr;
	m_wpViewPortUsing = nullptr;
	m_wpViewPort11 = nullptr;
	m_wpViewPort10 = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
}


//シーン作成直後に「SceneManager.cpp」の「SwitchScene」関数内で使用されている.
void clsSCENE_BASE::Create( const HWND hWnd )
{
	if( FAILED( CreateDepthStencilState() ) ){
		assert( !"デプスステンシル作成失敗" );
		return;
	}

	D3DXMatrixIdentity( &m_mView );
	D3DXMatrixIdentity( &m_mProj );	
	m_vLight = vLIGHT_DIR;

	m_wpBlackScreen->SetChangeSpd( fBLACK_SCREEN_DEFAULT_SPD );
	m_wpBlackScreen->GetBright();

#ifdef RENDER_SCREEN_TEXTURE_
	assert( !m_upScreenTexture );
	m_upScreenTexture = make_unique<clsSCREEN_TEXTURE>( hWnd, m_wpContext );
#endif//#ifdef RENDER_SCREEN_TEXTURE_


#ifdef _DEBUG
	//デバッグテキストの初期化.
	m_upText = make_unique< clsDebugText >();
	if( FAILED( m_upText->Init(
		m_wpContext,
		WND_W, WND_H, fDEBUG_TEXT_SIZE,
		vDEBUG_TEXT_COLOR ) ) )
	{
		assert( !"デバッグテキスト作成失敗" );
	}
#endif//#ifdef _DEBUG

	//各シーンのCreate.
	CreateProduct();

}

//ループ内の処理( 引数を関数内で変更すると今のシーンが破棄され、.
//				  指定したシーンが生成される ).
void clsSCENE_BASE::Update( enSCENE &enNextScene )
{
//	m_upKey->Update();

	//サウンドループ.
	m_wpSound->UpdateLoop();

	//enNextSceneは各シーンに直接は動かさせない.
	enSCENE tmpScene = enNextScene;

	//暗転更新.
	m_wpBlackScreen->Update();

#ifdef _DEBUG
	//BGMのチェック.
	DebugBgm();
#endif//#ifdef _DEBUG

	//各シーンのUpdate.
	UpdateProduct( tmpScene );

//	//UpdateProduct内での受け取り( 更新 )忘れ防止.
	m_wpBlackScreen->isBrightEnd();

	//シーン変更が命令されたら.
	if( tmpScene != enSCENE::NOTHING ){
		//暗転開始.
		m_wpBlackScreen->GetDark();
		m_enNextScene = tmpScene;//次に変更するシーンを覚えさせる.
	}

	//暗転しきったらご注文のシーンに切り替える.
	if( m_wpBlackScreen->isDarkEnd() ){
		enNextScene = m_enNextScene;//覚えていたシーンを教える.
		m_enNextScene = enSCENE::NOTHING;//初期化.//覚えていた忘れる.
	}

#ifdef RENDER_SCREEN_TEXTURE_	
	if( m_upScreenTexture->isNoiseFlag() ){
		//ノイズが起動中なら更新( 減衰 )する.
		m_upScreenTexture->NoiseUpdate();
	}
	else{
		if( m_bStopNoiseSe ){
			m_bStopNoiseSe = false;
			m_upScreenTexture->StopSe();
		}
	}
#endif//#ifdef RENDER_SCREEN_TEXTURE_
	
	//デバッグ用シーン切り替え.
	DebugChangeScene( enNextScene );
}

//シーン内のオブジェクトの描画関数のまとめ.
void clsSCENE_BASE::Render( 
	ID3D11RenderTargetView* const pBackBuffer_TexRTV,
	ID3D11DepthStencilView* const pDepthStencilView )
{
	//カメラ関数.
	Camera();
	//プロジェクション関数.
	Proj();	


#ifdef RENDER_SCREEN_TEXTURE_	
	if( m_upScreenTexture->isUse() ){
		//Rendertargetをテクスチャにする.
		m_upScreenTexture->SetRenderTargetTexture( pDepthStencilView );
	}
#endif//#ifdef RENDER_SCREEN_TEXTURE_


	//各シーンの描画.
	SetDepth( true );	//Zテスト:ON.
	RenderProduct( m_wpCamera->GetPos() );
	
	//エフェクト描画.
	m_wpEffects->Render( m_mView, m_mProj, m_wpCamera->GetPos() );

	//元通りのビューポート.
	SetViewPort( m_wpViewPort11, m_wpCamera->GetPos(), m_wpCamera->GetLookPos(), WND_W, WND_H );

	//各シーンのUIの描画.
	SetDepth( false );
	RenderUi();

	//元通りのビューポート.
	if( m_wpViewPortUsing != m_wpViewPort11 ){
		m_wpViewPortUsing = m_wpViewPort11;
		m_wpContext->RSSetViewports( 1, m_wpViewPort11 );
	}

	//暗転描画.
	SetDepth( true );	//Zテスト:ON.
	m_wpBlackScreen->Render();


#ifdef RENDER_SCREEN_TEXTURE_	
	if( m_upScreenTexture->isUse() ){
		//テクスチャの内容を画面に描画.
		m_upScreenTexture->RenderWindowFromTexture( pBackBuffer_TexRTV, pDepthStencilView );

		this->UpdateNoise();
	}

	if( GetAsyncKeyState( 'Z' ) & 0x1 ){
		NoiseStrong( 60 );
	}
	if( GetAsyncKeyState( 'X' ) & 0x8000 ){
		NoiseWeak( 10 );
	}
//	if( GetAsyncKeyState( 'C' ) & 0x1 ){
//		static bool nega = false;
//		m_upScreenTexture->SetNega( nega );
//		nega = !nega;
//	}
//	if( GetAsyncKeyState( 'V' ) & 0x8000 ){
//		m_upScreenTexture->SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } );
//	}
//	if( GetAsyncKeyState( 'B' ) & 0x8000 ){
//		m_upScreenTexture->SetColor( { 0.5f, 0.5f, 1.0f, 1.0f } );
//	}

#endif//#ifdef RENDER_SCREEN_TEXTURE_


#ifdef _DEBUG
	SetDepth( false );
	RenderDebugText();
	SetDepth( true );	//Zテスト:ON.
#endif//#ifdef _DEBUG


}

//メニュー操作に使ってね.
bool clsSCENE_BASE::isPressRight() const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_RIGHT ) ){
			return true;
		}
		else if( m_wpXInput->isSlopeEnter( XINPUT_RIGHT ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsLSRightEnter() ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_RIGHT ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressLeft() const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_LEFT ) ){
			return true;
		}
		else if( m_wpXInput->isSlopeEnter( XINPUT_LEFT ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsLSLeftEnter() ){
			return true;
		}
	}
	
	if( GetAsyncKeyState( VK_LEFT ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressUp()const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_UP ) ){
			return true;
		}
		else if( m_wpXInput->isSlopeEnter( XINPUT_UP ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsLSUpEnter() ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_UP ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressDown()const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_DOWN ) ){
			return true;
		}
		else if( m_wpXInput->isSlopeEnter( XINPUT_DOWN ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsLSDownEnter() ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_DOWN ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressEnter()const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_ENTER ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKeyEnter( DINPUT_ENTER ) ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_RETURN ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressExit()const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_EXIT ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKeyEnter( DINPUT_EXIT ) ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_BACK ) & 0x1 ){
		return true;
	}

	return false;
}

//メニュー操作に使ってね.
bool clsSCENE_BASE::isPressHoldRight( bool isWithStick )
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_RIGHT ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_RIGHT ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDxInput->IsLSRightStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldRight.iHoldFream ++;
		//最初.
		if(!m_HoldRight.iFirstPush ){
			m_HoldRight.iFirstPush ++;
			m_HoldRight.iHoldFream = 0;
			return true;
		}
		//二番目.
		else if(m_HoldRight.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldRight.iHoldFream >= iHOLD_FREAM ){
				m_HoldRight.iFirstPush ++;
				m_HoldRight.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldRight.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldRight.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldRight.iHoldFream = iHOLD_FREAM;
		m_HoldRight.iFirstPush = 0;
	}

	return false;
}
bool clsSCENE_BASE::isPressHoldLeft( bool isWithStick )
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_LEFT ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_LEFT ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDxInput->IsLSLeftStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_LEFT ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldLeft.iHoldFream ++;
		//最初.
		if(!m_HoldLeft.iFirstPush ){
			m_HoldLeft.iFirstPush ++;
			m_HoldLeft.iHoldFream = 0;
			return true;
		}
		//二番目.
		else if(m_HoldLeft.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldLeft.iHoldFream >= iHOLD_FREAM ){
				m_HoldLeft.iFirstPush ++;
				m_HoldLeft.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldLeft.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldLeft.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldLeft.iHoldFream = iHOLD_FREAM;
		m_HoldLeft.iFirstPush = 0;
	}

	return false;
}
bool clsSCENE_BASE::isPressHoldUp( bool isWithStick )
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_UP ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_UP ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDxInput->IsLSUpStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_UP ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldUp.iHoldFream ++;
		//最初.
		if(!m_HoldUp.iFirstPush ){
			m_HoldUp.iFirstPush ++;
			m_HoldUp.iHoldFream = 0;
			return true;
		}
		//二番目.
		else if(m_HoldUp.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldUp.iHoldFream >= iHOLD_FREAM ){
				m_HoldUp.iFirstPush ++;
				m_HoldUp.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldUp.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldUp.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldUp.iHoldFream = iHOLD_FREAM;
		m_HoldUp.iFirstPush = 0;
	}

	return false;
}
bool clsSCENE_BASE::isPressHoldDown( bool isWithStick )
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_DOWN ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_DOWN ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDxInput->IsLSDownStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldDown.iHoldFream ++;
		//最初.
		if( !m_HoldDown.iFirstPush ){
			m_HoldDown.iFirstPush ++;
			m_HoldDown.iHoldFream = 0;
			return true;
		}
		//二番目.
		else if( m_HoldDown.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldDown.iHoldFream >= iHOLD_FREAM ){
				m_HoldDown.iFirstPush ++;
				m_HoldDown.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldDown.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldDown.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldDown.iHoldFream = iHOLD_FREAM;
		m_HoldDown.iFirstPush = 0;
	}

	return false;
}

//3D座標をスクリーン( 2D )座標へと変換する conversion(変換) dimensions(次元).
D3DXVECTOR3 clsSCENE_BASE::ConvDimPos( const D3DXVECTOR3 &v3DPos )const
{
	D3DXVECTOR3 v2DPos;
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	D3DXVec3Project( &v2DPos, &v3DPos, m_wpViewPort10, &m_mProj, &m_mView, &mWorld );
	return v2DPos;
}

//深度テスト(Zテスト)ON/OFF切替.
void clsSCENE_BASE::SetDepth( const bool isOn )const
{
	if( isOn ){
		m_wpContext->OMSetDepthStencilState(
			m_pDepthStencilStateOn, 1 );
	}
	else{
		m_wpContext->OMSetDepthStencilState(
			m_pDepthStencilStateOff, 1 );
	}
}





#ifdef _DEBUG

void clsSCENE_BASE::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にだけ下にずらすか.

//	sprintf_s( strDbgTxt, 
//		"CameraPos : x[%f], y[%f], z[%f]",
//		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"CamLokPos : x[%f], y[%f], z[%f]",
//		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );


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


//BGMのチェック.
void clsSCENE_BASE::DebugBgm()
{
	if( !m_wpSound ) return;
	static int iBGM_DEBUG_NUMBER = 0;

	if( GetAsyncKeyState( 'N' ) & 0x1 ){
		if( iBGM_DEBUG_NUMBER > 0 )	iBGM_DEBUG_NUMBER --;
		m_wpSound->StopAllSound();
		m_wpSound->PlayBGM( iBGM_DEBUG_NUMBER );
	}
	if( GetAsyncKeyState( 'M' ) & 0x1 ){
		iBGM_DEBUG_NUMBER ++;
		m_wpSound->StopAllSound();
		m_wpSound->PlayBGM( iBGM_DEBUG_NUMBER );
	}

}


#endif //#ifdef _DEBUG



HRESULT clsSCENE_BASE::CreateDepthStencilState()
{
	assert( !m_pDepthStencilStateOn );
	assert( !m_pDepthStencilStateOff );

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc,
		sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	//ON.
	depthStencilDesc.DepthEnable = true;
	m_wpDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilStateOn );

	//OFF.
	depthStencilDesc.DepthEnable = false;
	m_wpDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilStateOff );

	return S_OK;
}



//カメラ関数.
void clsSCENE_BASE::Camera()
{
	assert( m_wpCamera );
	//ビュー(カメラ)変換.
	D3DXVECTOR3 vUpVec	( 0.0f, 1.0f, 0.0f );	//上方位置.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&m_wpCamera->GetPos(), &m_wpCamera->GetLookPos(), &vUpVec );

}
//プロジェクション関数.
void clsSCENE_BASE::Proj()
{
	//プロジェクション(射影行列)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,			//(out)プロジェクション計算結果.
		m_fZoom,	//y方向の視野(ラジアン指定)数字を大きくしたら視野が広くなるくなる.
		static_cast<FLOAT>( WND_W ) / static_cast<FLOAT>( WND_H ),//アスペクト比(幅/高さ).
		0.1f,				//近いビュー平面のz値.
		m_fRenderLimit );	//遠いビュー平面のz値.
}


//デバッグ用シーン切り替え.
void clsSCENE_BASE::DebugChangeScene( enSCENE &enNextScene ) const
{
	if( GetAsyncKeyState( 'Y' ) & 0x1 ){
		enNextScene = enSCENE::TITLE;
	}
	else if( GetAsyncKeyState( 'U' ) & 0x1 ){
		enNextScene = enSCENE::ASSEMBLE;
	}
	else if( GetAsyncKeyState( 'I' ) & 0x1 ){
		enNextScene = enSCENE::MISSION;
	}
	else if( GetAsyncKeyState( 'O' ) & 0x1 ){
		enNextScene = enSCENE::ENDING;
	}
	else if( GetAsyncKeyState( 'P' ) & 0x1 ){
		enNextScene = enSCENE::GAMEOVER;
	}
}

void clsSCENE_BASE::SetViewPort( 
	D3D11_VIEWPORT* const pVp, const 
	D3DXVECTOR3 &vCamPos, const D3DXVECTOR3 &vCamLookPos,
	const float fWndW, const float fWndH,
	const float fRenderLimit )
{
	if( !pVp ) return;
	if( m_wpViewPortUsing == pVp ) return;

	m_wpViewPortUsing = pVp;

	//ビュー(カメラ)変換.
	D3DXVECTOR3 vUpVec	( 0.0f, 1.0f, 0.0f );	//上方位置.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&vCamPos, &vCamLookPos, &vUpVec );

	//プロジェクション(射影行列)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,			//(out)プロジェクション計算結果.
		fZOOM,	//y方向の視野(ラジアン指定)数字を大きくしたら視野が狭くなる.
		fWndW / fWndH,//アスペクト比(幅/高さ).
		0.1f,				//近いビュー平面のz値.
		fRenderLimit );	//遠いビュー平面のz値.

	assert( m_wpContext );
	m_wpContext->RSSetViewports( 1, m_wpViewPortUsing );

//	//画面のクリア.
//	m_wpContext->ClearRenderTargetView(
//		pBackBuffer_TexRTV, g_fClearColor );

}




#ifdef RENDER_SCREEN_TEXTURE_	
//ノイズを起こす.
void clsSCENE_BASE::NoiseStrong( const int iPower )
{
	assert( m_upScreenTexture );

//	int iBlockFrame = iFrame;
//	const int iFRAME_MAX = 30;
//	if( iBlockFrame > iFRAME_MAX ){
//		iBlockFrame = iFRAME_MAX;
//	}

	m_iNoiseFrame = iPower;

//	//指数.
//	const float fEXPONENT_ADD = 1.0f;
//	m_fBlock = std::powf( fNOISE_ORIGINAL, fEXPONENT_ADD + iBlockFrame ) / 6.0f;
//	m_fPulse = std::powf( fNOISE_ORIGINAL, fEXPONENT_ADD + iFrame ) * ( fNOISE_PULSE_RATE / fNOISE_BLOCK_RATE );

	m_fBlock = std::sqrtf( static_cast<float>( m_iNoiseFrame ) ) / ( iNOISE_FRAME_RANGE_RATE * iNOISE_DOWN_RATE ) * fNOISE_BLOCK_RATE;
	m_fPulse = std::sqrtf( static_cast<float>( m_iNoiseFrame ) ) / ( iNOISE_FRAME_RANGE_RATE * iNOISE_DOWN_RATE ) * fNOISE_PULSE_RATE;

	m_upScreenTexture->SetNoiseFlag( true );

	m_upScreenTexture->SetBlock( static_cast<int>( m_fBlock ) );
	m_upScreenTexture->SetPulse( m_fPulse );

	m_upScreenTexture->PlaySeStrong();

	m_bStopNoiseSe = true;

	m_encNoise = encNOISE::BLOCK_AND_PULSE;
}
void clsSCENE_BASE::NoiseWeak( const int iFrame )
{
	assert( m_upScreenTexture );

	m_upScreenTexture->SetNoiseFlag( true );
	m_iNoiseFrame = iFrame;

	const int iNOISE_SMALL_BLOCK = 512;
	m_upScreenTexture->SetBlock( iNOISE_SMALL_BLOCK );
	m_upScreenTexture->SetPulse( 0.0f );

	m_upScreenTexture->PlaySeWeak();

	m_bStopNoiseSe = true;

	m_encNoise = encNOISE::MINUTE_BLOCK;
}

void clsSCENE_BASE::UpdateNoise()
{
	if( m_iNoiseFrame > 0 ){
		m_iNoiseFrame --;
	}

	switch( m_encNoise )
	{
	case encNOISE::BLOCK_AND_PULSE:
		//終了.
		if( m_fBlock <= fNOISE_ORIGINAL ){
			m_upScreenTexture->SetNoiseFlag( false );
			m_encNoise = encNOISE::NOTHING;
		}

		//ノイズ減衰.
		if( m_iNoiseFrame % iNOISE_FRAME_RANGE_RATE == 0 ){
			m_fBlock /= fNOISE_ORIGINAL;
			m_fPulse /= fNOISE_ORIGINAL;
			m_upScreenTexture->SetBlock( static_cast<int>( m_fBlock ) );
			m_upScreenTexture->SetPulse( m_fPulse );
		}
		break;

	case encNOISE::MINUTE_BLOCK:
		if( m_iNoiseFrame <= 0 ){
			m_upScreenTexture->SetNoiseFlag( false );
			m_encNoise = encNOISE::NOTHING;
		}
		break;
	}
}
#endif//#ifdef RENDER_SCREEN_TEXTURE_
