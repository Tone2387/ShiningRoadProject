#ifndef _C_FONT_H_
#define _C_FONT_H_

#define _CRT_SECURE_NO_WARNINGS

#include "Global.h"


//アラインメント設定(強制的に16バイトに設定する).
#define ALIGN16 _declspec( align ( 16 ) )

//const int TEXT_H = 256;	//行数.
//const int TEXT_W = 256;	//行数.



class clsFont
{
public:

	clsFont( 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext );
	~clsFont();

	void Create( const char *sTextFileName );//シーン開始時に使う.
	void Release();							//シーン終了時に使う.

	void Render( const int iTex, const int iCharNum );



	void SetPos( const D3DXVECTOR3 &vPos );
	D3DXVECTOR3 GetPos();

	void SetScale( const float fScale );
	float GetScale();

	void SetColor( const D3DXVECTOR4 &vColor );
	void SetAlpha( const float fAlpha );

	
private:

	//フォント構造体.
	struct strFont
	{
		int iFontDispSpeed;
		int iFontAutoFlg;
	}m_strFont;


	//構造体.
	struct FONTSHADER_CONSTANT_BUFFER
	{
		ALIGN16 D3DXMATRIX mW;			//位置とカメラ位置と表示画面設定.
		ALIGN16 float ViewPortWidth;	//カラー(RGBAの型に合わせる).
		ALIGN16 float ViewPortHeight;	//UV座標.
		ALIGN16 float Alpha;	//透過値.
		ALIGN16 D3DXVECTOR2 Uv;	//UV座標.
		ALIGN16 D3DXVECTOR4 Color;	//UV座標.
	};

	//頂点の構造体.
	struct FONT_VERTEX
	{
		D3DXVECTOR3 Pos;		//位置.
		D3DXVECTOR2 Tex;		//テクスチャ.
	};

	//ブレンドステート作成.
	HRESULT CreateBlendState();
	//フォント情報読込.
	bool LoadFont();
	//シェーダ作成.
	HRESULT CreateShader();
	//バーテックスシェーダ作成.
	HRESULT CreateVertexBuffer();
	//定数バッファ作成.
	HRESULT CreateConstantBuffer();
	HRESULT LoadTextFile( const char *FileName );//3行, 文字数.
	HRESULT	CreateTexture();

	void SetBlend( const bool isAlpha );

	D3DXVECTOR3		m_vPos;			//位置.
	float			m_fScale;		//拡縮.
	D3DXVECTOR4		m_vColor;		//色.
	float			m_fAlpha;
		

	int				m_iTextRow;//テキストの行数.

	int				m_iFontH;					//読み込んだ文章が何行あるか.

	DESIGNVECTOR		m_Design;
	RECT				m_Rect;			//指定幅設定.

	std::vector< std::string > 		m_sTextData;//[ TEXT_H ][ TEXT_W ]	//文章.

	int m_fFontSize;
	int m_fFontMarginX;
	int m_fFontMarginY;


	//テクスチャ関連.
	std::vector< ID3D11Texture2D* >							m_vpTex2D;//[ TEXT_H ];//2Ｄテクスチャ.
	std::vector< std::vector< ID3D11ShaderResourceView* > > m_vvpAsciiTexture;//[ TEXT_H ][ TEXT_W ]; //テクスチャ.


	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;

	//板ポリゴン用.
	ID3D11VertexShader* m_pVertexShader;	//頂点シェーダー.
	ID3D11InputLayout*	m_pVertexLayout;	//頂点レイアウト.
	ID3D11PixelShader*  m_pPixelShader;		//ピクセルシェーダー.
	ID3D11Buffer*		m_pConstantBuffer;	//コンスタントバッファ.
	ID3D11Buffer*		m_pVertexBuffer;	//頂点バッファ	.

	ID3D11SamplerState* m_pSampleLinear;	//テクスチャのサンプラー.

	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//ブレンドステート.



};

#endif _C_FONT_H_