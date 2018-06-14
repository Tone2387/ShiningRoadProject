#include "SceneBase.h"
#include <Windows.h>


//ライト方向.
const D3DXVECTOR3 vLIGHT_DIR = { 0.005f, 0.01f, -2.0f };
//カメラのより具合.
const float fZOOM = static_cast<float>( D3DX_PI / 4.0 );
//描画限界距離.
const float fRENDER_LIMIT = 640.0f;//150.0f.


//================================//
//========== 基底クラス ==========//
//================================//
clsSCENE_BASE::clsSCENE_BASE( clsPOINTER_GROUP* const ptrGroup )
	:m_wpDevice( ptrGroup->GetDevice() )
	,m_wpContext( ptrGroup->GetContext() )
	,m_wpViewPort( ptrGroup->GetViewPort() )
	,m_wpPtrGroup( ptrGroup )
	,m_wpDxInput( ptrGroup->GetDxInput() )
	,m_wpResource( ptrGroup->GetResource() )
	,m_wpEffects( ptrGroup->GetEffects() )
	,m_wpSound( ptrGroup->GetSound() )
	,m_wpCamera( ptrGroup->GetCamera() )
	,m_wpRoboStatus( ptrGroup->GetRoboStatus() )
#if _DEBUG
	,m_upText( nullptr )
#endif//#if _DEBUG
{
}

clsSCENE_BASE::~clsSCENE_BASE()
{
#if _DEBUG
	SAFE_DELETE( m_upText );
#endif//#if _DEBUG
	m_wpRoboStatus = nullptr;
	m_wpCamera = nullptr;
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
	m_wpDxInput = nullptr;
	m_wpPtrGroup = nullptr;
	m_wpViewPort = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
}


//シーン作成直後に「SceneManager.cpp」の「SwitchScene」関数内で使用されている.
void clsSCENE_BASE::Create()
{
#if _DEBUG
	//デバッグテキストの初期化.
	m_upText = new clsDebugText;
	D3DXVECTOR4 vColor( 1.0f, 1.0f, 1.0f, 1.0f );
	if( FAILED( m_upText->Init(
		m_wpContext,
		WND_W, WND_H, 50.0f,
		vColor ) ) )
	{
		MessageBox( NULL, "デバッグテキスト作成失敗", "clsMain::Loop", MB_OK );
	}
#endif//#if _DEBUG


	D3DXMatrixIdentity( &m_mView );
	D3DXMatrixIdentity( &m_mProj );	
	m_vLight = vLIGHT_DIR;

	//各シーンのCreate.
	CreateProduct();
}

//ループ内の処理( 引数を関数内で変更すると今のシーンが破棄され、.
//				  指定したシーンが生成される ).
void clsSCENE_BASE::Update( enSCENE &enNextScene )
{
	//各シーンのUpdate.
	UpdateProduct( enNextScene );

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

#if _DEBUG
	SetDepth( false );	//Zテスト:OFF.
	//デバッグテキスト.
	RenderDebugText();
	SetDepth( true );	//Zテスト:ON.
#endif//#if _DEBUG

}


//深度テスト(Zテスト)ON/OFF切替.
void clsSCENE_BASE::SetDepth( const bool isOn )
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc,
		sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	depthStencilDesc.DepthEnable = isOn;

	m_wpDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_wpDepthStencilState );
	m_wpContext->OMSetDepthStencilState(
		m_wpDepthStencilState, 1 );
}



D3DXVECTOR3 clsSCENE_BASE::GetCameraPos() const
{
	ASSERT_IF_NULL( m_wpCamera );
	return m_wpCamera->GetPos();
}
D3DXVECTOR3 clsSCENE_BASE::GetCameraLookPos() const
{
	ASSERT_IF_NULL( m_wpCamera );
	return m_wpCamera->GetLookPos();
}


//3D座標をスクリーン( 2D )座標へと変換する.dimensions(次元) conversion(変換)
D3DXVECTOR3 clsSCENE_BASE::ConvDimPos( const D3DXVECTOR3 &v3DPos )
{
	D3DXVECTOR3 v2DPos;
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	D3DXVec3Project( &v2DPos, &v3DPos, m_wpViewPort, &m_mProj, &m_mView, &mWorld );
	return v2DPos;
}



#if _DEBUG
void clsSCENE_BASE::RenderDebugText()
{
	//NULLチェック.
	ASSERT_IF_NULL( m_upText );

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
