#define _CRT_SECURE_NO_WARNINGS
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

//これがついていると起動時にフルスクリーンになる.
//#define STARTUP_FULLSCREEN_

//============================================================
//	インクルード.
//============================================================
#include "Global.h"

#if _DEBUG
class clsDX9Mesh;
//#include "DebugText.h"
#include "Ray.h"		//レイ表示クラス.

#endif//#if _DEBUG

#include "Game.h"


//メモリリーク検出用.
#include <crtdbg.h>




////============================================================
////	ライブラリ.
////============================================================
//#pragma comment( lib, "winmm.lib" )
//
//#pragma comment( lib, "d3dx11.lib" )
//#pragma comment( lib, "d3d11.lib" )
//
//#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.



//============================================================
//	定数.
//============================================================
#define WND_TITLE	"ShiningRoadProject"
#define APR_NAME	"ShiningRoadProject"





//============================================================
//	メインクラス.
//============================================================
class clsMain
{
public:
	clsMain();	//コンストラクタ.
	~clsMain();	//デストラクタ.

	//ウィンドウ初期化関数.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y, INT width, INT height,
		LPSTR WindowName);

	//ウィンドウ関数(メッセージ毎の処理).
	LRESULT MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam );

	//DirectX初期化.
	HRESULT InitD3D();
	//Direct3D終了処理.
	void DestroyD3D();

	//メッセージループ.
	void Loop();

private:

	//起動中の描画.

	//アプリケーションメイン処理.
	bool AppMain();
	//描画(レンダリング).
	void Render();


	//メッシュ読み込み関数(まとめた).
	HRESULT ReadMesh();

#if _DEBUG
	//スフィア作成.
	HRESULT InitSphere( clsDX9Mesh* pMesh, float fScale = 0.7f );
	//スフィア衝突判定関数.
	bool Collision( clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget );
	//バウンディングボックス作成.
	HRESULT InitBBox( clsDX9Mesh* pMesh );
	//ボックス衝突判定関数.
	bool BBoxCollision( clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget );

	//レイ表示クラス.
	clsRay*			m_pRayV;	//垂直.
	clsRay*			m_pRayFB;	//前後.
	clsRay*			m_pRayH;	//左右.


#endif //#if _DEBUG


	//ゲーム.
	std::unique_ptr< clsGAME >	m_upGame;



	HWND	m_hWnd;	//ウィンドウハンドル.

	//↓アプリに一つ.
	ID3D11Device*			m_pDevice;			//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext;	//デバイスコンテキスト.
	IDXGISwapChain*			m_pSwapChain;		//スワップチェーン.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;//レンダーターゲットビュー.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;//バックバッファ.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;//デプスステンシルビュー.


	//シーンクラスにて使用する関数に必要.
	//ConvDimPosの事前準備.
	void SetViewPort10( D3D11_VIEWPORT* const Vp );
	//2DSp用.
	D3D10_VIEWPORT* m_spViewPort10;
	
	
	//分割用.
	D3D11_VIEWPORT* m_spViewPort11;


#ifdef STARTUP_FULLSCREEN_
	HRESULT ChangeWindowMode();
	void SetWindowMode();
//	//表示モード切替最適化.
//	HRESULT ChangeWindowModeOptimization( const UINT Width, const UINT Height );
#endif//#ifdef STARTUP_FULLSCREEN_

};