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
#if _DEBUG
	,m_upText( nullptr )
#endif//#if _DEBUG
	,m_enNextScene(				enSCENE::NOTHING )
	,m_wpViewPortUsing(			m_wpViewPort11 )
	,m_pDepthStencilStateOn(	nullptr )
	,m_pDepthStencilStateOff(	nullptr )
{
//	m_wpViewPortUsing = m_wpViewPort11;
}

clsSCENE_BASE::~clsSCENE_BASE()
{
#if _DEBUG
//	SAFE_DELETE( m_upText );
	if( m_upText ){
		m_upText.reset();
	}
#endif//#if _DEBUG

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


//	m_upKey = make_unique< clsKEY_INPUT >();


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
void clsSCENE_BASE::Render()
{
	//カメラ関数.
	Camera();
	//プロジェクション関数.
	Proj();	

	//各シーンの描画.
	RenderProduct( m_wpCamera->GetPos() );
	
	//エフェクト描画.
	m_wpEffects->Render( m_mView, m_mProj, m_wpCamera->GetPos() );

	//元通りのビューポート.
	SetViewPort( m_wpViewPort11, m_wpCamera->GetPos(), m_wpCamera->GetLookPos(), WND_W, WND_H );

	//各シーンのUIの描画.
	SetDepth( false );
	RenderUi();
	SetDepth( true );

	//元通りのビューポート.
	if( m_wpViewPortUsing != m_wpViewPort11 ){
		m_wpViewPortUsing = m_wpViewPort11;
		m_wpContext->RSSetViewports( 1, m_wpViewPort11 );
	}

	//暗転描画.
	m_wpBlackScreen->Render();

#if _DEBUG
	//デバッグテキスト.
	SetDepth( false );	//Zテスト:OFF.
	RenderDebugText();
	SetDepth( true );	//Zテスト:ON.
#endif//#if _DEBUG

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


//3D座標をスクリーン( 2D )座標へと変換する.dimensions(次元) conversion(変換)
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
