#pragma once

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//============================================================
//	インクルード.
//============================================================
#include	"MyMacro.h"

#include "TextSpriteStruct.h"




class clsUiText
{
public:
	clsUiText();
	~clsUiText();


	HRESULT Create( ID3D11DeviceContext* pContext,
		DWORD dwWidth, DWORD dwHeight,
		float fScale, D3DXVECTOR4 vColor = { 1.0f, 1.0f, 1.0f, 1.0f } );


	//レンダリング関数.
	void Render( const char* text, float x = -999.0f, float y = -999.0f );

	void SetPos( const D3DXVECTOR2 &vPos );
	void AddPos( const D3DXVECTOR2 &vPos );

private:

	//フォントレンダリング関数.
	void RenderFont( int FontIndex, float x, float y, float z );

	//透過(アルファブレンド)設定の切り替え.
	void SetBlend( bool flg );

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

	float		m_fKerning[100];	//カーリング(100個分).
	D3DXVECTOR2 m_vPos;
	float		m_fScale;			//拡縮地(25pixelを基準 25pixel=1.0f).
	float		m_fAlpha;			//透過値.
	D3DXVECTOR4	m_vColor;		//色.

	D3DXMATRIX m_mView;	//ビュー行列.
	D3DXMATRIX m_mProj;	//プロジェクション行列.
	


};
