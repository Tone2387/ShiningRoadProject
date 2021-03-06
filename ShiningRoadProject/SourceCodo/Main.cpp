#include "Main.h"
#include "RenderAtStartUp.h"
#include "Game.h"
#include "Singleton.h"
#include "BlendState.h"
#include <stdio.h>
#include <thread>

using namespace std;

//============================================================
//	定数.
//============================================================
#define WND_TITLE	"Gigant Weapons"
#define APR_NAME	"Gigant Weapons"


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//グローバル変数.
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
unique_ptr< clsMain >	g_upMain;


//============================================================
//	メイン関数.
//============================================================
INT WINAPI WinMain(
	HINSTANCE hInstance,		//インスタン番号(ウィンドウの番号).
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	INT nCmdShow )
{
	// メモリリーク検出
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

//	g_pClsMain = new clsMain;	//初期化&クラスの宣言.
	g_upMain = make_unique< clsMain >();

	//クラスが存在しているかチェック.
	if( g_upMain != nullptr ){
		//ウィンドウ作成成功.
		if( SUCCEEDED( g_upMain->InitWindow(
			hInstance,
			64, 64,
			WND_W, WND_H,
			WND_TITLE ) ) )
		{
			//Dx11用の初期化
			if( SUCCEEDED( g_upMain->InitD3D() ) ){
				g_upMain->CreateResource();
				//メッセージループ.
				g_upMain->Loop();
			}
		}
		//終了.
		g_upMain->DestroyD3D();//Direct3Dの解放.

		g_upMain.reset( nullptr );
	}

	return 0;
}


//============================================================
//	ウィンドウプロシージャ.
//============================================================
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam )
{
	//プロシージャ.
	return g_upMain->MsgProc( hWnd, uMsg, wParam, lParam );
}


//============================================================
//	メインクラス.
//============================================================

//============================================================
//	コンストラクタ.
//============================================================
clsMain::clsMain()
	:m_hWnd( nullptr )
	,m_pDevice( nullptr )
	,m_pDeviceContext( nullptr )
	,m_pSwapChain( nullptr )
	,m_pBackBuffer_TexRTV( nullptr )
	,m_pBackBuffer_DSTex( nullptr )
	,m_pBackBuffer_DSTexDSV( nullptr )
	,m_spViewPort10( nullptr )
	,m_spViewPort11( nullptr )
{
}

//============================================================
//	デストラクタ.
//============================================================
clsMain::~clsMain()
{
	DestroyD3D();
}

//============================================================
//ウィンドウ初期化関数.
//============================================================
HRESULT clsMain::InitWindow(
	HINSTANCE hInstance,	//インスタンス.
	INT x, INT y,			//ウィンドウのx,y座標.
	INT width, INT height,	//ウィンドウの幅,高さ.
	LPSTR WindowName )		//ウィンドウ名.
{
	//ウィンドウの定義.
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) );//初期化.

	wc.cbSize			= sizeof( wc );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	wc.lpszClassName	= APR_NAME;
	wc.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );

	//ウィンドウクラスをWindowsに登録.
	if( !RegisterClassEx( &wc ) ){
		MessageBox( NULL, "ウィンドウクラスノ登録にミス", "clsMainInitWindow", MB_OK );
		return E_FAIL;
	}

	//ウィンドウの作成.
	m_hWnd = CreateWindow(
		APR_NAME,			//アプリ名.
		WindowName,			//ウィンドウタイトル.
		WS_OVERLAPPEDWINDOW,//ウィンドウ種別.
		x, y,				//表示座標.
		width, height,		//ウィンドウの幅,高さ.
		NULL,				//親ウィンドウハンドル.
		NULL,				//メニュー設定.
		hInstance,			//インスタンス番号.
		NULL );				//ウィンドウ作成時に発生するイベントに渡すデータ.

	if( !m_hWnd ){
		MessageBox( NULL, "ウィンドウ作成にミス", "clsMain::InitWindow", MB_OK );
		return E_FAIL;
	}
#ifdef Inoue
#ifdef _DEBUG
	DragAcceptFiles(
		m_hWnd,    // 登録するウィンドウ
		true // アクセプトオプション
	   );
#endif	//#ifdef _DEBUG
#endif//#ifdef Inoue

	//ウィンドウの表示.
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	//マウスカーソルの非表示.
#ifndef _DEBUG
	ShowCursor( false );
#endif//#ifndef _DEBUG

	return S_OK;
}

//============================================================
//	ウィンドウ関数(メッセージ毎の処理).
//============================================================
LRESULT clsMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_DESTROY:	//ウィンドウ破棄時.
		//アプリケーションの終了をWindowsに通知する.
		PostQuitMessage( 0 );
		break;
	case WM_DROPFILES:
//		{
//		HDROP hDrop;
//		UINT uiFileNo;
//		g_bFileActive = true;
//		hDrop = (HDROP)wParam;
//		uiFileNo = DragQueryFile( (HDROP)wParam, 0xFFFFFFFF, NULL, 0 );
//		char Path[MAX_PATH];
//		for (UINT i = 0; i < uiFileNo; i++){
//			DragQueryFile( hDrop, i, Path, sizeof( Path ) );
//			g_vsFilePath.push_back( Path );
//		}
//		DragFinish( hDrop );
//		}
		break;
	case WM_KEYDOWN:	//キーボードが押されたとき.
		//キー別の処理.
		switch( (char)wParam )
		{
		case VK_ESCAPE:	//ESCキー.
			if( MessageBox( NULL,
				"ゲームを終了しますか?", "Message",
				MB_YESNO ) == IDYES )
			{
				//ウィンドウを破棄する.
				DestroyWindow( hWnd );
			}
			break;
		}
		break;
	}

	//メインに返す情報.
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


void clsMain::CreateResource()
{
	//起動中の描画.
	unique_ptr< clsRENDER_AT_START_UP > upRenderAtStartUp =
		make_unique< clsRENDER_AT_START_UP >( 
			m_pDevice,
			m_pDeviceContext,
			m_pSwapChain,
			m_pBackBuffer_TexRTV,
			m_pBackBuffer_DSTexDSV );
	//別スレッドで描画.
	thread thStartUpRender( &clsRENDER_AT_START_UP::Loop, upRenderAtStartUp.get() );


	//メッシュ読み込み関数をまとめたもの.
	ReadMesh();

	//必要なくなったので閉じる.
	upRenderAtStartUp->FinishLoad();
	thStartUpRender.join();
	upRenderAtStartUp.reset();

	//ロード画面終了後にCreateしないと効果音のタイミングがおかしくなる.
	m_upGame->Create();

}


//============================================================
//	メッセージループとアプリケーション処理の入り口.
//============================================================
void clsMain::Loop()
{
	//----------------------------------------------------------
	//	フレームレート.
	//----------------------------------------------------------
	float fRate		= 0.0f;	//レート.
	DWORD sync_old	= timeGetTime();	//過去時間.
	DWORD sync_now;
	//時間処理の為、最小単位を1ミリ秒に変更.
	timeBeginPeriod( 1 );

	//メッセージループ.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof( msg ) );

	//終了フラグ.
	bool isExitApp = true;

	while( msg.message != WM_QUIT && isExitApp )
	{
		Sleep( 1 );
		sync_now = timeGetTime();	//現在時間を取得.

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		fRate = 1000.0f / g_fFPS;	//理想時間を算出.

		if( sync_now - sync_old >= fRate ){
			sync_old = sync_now;	//現在時間に置きかえ.

			//アプリケーション処理はここから飛ぶ.
			isExitApp = AppMain();
		}
#ifdef STARTUP_FULLSCREEN_
		if( GetAsyncKeyState( VK_F4 ) & 0x1 )ChangeWindowMode();
#endif//#ifdef STARTUP_FULLSCREEN_

	}
	//アプリケーションの終了.
	timeEndPeriod( 1 );	//解除.
}

//============================================================
//	アプリケーションメイン処理.
//============================================================
bool clsMain::AppMain()
{
	bool isExitApp;

	//ゲームループ.
	assert( m_upGame );
	isExitApp = m_upGame->Update();

	//レンダリング.
	Render();

	return isExitApp;
}

//============================================================
//	描画(レンダリング).
//	シーン(場面)を画面にレンダリング(描画).
//	(※DX9MESH内とMain内で2つ存在するので注意).
//============================================================
void clsMain::Render()
{
	//このRender関数の前のAppMain関数でチェックしているのでアサートは省く.
	m_upGame->Render();
	
	//レンダリングされたイメージを表示.
	m_pSwapChain->Present( 0, 0 );

}



//============================================================
//	DirectX初期化.
//============================================================
HRESULT clsMain::InitD3D()
{
	//---------------------------------------------------
	//	デバイスとスワップチェーン関係.
	//---------------------------------------------------

	//スワップチェーン構造体.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount		= 1;		//バックバッファの数.1.
	sd.BufferDesc.Width	= WND_W;
	sd.BufferDesc.Height= WND_H;
	sd.BufferDesc.Format= DXGI_FORMAT_R8G8B8A8_UNORM;
									//フォーマット(32ビットカラー).
	sd.BufferDesc.RefreshRate.Numerator = g_fFPS;
									//リフレッシュレート(分母) ※FPS:60.
	sd.BufferDesc.RefreshRate.Denominator = 1;
									//リフレッシュレート(分子).
	sd.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
									//使い方(表示先).
	sd.OutputWindow		= m_hWnd;	//ウィンドウハンドル.
	sd.SampleDesc.Count	= 1;		//マルチサンプルの数.
	sd.SampleDesc.Quality=0;		//マルチサンプルのクオリティ.
	sd.Windowed			= TRUE;		//ウィンドウモード(フルスク時はFALSE).


	//作成を試みる機能レベルの優先を指定.
	//	(GPUがサポートする機能セットの定義).
	//	D3D_FEATURE_LEVEL列挙体の配列.
	//	D3D_FEATURE_LEVEL_10_1:Direct3D 10.1のGPUレベル.
	D3D_FEATURE_LEVEL	pFeatureLevels	= D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL*	pFeatureLevel	= nullptr;	//配列の要素数.


	//デバイスとスワップチェーンの作成.
	//	ハードウェア(GPU)デバイスで作成.
	if( FAILED( D3D11CreateDeviceAndSwapChain(
				NULL,					//ビデオアダプタへのポインタ.
				D3D_DRIVER_TYPE_HARDWARE,//作成するデバイスの種類.
				NULL,					//ソフトウェアラスタライザーを実装するDLLのハンドル.
				0,						//有効にするランタイムレイヤー.
				&pFeatureLevels,		//作成を試みる機能レベルの順序を指定する配列.
				1,						//↑の要素数.
				D3D11_SDK_VERSION,		//SDKのバージョン.
				&sd,					//スワップチェーンの初期化パラメータのポインタ.
				&m_pSwapChain,			//(out)レンダリングに使用されたスワップチェーン.
				&m_pDevice,				//(out)作成されたデバイス.
				pFeatureLevel,			//機能レベルの配列にある最初の要素を表すポインタ.
				&m_pDeviceContext ) ) )	//(out)デバイスコンテキスト.
	{
		//WARPデバイスの作成.
		// D3D_FEATURE_LEVEL_9_1 〜 D3D_FEATURE_LEVEL_10_1.
		if( FAILED( D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_WARP,
				NULL, 0, &pFeatureLevels, 1,
				D3D11_SDK_VERSION, &sd,
				&m_pSwapChain, &m_pDevice,
				pFeatureLevel, &m_pDeviceContext ) ) )
		{
			//リファレンスデバイスの作成.
			//	DirectX SDKがインストールされていないと使えない.
			if( FAILED( D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_REFERENCE,
				NULL, 0, &pFeatureLevels, 1,
				D3D11_SDK_VERSION, &sd,
				&m_pSwapChain, &m_pDevice,
				pFeatureLevel, &m_pDeviceContext ) ) )
			{
				MessageBox( NULL, "デバイスとスワップチェーンの作成にミス", "error(main.cpp)", MB_OK );
				return E_FAIL;
			}
		}
	}


	//各種テクスチャーと、それに付帯する各種ビュー(画面)を作成.

	//---------------------------------------------------
	//	バックバッファ準備:カラーバッファ設定.
	//---------------------------------------------------

	//バックバッファテクスチャーを取得(既にあるので作成はない).
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),	//__uuidof:式に関連付けたGUIDを取得.
									//		   Texture2Dの唯一の物として扱う.
		(LPVOID*)&pBackBuffer_Tex );//(out)バックバッファテクスチャ.

	//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成.
	m_pDevice->CreateRenderTargetView(
		pBackBuffer_Tex,
		NULL,
		&m_pBackBuffer_TexRTV );//(out)RTV.
	//バックバッファテクスチャーを解放.
	SAFE_RELEASE( pBackBuffer_Tex );//マクロ内にセミコロンがあるのでいらないが見た目の統一でつけている.

	//---------------------------------------------------
	//	バックバッファ準備:デプス(深度)ステンシル関係.
	//---------------------------------------------------

	//デプス(深さor深度)ステンシルビュー用のテクスチャーを作成.
	D3D11_TEXTURE2D_DESC descDepth;

	descDepth.Width				= WND_W;					//幅.
	descDepth.Height			= WND_H;					//高さ.
	descDepth.MipLevels			= 1;						//ミップマップレベル:1.
	descDepth.ArraySize			= 1;						//配列数:1.
	descDepth.Format			= DXGI_FORMAT_D32_FLOAT;	//32ビットフォーマット.
	descDepth.SampleDesc.Count	= 1;						//マルチサンプルの数.
	descDepth.SampleDesc.Quality= 0;						//マルチサンプルのクオリティ.
	descDepth.Usage				= D3D11_USAGE_DEFAULT;		//使用方法:デフォルト.
	descDepth.BindFlags			= D3D11_BIND_DEPTH_STENCIL;	//深度(ステンシルとして使用).
	descDepth.CPUAccessFlags	= 0;						//CPUからアクセスしない.
	descDepth.MiscFlags			= 0;						//その他設定なし.

	m_pDevice->CreateTexture2D(
		&descDepth,
		NULL,
		&m_pBackBuffer_DSTex );//(out)デプスステンシル用テクスチャ.

	//そのテクスチャに対しデプスステンシルビュー(DSV)を作成.
	m_pDevice->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		NULL,
		&m_pBackBuffer_DSTexDSV );//(out)DSV.

	//レンダーターゲットビューとデプスステンシルビューをパイプラインにセット.
	m_pDeviceContext->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV );


//	//深度テスト(Zテスト)を有効にする.
//	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
//	ZeroMemory( &depthStencilDesc,
//		sizeof( D3D11_DEPTH_STENCIL_DESC ) );
//	depthStencilDesc.DepthEnable = true;
//
//	if( SUCCEEDED( m_pDevice->CreateDepthStencilState(
//		&depthStencilDesc, &m_spDepthStencilState ) ) )
//	{
//		m_pDeviceContext->OMSetDepthStencilState(
//			m_spDepthStencilState, 1 );
//	}

	//ビューポートの設定.
	if( !m_spViewPort11 ){
		m_spViewPort11 = new D3D11_VIEWPORT;
	}
	m_spViewPort11->Width		= WND_W;//幅.
	m_spViewPort11->Height		= WND_H;//高さ.
	m_spViewPort11->MinDepth	= 0.0f;	//最小深度(手前).
	m_spViewPort11->MaxDepth	= 1.0f;	//最大深度(奥).
	m_spViewPort11->TopLeftX	= 0.0f;	//左上位置x.
	m_spViewPort11->TopLeftY	= 0.0f;	//左上位置y.
	m_pDeviceContext->RSSetViewports( 1, m_spViewPort11 );

	//Sp2D用.
	SetViewPort10( m_spViewPort11 );

	//ラスタライズ(面の塗りつぶし方)設定.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory( &rdc, sizeof( rdc ) );
	rdc.FillMode = D3D11_FILL_SOLID;	//塗りつぶし(ソリッド).
	rdc.CullMode = D3D11_CULL_BACK;
					//D3D11_CULL_NONE :カリングを切る(正背面を描画する).
					//D3D11_CULL_BACK :背面を描画しない.
					//D3D11_CULL_FRONT:正面を描画しない.
	rdc.FrontCounterClockwise	= FALSE;
								//ポリゴンの表裏を決定するフラグ.
								//TRUE :左回りなら前向き,右回りなら後ろ向き.
								//FALSE:↑の逆になる.
	rdc.DepthClipEnable	= FALSE;	//距離についてのクリッピング有効.

	ID3D11RasterizerState* pIr	= nullptr;
	m_pDevice->CreateRasterizerState( &rdc, &pIr );
	m_pDeviceContext->RSSetState( pIr );
	SAFE_RELEASE( pIr );

	//===== シングルトン =====//.
	clsBLEND_STATE* pBlend = &clsSINGLETON<clsBLEND_STATE>::GetInstance();
	pBlend->Create( m_pDevice, m_pDeviceContext );

	return S_OK;
}

//============================================================
//	Direct3D終了処理.
//============================================================
void clsMain::DestroyD3D()
{

#ifdef _DEBUG
	if( m_pRayH != nullptr ){
		delete m_pRayH;
		m_pRayH = nullptr;
	}
	if( m_pRayFB != nullptr ){
		delete m_pRayFB;
		m_pRayFB = nullptr;
	}
	if( m_pRayV != nullptr ){
		delete m_pRayV;
		m_pRayV = nullptr;
	}
#endif //#ifdef _DEBUG


#ifdef Tahara
	m_upGame.reset( nullptr );
#endif //#ifdef Tahara


#ifdef STARTUP_FULLSCREEN_
	//フルスクリーンならWindowModeへ.
	SetWindowMode();
#endif//#ifdef STARTUP_FULLSCREEN_

	clsSINGLETON_FINALIZER::Finalize();//シングルトンの消去.

	SAFE_DELETE( m_spViewPort10 );
	SAFE_RELEASE( m_pBackBuffer_DSTexDSV );
	SAFE_RELEASE( m_pBackBuffer_DSTex );
	SAFE_RELEASE( m_pBackBuffer_TexRTV );
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pDeviceContext );
	SAFE_RELEASE( m_pDevice );
}

////============================================================
//	メッシュ読み込み関数(まとめた).
////============================================================
HRESULT clsMain::ReadMesh()
{
	m_upGame = make_unique< clsGAME >( 
		m_hWnd, 
		m_pDevice, 
		m_pDeviceContext, 
		m_spViewPort10, 
		m_spViewPort11,
		m_pSwapChain,
		m_pBackBuffer_TexRTV, 
		m_pBackBuffer_DSTexDSV );
//	m_upGame->Create();//起動時に効果音タイミングずれ対策でロード画面終了瞬間に移動する.



#ifdef _DEBUG


	//レイ表示の初期化(垂直).
	m_pRayV = new clsRay;
	m_pRayV->m_Ray.vPoint[0] = D3DXVECTOR3( 0.0f,-5.0f, 0.0f);
	m_pRayV->m_Ray.vPoint[1] = D3DXVECTOR3( 0.0f, 5.0f, 0.0f);
	m_pRayV->Init( m_pDevice, m_pDeviceContext );
	//レイ表示の初期化(前後).
	m_pRayFB = new clsRay;
	m_pRayFB->m_Ray.vPoint[0] = D3DXVECTOR3( 0.0f, 0.0f,-5.0f);
	m_pRayFB->m_Ray.vPoint[1] = D3DXVECTOR3( 0.0f, 0.0f, 5.0f);
	m_pRayFB->Init( m_pDevice, m_pDeviceContext );
	//レイ表示の初期化(左右).
	if( m_pRayH == nullptr ){
		m_pRayH = new clsRay;
		m_pRayH->m_Ray.vPoint[0] = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
		m_pRayH->m_Ray.vPoint[1] = D3DXVECTOR3( 5.0f, 0.0f, 0.0f);
		m_pRayH->Init( m_pDevice, m_pDeviceContext );
	}

#endif //#ifdef _DEBUG

	return S_OK;
}


//ConvDimPosの事前準備.
void clsMain::SetViewPort10( D3D11_VIEWPORT* const Vp )
{
	if( m_spViewPort10 == nullptr ){
		m_spViewPort10 = new D3D10_VIEWPORT;
	}

	m_spViewPort10->TopLeftX	= static_cast<INT>( Vp->TopLeftX );
	m_spViewPort10->TopLeftY	= static_cast<INT>( Vp->TopLeftY );
	m_spViewPort10->MaxDepth	= Vp->MaxDepth;
	m_spViewPort10->MinDepth	= Vp->MinDepth;
	m_spViewPort10->Width		= static_cast<UINT>( Vp->Width );
	m_spViewPort10->Height	= static_cast<UINT>( Vp->Height );
};


#ifdef STARTUP_FULLSCREEN_
HRESULT clsMain::ChangeWindowMode()
{
	if( !m_pSwapChain ) return E_FAIL;

	HRESULT hr = E_FAIL;

	DXGI_SWAP_CHAIN_DESC desc;
	hr = m_pSwapChain->GetDesc( &desc );
	if( FAILED( hr ) ) return E_FAIL;

	BOOL isFullScreen;

	//フルスクリーン情報取得.
	hr = m_pSwapChain->GetFullscreenState( &isFullScreen, NULL );
	if( FAILED( hr ) ) return E_FAIL;

	//フルスクリーンのon, offを逆転する.
	hr = m_pSwapChain->SetFullscreenState( !isFullScreen, NULL );
	if( FAILED( hr ) ) return E_FAIL;

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

//	ChangeWindowModeOptimization( 0, 0 );

	return S_OK;
}

void clsMain::SetWindowMode()
{
	if( !m_pSwapChain ) return;

	HRESULT hr = E_FAIL;

	DXGI_SWAP_CHAIN_DESC desc;
	hr = m_pSwapChain->GetDesc( &desc );
	if( FAILED( hr ) ) return;

	BOOL isFullScreen;

	//フルスクリーン情報取得.
	hr = m_pSwapChain->GetFullscreenState( &isFullScreen, NULL );
	if( FAILED( hr ) ) return;

	//Windowモードなら.
	if( !isFullScreen ){
		return;
	}

	//フルスクリーンのon, offを逆転する.
	hr = m_pSwapChain->SetFullscreenState( !isFullScreen, NULL );
	if( FAILED( hr ) ) return;

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

}


//HRESULT clsMain::ChangeWindowModeOptimization( const UINT Width, const UINT Height )
//{
//	HRESULT hr = E_FAIL;
//
//	if( !m_pSwapChain ) return E_FAIL;
//
//	DXGI_SWAP_CHAIN_DESC desc;
//	hr = m_pSwapChain->GetDesc( &desc );
//	if( FAILED( hr ) ) return E_FAIL;
//
//	//ターゲットビュー解除.
//	m_pDeviceContext->OMSetRenderTargets( 0, NULL, NULL );
//	SAFE_RELEASE( m_pBackBuffer_DSTexDSV );
//	SAFE_RELEASE( m_pBackBuffer_TexRTV );
//
//	//スワップチェーンのバックバッファサイズ、フォーマット、およびバッファー数を変更する.
//	//アプリケーションウィンドウのサイズが変更されたときに呼び出す必要がある.
//	hr = m_pSwapChain->ResizeBuffers( desc.BufferCount, Width, Height, desc.BufferDesc.Format, desc.Flags );
//	if( FAILED( hr ) ) return E_FAIL;
//
//	hr = CreateRenderTargetView( &desc );
//	if( FAILED( hr ) ) return E_FAIL;
//
//
//
//
//	return S_OK;
//}
#endif//#ifdef STARTUP_FULLSCREEN_
