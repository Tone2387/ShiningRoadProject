//#define CAN_CHECK_STRING_BYTE_TYPE//文字の英数字、日本語文字の1バイト目、2バイト目の判定ができるならつける.

#include "CFont.h"

#include "File.h"

#include <assert.h>



using namespace std;

const char cFontStyle[] = "Data\\Font\\Makinas-Scrap-5.otf";//Makinas-Scrap-5.otf.//aozora.ttf.

//#define sFONT_NAME "あおぞら明朝 Medium"
#define sFONT_NAME "マキナス Scrap 5"

#define FONT_SHADER		"Shader\\FontText.hlsl"


clsFont::clsFont( 
	ID3D11Device* const pDevice, 
	ID3D11DeviceContext* const pContext,
	const char *sTextFileName )
	:m_pDevice( pDevice )
	,m_pContext( pContext )
	,m_pVertexShader( nullptr )
	,m_pVertexLayout( nullptr )
	,m_pPixelShader( nullptr )
	,m_pVertexBuffer( nullptr )
	,m_pSampleLinear( nullptr )
	,m_pConstantBuffer( nullptr )
	,m_vpTex2D()
	,m_vvpAsciiTexture()
	,m_pBlendState( nullptr )
	,m_sTextData()
	,m_fAlpha( 1.0f )
	,m_vColor( { 1.0f, 1.0f, 1.0f, 1.0f } )
	,m_Design()
	,m_iTextRow( 0 )
{
//	for (int iTex = 0; iTex<TEXT_H; iTex++){
//		m_pTex2D[iTex] = nullptr;
//	}
//	for (int iTex = 0; iTex<TEXT_H; iTex++){
//		for (int i = 0; i < TEXT_W; i++){
//			m_pAsciiTexture[iTex][i] = nullptr;
//		}
//	}

	if( !AddFontResourceEx(
		cFontStyle,
		FR_PRIVATE,
		&m_Design ) )
	{
		ERR_MSG("フォント失敗", "Error");
	}

	////ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄをｺﾋﾟｰ
	//m_pContext->GetDevice(&m_pDevice);

	m_Rect = {  WND_W / 8,/*left*/
				WND_H / 2,/*top*/
				WND_W / 3 + WND_W / 2,/*right*/
				WND_H / 4 + WND_H / 2 /*bottom*/
	};    

	LoadFont();

	//外部情報受け渡し.
	m_vPos = D3DXVECTOR3(0.0f, m_fFontSize, 0.01f);
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.01f);
	m_fScale = m_fFontSize;

	if( FAILED( CreateShader() ) ){
		assert( !"Can't Create Shader" );
	}
	if( FAILED( CreateVertexBuffer() ) ){
		assert( !"Can't Create VertexBuffer" );
	}
	if( FAILED( CreateConstantBuffer() ) ){
		assert( !"Can't Create ConstantBuffer" );
	}

	LoadTextFile( sTextFileName );

	if( FAILED( CreateTexture() ) ){
		assert( !"Can't Create Texture" );
	}
}

clsFont::~clsFont()
{
	for( unsigned int iTex=0; iTex<m_vvpAsciiTexture.size(); iTex++ )
	{
		for( unsigned int i=0; i<m_vvpAsciiTexture[iTex].size(); i++ )
		{
//			SAFE_DELETE(m_pAsciiTexture[iTex][i]);
			if( !m_vvpAsciiTexture[iTex][i] ) continue;
			m_vvpAsciiTexture[iTex][i]->Release();
			m_vvpAsciiTexture[iTex][i] = nullptr;
		}
		m_vvpAsciiTexture[iTex].clear();
		m_vvpAsciiTexture[iTex].shrink_to_fit();
	}
	m_vvpAsciiTexture.clear();
	m_vvpAsciiTexture.shrink_to_fit();

	for( unsigned int iTex=0; iTex<m_vpTex2D.size(); iTex++ ){
//		SAFE_DELETE(m_pTex2D[iTex]);
		if( !m_vpTex2D[iTex] ) continue;
		m_vpTex2D[iTex]->Release();
		m_vpTex2D[iTex] = nullptr;
	}
	m_vpTex2D.clear();
	m_vpTex2D.shrink_to_fit();

	//板ﾎﾟﾘｺﾞﾝ解放.
	SAFE_RELEASE( m_pBlendState );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pPixelShader ) ;
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );

	m_pContext = nullptr;
	m_pDevice = nullptr;

	//ﾘｿｰｽ削除.
	if( !RemoveFontResourceEx(
		cFontStyle,
		FR_PRIVATE,
		&m_Design ) )
	{
		assert( !"フォントリソース解放失敗" );
	}
}

//ﾌｫﾝﾄ情報読込.
bool clsFont::LoadFont()
{
	m_fFontSize = 32;//22;
	m_fFontMarginX = 4;//5;
	m_fFontMarginY = 4;//5;
	m_strFont.iFontDispSpeed = 2;
	m_strFont.iFontAutoFlg = 120;

	return true;
}

/* シェーダの作成 */
HRESULT clsFont::CreateShader()
{
	ID3DBlob *pCompileShader = NULL;
	ID3DBlob *pErrors = NULL;

	char strDir[512];
	GetCurrentDirectory(sizeof(strDir), strDir);


	//ﾌﾞﾛﾌﾞからﾊﾞｰﾃｯｸｽｼｪｰﾀﾞｰ作成
	if (FAILED(D3DX11CompileFromFile(
		FONT_SHADER, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL,
		&pCompileShader, &pErrors, NULL)))
	{
		assert( "ﾌﾞﾛﾌﾞ作成失敗(VS)" );
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//Vertex Shaderの作成
	if (FAILED(m_pDevice->CreateVertexShader(
		(pCompileShader)->GetBufferPointer(),
		(pCompileShader)->GetBufferSize(),
		NULL, &m_pVertexShader)))	//(out)頂点ｼｪｰﾀﾞｰ
	{
		assert( "Vertex Shader作成失敗(VS)" );
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義
	D3D11_INPUT_ELEMENT_DESC layout_Ita[] =
	{
		{ "POSITION",		//位置
		0, DXGI_FORMAT_R32G32B32_FLOAT,		//DXGIのﾌｫｰﾏｯﾄ32bitfloat型
		0,
		0,		//ﾃﾞｰﾀの開始位置
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		//ﾃｸｽﾁｬ位置
		0, DXGI_FORMAT_R32G32_FLOAT,
		0,
		12,		//ﾃｸｽﾁｬﾃﾞｰﾀの開始位置
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements_Ita = sizeof(layout_Ita) / sizeof(layout_Ita[0]);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成
	if (FAILED(m_pDevice->CreateInputLayout(layout_Ita,
		numElements_Ita, pCompileShader->GetBufferPointer(),	//(out)頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ
		pCompileShader->GetBufferSize(), &m_pVertexLayout)))
	{
		assert( "頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの作成失敗" );
		return FALSE;
	}

	//ﾌﾞﾛﾌﾞからﾋﾟｸｾﾙｼｪｰﾀﾞｰ作成
	if (FAILED(D3DX11CompileFromFile(
		FONT_SHADER, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL,
		&pCompileShader, &pErrors, NULL)))
	{
		assert( "ﾌﾞﾛﾌﾞ作成失敗(PS)" );
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//Pixel Shaderの作成
	if (FAILED(m_pDevice->CreatePixelShader(
		(pCompileShader)->GetBufferPointer(),
		(pCompileShader)->GetBufferSize(), NULL, &m_pPixelShader)))	//(out)頂点
	{
		assert( "ﾋﾟｸｾﾙｼｪｰﾀﾞｰ作成失敗" );
		return E_FAIL;
	}

	SAFE_RELEASE( pCompileShader );	//ﾌﾞﾛﾌﾞ解放



	return S_OK;
}

/* バーテックスバッファの作成 */
HRESULT clsFont::CreateVertexBuffer()
{
	// Create VB
	//ﾊﾞｰﾃｯｸｽﾊﾞｯﾌｧｰ作成
	FONT_VERTEX vertices[4];

	FONT_VERTEX vertices2[] =
	{
		//頂点座標（ｘ、ｙ、ｚ）　　　　UV座標（u,v）
		D3DXVECTOR3(0, 1, 0.0f), D3DXVECTOR2(0, 1),	//頂点１
		D3DXVECTOR3(0, -1, 0.0f), D3DXVECTOR2(0, 0),	//頂点２
		D3DXVECTOR3(1, 1, 0.0f), D3DXVECTOR2(1, 1),	//頂点３
		D3DXVECTOR3(1, -1, 0.0f), D3DXVECTOR2(1, 0),	//頂点４
	};
	vertices[0] = vertices2[0];
	vertices[1] = vertices2[1];
	vertices[2] = vertices2[2];
	vertices[3] = vertices2[3];


	D3D11_BUFFER_DESC bd;	//ﾊﾞｯﾌｧの構造体
	bd.Usage = D3D11_USAGE_DEFAULT;		//使用方法
	bd.ByteWidth = sizeof(FONT_VERTEX) * 4;	//頂点のｻｲｽﾞ（頂点*３）
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//頂点ﾊﾞｯﾌｧとして扱う
	bd.CPUAccessFlags = 0;						//CPUからはｱｸｾｽしない
	bd.MiscFlags = 0;						//その他の設定なし

	//ｻﾌﾞﾘｿｰｽﾃﾞｰﾀ構造体
	D3D11_SUBRESOURCE_DATA InitDate;
	InitDate.pSysMem = vertices;		//三角形の頂点をﾘｾｯﾄ
	//頂点ﾊﾞｯﾌｧの作成
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitDate, &m_pVertexBuffer)))
	{
		ERR_MSG("頂点ﾊﾞｯﾌｧ(m_pItaVB)の作成に失敗", "InitPolygon");
		return E_FAIL;
	}

	//頂点ﾊﾞｯﾌｧをｾｯﾄ
	UINT stride = sizeof(FONT_VERTEX);	//頂点間のｻｲｽﾞ
	UINT offset = 0;						//ｵﾌｾｯﾄ値
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//ﾃｸｽﾁｬ用ｻﾝﾌﾟﾗｰ作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//ﾘﾆｱﾌｨﾙﾀｰ（線形補間）
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//ﾗｯﾋﾟﾝｸﾞﾓｰﾄﾞ（WRAP：繰り返し)
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);	//（out)ｻﾝﾌﾟﾗｰ

	return S_OK;
}

/* 定数バッファの作成 */
HRESULT clsFont::CreateConstantBuffer()
{
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成(板ﾎﾟﾘ用)
	D3D11_BUFFER_DESC ItaBD;
	ItaBD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//ｺﾝｽﾀﾝｽﾄﾊﾞｯﾌｧを指定	
	ItaBD.ByteWidth = sizeof(FONTSHADER_CONSTANT_BUFFER);	//ｺﾝｽﾀﾝｽﾄﾊﾞｯﾌｧのｻｲｽﾞ	
	ItaBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//書き込みでｱｸｾｽ
	ItaBD.MiscFlags = 0;
	ItaBD.StructureByteStride = 0;
	ItaBD.Usage = D3D11_USAGE_DYNAMIC;					//使用法：直接書き込み

	//ｺﾝｽﾀﾝｽﾄﾊﾞｯﾌｧ作成
	if (FAILED(m_pDevice->CreateBuffer(
		&ItaBD, NULL, &m_pConstantBuffer)))
	{
		ERR_MSG("ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(Ita)の作成に失敗", "InitShader");
		MessageBox(0, "ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(Ita)の作成に失敗", "InitShader", MB_OK);
		return E_FAIL;
	}


	return S_OK;
}

/* ﾌｧｲﾙ読み込み */
void clsFont::LoadTextFile( const char *FileName )
{
	//ファイルポインタ.
	clsFILE File;

	File.Open( FileName );

	//行数分繰り返し
	int iLoad = 0;		//一行ずつ文字列として読み込み

	//初期化.
	//行数.
	m_iTextRow = File.GetSizeRow();
	m_sTextData.resize( m_iTextRow );
	m_vpTex2D.resize( m_iTextRow, nullptr );
	m_vvpAsciiTexture.resize( m_iTextRow );

	for( unsigned int i=0; i<File.GetSizeRow(); i++ )
	{
		const int iCol = File.GetDataString( i, 0 ).size();//文字列の長さ.
		//一行ずつコピー.
		m_sTextData[i] = File.GetDataString( i, 0 );
#ifndef CAN_CHECK_STRING_BYTE_TYPE
		m_vvpAsciiTexture[i].resize( iCol, nullptr );
#else//#ifndef CAN_CHECK_STRING_BYTE_TYPE
		m_vvpAsciiTexture[i].reserve( m_sTextData[i].size() );

		const int iNullPlus = 1;//ヌル文字の分.
		for( unsigned int j=0; j<m_sTextData[i].size()-iNullPlus; j++ )
		{
			if( IsDBCSLeadByte( m_sTextData[i][j] ) ){
				m_vvpAsciiTexture[i].push_back( nullptr );
			}
		}
		m_vvpAsciiTexture[i].shrink_to_fit();
#endif//#ifndef CAN_CHECK_STRING_BYTE_TYPE
		iLoad++;
	}

//			if( IsDBCSLeadByte( m_sTextData[iTex][i] ) )


	//読込ﾌｫﾝﾄ行を渡す.
	m_iFontH = iLoad;
	//脱出！.
	iLoad = 0;

	//ファイルのクローズ
	File.Close();

}

/* ﾃｸｽﾁｬ作成 */
HRESULT clsFont::CreateTexture()
{
	//fontCreate
	LOGFONT lf = {
		m_fFontSize,
		0, 0, 0, 0, 0, 0, 0,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FIXED_PITCH | FF_ROMAN,
		TEXT( sFONT_NAME )
	};

	for( int iTex=0; iTex<m_iTextRow; iTex++ )
	{
		HFONT hFont = CreateFontIndirect(&lf);
		if (!(hFont))
		{
			return E_FAIL;
			ERR_MSG("フォント作成不可", "Error");
		}

		HDC hdc = GetDC(NULL);

		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

		int iByteMax = strlen( m_sTextData[iTex].c_str() );

		for( int i=0; i<iByteMax; i++ )
		{
			if( IsDBCSLeadByte( m_sTextData[iTex][i] ) ){
				i++;
			}
		}

		UINT code;
		int iCharCnt = 0;//文字列ｶｳﾝﾄ.
		for (int i = 0; i < iByteMax; i++)
		{
			//文字コード取得.
			if( IsDBCSLeadByte( m_sTextData[iTex][i] ) )
			{
				code = (BYTE)m_sTextData[iTex][i] << 8 | (BYTE)m_sTextData[iTex][i + 1];
			}
			else{
				code = m_sTextData[iTex][i];
			}
			//ﾌｫﾝﾄﾋﾞｯﾄﾏｯﾌﾟ取得.
			TEXTMETRIC TM;
			GetTextMetrics(hdc, &TM);
			GLYPHMETRICS GM;
			const int grad = GGO_GRAY4_BITMAP;

			CONST MAT2 Mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
			DWORD size = GetGlyphOutline(hdc, code, grad, &GM, 0, NULL, &Mat);
			BYTE* ptr = new BYTE[size];
			GetGlyphOutline(hdc, code, grad, &GM, size, ptr, &Mat);

			/* 書き込み可能ﾃｸｽﾁｬ作成 */

			//CPUで書き込みができるﾃｸｽﾁｬ作成.
			//ﾃｸｽﾁｬ作成.
			D3D11_TEXTURE2D_DESC desc;
			memset(&desc, 0, sizeof(desc));
			desc.Width = GM.gmCellIncX;
			desc.Height = TM.tmHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			if( FAILED( m_pDevice->CreateTexture2D( &desc, 0, &m_vpTex2D[iTex] ) ) )
			{
				MessageBox(0, "ﾃｸｽﾁｬ作成失敗", "CreateTexture", MB_OK);
				return E_FAIL;
			}

			D3D11_MAPPED_SUBRESOURCE hMappedResource;
			if( FAILED
				( m_pContext->Map(
				m_vpTex2D[iTex],
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&hMappedResource ) ) )
			{
				MessageBox(NULL, "ﾃｸｽﾁｬ作成失敗",
					"CreateTex::->Map", MB_OK);
				return E_FAIL;
			}
			// ここで書き込む
			BYTE* pBits = (BYTE*)hMappedResource.pData;

			// フォント情報の書き込み
			// iOfs_x, iOfs_y : 書き出し位置(左上)
			// iBmp_w, iBmp_h : フォントビットマップの幅高
			// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
			int iOfs_x = GM.gmptGlyphOrigin.x;
			int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
			int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
			int iBmp_h = GM.gmBlackBoxY;
			int Level = 17;
			int x, y;

			DWORD Alpha, Color;
			memset(pBits, 0, hMappedResource.RowPitch * TM.tmHeight);
			for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
			{
				for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
				{
					Alpha = (255 * ptr[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);
					Color = 0x00ffffff | (Alpha << 24);

					memcpy((BYTE*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
				}
			}

			m_pContext->Unmap( m_vpTex2D[iTex], 0 );

			//ﾃｸｽﾁｬ情報を取得する.
			D3D11_TEXTURE2D_DESC texDesc;
			m_vpTex2D[iTex]->GetDesc( &texDesc );

			//ShaderResourceViewの情報を作成する.
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = texDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

			if (FAILED(
				m_pDevice->CreateShaderResourceView(
				m_vpTex2D[iTex], &srvDesc, &m_vvpAsciiTexture[iTex][iCharCnt] ) ) )
			{
				assert( !"テクスチャ作成失敗" );
				return E_FAIL;
			}

			//文字コード取得.
			if( IsDBCSLeadByte( m_sTextData[iTex][i] ) )
			{
				i++;
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



//						↓段	　↓文字数
void clsFont::Render( int iTex, int iCharNum )
{
	//文字列の左上を座標の位置に持ってくるために必要.
	const D3DXVECTOR3 vOFFSET_POS = { -m_fScale, m_fScale * 0.5f, 0.0f };
	D3DXVECTOR3 vPos = m_vPos + vOFFSET_POS;

	if( iTex <= -1 ) return;
	if( iTex >= m_vvpAsciiTexture.size() ) return;
	if( iCharNum <= -1 ) return;

	//使用するｼｪｰﾀﾞｰの登録
	m_pContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pContext->GSSetShader(NULL, NULL, 0);

	int ii = 0;
	int iCnt = 0;

	for ( unsigned int i = 0; i<iCharNum; i++ )
	{
		if( i >= m_vvpAsciiTexture[iTex].size() ){
			break;
		}

		if( vPos.x + m_fFontSize +  (m_fScale + m_fFontMarginX ) * iCnt <= 
			m_Rect.right)
		{
			//指定範囲の中
			iCnt++;
		}
		else{
			//範囲指定外、一段ずらす.
			ii++;
			iCnt = 1;
		}

		//ﾜｰﾙﾄﾞ変換
		D3DXMATRIX mWorld, mScale, mTran;		//ワールド行列
		D3DXMatrixIdentity( &mWorld );
		D3DXMatrixTranslation( &mTran, 
			vPos.x + (m_fScale + m_fFontMarginX)*iCnt,
			vPos.y + (m_fScale + m_fFontMarginY)*ii * 2,
			vPos.z );

		D3DXMatrixScaling(&mScale, m_fScale, m_fScale, 1.0f );
		mWorld = mScale * mTran;


		//ｼｪｰﾀﾞｰのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す
		D3D11_MAPPED_SUBRESOURCE	pData;
		FONTSHADER_CONSTANT_BUFFER	cb;
		if (SUCCEEDED(m_pContext->Map(
			m_pConstantBuffer, 0,
			D3D11_MAP_WRITE_DISCARD,
			0, &pData)))
		{
			//ﾜｰﾙﾄﾞ行列を渡す
			cb.mW = mWorld;
			D3DXMatrixTranspose(&cb.mW, &cb.mW);
			cb.ViewPortWidth = WND_W;
			cb.ViewPortHeight = WND_H;


			cb.Uv = { 0.0f, 0.0f };

			cb.Color = m_vColor;
			//透明度を渡す
			cb.Alpha = m_fAlpha;

			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			m_pContext->Unmap(m_pConstantBuffer, 0);
		}

		//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを使うｼｪｰﾀﾞｰの登録
		m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

		//ﾊﾞｰﾃｯｸｽﾊﾞｯﾌｧをｾｯﾄ
		UINT stride = sizeof(FONT_VERTEX);
		UINT offset = 0;
		m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		//ﾄﾎﾟﾛｼﾞｰ
		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ
		m_pContext->IASetInputLayout(m_pVertexLayout);

		//ﾃｸｽﾁｬをｼｪｰﾀﾞｰに渡す
		m_pContext->PSSetSamplers( 0, 1, &m_pSampleLinear );
		m_pContext->PSSetShaderResources( 0, 1, &m_vvpAsciiTexture[iTex][i] );

		//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成
		SetBlendSprite( true );

		//描画
		m_pContext->Draw(4, 0);
	}
}



void clsFont::SetPos( const D3DXVECTOR3 &vPos )
{
	m_vPos = vPos;
}

D3DXVECTOR3 clsFont::GetPos()
{
	return m_vPos;
}

void clsFont::SetScale( const float fScale )
{
	m_fScale = fScale;
}

float clsFont::GetScale()
{
	return m_fScale;
}
	 
void clsFont::SetColor( const D3DXVECTOR4 &vColor )
{
	const float fNOT_ALPHA = 1.0f;
	m_vColor = vColor;
	m_vColor.w = fNOT_ALPHA;
}

void clsFont::SetAlpha( const float fAlpha )
{
	m_fAlpha = fAlpha;
}

void clsFont::SetBlendSprite( const bool isAlpha )
{
	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成
	//pngﾌｧｲﾙ内にｱﾙﾌｧ情報があるので、透過するようにﾌﾞﾚﾝﾄﾞｽﾃｰﾄで設定する
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));				//初期化
	blendDesc.IndependentBlendEnable = false;					//false:RenderTarget[0]のﾒﾝﾊﾞｰのみ使用する。true:RenderTarget[0～7]が使用できる(ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄ毎に独立したﾌﾞﾚﾝﾄﾞ処理)
	blendDesc.AlphaToCoverageEnable = false;						//true:ｱﾙﾌｧﾄｩｶﾊﾞﾚｯｼﾞを使用する

	//表示ﾀｲﾌﾟ
	blendDesc.RenderTarget[0].BlendEnable = isAlpha;					//true:ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを使用する
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを指定
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞの反転を指定
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			//ADD：加算合成
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			//そのまま使用
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;			//何もしない
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;		//ADD：加算合成
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//全ての成分(RGBA)へのﾃﾞｰﾀの格納を許可する

	if( FAILED( m_pDevice->CreateBlendState( &blendDesc, &m_pBlendState ) ) )
	{
		MessageBox(0, "ﾌﾞﾚﾝﾄﾞｽﾃｰﾄの作成に失敗", "error(InitPolygon)", MB_OK);
	}

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄの設定
	UINT mask = 0xffffffff;	//ﾏｽｸ値白
	m_pContext->OMSetBlendState( m_pBlendState, NULL, mask );

}

