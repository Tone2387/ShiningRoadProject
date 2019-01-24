#ifndef C_FONT_H_
#define C_FONT_H_

#define _CRT_SECURE_NO_WARNINGS

#include "Common.h"



//日本語のUI用.
//これに読み込ませる文字列データは全て全角にすること.
class clsFont : public clsCommon
{
public:

	clsFont( 
		const HWND hWnd,
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext );
	~clsFont();

	enum class encPOS : UCHAR
	{
		LEFT = 0,
		CENTER,
		RIGHT,
	};

	enum enTEXT_SCENE : unsigned int
	{
		enTEXT_SCENE_TITLE = 0,
		enTEXT_SCENE_ASSEMBLE,
		enTEXT_SCENE_MISSION,
		enTEXT_SCENE_ENDING,
		enTEXT_SCENE_GAMEOVER,

		enTEXT_SCENE_size
	};

//	void Create( const char *sTextFileName );//シーン開始時に使う.
//	void Release();							//シーン終了時に使う.
//	void Release();							//シーン終了時に使う.
	void SetScene( const enTEXT_SCENE enTextScene ){
		if( enTextScene >= enTEXT_SCENE_size ){ m_uiTextRowIndex = 0; }
		else { m_uiTextRowIndex = enTextScene; }
	}

	void Render( const int iTextRow, const int iCharNum = -1 );



	D3DXVECTOR3 GetPos()	const			{ return m_vPos; }
	D3DXVECTOR3 GetPosLast()const			{ return m_vPosLast; }
	void SetPos( const D3DXVECTOR3 &vPos )	{ m_vPos = vPos; };
	void AddPos( const D3DXVECTOR3 &vPos )	{ SetPos( m_vPos + vPos ); }

	float GetScale() const				{ return m_fScale; }
	void SetScale( const float fScale )	{ m_fScale = fScale; };

	void SetColor( const D3DXVECTOR4 &vColor );
	void SetAlpha( const float fAlpha )	{ m_fAlpha = fAlpha; }

	//折り返し位置.
	void SetIndentPos( const float fPosX ){ m_fIndentionPosint = fPosX; }
	
	//読み込んだテキストの数( Createしていないと-1が返る ).
	int GetTextRow() const { 
		if( m_vecupText.size() <= m_uiTextRowIndex ){ return -1; }
		if( !m_vecupText[ m_uiTextRowIndex ] ){ return -1; }
		return m_vecupText[ m_uiTextRowIndex ]->iTextRow; 
	}
	//テキストの内容.
	std::string GetText( const int iRow ) const;

private:

	//シェーダ作成.
	HRESULT CreateShader();
	//バーテックスシェーダ作成.
	HRESULT CreateVertexBuffer();
	//定数バッファ作成.
	HRESULT CreateConstantBuffer();


	void CreateTextSprite();//フォントスプライト作成.
	HRESULT LoadTextFile( const unsigned int uiINDEX, const char *FileName );//3行, 文字数.
	HRESULT	CreateTexture( const unsigned int uiINDEX, const char* sErrFilePath );


	//文字を細くする倍率を返す( 問題ないなら1.0f ).
	//第二引数は( 全角で )何文字目か.
	//最後の引数は文字同士の間隔に何を掛けるか.
	float GetFineCharactorRate(
		const int iTextRow, 
		const int iCharNum, 
		float* outfAddLeft ) const;

	//戻り値用列挙体.
	enum class encCHARACTOR_TYPE : UCHAR
	{
		ALPHABET = 0, //半角.
		JAPANESE_HEAD,//日本語先頭バイト.
		JAPANESE_FOOT,//日本語末尾バイト.
	};
	encCHARACTOR_TYPE GetCharactorType( 
		const int iTextRow, const int iCharNum ) const;


private:

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





	DESIGNVECTOR		m_Design;



	//板ポリゴン用.
	ID3D11VertexShader* m_pVertexShader;	//頂点シェーダー.
	ID3D11InputLayout*	m_pVertexLayout;	//頂点レイアウト.
	ID3D11PixelShader*  m_pPixelShader;		//ピクセルシェーダー.
	ID3D11Buffer*		m_pConstantBuffer;	//コンスタントバッファ.
	ID3D11Buffer*		m_pVertexBuffer;	//頂点バッファ	.

	ID3D11SamplerState* m_pSampleLinear;	//テクスチャのサンプラー.

	HWND m_hWnd;

	//描画に使う.
	struct TEXT_SPRITE
	{
		//読み込んだテキストの行数( Createしていないと-1が返る ).
		int iTextRow;
		std::vector< std::string > 		vecsTextData;//[ TEXT_H ][ TEXT_W ]	//文章.
		//テクスチャ関連.
		std::vector< ID3D11Texture2D* >							vecpTex2D;//2Ｄテクスチャのリソース.
		std::vector< std::vector< ID3D11ShaderResourceView* > > vecvecpAsciiTexture;//文字列テクスチャ.
	
		
		TEXT_SPRITE() : iTextRow( -1 ){}
		~TEXT_SPRITE()
		{
			vecsTextData.clear();
			vecsTextData.shrink_to_fit();

			for( unsigned int iTex=0; iTex<vecvecpAsciiTexture.size(); iTex++ )
			{
				for( unsigned int i=0; i<vecvecpAsciiTexture[ iTex ].size(); i++ )
				{
					if( !vecvecpAsciiTexture[ iTex ][i] ) continue;
					vecvecpAsciiTexture[ iTex ][i]->Release();
					vecvecpAsciiTexture[ iTex ][i] = nullptr;
				}
				vecvecpAsciiTexture[ iTex ].clear();
				vecvecpAsciiTexture[ iTex ].shrink_to_fit();
			}
			vecvecpAsciiTexture.clear();
			vecvecpAsciiTexture.shrink_to_fit();

			DeleteTex2D();

			iTextRow = -1;
		}
		void DeleteTex2D()
		{
			for( unsigned int iTex=0; iTex<vecpTex2D.size(); iTex++ ){
				if( !vecpTex2D[ iTex ] ) continue;
				vecpTex2D[ iTex ]->Release();
				vecpTex2D[ iTex ] = nullptr;
			}
			vecpTex2D.clear();
			vecpTex2D.shrink_to_fit();
		}
	};
	//シーンの数だけ作成する.
	std::vector< std::unique_ptr< TEXT_SPRITE > >	m_vecupText;
	//上記のvectorの添え字.
	unsigned int m_uiTextRowIndex;

};

#endif C_FONT_H_