#ifndef UI_TEXT_H_
#define UI_TEXT_H_

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//============================================================
//	インクルード.
//============================================================
#include "Common.h"

#include "TextSpriteStruct.h"


//UIとして文字を扱う.
class clsUiText : public clsCommon
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
	//デフォルト引数はRIGHTにすると右端が指定座標に来る.
	void Render( const enPOS enPos = enPOS::LEFT );

	D3DXVECTOR3 GetPos() const				{ return D3DXVECTOR3( m_vPos.x, m_vPos.y, 0.0f ); };
	void SetPos( const D3DXVECTOR2 &vPos )	{ m_vPos = vPos; };
	void AddPos( const D3DXVECTOR2 &vPos )	{ m_vPos += vPos; };

	void SetScale( const float fScale )		{ m_fScale = fScale; };

	void SetText( const char* sText )		{ m_sText = sText; }

	void SetColor( const D3DXVECTOR4 &vColor = { 1.0f, 1.0f, 1.0f, 1.0f } )	{ m_vColor = vColor; }
	void SetAlpha( const float fAlpha )										{ m_fAlpha = fAlpha; };

private:


	//フォントレンダリング関数.
	void RenderFont( const int FontIndex, const float x, const float y, const float z ) const;


private:


	//↓モデルの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;		//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer;	//コンスタントバッファ.

	//↓モデルごとに用意.
	ID3D11Buffer*			m_pVertexBuffer[100];	//頂点バッファ(100個分).

	ID3D11ShaderResourceView*	m_pAsciiTexture;//アスキーテクスチャ.
	ID3D11SamplerState*			m_pSampleLinear;//テクスチャのサンプラー:/テクスチャに各種フィルタをかける.


	DWORD	m_dwWindowWidth;	//ウィンドウ幅.
	DWORD	m_dwWindowHeight;	//ウィンドウ高さ.

	float		m_fKerning[100];	//カーリング(100個分).
	D3DXVECTOR2 m_vPos;
	float		m_fScale;			//拡縮値.
	float		m_fAlpha;			//透過値.
	D3DXVECTOR4	m_vColor;		//色.

	std::string m_sText;//描画文字.

	D3DXMATRIX m_mView;	//ビュー行列.
	D3DXMATRIX m_mProj;	//プロジェクション行列.
	


};
#endif//#ifndef UI_TEXT_H_