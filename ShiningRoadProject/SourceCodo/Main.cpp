#include "Main.h"
#include <stdio.h>

//Using宣言.
using namespace std;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//グローバル変数.
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
clsMain* g_pClsMain = nullptr;


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//	定数.
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝








//ライト方向.
const D3DXVECTOR3 vLIGHT_DIR = { 0.0f, 0.01f, 0.02f };

//カメラのより具合.
const float fZOOM = static_cast<float>( D3DX_PI / 4.0 );

//描画限界距離.
const float fRENDER_LIMIT = 150.0f;











//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//	定数終了.
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	g_pClsMain = new clsMain;	//初期化&クラスの宣言.

	//クラスが存在しているかチェック.
	if( g_pClsMain != nullptr ){
		//ウィンドウ作成成功.
		if( SUCCEEDED(
			g_pClsMain->InitWindow(
				hInstance,
				64, 64,
				WND_W, WND_H,
				WND_TITLE ) ) )
		{
			//Dx11用の初期化
			if( SUCCEEDED( g_pClsMain->InitD3D() ) ){
				//メッセージループ.
				g_pClsMain->Loop();
			}
		}
		//終了.
		g_pClsMain->DestroyD3D();//Direct3Dの解放.

		delete g_pClsMain;		//クラスの破棄.
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
	return g_pClsMain->MsgProc( hWnd, uMsg, wParam, lParam );
}


//============================================================
//	メインクラス.
//============================================================

//============================================================
//	コンストラクタ.
//============================================================
clsMain::clsMain() :
	m_hWnd( nullptr ),
	m_pDevice( nullptr ),
	m_pDeviceContext( nullptr ),
	m_pSwapChain( nullptr ),
	m_pBackBuffer_TexRTV( nullptr ),
	m_pBackBuffer_DSTex( nullptr ),
	m_pBackBuffer_DSTexDSV( nullptr ),
	m_pDepthStencilState( nullptr ),
	m_pGame( nullptr )
{

	//ライト方向.
	m_vLight = vLIGHT_DIR;

}

//============================================================
//	デストラクタ.
//============================================================
clsMain::~clsMain()
{

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
	ShowCursor(false);
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
//		UINT uFileNo;
//		g_bFileActive = true;
//		hDrop = (HDROP)wParam;
//		uFileNo = DragQueryFile( (HDROP)wParam, 0xFFFFFFFF, NULL, 0 );
//		char Path[MAX_PATH];
//		for (int i = 0; i < (int)uFileNo; i++){
//			DragQueryFile( hDrop, i, Path, sizeof( Path ) );
//			g_vsFilePath.push_back(Path);
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


//============================================================
//	メッセージループとアプリケーション処理の入り口.
//============================================================
void clsMain::Loop()
{
	//メッシュ読み込み関数をまとめたもの.
	ReadMesh();


	//----------------------------------------------------------
	//	フレームレート.
	//----------------------------------------------------------
	float fRate		= 0.0f;	//レート.
	float fFPS		= 60.0f;//FPS値.
	DWORD sync_old	= timeGetTime();	//過去時間.
	DWORD sync_now;
	//時間処理の為、最小単位を1ミリ秒に変更.
	timeBeginPeriod( 1 );


	//メッセージループ.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof( msg ) );

	while( msg.message != WM_QUIT )
	{
		Sleep( 1 );
		sync_now = timeGetTime();	//現在時間を取得.

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		fRate = 1000.0f / fFPS;	//理想時間を算出.

		if( sync_now - sync_old >= fRate ){
			sync_old = sync_now;	//現在時間に置きかえ.

			//アプリケーション処理はここから飛ぶ.
			AppMain();
		}
	}
	//アプリケーションの終了.
	timeEndPeriod( 1 );	//解除.
}

//============================================================
//	アプリケーションメイン処理.
//============================================================
void clsMain::AppMain()
{
#if _DEBUG

#endif //#if _DEBUG

	//ゲームループ.
	ASSERT_IF_NULL( m_pGame );
	m_pGame->Update();

	//レンダリング.
	Render();
}

//============================================================
//	描画(レンダリング).
//	シーン(場面)を画面にレンダリング(描画).
//	(※DX9MESH内とMain内で2つ存在するので注意).
//============================================================
void clsMain::Render()
{
	//画面のクリア.
	float ClearColor[4] = { 0.5f, 0.25f, 2.0f, 1.0f };//クリア色(RGBA順)(0.0f~1.0f).
	//カラーバックバッファ.
	m_pDeviceContext->ClearRenderTargetView(
		m_pBackBuffer_TexRTV, ClearColor );
	//デプスステンシルビューバックバッファ.
	m_pDeviceContext->ClearDepthStencilView(
		m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );

	//カメラ関数.
	Camera();
	//プロジェクション関数.
	Proj();

	//このRender関数の前のAppMain関数でチェックしているのでアサートは省く.
//	ASSERT_IF_NULL( m_pGame );
	m_pGame->Render( m_mView, m_mProj, m_vLight );
	
	//2D?.
//	SetDepth( false );	//Zテスト:OFF.
//	SetDepth( true );	//Zテスト:ON.


#if _DEBUG
	SetDepth( false );	//Zテスト:OFF.
	//デバッグテキスト.
	RenderDebugText();
	SetDepth( true );	//Zテスト:ON.
#endif//#if _DEBUG

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
	sd.BufferCount		= 1;		//バックバッファの数.
	sd.BufferDesc.Width	= WND_W;
	sd.BufferDesc.Height= WND_H;
	sd.BufferDesc.Format= DXGI_FORMAT_R8G8B8A8_UNORM;
									//フォーマット(32ビットカラー).
	sd.BufferDesc.RefreshRate.Numerator = 60;
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
	if( FAILED(
		D3D11CreateDeviceAndSwapChain(
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
		// D3D_FEATURE_LEVEL_9_1 ～ D3D_FEATURE_LEVEL_10_1.
		if( FAILED(
			D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_WARP,
				NULL, 0, &pFeatureLevels, 1,
				D3D11_SDK_VERSION, &sd,
				&m_pSwapChain, &m_pDevice,
				pFeatureLevel, &m_pDeviceContext ) ) )
		{
			//リファレンスデバイスの作成.
			//	DirectX SDKがインストールされていないと使えない.
			if( FAILED(
				D3D11CreateDeviceAndSwapChain(
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
		__uuidof(ID3D11Texture2D),	//__uuidof:式に関連付けたGUIDを取得.
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


	//深度テスト(Zテスト)を有効にする.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc,
		sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	depthStencilDesc.DepthEnable = true;

	if( SUCCEEDED( m_pDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilState ) ) )
	{
		m_pDeviceContext->OMSetDepthStencilState(
			m_pDepthStencilState, 1 );
	}

	//ビューポートの設定.
	D3D11_VIEWPORT vp;
	vp.Width	= WND_W;//幅.
	vp.Height	= WND_H;//高さ.
	vp.MinDepth	= 0.0f;	//最小深度(手前).
	vp.MaxDepth	= 1.0f;	//最大深度(奥).
	vp.TopLeftX	= 0.0f;	//左上位置x.
	vp.TopLeftY	= 0.0f;	//左上位置y.
	m_pDeviceContext->RSSetViewports( 1, &vp );

	//Sp2D用.
	SetViewPort10( &vp );

	//ラスタライズ(面の塗りつぶし方)設定.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory( &rdc, sizeof( rdc ) );
	rdc.FillMode = D3D11_FILL_SOLID;	//塗りつぶし(ソリッド).
	rdc.CullMode = D3D11_CULL_NONE;
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

	return S_OK;
}

//============================================================
//	Direct3D終了処理.
//============================================================
void clsMain::DestroyD3D()
{
#if _DEBUG
	//デバッグテキスト.
	if( m_pText != nullptr ){
		delete m_pText;
		m_pText = nullptr;
	}

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



#endif //#if _DEBUG

#ifdef Tahara


	SAFE_DELETE( m_pGame );


#endif //#ifdef Tahara

	SAFE_RELEASE( m_pBackBuffer_DSTexDSV );
	SAFE_RELEASE( m_pBackBuffer_DSTex );
	SAFE_RELEASE( m_pBackBuffer_TexRTV );
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pDeviceContext );
	SAFE_RELEASE( m_pDevice );
}



#if _DEBUG

////============================================================
//	スフィア作成.
////============================================================
HRESULT clsMain::InitSphere( clsDX9Mesh* pMesh, float fScale )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;	//頂点バッファ.
	void*	pVertices = nullptr;	//頂点.
	D3DXVECTOR3	vCenter;		//中心.
	float	fRadius;			//半径.

	//頂点バッファを取得.
	if( FAILED( pMesh->m_pMesh->GetVertexBuffer( &pVB ) ) ){
		return E_FAIL;
	}

	//メッシュの頂点バッファをロックする.
	if( FAILED( pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		SAFE_RELEASE( pVB );
		return E_FAIL;
	}

	//メッシュの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		pMesh->m_pMesh->GetNumVertices(),	//頂点の数.
		D3DXGetFVFVertexSize( pMesh->m_pMesh->GetFVF()),	//頂点の情報.
		&vCenter,	//(out)中心座標.
		&fRadius );	//(out)半径.

	//アンロック.
	pVB->Unlock();
	SAFE_RELEASE( pVB );

	//中心と半径を構造体に設定.
	pMesh->m_Sphere.vCenter	= vCenter;
	pMesh->m_Sphere.fRadius	= fRadius * fScale;


	return S_OK;
}

////============================================================
//	バウンディングボックス作成.
////============================================================
HRESULT clsMain::InitBBox( clsDX9Mesh* pMesh )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	VOID* pVertices = nullptr;
	D3DXVECTOR3 Max, Min;

	//メッシュの頂点バッファをロックする.
	if( FAILED( pMesh->m_pMesh->GetVertexBuffer( &pVB ) ) ){
		MessageBox( NULL, "頂点バッファ取得失敗", "clsMain::InitBBox()", MB_OK );
		return E_FAIL;
	}
	if( FAILED( pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		MessageBox( NULL, "頂点バッファのロックに失敗", "clsMain::InitBBox()", MB_OK );
		return E_FAIL;
	}
	//メッシュ内の頂点位置の最大と最小を検索する.
	D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pVertices,
		pMesh->m_pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
			pMesh->m_pMesh->GetFVF() ),
			&Min, &Max );	//(out)最小、最大頂点.
	pVB->Unlock();			//アンロック.
	SAFE_RELEASE( pVB );	//使用済頂点バッファの解放.

	pMesh->m_BBox.vPosMax = Max;
	pMesh->m_BBox.vPosMin = Min;

	//軸ベクトル,軸の長さ(この場合ボックスの各半径)を初期化する.
	pMesh->m_BBox.fLengthX = ( Max.x - Min.x ) / 2.0f;
	pMesh->m_BBox.fLengthY = ( Max.y - Min.y ) / 2.0f;
	pMesh->m_BBox.fLengthZ = ( Max.z - Min.z ) / 2.0f;


	return S_OK;
}
#endif //#if _DEBUG



////============================================================
//	メッシュ読み込み関数(まとめた).
////============================================================
HRESULT clsMain::ReadMesh()
{
	m_pGame = new clsGAME( m_hWnd, m_pDevice, m_pDeviceContext );
	m_pGame->Create();



#if _DEBUG


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
		int r = sizeof( clsRay );
		m_pRayH->m_Ray.vPoint[0] = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
		m_pRayH->m_Ray.vPoint[1] = D3DXVECTOR3( 5.0f, 0.0f, 0.0f);
		m_pRayH->Init( m_pDevice, m_pDeviceContext );
	}

	//デバッグテキストの初期化.
	m_pText = new clsDebugText;
	D3DXVECTOR4 vColor( 1.0f, 1.0f, 1.0f, 1.0f );
	if( FAILED( m_pText->Init(
		m_pDeviceContext,
		WND_W, WND_H, 50.0f,
		vColor ) ) )
	{
		MessageBox( NULL, "デバッグテキスト作成失敗", "clsMain::Loop", MB_OK );
	}
#endif //#if _DEBUG

	return S_OK;
}



//============================================================
//	深度テスト(Zテスト)ON/OFF切替.
//============================================================
void clsMain::SetDepth( const bool bFlg )
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc,
		sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	depthStencilDesc.DepthEnable = bFlg;

	m_pDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilState );
	m_pDeviceContext->OMSetDepthStencilState(
		m_pDepthStencilState, 1 );
}


#if _DEBUG
////============================================================
//	衝突判定関数.
////============================================================
bool clsMain::Collision( clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget)
{
	//2つの物体の中心間の距離を求める.
	D3DXVECTOR3 vLength
		= pTarget->m_Trans.vPos - pAttacker->m_Trans.vPos;
	//長さに変換する.
	float Length = D3DXVec3Length( &vLength );

	//2物体間の距離が、2物体の半径を足したものより.
	//小さいということは、スフィア同士が重なっている.
	//(衝突している)ということ.
	if( Length <=
		pAttacker->m_Sphere.fRadius + pTarget->m_Sphere.fRadius )
	{
		return true;//衝突した.
	}
	return false;	//衝突していない.
}
//============================================================
//	ボックス衝突判定関数.
//============================================================
bool clsMain::BBoxCollision( clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget )
{
	//攻撃側用に2つの頂点を用意し初期化する.
	D3DXVECTOR3 vecMaxAtk, vecMinAtk;	//最大、最小頂点.
	vecMaxAtk = pAttacker->m_BBox.vPosMax
				+ pAttacker->m_Trans.vPos;
	vecMinAtk = pAttacker->m_BBox.vPosMin
				+ pAttacker->m_Trans.vPos;

	//目標側用に2つの頂点を用意し初期化する.
	D3DXVECTOR3 vecMaxTrg, vecMinTrg;	//最大、最小頂点.
	vecMaxTrg = pTarget->m_BBox.vPosMax
				+ pTarget->m_Trans.vPos;
	vecMinTrg = pTarget->m_BBox.vPosMin
				+ pTarget->m_Trans.vPos;

	//2つの物体それぞれの最大、最小位置という,.
	//4つの情報があれば、衝突を検出できる.
	if( vecMinAtk.x < vecMaxTrg.x  &&  vecMaxAtk.x > vecMinTrg.x &&
		vecMinAtk.y < vecMaxTrg.y  &&  vecMaxAtk.y > vecMinTrg.y &&
		vecMinAtk.z < vecMaxTrg.z  &&  vecMaxAtk.z > vecMinTrg.z )
	{
		return true;//衝突している.
	}

	return false;	//衝突していない.
}
#endif //#if _DEBUG


//============================================================
//	ボーンの座標をとる.
//============================================================
void clsMain::GetPosFromBone( clsD3DXSKINMESH* skinMesh, char BoneName[], D3DXVECTOR3& Pos )
{
	D3DXVECTOR3 vBonePos;
	if( skinMesh->GetPosFromBone( BoneName, &vBonePos ) ){
		Pos = vBonePos;
	}
}


//============================================================
//	カメラ関数.
//============================================================
void clsMain::Camera()
{

////----------------------------.
////	カメラ追従処理ここから.
////----------------------------.
////カメラ位置(自機の背中から)の位置.
//m_Camera.vEye = m_Camera.vLook = m_pOldPlayer->m_vPos;
//m_Camera.fYaw = m_pOldPlayer->m_fYaw;
////Y軸回転行列の作成.
//D3DXMatrixRotationY( &m_Camera.mRot, m_Camera.fYaw );
////軸ベクトル.
//D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, 1.0f );
////Z軸ベクトルそのものを回転.
//D3DXVec3TransformCoord( &vecAxisZ, &vecAxisZ, &m_Camera.mRot );
//m_Camera.vEye	-= vecAxisZ * 4.0f;//自機の背中側.
//m_Camera.vLook	+= vecAxisZ * 2.0f;//自機の前側.
//m_Camera.vEye.y		+= 2.0f;
//m_Camera.vLook.y	+= 0.2f;
////----------------------------.
////	カメラ追従処理ここまで.
////----------------------------.


	//ビュー(カメラ)変換.
	D3DXVECTOR3 vUpVec	( 0.0f, 1.0f, 0.0f );	//上方位置.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&m_pGame->GetCameraPos(), &m_pGame->GetCameraLookPos(), &vUpVec );

}
//============================================================
//	プロジェクション関数.
//============================================================
void clsMain::Proj()
{
	//プロジェクション(射影行列)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,			//(out)プロジェクション計算結果.
		fZOOM,	//y方向の視野(ラジアン指定)数字を大きくしたら視野が狭くなる.
		(FLOAT)WND_W / (FLOAT)WND_H,//アスペクト比(幅/高さ).
		0.1f,				//近いビュー平面のz値.
		fRENDER_LIMIT );	//遠いビュー平面のz値.100.f

}














//==========

void clsMain::RenderDebugText()
{
#if _DEBUG

	//NULLチェック.
	if ( m_pText != nullptr ){
		char strDbgTxt[256];
		int dbgtxty = 0;
		int dbgTxtx = 20;



		sprintf_s( strDbgTxt, 
			"CameraPos : x[%f], y[%f], z[%f]",
			m_pGame->GetCameraPos().x, m_pGame->GetCameraPos().y, m_pGame->GetCameraPos().z );
		m_pText->Render(strDbgTxt, 0, dbgtxty);
		dbgtxty += 10;

		sprintf_s( strDbgTxt, 
			"CamLokPos : x[%f], y[%f], z[%f]",
			m_pGame->GetCameraLookPos().x, m_pGame->GetCameraLookPos().y, m_pGame->GetCameraLookPos().z );
		m_pText->Render(strDbgTxt, 0, dbgtxty);
		dbgtxty += 10;

//		sprintf_s( strDbgTxt, 
//			"CharacPos : x[%f], y[%f], z[%f]",
//			m_pTestChara->GetPositionX(), m_pTestChara->GetPositionY(), m_pTestChara->GetPositionZ() );
//		m_pText->Render(strDbgTxt, 0, dbgtxty);
//		dbgtxty += 10;



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

}




#ifdef Tahara
//ConvDimPosの事前準備.
void clsMain::SetViewPort10( D3D11_VIEWPORT* Vp )
{
	m_ViewPort.TopLeftX = static_cast<INT>(Vp->TopLeftX);
	m_ViewPort.TopLeftY = static_cast<INT>(Vp->TopLeftY);
	m_ViewPort.MaxDepth = Vp->MaxDepth;
	m_ViewPort.MinDepth = Vp->MinDepth;
	m_ViewPort.Width	= static_cast<UINT>(Vp->Width);
	m_ViewPort.Height	= static_cast<UINT>(Vp->Height);
};

//3D座標のスクリーン( 2D )座標変換.dimensions(次元) conversion(変換)
D3DXVECTOR3 clsMain::ConvDimPos( D3DXVECTOR3 v2DPos, D3DXVECTOR3 v3DPos )
{
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	D3DXVec3Project( &v2DPos, &v3DPos, &m_ViewPort, &m_mProj, &m_mView, &mWorld );
	return v2DPos;
}
#endif//#ifdef Tahara
