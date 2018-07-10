#ifndef DEBUG_TEXT_H_
#define DEBUG_TEXT_H_

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//============================================================
//	インクルード.
//============================================================
#include "MyMacro.h"
#include "TextSpriteStruct.h"



//============================================================
//	定数.
//============================================================
#define DIMENSION		(12.0f)
#define WDIMENSION		(10.0f)
#define TEX_DIMENSION	(128.0f)




//テキストクラス.
class clsDebugText
{
public:
	clsDebugText();	//コンストラクタ.
	~clsDebugText();//デストラクタ.

	HRESULT Init( ID3D11DeviceContext* pContext,
		DWORD dwWidth, DWORD dwHeight,
		float fSize, D3DXVECTOR4 vColor );



	//透過(アルファブレンド)設定の切り替え.
	void SetBlend( bool flg );

	//レンダリング関数.
	void Render( char* text, int x, int y );

	//フォントレンダリング関数.
	void RenderFont( int FontIndex, int x, int y );

private:
	//↓アプリに一つ.
	ID3D11Device*			m_pDevice11;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext11;	//デバイスコンテキスト.

	//↓モデルの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;		//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer;	//コンスタントバッファ.

	//↓モデルごとに用意.
	ID3D11Buffer*			m_pVertexBuffer[100];	//頂点バッファ(100個分).

	ID3D11ShaderResourceView*	m_pAsciiTexture;//アスキーテクスチャ.
	ID3D11SamplerState*			m_pSampleLinear;//テクスチャのサンプラー:/テクスチャに各種フィルタをかける.

	ID3D11BlendState*			m_pBlendState;	//ブレンドステート.


	DWORD	m_dwWindowWidth;	//ウィンドウ幅.
	DWORD	m_dwWindowHeight;	//ウィンドウ高さ.

	float	m_fKerning[100];	//カーリング(100個分).
	float	m_fScale;			//拡縮地(25pixelを基準 25pixel=1.0f).
	float	m_fAlpha;			//透過値.
	D3DXVECTOR4	m_vColor;		//色.

	D3DXMATRIX m_mView;	//ビュー行列.
	D3DXMATRIX m_mProj;	//プロジェクション行列.
	




};


#endif//#define DEBUG_TEXT_H_