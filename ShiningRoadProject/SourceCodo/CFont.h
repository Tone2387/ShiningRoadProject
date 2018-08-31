#ifndef _C_FONT_H_
#define _C_FONT_H_

#define _CRT_SECURE_NO_WARNINGS

#include "Global.h"


//ｱﾗｲﾝﾒﾝﾄ設定(強制的に16ﾊﾞｲﾄに設定する)
#define ALIGN16 _declspec(align(16))

const int TEXT_H = 256;	//行数
const int TEXT_W = 256;	//行数



class clsFont
{
public:

	//ﾌｫﾝﾄ構造体.
	struct strFont
	{
		int iFontDispSpeed;
		int iFontAutoFlg;
	};

	clsFont( 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext,
		const char *sTextFileName );
	~clsFont();

	void Render( int iTex, int iCharNum );


	D3DXVECTOR3		m_vPos;			//位置
	float			m_fScale;		//拡縮
	float			m_fAlpha;
		


	//
	strFont			m_strFont;

	int				m_iFontH;					//読み込んだ文章が何行あるか.

	
private:

	//構造体
	struct FONTSHADER_CONSTANT_BUFFER
	{
		ALIGN16 D3DXMATRIX mW;			//位置とｶﾒﾗ位置と表示画面設定
		ALIGN16 float ViewPortWidth;	//ｶﾗｰ(RGBAの型に合わせる)
		ALIGN16 float ViewPortHeight;	//UV座標
		ALIGN16 float Alpha;	//透過値
		ALIGN16 D3DXVECTOR2 Uv;	//UV座標
		ALIGN16 D3DXVECTOR4 Color;	//UV座標
	};

	//頂点の構造体
	struct FONT_VERTEX
	{
		D3DXVECTOR3 Pos;		//位置
		D3DXVECTOR2 Tex;		//ﾃｸｽﾁｬ
	};

	//ﾌｫﾝﾄ情報読込.
	bool LoadFont();
	//ｼｪｰﾀﾞ作成.
	HRESULT CreateShader();
	//ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ作成.
	HRESULT CreateVertexBuffer();
	//定数ﾊﾞｯﾌｧ作成.
	HRESULT CreateConstantBuffer();
	void LoadTextFile( const char *FileName );//3行, 文字数.
	HRESULT	CreateTexture();
	void SetBlendSprite(bool alpha_flg);

	DESIGNVECTOR		m_Design;
	D3DXVECTOR4			m_vColor;		//色
	RECT				m_Rect;			//指定幅設定.

	char				m_cTextData[TEXT_H][TEXT_W];	//文章.

	int m_fFontSize;
	int m_fFontMarginX;
	int m_fFontMarginY;


	//テクスチャ関連
	ID3D11Texture2D*		  m_pTex2D[TEXT_H];//2Ｄテクスチャ
	ID3D11ShaderResourceView* m_pAsciiTexture[TEXT_H][TEXT_W]; //ﾃｸｽﾁｬｰ


	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;

	//板ﾎﾟﾘｺﾞﾝ用
	ID3D11VertexShader* m_pVertexShader;	//頂点ｼｪｰﾀﾞｰ
	ID3D11InputLayout*	m_pVertexLayout;	//頂点ﾚｲｱｳﾄ
	ID3D11PixelShader*  m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞｰ
	ID3D11Buffer*		m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ
	ID3D11Buffer*		m_pVertexBuffer;	//頂点バッファ	

	ID3D11BlendState*	m_pBlendState;		//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ
	ID3D11SamplerState* m_pSampleLinear;	//ﾃｸｽﾁｬのｻﾝﾌﾟﾗｰ



};

#endif _C_FONT_H_