#ifndef _C_FONT_H_
#define _C_FONT_H_

#define _CRT_SECURE_NO_WARNINGS

#include "Global.h"


//アラインメント設定(強制的に16バイトに設定する).
#define ALIGN16 _declspec( align ( 16 ) )


//日本語のUI用.
//これに読み込ませる文字列データは全て全角にすること.
class clsFont
{
public:

	clsFont( 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext );
	~clsFont();

	enum class encPOS : UCHAR
	{
		LEFT = 0,
		CENTER,
		RIGHT,
	};


	void Create( const char *sTextFileName );//シーン開始時に使う.
	void Release();							//シーン終了時に使う.

	void Render( const int iTextRow, const int iCharNum = -1 );



	void SetPos( const D3DXVECTOR3 &vPos );
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetPosLast();
	void AddPos( const D3DXVECTOR3 &vPos ){
		SetPos( m_vPos + vPos );
	}

	void SetScale( const float fScale );
	float GetScale();

	void SetColor( const D3DXVECTOR4 &vColor );
	void SetAlpha( const float fAlpha );

	//折り返し位置.
	void SetIndentPos( const float fPosX );
	
	//読み込んだテキストの数( Createしていないと-1が返る ).
	int GetTextRow();
	//テキストの内容.
	std::string GetText( const int iRow );

private:

	//ブレンドステート作成.
	HRESULT CreateBlendState();
	//シェーダ作成.
	HRESULT CreateShader();
	//バーテックスシェーダ作成.
	HRESULT CreateVertexBuffer();
	//定数バッファ作成.
	HRESULT CreateConstantBuffer();
	HRESULT LoadTextFile( const char *FileName );//3行, 文字数.
	HRESULT	CreateTexture();

	void SetBlend( const bool isAlpha );

	//文字を細くする倍率を返す( 問題ないなら1.0f ).
	//第二引数は( 全角で )何文字目か.
	//最後の引数は文字同士の間隔に何を掛けるか.
	float GetFineCharactorRate(
		const int iTextRow, 
		const int iCharNum, 
		float* outfAddLeft );

	//戻り値用列挙体.
	enum class encCHARACTOR_TYPE : UCHAR
	{
		ALPHABET = 0,//半角.
		JAPANESE_HEAD,//日本語先頭バイト.
		JAPANESE_FOOT,//日本語末尾バイト.
	};
	encCHARACTOR_TYPE GetCharactorType( const int iTextRow, const int iCharNum );


private:

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

	D3DXVECTOR3		m_vPos;			//位置.
	float			m_fScale;		//拡縮.
	D3DXVECTOR4		m_vColor;		//色.
	float			m_fAlpha;
		
	//テクスチャサイズ( 解像度 ).
	int		m_iFontSize;
	//文字と文字の隙間.
	float	m_fFontMarginX;
	float	m_fFontMarginY;
	//改行するx座標.
	float	m_fIndentionPosint;
	//最後の文字の座標.
	D3DXVECTOR3	m_vPosLast;

	std::vector< std::string > 		m_vecsTextData;//[ TEXT_H ][ TEXT_W ]	//文章.

	//読み込んだテキストの数( Createしていないと-1が返る ).
	int m_iTextRow;



	DESIGNVECTOR		m_Design;

	//テクスチャ関連.
	std::vector< ID3D11Texture2D* >							m_vecpTex2D;//[ TEXT_H ];//2Ｄテクスチャ.
	std::vector< std::vector< ID3D11ShaderResourceView* > > m_vecvecpAsciiTexture;//[ TEXT_H ][ TEXT_W ]; //テクスチャ.


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