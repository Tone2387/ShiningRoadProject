#include "SceneBase.h"
#include <Windows.h>

using namespace std;

namespace{

	//ライト方向.
	const D3DXVECTOR3 vLIGHT_DIR = { 0.005f, 0.01f, -2.0f };
	//カメラのより具合.
	const float fZOOM = static_cast<float>( D3DX_PI / 4.0 );
	//描画限界距離.
	const float fRENDER_LIMIT = 640.0f;//150.0f.


	#define XINPUT_ENTER  ( XINPUT_START | XINPUT_B )
	#define XINPUT_EXIT  ( XINPUT_A )

	//ボタンのホールドフレーム.
	const int iHOLD_FREAM = 30;
	const int iHOLD_FREAM_FIRST = 6;
	const int iHOLD_FREAM_FIRST_STEP = 1;

#if _DEBUG
	const D3DXVECTOR4 vDEBUG_TEXT_COLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	const float fDEBUG_TEXT_SIZE = 50.0f;
#endif//#if _DEBUG
}


//================================//
//========== 基底クラス ==========//
//================================//
clsSCENE_BASE::clsSCENE_BASE( clsPOINTER_GROUP* const ptrGroup )
	:m_wpPtrGroup(		ptrGroup )
	,m_wpDevice(		m_wpPtrGroup->GetDevice() )
	,m_wpContext(		m_wpPtrGroup->GetContext() )
	,m_wpViewPort10(	m_wpPtrGroup->GetViewPort10() )
	,m_wpViewPort11(	m_wpPtrGroup->GetViewPort11() )
	,m_wpDxInput(		m_wpPtrGroup->GetDxInput() )
	,m_wpXInput(		m_wpPtrGroup->GetXInput() )
	,m_wpResource(		m_wpPtrGroup->GetResource() )
	,m_wpEffects(		m_wpPtrGroup->GetEffects() )
	,m_wpSound(			m_wpPtrGroup->GetSound() )
	,m_wpCamera(		m_wpPtrGroup->GetCamera() )
	,m_wpRoboStatus(	m_wpPtrGroup->GetRoboStatus() )
	,m_wpBlackScreen(	m_wpPtrGroup->GetBlackScreen() )
	,m_wpFont(			m_wpPtrGroup->GetFont() )
	,m_enNextScene(		enSCENE::NOTHING )
	,m_wpViewPortUsing(	m_wpViewPort11 )
	,m_pDepthStencilStateOn( nullptr )
	,m_pDepthStencilStateOff(nullptr )
#ifdef RENDER_SCREEN_TEXTURE_	
	,m_pScreenTex( nullptr )
	,m_pScreenRTV( nullptr )
	,m_pScreenSRV( nullptr )
	,m_pScreenSmp( nullptr )
	,m_pScreenVS( nullptr )
	,m_pScreenPS( nullptr )
#endif//#ifdef RENDER_SCREEN_TEXTURE_
{
}

clsSCENE_BASE::~clsSCENE_BASE()
{
#if _DEBUG
//	SAFE_DELETE( m_upText );
	if( m_upText ){
		m_upText.reset();
	}
#endif//#if _DEBUG

#ifdef RENDER_SCREEN_TEXTURE_	
	SAFE_RELEASE( m_pScreenPS );
	SAFE_RELEASE( m_pScreenVS );
	SAFE_RELEASE( m_pScreenSmp );
	SAFE_RELEASE( m_pScreenSRV );
	SAFE_RELEASE( m_pScreenRTV );
	SAFE_RELEASE( m_pScreenTex );
#endif//#ifdef RENDER_SCREEN_TEXTURE_

	m_wpFont->Release();

//	//音を止める.
//	m_wpSound->StopAllSound();
	//次のシーンに余計なエフェクトを持ち込まない.
	m_wpEffects->StopAll();

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
void clsSCENE_BASE::Create()
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
	//描画先テクスチャ作成.
	if( FAILED( CreateScreenTexture() ) ){
		ERR_MSG( "描画先テクスチャ作成失敗", "" );
	}
#endif//#ifdef RENDER_SCREEN_TEXTURE_


#if _DEBUG
	//デバッグテキストの初期化.
	m_upText = make_unique< clsDebugText >();
	if( FAILED( m_upText->Init(
		m_wpContext,
		WND_W, WND_H, fDEBUG_TEXT_SIZE,
		vDEBUG_TEXT_COLOR ) ) )
	{
		assert( !"デバッグテキスト作成失敗" );
	}
#endif//#if _DEBUG

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

#if _DEBUG
	//BGMのチェック.
	DebugBgm();
#endif//#if _DEBUG

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
	//Rendertargetをテクスチャにする.
	SetRenderTargetTexture( pDepthStencilView );
#endif//#ifdef RENDER_SCREEN_TEXTURE_


	//各シーンの描画.
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
	m_wpBlackScreen->Render();

#if _DEBUG
	RenderDebugText();
#endif//#if _DEBUG

	SetDepth( true );	//Zテスト:ON.


#ifdef RENDER_SCREEN_TEXTURE_	
	//テクスチャの内容を画面に描画.
	RenderWindowFromTexture( pBackBuffer_TexRTV, pDepthStencilView );
#endif//#ifdef RENDER_SCREEN_TEXTURE_


}

//メニュー操作に使ってね.
bool clsSCENE_BASE::isPressRight()
{
	if( m_wpXInput->isPressEnter( XINPUT_RIGHT ) ){
		return true;
	}
	else if( m_wpXInput->isSlopeEnter( XINPUT_RIGHT ) ){
		return true;
	}
	else if( m_wpDxInput->IsLSRightEnter() ){
		return true;
	}
	else if( GetAsyncKeyState( VK_RIGHT ) & 0x1 ){
		return true;
	}
	return false;
}

bool clsSCENE_BASE::isPressLeft()
{
	if( m_wpXInput->isPressEnter( XINPUT_LEFT ) ){
		return true;
	}
	else if( m_wpXInput->isSlopeEnter( XINPUT_LEFT ) ){
		return true;
	}
	else if( m_wpDxInput->IsLSLeftEnter() ){
		return true;
	}
	else if( GetAsyncKeyState( VK_LEFT ) & 0x1 ){
		return true;
	}
	return false;
}

bool clsSCENE_BASE::isPressUp()
{
	if( m_wpXInput->isPressEnter( XINPUT_UP ) ){
		return true;
	}
	else if( m_wpXInput->isSlopeEnter( XINPUT_UP ) ){
		return true;
	}
	else if( m_wpDxInput->IsLSUpEnter() ){
		return true;
	}
	else if( GetAsyncKeyState( VK_UP ) & 0x1 ){
		return true;
	}
	return false;
}

bool clsSCENE_BASE::isPressDown()
{
	if( m_wpXInput->isPressEnter( XINPUT_DOWN ) ){
		return true;
	}
	else if( m_wpXInput->isSlopeEnter( XINPUT_DOWN ) ){
		return true;
	}
	else if( m_wpDxInput->IsLSDownEnter() ){
		return true;
	}
	else if( GetAsyncKeyState( VK_DOWN ) & 0x1 ){
		return true;
	}
	return false;
}

bool clsSCENE_BASE::isPressEnter()
{
	if( m_wpXInput->isPressEnter( XINPUT_ENTER ) ){
		return true;
	}
//	else if( m_wpDxInput->IsPressKey( enPKey_00 ) ){
//		return true;
//	}
	else if( GetAsyncKeyState( VK_RETURN ) & 0x1 ){
		return true;
	}
	return false;
}

bool clsSCENE_BASE::isPressExit()
{
	if( m_wpXInput->isPressEnter( XINPUT_EXIT ) ){
		return true;
	}
	else if( GetAsyncKeyState( VK_BACK ) & 0x1 ){
		return true;
	}
	return false;
}

//メニュー操作に使ってね.
bool clsSCENE_BASE::isPressHoldRight( bool isWithStick )
{
	bool isPush = false;
	if( m_wpXInput->isPressStay( XINPUT_RIGHT ) ){
		isPush = true;
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_RIGHT ) && isWithStick ){
		isPush = true;
	}
	else if( m_wpDxInput->IsLSRightStay() && isWithStick ){
		isPush = true;
	}
	else if( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ){
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
	if( m_wpXInput->isPressStay( XINPUT_LEFT ) ){
		isPush = true;
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_LEFT ) && isWithStick ){
		isPush = true;
	}
	else if( m_wpDxInput->IsLSLeftStay() && isWithStick ){
		isPush = true;
	}
	else if( GetAsyncKeyState( VK_LEFT ) & 0x8000 ){
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
	if( m_wpXInput->isPressStay( XINPUT_UP ) ){
		isPush = true;
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_UP ) && isWithStick ){
		isPush = true;
	}
	else if( m_wpDxInput->IsLSUpStay() && isWithStick ){
		isPush = true;
	}
	else if( GetAsyncKeyState( VK_UP ) & 0x8000 ){
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
	if( m_wpXInput->isPressStay( XINPUT_DOWN ) ){
		isPush = true;
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_DOWN ) && isWithStick ){
		isPush = true;
	}
	else if( m_wpDxInput->IsLSDownStay() && isWithStick ){
		isPush = true;
	}
	else if( GetAsyncKeyState( VK_DOWN ) & 0x8000 ){
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
D3DXVECTOR3 clsSCENE_BASE::ConvDimPos( const D3DXVECTOR3 &v3DPos )
{
	D3DXVECTOR3 v2DPos;
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	D3DXVec3Project( &v2DPos, &v3DPos, m_wpViewPort10, &m_mProj, &m_mView, &mWorld );
	return v2DPos;
}


//深度テスト(Zテスト)ON/OFF切替.
void clsSCENE_BASE::SetDepth( const bool isOn )
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



D3DXVECTOR3 clsSCENE_BASE::GetCameraPos() const
{
	assert( m_wpCamera );
	return m_wpCamera->GetPos();
}
D3DXVECTOR3 clsSCENE_BASE::GetCameraLookPos() const
{
	assert( m_wpCamera );
	return m_wpCamera->GetLookPos();
}




#if _DEBUG

void clsSCENE_BASE::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にだけ下にずらすか.

	sprintf_s( strDbgTxt, 
		"CameraPos : x[%f], y[%f], z[%f]",
		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamLokPos : x[%f], y[%f], z[%f]",
		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
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


#endif //#if _DEBUG



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
	//ビュー(カメラ)変換.
	D3DXVECTOR3 vUpVec	( 0.0f, 1.0f, 0.0f );	//上方位置.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&GetCameraPos(), &GetCameraLookPos(), &vUpVec );

}
//プロジェクション関数.
void clsSCENE_BASE::Proj()
{
	//プロジェクション(射影行列)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,			//(out)プロジェクション計算結果.
		fZOOM,	//y方向の視野(ラジアン指定)数字を大きくしたら視野が狭くなる.
		static_cast<FLOAT>( WND_W ) / static_cast<FLOAT>( WND_H ),//アスペクト比(幅/高さ).
		0.1f,				//近いビュー平面のz値.
		fRENDER_LIMIT );	//遠いビュー平面のz値.
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
	const float fWndW, const float fWndH )
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
		fRENDER_LIMIT );	//遠いビュー平面のz値.

	assert( m_wpContext );
	m_wpContext->RSSetViewports( 1, m_wpViewPortUsing );

//	//画面のクリア.
//	m_wpContext->ClearRenderTargetView(
//		pBackBuffer_TexRTV, g_fClearColor );

}


D3D11_VIEWPORT* clsSCENE_BASE::GetViewPortMainPtr()
{
	assert( m_wpViewPort11 );
	return m_wpViewPort11;
}




#ifdef RENDER_SCREEN_TEXTURE_
//レンダリングテクスチャ用.
HRESULT clsSCENE_BASE::CreateScreenTexture()
{
	//テクスチャ.
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.Format				= DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width				= WND_W;
	texDesc.Height				= WND_H;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.MiscFlags			= 0;
	
	HRESULT hr = m_wpDevice->CreateTexture2D( &texDesc, nullptr, &m_pScreenTex );
	if( FAILED( hr ) ){
		ERR_MSG( "スクリーンテクスチャ作成失敗", "" );
		assert( !"スクリーンテクスチャ作成失敗" );
		return hr;
	}

	//レンダーターゲットビュー.
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Buffer.ElementOffset					= 0;
	rtvDesc.Buffer.ElementWidth						= 0;
	rtvDesc.Buffer.FirstElement						= 0;
	rtvDesc.Buffer.NumElements						= 0;
	rtvDesc.Texture1D.MipSlice						= 0;
	rtvDesc.Texture1DArray.ArraySize				= 0;
	rtvDesc.Texture1DArray.FirstArraySlice			= 0;
	rtvDesc.Texture1DArray.MipSlice					= 0;
	rtvDesc.Texture2D.MipSlice						= 0;
	rtvDesc.Texture2DArray.ArraySize				= 0;
	rtvDesc.Texture2DArray.FirstArraySlice			= 0;
	rtvDesc.Texture2DArray.MipSlice					= 0;
	rtvDesc.Texture2DMS.UnusedField_NothingToDefine	= 0;
	rtvDesc.Texture2DMSArray.ArraySize				= 0;
	rtvDesc.Texture2DMSArray.FirstArraySlice		= 0;
	rtvDesc.Texture3D.FirstWSlice					= 0;
	rtvDesc.Texture3D.MipSlice						= 0;
	rtvDesc.Texture3D.WSize							= 0;
	
	hr = m_wpDevice->CreateRenderTargetView( m_pScreenTex, &rtvDesc, &m_pScreenRTV );
	if( FAILED( hr ) ){
		ERR_MSG( "スクリーンレンダーターゲットビュー作成失敗", "" );
		assert( !"スクリーンレンダーターゲットビュー作成失敗" );
		return hr;
	}

	//シェーダーリソースビュー.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof( srvDesc ) );
	srvDesc.Format				= rtvDesc.Format;
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels	= 1;

	hr = m_wpDevice->CreateShaderResourceView( m_pScreenTex, &srvDesc, &m_pScreenSRV );
	if( FAILED( hr ) ){
		ERR_MSG( "スクリーンシェーダーリソースビュー作成失敗", "" );
		assert( !"スクリーンシェーダーリソースビュー作成失敗" );
		return hr;
	}

	//サンプラーステート.
	D3D11_SAMPLER_DESC smpDesc;
	ZeroMemory( &smpDesc, sizeof( smpDesc ) );
	smpDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	smpDesc.MinLOD			= 0;
	smpDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = m_wpDevice->CreateSamplerState( &smpDesc, &m_pScreenSmp );
	if( FAILED( hr ) ){
		ERR_MSG( "スクリーンサンプラーステート作成失敗", "" );
		assert( !"スクリーンサンプラーステート作成失敗" );
		return hr;
	}

	//シェーダー.
	hr = this->CreateScreenShaderTexture();
	if( FAILED( hr ) ){
		ERR_MSG( "スクリーンシェーダー作成失敗", "" );
		assert( !"スクリーンシェーダー作成失敗" );
		return hr;
	}


	return S_OK;
}

HRESULT clsSCENE_BASE::CreateScreenShaderTexture()
{
	const char sSHADER_NAME[] = "Shader\\Sprite2D.hlsl";
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;

	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION; 
#endif//#ifdef _DEBUG


	//HLSLからバーテックスシェーダのブロブを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			sSHADER_NAME,	//シェーダファイル名(HLSLファイル).
			NULL,			//マクロ定義の配列へのポインタ(未使用).
			NULL,			//インクルードファイルを扱うインターフェースへのポインタ(未使用).
			"VS_Main",			//シェーダエントリーポイント関数の名前.
			"vs_5_0",		//シェーダのモデルを指定する文字列(プロファイル).
			uCompileFlag,	//シェーダコンパイルフラグ.
			0,				//エフェクトコンパイルフラグ(未使用).
			NULL,			//スレッドポンプインターフェースへのポインタ(未使用).
			&pCompiledShader,//ブロブを格納するメモリへのポインタ.
			&pErrors,		//エラーと警告一覧を格納するメモリへのポインタ.
			NULL ) ) )		//戻り値へのポインタ(未使用).
	{
		MessageBox( NULL, "hlsl(vs)読み込み失敗", "エラー", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if( FAILED(
		m_wpDevice->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pScreenVS ) ) )//(out)バーテックスシェーダ.
	{
		MessageBox( NULL, "vs作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}

	SAFE_RELEASE( pCompiledShader );





	//HLSLからピクセルシェーダのブロブを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			sSHADER_NAME,	//シェーダファイル名(HLSLファイル).
			NULL,
			NULL,
			"PS_Main",			//シェーダエントリーポイント関数の名前.
			"ps_5_0",		//シェーダのモデルを指定する文字列(プロファイル).
			uCompileFlag,	//シェーダコンパイルフラグ.
			0,
			NULL,
			&pCompiledShader,//ブロブを格納するメモリへのポインタ.
			&pErrors,
			NULL ) ) )
	{
		MessageBox( NULL, "hlsl(ps)読み込み失敗", "エラー", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if( FAILED(
		m_wpDevice->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pScreenPS ) ) )//(out)ピクセルシェーダ.
	{
		MessageBox( NULL, "ps作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );//ブロブ解放.

	return S_OK;
}


//Rendertargetをテクスチャにする.
void clsSCENE_BASE::SetRenderTargetTexture( ID3D11DepthStencilView* const pDepthStencilView )
{
	//レンダーターゲットをテクスチャに.
	float clearcolor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_wpContext->OMSetRenderTargets( 1, &m_pScreenRTV, pDepthStencilView );
	m_wpContext->ClearRenderTargetView( m_pScreenRTV, clearcolor );
	m_wpContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

//テクスチャの内容を画面に描画.
void clsSCENE_BASE::RenderWindowFromTexture( 
	ID3D11RenderTargetView* const pBackBuffer_TexRTV,
	ID3D11DepthStencilView* const pDepthStencilView )
{
	float clearcolor[] = { 1.5f, 0.5f, 0.5f, 1.0f };
	m_wpContext->OMSetRenderTargets( 1, &pBackBuffer_TexRTV, pDepthStencilView );
	m_wpContext->ClearRenderTargetView( pBackBuffer_TexRTV, clearcolor );
	m_wpContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	ID3D11Buffer* pBuffer;
	//板ポリ(四角形)の頂点を作成.
	float tmpw = WND_W;
	float tmph = WND_H;
	SpriteVertex vertices[] = 
	{
#if 0
		//頂点座標(x,y,z).					//UV座標( u, v ).
		D3DXVECTOR3( 0.0f,  tmph,  0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),//頂点1(左下).
		D3DXVECTOR3( 0.0f,  0.0f,  0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),//頂点2(左上).
		D3DXVECTOR3( tmpw,  tmph,  0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),//頂点3(右下).
		D3DXVECTOR3( tmpw,  0.0f,  0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),//頂点4(右上).
#else
		//頂点座標(x,y,z).					//UV座標( u, v ).
		D3DXVECTOR3( tmpw,  0.0f,  0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),//頂点4(右上).
		D3DXVECTOR3( tmpw,  tmph,  0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),//頂点3(右下).
		D3DXVECTOR3( 0.0f,  tmph,  0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),//頂点1(左下).
		D3DXVECTOR3( tmpw,  0.0f,  0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),//頂点4(右上).
		D3DXVECTOR3( 0.0f,  tmph,  0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),//頂点1(左下).
		D3DXVECTOR3( 0.0f,  0.0f,  0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),//頂点2(左上).
#endif
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof( vertices ) / sizeof( vertices[0] );

	//バッファ構造体.
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage				= D3D11_USAGE_DEFAULT;				//使用法(デフォルト).
	bd.ByteWidth			= sizeof( SpriteVertex ) * uVerMax;	//頂点サイズ(頂点*4).
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;			//頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;								//CPUからはアクセスしない.
	bd.MiscFlags			= 0;								//その他のフラグ(未使用).
	bd.StructureByteStride	= 0;								//構造体サイズ(未使用).

	//サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem	= vertices;	//板ポリの頂点をセット.

	//頂点バッファの作成.
	if( FAILED(
		m_wpDevice->CreateBuffer(
			&bd, &InitData, &pBuffer ) ) )
	{
		ERR_MSG( "バッファ作成失敗", "" );
		assert( !"バッファ作成失敗" );
		return ;
	}
	//頂点バッファをセット.
	UINT stride = sizeof( SpriteVertex );//データ間隔.

	m_wpContext->VSSetShader( m_pScreenVS, nullptr, 0 );
	m_wpContext->PSSetShader( m_pScreenPS, nullptr, 0 );

	m_wpContext->PSSetShaderResources( 0, 1, &m_pScreenSRV );
	m_wpContext->PSSetSamplers( 0, 1, &m_pScreenSmp );
	uint32_t offset = 0;
	m_wpContext->IASetVertexBuffers( 0, 1, &pBuffer, &stride, &offset );
	m_wpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	m_wpContext->Draw( uVerMax, 0 );

}
#endif//#ifdef RENDER_SCREEN_TEXTURE_