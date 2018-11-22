#ifndef DEBUG_TEXT_H_
#define DEBUG_TEXT_H_

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//============================================================
//	インクルード.
//============================================================
#include "Common.h"
#include "TextSpriteStruct.h"



//============================================================
//	定数.
//============================================================



//テキストクラス.
class clsDebugText : public clsCommon
{
public:
	clsDebugText();	//コンストラクタ.
	~clsDebugText();//デストラクタ.

	HRESULT Init( ID3D11DeviceContext* pContext,
		DWORD dwWidth, DWORD dwHeight,
		float fSize, D3DXVECTOR4 vColor );


	//レンダリング関数.
	void Render( char* text, int x, int y );

	//フォントレンダリング関数.
	void RenderFont( int FontIndex, int x, int y );

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

	float	m_fKerning[100];	//カーリング(100個分).
	float	m_fScale;			//拡縮地(25pixelを基準 25pixel=1.0f).
	float	m_fAlpha;			//透過値.
	D3DXVECTOR4	m_vColor;		//色.

	D3DXMATRIX m_mView;	//ビュー行列.
	D3DXMATRIX m_mProj;	//プロジェクション行列.
	

};


#endif//#define DEBUG_TEXT_H_