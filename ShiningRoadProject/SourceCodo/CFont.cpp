//#define CAN_CHECK_STRING_BYTE_TYPE//文字の英数字、日本語文字の1バイト目、2バイト目の判定ができるならつける.

#include "CFont.h"

#include "BlendState.h"

#include "File.h"
//#include "OperationString.h"




using namespace std;

namespace{

//#define sFONT_NAME "あおぞら明朝 Medium"
#define sFONT_NAME "マキナス Scrap 5"

#define FONT_SHADER		"Shader\\FontText.hlsl"

//アラインメント設定(強制的に16バイトに設定する).
#define ALIGN16 _declspec( align ( 16 ) )

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


	//フォントのパス.
	const char* sFONT_STYLE_PATH = "Data\\Font\\FontType\\Makinas-Scrap-5.otf";//Makinas-Scrap-5.otf.//aozora.ttf.


	const int iFULL_MESSAGE_NUMBER = -1;//デフォルト引数.


	const int iFONT_SIZE = 32;
	const float fFONT_MARGIN_X = 2.0f;//4.0f.
	const float fFONT_MARGIN_Y = 8.0f;//4.0f.

	//Createしていないときの行数.
	const int iERROR_TEXT_ROW_NUM = -1;

}

clsFont::clsFont( 
	ID3D11Device* const pDevice, 
	ID3D11DeviceContext* const pContext )
	:m_pVertexShader( nullptr )
	,m_pVertexLayout( nullptr )
	,m_pPixelShader( nullptr )
	,m_pVertexBuffer( nullptr )
	,m_pSampleLinear( nullptr )
	,m_pConstantBuffer( nullptr )
	,m_vecpTex2D()
	,m_vecvecpAsciiTexture()
	,m_vecsTextData()
	,m_Design()
	,m_fIndentionPosint( static_cast<float>( WND_W ) )
	,m_iFontSize( iFONT_SIZE )
	,m_fFontMarginX( fFONT_MARGIN_X )
	,m_fFontMarginY( fFONT_MARGIN_Y )
	,m_vPos( { 0.0f, 0.0f, 0.0f } )
	,m_fScale( static_cast<float>( m_iFontSize ) )
	,m_fAlpha( 1.0f )
	,m_vColor( { 1.0f, 1.0f, 1.0f, 1.0f } )
	,m_vPosLast( m_vPos )
	,m_iTextRow( iERROR_TEXT_ROW_NUM )
{
	m_wpDevice = pDevice;
	m_wpContext = pContext;

//	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
//		m_pBlendState[i] = nullptr;
//	}

	int iReturn = AddFontResourceEx(
		sFONT_STYLE_PATH,
		FR_PRIVATE,
		&m_Design );

	assert( iReturn );


	if( FAILED( CreateShader() ) ){
		assert( !"Can't Create Shader" );
	}
	if( FAILED( CreateVertexBuffer() ) ){
		assert( !"Can't Create VertexBuffer" );
	}
	if( FAILED( CreateConstantBuffer() ) ){
		assert( !"Can't Create ConstantBuffer" );
	}

}

clsFont::~clsFont()
{
	Release();

	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pPixelShader ) ;
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );


	//リソース削除.
	BOOL Return = RemoveFontResourceEx(
		sFONT_STYLE_PATH,
		FR_PRIVATE,
		&m_Design );

	if( !Return ){
		ERR_MSG( "フォントリソースの破棄に失敗", "" );
	}

}

//シェーダの作成.
HRESULT clsFont::CreateShader()
{
	ID3DBlob *pCompileShader = NULL;
	ID3DBlob *pErrors = NULL;

	char strDir[512];
	GetCurrentDirectory( sizeof( strDir ), strDir );


	//ブロブからバーテックスシェーダー作成.
	if( FAILED( D3DX11CompileFromFile(
		FONT_SHADER, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL,
		&pCompileShader, &pErrors, NULL ) ) )
	{
		assert( "CFontブロブ作成失敗(VS)" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//Vertex Shaderの作成.
	if( FAILED( m_wpDevice->CreateVertexShader(
		(pCompileShader)->GetBufferPointer(),
		(pCompileShader)->GetBufferSize(),
		NULL, &m_pVertexShader ) ) )	//(out)頂点シェーダー.
	{
		assert( "CFontVertex Shader作成失敗(VS)" );
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout_Ita[] =
	{
		{ 
			"POSITION",							//位置.
			0, DXGI_FORMAT_R32G32B32_FLOAT,		//DXGIのフォーマット32bitfloat型.
			0,
			0,									//データの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0 
		},
		{ 
			"TEXCOORD",							//テクスチャ位置.
			0, DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,									//テクスチャデータの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0 
		},
	};
	UINT numElements_Ita = sizeof( layout_Ita ) / sizeof( layout_Ita[0] );

	//頂点インプットレイアウト作成.
	if( FAILED( m_wpDevice->CreateInputLayout( layout_Ita,
		numElements_Ita, pCompileShader->GetBufferPointer(),	//(out)頂点インプットレイアウト.
		pCompileShader->GetBufferSize(), &m_pVertexLayout ) ) )
	{
		assert( "CFont頂点インプットレイアウトの作成失敗" );
		return FALSE;
	}

	//ブロブからピクセルシェーダー作成.
	if( FAILED( D3DX11CompileFromFile(
		FONT_SHADER, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL,
		&pCompileShader, &pErrors, NULL ) ) )
	{
		assert( "CFontブロブ作成失敗(PS)" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//Pixel Shaderの作成.
	if( FAILED( m_wpDevice->CreatePixelShader(
		(pCompileShader)->GetBufferPointer(),
		(pCompileShader)->GetBufferSize(), NULL, &m_pPixelShader ) ) )	//(out)頂点.
	{
		assert( "CFontピクセルシェーダー作成失敗" );
		return E_FAIL;
	}

	SAFE_RELEASE( pCompileShader );	//ブロブ解放.



	return S_OK;
}
//バーテックスバッファの作成.
HRESULT clsFont::CreateVertexBuffer()
{
	// Create VB.
	//バーテックスバッファー作成.
	FONT_VERTEX vertices[4];

	FONT_VERTEX vertices2[] ={
		//頂点座標（ｘ、ｙ、ｚ）　　　　UV座標（u,v）.
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f ), D3DXVECTOR2( 0.0f, 1.0f ),	//頂点１.
		D3DXVECTOR3( 0.0f,-1.0f, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ),	//頂点２.
		D3DXVECTOR3( 1.0f, 1.0f, 0.0f ), D3DXVECTOR2( 1.0f, 1.0f ),	//頂点３.
		D3DXVECTOR3( 1.0f,-1.0f, 0.0f ), D3DXVECTOR2( 1.0f, 0.0f ),	//頂点４.
	};
	vertices[0] = vertices2[0];
	vertices[1] = vertices2[1];
	vertices[2] = vertices2[2];
	vertices[3] = vertices2[3];


	D3D11_BUFFER_DESC bd;					//バッファの構造体.
	bd.Usage = D3D11_USAGE_DEFAULT;			//使用方法.
	bd.ByteWidth = sizeof( FONT_VERTEX ) * 4;//頂点のサイズ（頂点*３）.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//頂点バッファとして扱う.
	bd.CPUAccessFlags = 0;					//CPUからはアクセスしない.
	bd.MiscFlags = 0;						//その他の設定なし.

	//サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitDate;
	InitDate.pSysMem = vertices;		//三角形の頂点をリセット.
	//頂点バッファの作成.
	if( FAILED( m_wpDevice->CreateBuffer( &bd, &InitDate, &m_pVertexBuffer ) ) ){
		ERR_MSG("頂点バッファ(m_pItaVB)の作成に失敗", "CFont");
		return E_FAIL;
	}

	//頂点バッファをセット.
	UINT stride = sizeof(FONT_VERTEX);	//頂点間のサイズ.
	UINT offset = 0;					//オフセット値.
	m_wpContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	//テクスチャ用サンプラー作成.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory( &SamDesc, sizeof( D3D11_SAMPLER_DESC ) );
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//リニアフィルター（線形補間）.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//ラッピングモード（WRAP：繰り返し).
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_wpDevice->CreateSamplerState( &SamDesc, &m_pSampleLinear );	//（out)サンプラー.

	return S_OK;
}
//定数バッファの作成.
HRESULT clsFont::CreateConstantBuffer()
{
	//コンスタントバッファ作成(板ポリ用).
	D3D11_BUFFER_DESC ItaBD;
	ItaBD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//コンスタンストバッファを指定.
	ItaBD.ByteWidth = sizeof( FONTSHADER_CONSTANT_BUFFER );	//コンスタンストバッファのサイズ.
	ItaBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			//書き込みでアクセス.
	ItaBD.MiscFlags = 0;
	ItaBD.StructureByteStride = 0;
	ItaBD.Usage = D3D11_USAGE_DYNAMIC;					//使用法：直接書き込み.

	//コンスタンストバッファ作成.
	if( FAILED( m_wpDevice->CreateBuffer(
		&ItaBD, NULL, &m_pConstantBuffer ) ) )
	{
		ERR_MSG("コンスタントバッファ(Ita)の作成に失敗", "CFont");
		return E_FAIL;
	}


	return S_OK;
}




void clsFont::Create( const char *sTextFileName )
{
	//既に作成されたものがあるなら破棄してから作る.
	if( m_iTextRow != iERROR_TEXT_ROW_NUM ){
		Release();
		assert( !"中身があるclsFontを初期化しようとしたな?\n\
				 まあ消しといてあげたけど" );
	}

	if( FAILED( LoadTextFile( sTextFileName ) ) ){
		assert( !"Can't Load Text File" );
	}

	if( FAILED( CreateTexture( sTextFileName ) ) ){
		assert( !"Can't Create Texture" );
	}

	SetPos( { 0.0f, 0.0f, 0.0f } );
	SetScale( 0.0f );
	SetAlpha( 1.0f );
	SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } );
}
//ファイル読み込み.
HRESULT clsFont::LoadTextFile( const char *sTextFileName )
{
	//ファイルポインタ.
	clsFILE File;

	if( !File.Open( sTextFileName ) ){
		return E_FAIL;
	}

	//初期化.
	//行数.
	m_iTextRow = static_cast<int>( File.GetSizeRow() );
	m_vecsTextData.resize( m_iTextRow );
	m_vecpTex2D.resize( m_iTextRow, nullptr );
	m_vecvecpAsciiTexture.resize( m_iTextRow );

	for( unsigned int i=0; i<File.GetSizeRow(); i++ )
	{
		const int iCol = File.GetDataString( i, 0 ).size();//文字列の長さ.
		//一行ずつコピー.
		m_vecsTextData[i] = File.GetDataString( i, 0 );
#ifndef CAN_CHECK_STRING_BYTE_TYPE
		m_vecvecpAsciiTexture[i].resize( iCol, nullptr );
#else//#ifndef CAN_CHECK_STRING_BYTE_TYPE
		m_vecvecpAsciiTexture[i].reserve( m_vecsTextData[i].size() );

		const int iNullPlus = 1;//ヌル文字の分.
		for( unsigned int j=0; j<m_vecsTextData[i].size()-iNullPlus; j++ )
		{
			if( IsDBCSLeadByte( m_vecsTextData[i][j] ) ){
				m_vecvecpAsciiTexture[i].push_back( nullptr );
			}
		}
		m_vecvecpAsciiTexture[i].shrink_to_fit();
#endif//#ifndef CAN_CHECK_STRING_BYTE_TYPE
	}

//	if( IsDBCSLeadByte( m_vecsTextData[iTex][i] ) )


	//ファイルのクローズ.
	File.Close();

	return S_OK;
}
//テクスチャ作成.
HRESULT clsFont::CreateTexture( const char* sErrFilePath )
{
	//fontCreate.
	LOGFONT lf = {
		m_iFontSize, 0, 0, 0, 
		0, 0, 0, 0,
		//DEFAULT_CHARSET,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FIXED_PITCH | FF_ROMAN,
		TEXT( sFONT_NAME )
	};
	

	//行数分繰り返し.
	for( unsigned int iTex=0; iTex<m_vecsTextData.size(); iTex++ )
	{
		HFONT hFont = CreateFontIndirect( &lf );
		if( !hFont ){
			ERR_MSG( "フォント作成不可", sErrFilePath );
			return E_FAIL;
		}

		HDC hdc = GetDC( NULL );

		HFONT oldFont = (HFONT)SelectObject( hdc, hFont );

		int iByteMax = strlen( m_vecsTextData[ iTex ].c_str() );
		
//		for( int i=0; i<iByteMax; i++ )
//		{
//			if( IsDBCSLeadByte( m_vecsTextData[ iTex ][i] ) ){
//				i++;
//			}
//		}

		UINT code;
		int iCharCnt = 0;//文字列カウント.
		for( int i=0; i<iByteMax; i++ ){
			//文字コード取得.
			if( IsDBCSLeadByte( m_vecsTextData[ iTex ][i] ) ){
				code = (BYTE)m_vecsTextData[ iTex ][i] << 8 | (BYTE)m_vecsTextData[ iTex ][ i + 1 ];
			}
			else{
				code = m_vecsTextData[ iTex ][i];
			}
			//フォントビットマップ取得.
			TEXTMETRIC TM;
			GetTextMetrics( hdc, &TM );
			GLYPHMETRICS GM;
			const int grad = GGO_GRAY4_BITMAP;

			CONST MAT2 Mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
			DWORD size = GetGlyphOutline( hdc, code, grad, &GM, 0, NULL, &Mat );
			BYTE* ptr = new BYTE[ size ];
			GetGlyphOutline( hdc, code, grad, &GM, size, ptr, &Mat );

			//----- 書き込み可能テクスチャ作成 -----//.

			//CPUで書き込みができるテクスチャ作成.
			//テクスチャ作成.
			D3D11_TEXTURE2D_DESC desc;
			memset( &desc, 0, sizeof( desc ) );
			desc.Width = GM.gmCellIncX;
	//		desc.Width = ( GM.gmBlackBoxX + 3 ) / 4 * 4;
			desc.Height = TM.tmHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

//			if( m_vecsTextData[ iTex ][i] ){
//				GM.
//			}
			

			if( FAILED( m_wpDevice->CreateTexture2D( &desc, 0, &m_vecpTex2D[ iTex ] ) ) ){
				MessageBox( 0, "テクスチャ作成失敗", sErrFilePath, MB_OK );
				return E_FAIL;
			}

			D3D11_MAPPED_SUBRESOURCE hMappedResource;
			if( FAILED( m_wpContext->Map(
				m_vecpTex2D[ iTex ],
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&hMappedResource ) ) )
			{
				MessageBox(NULL, "テクスチャ作成失敗",
					sErrFilePath, MB_OK);
				return E_FAIL;
			}
			// ここで書き込む.
			BYTE* pBits = (BYTE*)hMappedResource.pData;

			// フォント情報の書き込み.
			// iOfs_x, iOfs_y : 書き出し位置(左上).
			// iBmp_w, iBmp_h : フォントビットマップの幅高.
			// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階).
			int iOfs_x = GM.gmptGlyphOrigin.x;
			int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
			int iBmp_w = GM.gmBlackBoxX + ( 4 - ( GM.gmBlackBoxX % 4 ) ) % 4;
			int iBmp_h = GM.gmBlackBoxY;
			int Level = 17;
			int x, y;

			DWORD Alpha, Color;
			memset( pBits, 0, hMappedResource.RowPitch * TM.tmHeight );
			for( y=iOfs_y; y<iOfs_y+iBmp_h; y++ )
			{
				for( x=iOfs_x; x<iOfs_x+iBmp_w; x++ )
				{
					Alpha = ( 255 * ptr[ x - iOfs_x + iBmp_w * ( y - iOfs_y ) ] ) / ( Level - 1 );
					Color = 0x00ffffff | ( Alpha << 24 );

					memcpy( (BYTE*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof( DWORD ) );
				}
			}

			m_wpContext->Unmap( m_vecpTex2D[ iTex ], 0 );

			//テクスチャ情報を取得する.
			D3D11_TEXTURE2D_DESC texDesc;
			m_vecpTex2D[ iTex ]->GetDesc( &texDesc );

			//ShaderResourceViewの情報を作成する.
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory( &srvDesc, sizeof( srvDesc ) );
			srvDesc.Format = texDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

			if( FAILED( m_wpDevice->CreateShaderResourceView(
				m_vecpTex2D[ iTex ], &srvDesc, &m_vecvecpAsciiTexture[ iTex ][ iCharCnt ] ) ) )
			{
				ERR_MSG( "テクスチャ作成失敗", sErrFilePath );
				return E_FAIL;
			}

			//文字コード取得.
			if( IsDBCSLeadByte( m_vecsTextData[ iTex ][i] ) ){
				i++;
				iCharCnt++;//文字.//先行バイトの判断がうまくいかないから.
			}
			iCharCnt++;//文字.
			delete[] ptr;
			ptr = nullptr;
		}
		//デバイスコンテキストとフォントハンドルの解放.
		SelectObject( hdc, oldFont );
		DeleteObject( hFont );
		ReleaseDC( NULL, hdc );
	}
	return S_OK;
}


void clsFont::Release()
{
	m_vecsTextData.clear();
	m_vecsTextData.shrink_to_fit();

	for( unsigned int iTex=0; iTex<m_vecvecpAsciiTexture.size(); iTex++ )
	{
		for( unsigned int i=0; i<m_vecvecpAsciiTexture[ iTex ].size(); i++ )
		{
			if( !m_vecvecpAsciiTexture[ iTex ][i] ) continue;
			m_vecvecpAsciiTexture[ iTex ][i]->Release();
			m_vecvecpAsciiTexture[ iTex ][i] = nullptr;
		}
		m_vecvecpAsciiTexture[ iTex ].clear();
		m_vecvecpAsciiTexture[ iTex ].shrink_to_fit();
	}
	m_vecvecpAsciiTexture.clear();
	m_vecvecpAsciiTexture.shrink_to_fit();

	for( unsigned int iTex=0; iTex<m_vecpTex2D.size(); iTex++ ){
		if( !m_vecpTex2D[ iTex ] ) continue;
		m_vecpTex2D[ iTex ]->Release();
		m_vecpTex2D[ iTex ] = nullptr;
	}
	m_vecpTex2D.clear();
	m_vecpTex2D.shrink_to_fit();

	m_iTextRow = iERROR_TEXT_ROW_NUM;
}
//							↓段( 何行目? )	　		↓文字数.
void clsFont::Render( const int iTextRow, const int iCharNum )
{
	if( iTextRow <= -1 ) return;
	if( iTextRow >= static_cast<int>( m_vecvecpAsciiTexture.size() ) ) return;


	//使用するシェーダーの登録.
	m_wpContext->VSSetShader( m_pVertexShader,	NULL, 0 );
	m_wpContext->PSSetShader( m_pPixelShader,	NULL, 0 );
	m_wpContext->GSSetShader( NULL,				NULL, 0 );

	//文字列の左上を座標の位置に持ってくるために必要.
	const D3DXVECTOR3 vOFFSET_POS = { -m_fScale, m_fScale * 0.5f, 0.0f };
	D3DXVECTOR3 vPos = m_vPos + vOFFSET_POS;

	int iRow = 0;//行.
	int iCnt = 0;//その行の何文字目か.
	float fAddLeft = 0.0f;//細い文字を入れた際の左に寄せる量.

	const int iNUM_MAX = static_cast<int>( m_vecvecpAsciiTexture[ iTextRow ].size() );
	for( int i=0; i<iNUM_MAX; i++ )
	{
		//デフォルト引数.
		if( iCharNum == iFULL_MESSAGE_NUMBER ){}
		//ではない場合の判定.
		else if( i >= iCharNum ){
			break;
		}
#if 1
		if( vPos.x + m_iFontSize + ( m_fScale + m_fFontMarginX ) * iCnt + ( m_fScale + m_fFontMarginX ) * fAddLeft <= 
			m_fIndentionPosint )
		{
			//指定範囲の中.
			iCnt++;
		}
		else{
			//範囲指定外、一段ずらす.
			iRow++;
			iCnt = 1;
			fAddLeft = 0.0f;
		}
#else
		//勝手に改行しない.
		iCnt++;
#endif
		//ワールド変換.
		D3DXMATRIX mWorld, mScale, mTran;		//ワールド行列.
		D3DXMatrixIdentity( &mWorld );
		D3DXMatrixTranslation( &mTran, 
			vPos.x + ( m_fScale + m_fFontMarginX ) * static_cast<float>( iCnt ) + ( m_fScale + m_fFontMarginX ) * fAddLeft,
			vPos.y + ( m_fScale + m_fFontMarginY ) * static_cast<float>( iRow ),
			vPos.z );


		float fScaleW = 1.0f;
			//文字の大きさや詰めを定める.iTextRow, const int iCharNum
		float fTmpAddLeft = 0.0f;
		fScaleW = GetFineCharactorRate( iTextRow, i, &fTmpAddLeft );
		fAddLeft -= fTmpAddLeft;


		D3DXMatrixScaling( &mScale, m_fScale * fScaleW, m_fScale, 1.0f );
		mWorld = mScale * mTran;

		//シェーダーのコンスタントバッファに各種データを渡す.
		D3D11_MAPPED_SUBRESOURCE	pData;
		FONTSHADER_CONSTANT_BUFFER	cb;
		if( SUCCEEDED( m_wpContext->Map(
			m_pConstantBuffer, 0,
			D3D11_MAP_WRITE_DISCARD,
			0, &pData ) ) )
		{
			//ワールド行列を渡す.
			cb.mW = mWorld;
			D3DXMatrixTranspose( &cb.mW, &cb.mW );
			cb.ViewPortWidth = WND_W;
			cb.ViewPortHeight = WND_H;


			cb.Uv = { 0.0f, 0.0f };

			cb.Color = m_vColor;
			//透明度を渡す.
			cb.Alpha = m_fAlpha;

			memcpy_s( pData.pData, pData.RowPitch, (void*)( &cb ), sizeof( cb ) );
			m_wpContext->Unmap( m_pConstantBuffer, 0 );
		}

		//最後の文字の位置を取得.
		m_vPosLast = D3DXVECTOR3(
			vPos.x + ( m_fScale + m_fFontMarginX ) * static_cast<float>( iCnt )	  + static_cast<float>( m_iFontSize * 2 ),
			vPos.y + ( m_fScale + m_fFontMarginY ) * static_cast<float>( iRow * 2 ) - static_cast<float>( m_iFontSize ),
			0.0f );

		//このコンスタントバッファを使うシェーダーの登録.
		m_wpContext->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
		m_wpContext->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );

		//バーテックスバッファをセット.
		UINT stride = sizeof( FONT_VERTEX );
		UINT offset = 0;
		m_wpContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

		//トポロジー.
		m_wpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
		//頂点インプットレイアウトをセット.
		m_wpContext->IASetInputLayout(m_pVertexLayout);

		//テクスチャをシェーダーに渡す.
		m_wpContext->PSSetSamplers( 0, 1, &m_pSampleLinear );
		m_wpContext->PSSetShaderResources( 0, 1, &m_vecvecpAsciiTexture[ iTextRow ][i] );

		//アルファブレンド用ブレンドステート作成.
		m_psinBlend->SetBlend( true );

		//描画.
		m_wpContext->Draw( 4, 0 );
	}
}
	 
void clsFont::SetColor( const D3DXVECTOR4 &vColor )
{
	const float fNOT_ALPHA = 1.0f;
	m_vColor = vColor;
	m_vColor.w = fNOT_ALPHA;
}


//テキストの内容.
std::string clsFont::GetText( const int iRow ) const
{
	assert( static_cast<unsigned int>( iRow ) < m_vecsTextData.size() );

	return m_vecsTextData[ iRow ];
}

//文字を細くする倍率を返す( 問題ないなら1.0f ).
//第二引数は( 全角で )何文字目か.
//最後の引数は文字同士の間隔に何を掛けるか.
float clsFont::GetFineCharactorRate( 
	const int iTextRow, 
	const int iCharNum, 
	float* outfAddLeft ) const
{
	const float fDEFAULT_RATE	= 1.0f;
	const float fSLENDER_RATE	= 0.5f;
	const float fSLENDER_RATE_2 = 0.75f;
	const float fSLENDER_RATE_3 = 0.875f;

	float fReturn = fDEFAULT_RATE;
	*outfAddLeft = 0.0f;

	//何の文字か判定( 半角か日本語か ).
	encCHARACTOR_TYPE encCharaType = GetCharactorType( iTextRow, iCharNum );

	//日本語の先頭バイト.
	if( encCharaType == encCHARACTOR_TYPE::JAPANESE_HEAD ){
		fReturn = fDEFAULT_RATE;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		return fReturn;
	}
	//日本語の末尾バイト.
	else if( encCharaType == encCHARACTOR_TYPE::JAPANESE_FOOT ){
		fReturn = 0.0f;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		return fReturn;
	}

	switch( m_vecsTextData[ iTextRow ][ iCharNum ] )
	{
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'g':
	case 'h':
	case 'k':
	case 'n':
	case 'o':
	case 's':
		fReturn = fSLENDER_RATE_3;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		break;

	case 'f':
	case 'r':
	case 't':
	case 'v':
	case 'z':
		fReturn = fSLENDER_RATE_2;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		break;

	case 'i':
	case 'j':
	case 'l':
	case 'I':
	case '.':
	case ':':
	case '/':
	case '(':
	case ')':
	case '1':
		fReturn = fSLENDER_RATE;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		break;

	default:
		fReturn = fDEFAULT_RATE;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		break;
	}

	return fReturn;
}

clsFont::encCHARACTOR_TYPE clsFont::GetCharactorType( 
	const int iTextRow, const int iCharNum ) const
{
	//次がないならそれは文末すなわち、とりあえず半角を返しておけばよい( 仮に日本語の末尾だったとしても見えないから影響しない ).
	if( iCharNum + 1 >= static_cast<int>( m_vecsTextData[ iTextRow ].size() ) ){
		return encCHARACTOR_TYPE::ALPHABET;
	}
	//マイナスに行こうとすれば許さない( それは絶対半角 ).
	else if( iCharNum - 1 < 0 ){
		return encCHARACTOR_TYPE::ALPHABET;
	}

	//今先頭バイト?.
	if(  IsDBCSLeadByte( m_vecsTextData[ iTextRow ][ iCharNum ] ) ){
		//そいつのテクスチャがnullなら.
		if( !m_vecvecpAsciiTexture[ iTextRow ][ iCharNum ] ){
			//日本語末尾バイト.
			return encCHARACTOR_TYPE::JAPANESE_FOOT;
		}
		//日本語先頭バイト.
		return encCHARACTOR_TYPE::JAPANESE_HEAD;
	}
	//1バイト前が先頭文字なら.
	else if( IsDBCSLeadByte( m_vecsTextData[ iTextRow ][ iCharNum - 1 ] ) ){
		//日本語末尾バイト.
		return encCHARACTOR_TYPE::JAPANESE_FOOT;
	}

	//半角文字です.
	return encCHARACTOR_TYPE::ALPHABET;
}
