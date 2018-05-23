#define _CRT_SECURE_NO_WARNINGS
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )
//============================================================
//	インクルード.
//============================================================
#include <Windows.h>

//メモリリーク検出用.
#include <crtdbg.h>

#include <D3DX11.h>
#include <D3D11.h>

#include <D3DX10.h>//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "Global.h"
#include "MyMacro.h"

#if _DEBUG
#include "DebugText.h"
#include "Ray.h"		//レイ表示クラス.
#endif//#if _DEBUG



#include "Game.h"



//============================================================
//	ライブラリ.
//============================================================
#pragma comment( lib, "winmm.lib" )

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )

#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.



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

	//アプリケーションメイン処理.
	void AppMain();
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

	//デバッグテキストクラス.
	clsDebugText*	m_pText;


	//レイ表示クラス.
	clsRay*			m_pRayV;	//垂直.
	clsRay*			m_pRayFB;	//前後.
	clsRay*			m_pRayH;	//左右.


#endif //#if _DEBUG


	//深度テスト(Zテスト)　ON/OFF切替.
	void SetDepth( const bool bFlg );

	//ボーンの座標をとる(ラップ関数).
	void GetPosFromBone( clsD3DXSKINMESH* skinMesh, char BoneName[], D3DXVECTOR3& Pos );





	//デバック゛テキストの表示.
	void RenderDebugText();




	



	HWND	m_hWnd;	//ウィンドウハンドル.

	//↓アプリに一つ.
	ID3D11Device*			m_pDevice;			//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext;	//デバイスコンテキスト.
	IDXGISwapChain*			m_pSwapChain;		//スワップチェーン.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;//レンダーターゲットビュー.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;//バックバッファ.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;//デプスステンシルビュー.


	//深度(Z)テスト設定.
	ID3D11DepthStencilState* m_pDepthStencilState;



	//ゲーム.
	clsGAME*		m_pGame;






#ifdef Tahara
	//ConvDimPosの事前準備.
	void SetViewPort10( D3D11_VIEWPORT* const Vp );
//	//3D座標をスクリーン( 2D )座標へと変換する.dimensions(次元) conversion(変換)
//	D3DXVECTOR3 ConvDimPos( D3DXVECTOR3 &v2DPos, const D3DXVECTOR3 &v3DPos );
	//2DSp用.
	D3D10_VIEWPORT* m_spViewPort;

#endif//#ifdef Tahara






};