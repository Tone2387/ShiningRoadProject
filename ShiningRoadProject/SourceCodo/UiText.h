#pragma once

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//============================================================
//	インクルード.
//============================================================
#include "MyMacro.h"

#include "TextSpriteStruct.h"

#include <string>

//UIとして文字を扱う.
class clsUiText
{
public:
	clsUiText();
	~clsUiText();

	//テキストの左よせ、右寄せ.
	enum class enPOS : UINT
	{
		LEFT,
		RIGHT,
		MIDDLE,
	};

	HRESULT Create( 
		ID3D11DeviceContext* const pContext,
		const DWORD &dwWidth, const DWORD &dwHeight,
		const float fScale );


	//レンダリング関数.
	//デフォルト引数はtrueにすると右端が指定座標に来る.
	void Render( const enPOS enPos = enPOS::LEFT );

	void SetPos( const D3DXVECTOR2 &vPos );
	D3DXVECTOR3 GetPos(){
		D3DXVECTOR3 g;
		g.x = m_vPos.x;
		g.y = m_vPos.y;
		g.z = 0.0f;
		return g;
	};
	void AddPos( const D3DXVECTOR2 &vPos );

	void SetScale( const float fScale );

	void SetText( const char* sText );

	void SetColor( const D3DXVECTOR4 &vColor = { 1.0f, 1.0f, 1.0f, 1.0f } );

private:

	//フォントレンダリング関数.
	void RenderFont( const int FontIndex, const float x, const float y, const float z ) const;

	//透過(アルファブレンド)設定の切り替え.
	void SetBlend( const bool flg );

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
	float		m_fScale;			//拡縮値.
//	float		m_fAlpha;			//透過値.
	D3DXVECTOR4	m_vColor;		//色.

	std::string m_sText;//描画文字.

	D3DXMATRIX m_mView;	//ビュー行列.
	D3DXMATRIX m_mProj;	//プロジェクション行列.
	


};
