#include "DebugText.h"


namespace{
	//シェーダファイル名(パスも含む).
	const char SHADER_NAME[] = "Shader\\DebugText.hlsl";

	const char* FILE_PATH = "Data\\Image\\DebugText\\ascii0.png";

	const float DIMENSION		= 12.0f;
	const float WDIMENSION		= 10.0f;
	const float TEX_DIMENSION	= 128.0f;
}

//============================================================
//	コンストラクタ.
//============================================================
clsDebugText::clsDebugText()
{
//	ZeroMemory( this, sizeof( clsDebugText ) );	//初期化.

	m_pVertexShader = nullptr;	//頂点シェーダ.
	m_pVertexLayout = nullptr;	//頂点レイアウト.
	m_pPixelShader = nullptr;		//ピクセルシェーダ.
	m_pConstantBuffer = nullptr;	//コンスタントバッファ.

	for( int i=0; i<100; i++ ){
		m_pVertexBuffer[i] = nullptr;	//頂点バッファ(100個分).
	}

	m_pAsciiTexture = nullptr;//アスキーテクスチャ.
	m_pSampleLinear = nullptr;//テクスチャのサンプラー:/テクスチャに各種フィルタをかける.

	for( int i=0; i<100; i++ ){
		m_fKerning[i] = 0.0f;
	}
	m_fScale = 1.0f;

	D3DXMatrixIdentity( &m_mView );
	D3DXMatrixIdentity( &m_mProj );
}


//============================================================
//	デストラクタ.
//============================================================
clsDebugText::~clsDebugText()
{
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pAsciiTexture );
	SAFE_RELEASE( m_pSampleLinear );
	for( int i=0; i<100; i++ ){
		SAFE_RELEASE( m_pVertexBuffer[i] );
	}

	for( int i=0; i<100; i++ ){
		m_fKerning[i] = 0.0f;
	}
	m_fScale = 1.0f;

//	D3DXMatrixIdentity( &m_mView );
//	D3DXMatrixIdentity( &m_mProj );

}


//============================================================
//	初期化関数.
//============================================================
HRESULT clsDebugText::Init( ID3D11DeviceContext* pContext,
							DWORD dwWidth, DWORD dwHeight,
							float fSize, D3DXVECTOR4 vColor )
{
	m_fAlpha = vColor.w;
	m_vColor = vColor;
	m_fScale = fSize / 25.0f;

	//95文字分繰り返し.
	for( int i=0; i<95; i++ ){
		m_fKerning[i] = 10.0f;
	}
	//デバイスコンテキストをコピー.
	m_pContext = pContext;
	m_pContext->GetDevice( &m_pDevice );

	//ブレンドステート作成.
	CreateBlendState();

	//windowサイズ.
	m_dwWindowWidth = dwWidth;
	m_dwWindowHeight= dwHeight;

	//フォント毎にクアッド(矩形)作成.
	float left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	int cnt = 0;
	for( int k=0; k<10; k++ ){
		for( int i=0; i<10; i++ ){
			left	= k * WDIMENSION;
			top		= i * DIMENSION;
			right	= left + m_fKerning[cnt];
			bottom	= top + DIMENSION;

			left	/= TEX_DIMENSION;
			top		/= TEX_DIMENSION;
			right	/= TEX_DIMENSION;
			bottom	/= TEX_DIMENSION;

			TextVertex vertices[] =
			{
				D3DXVECTOR3(	  0.0f,	DIMENSION, 0.0f ), D3DXVECTOR2( left, bottom ),		//頂点1.
				D3DXVECTOR3(	  0.0f,		 0.0f, 0.0f ), D3DXVECTOR2( left, top ),		//頂点2.
				D3DXVECTOR3( DIMENSION, DIMENSION, 0.0f ), D3DXVECTOR2( right, bottom ),	//頂点3.
				D3DXVECTOR3( DIMENSION,		 0.0f, 0.0f ), D3DXVECTOR2( right, top )		//頂点4.
			};
			D3D11_BUFFER_DESC bd;
			bd.Usage		= D3D11_USAGE_DEFAULT;
			bd.ByteWidth	= sizeof( TextVertex ) * 4;
			bd.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags=0;
			bd.MiscFlags	= 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;
			if( FAILED( m_pDevice->CreateBuffer(
				&bd, &InitData, &m_pVertexBuffer[cnt]) ) )
			{
				MessageBox( NULL,
					"頂点バッファ作成失敗(DebugText:Init)",
					"error", MB_OK );
				return E_FAIL;
			}
			cnt ++;
		}
	}

	//テクスチャ用サンプラ作成.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory( &SamDesc, sizeof( D3D11_SAMPLER_DESC ) );
	//テクスチャフィルタと貼り付け方の指定.
	SamDesc.Filter	= D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU= D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV= D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW= D3D11_TEXTURE_ADDRESS_WRAP;
	if( FAILED( m_pDevice->CreateSamplerState(
		&SamDesc, &m_pSampleLinear) ) )
	{
		MessageBox( NULL,
			"サンプル作成失敗(DebugText:Init)",
			"", MB_OK );
		return E_FAIL;
	}

	//フォントのテクスチャ作成.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice,
			FILE_PATH,
			NULL, NULL,
			&m_pAsciiTexture, NULL ) ) )
	{
		MessageBox( NULL,
			"フォントテクスチャ作成失敗(DebugText:Init)",
			FILE_PATH, MB_OK );
		return E_FAIL;
	}

	//============================================================
		//hlslファイル読込.
	//============================================================
	ID3DBlob*	pCompileShader = nullptr;
	ID3DBlob*	pErrors	= nullptr;
	if( FAILED( D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL,
		"VS", "vs_5_0", 0, 0,
		NULL, &pCompileShader,
		&pErrors, NULL ) ) )
	{
		MessageBox( NULL, "hlsl読込失敗(VS)", "DebugText:Init", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	if( FAILED( m_pDevice->CreateVertexShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		NULL, &m_pVertexShader ) ) )
	{
		SAFE_RELEASE( pCompileShader );
		MessageBox( NULL, "バーテックスバッファ作成失敗", "DebugText:Init", MB_OK );
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	//頂点インプットレイアウト作成.
	if( FAILED( m_pDevice->CreateInputLayout(
		layout, numElements,
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		&m_pVertexLayout ) ) )
	{
		MessageBox( NULL, "頂点インプットレイアウト作成", "DebugText:Init", MB_OK );
		return E_FAIL;
	}

	//ピクセルシェーダ作成.
	if( FAILED( D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL,
		"PS", "ps_5_0", 0, 0,
		NULL, &pCompileShader,
		&pErrors, NULL ) ) )
	{
		MessageBox( NULL, "hlsl読込失敗(PS)", "DebugText:Init", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );


	if( FAILED( m_pDevice->CreatePixelShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		NULL, &m_pPixelShader ) ) )
	{
		SAFE_RELEASE( pCompileShader );
		MessageBox( NULL, "ピクセルシェーダ作成失敗", "DebugText:Init", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompileShader );

	//コンスタントバッファ作成.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth	= sizeof( TEXT_CONSTANT_BUFFER );
	cb.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags	= 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if( FAILED(m_pDevice->CreateBuffer(
		&cb, NULL, &m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "コンスタントバッファ作成", "DebugText:Init", MB_OK );
		return E_FAIL;
	}




	return S_OK;
}



//============================================================
//	レンダリング関数.
//============================================================
void clsDebugText::Render( char* text, int x, int y )
{
	//ビュートランスフォーム.
	D3DXVECTOR3	vEye( 0.0f, 0.0f,-1.0f );
	D3DXVECTOR3	vLook(0.0f, 0.0f, 0.0f );
	D3DXVECTOR3	vUp(  0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH(
		&m_mView, &vEye, &vLook, &vUp );


#if 0
	//プロジェクショントランスフォーム(固定).
	D3DMATRIX mOtho = {
		2.0f / (float)( m_dwWindowWidth ), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)( m_dwWindowHeight), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
#else
	D3DXMATRIX mOtho;
	D3DXMatrixIdentity( &mOtho );//単位行列.
	mOtho._11	= 2.0f / (float)( m_dwWindowWidth );
	mOtho._22	= -2.0f / (float)( m_dwWindowHeight );
	mOtho._41	= -1.0f;
	mOtho._42	= 1.0f;
#endif
	m_mProj = mOtho;

	//プリミティブ・トポロジー.
	m_pContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	//頂点インプットレイアウトをセット.
	m_pContext->IASetInputLayout(
		m_pVertexLayout );

	//使用するシェーダの登録.
	m_pContext->VSSetShader(
		m_pVertexShader, NULL, 0 );
	m_pContext->PSSetShader(
		m_pPixelShader, NULL, 0 );

	//このコンスタントバッファを使うシェーダの登録.
	m_pContext->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );
	m_pContext->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );

	//テクスチャをシェーダに渡す.
	m_pContext->PSSetSamplers(
		0, 1, &m_pSampleLinear );
	m_pContext->PSSetShaderResources(
		0, 1, &m_pAsciiTexture );


	SetBlend( true );
	//文字数分ループ.
	for( UINT i=0; i<strlen( text ); i++ ){
		char font = text[i];
		int index = font - 32;	//フォントインデックス作成.

		//フォントレンダリング.
		RenderFont( index, x, y );

		x += static_cast<int>(m_fKerning[index]);
	}



}


//============================================================
//フォントレンダリング関数.
//============================================================
void clsDebugText::RenderFont( int FontIndex, int x, int y )
{
	//ワールド変換.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	D3DXMatrixTranslation(
		&mWorld, (float)x, (float)y,
		-100.0f );	//Z(-100)で手前表示可能.

	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE	pData;
	TEXT_CONSTANT_BUFFER		cb;
	if( SUCCEEDED( m_pContext->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//ワールド,ビュー,プロジェクションの合成行列を渡す.
		D3DXMATRIX m = mWorld * m_mView * m_mProj;
		D3DXMatrixTranspose( &m, &m );
		cb.mWVP = m;
		//カラーを渡す.
		cb.vColor = m_vColor;
		//透明度を渡す.
		cb.fAlpha.x = m_fAlpha;

		memcpy_s( pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb) );
		m_pContext->Unmap(
			m_pConstantBuffer, 0 );
	}
	//バーテックスバッファをセット.
	UINT stride = sizeof( TextVertex );
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer[FontIndex],
		&stride, &offset );

	//描画.
//	SetBlend( true );
	m_pContext->Draw( 4, 0 );


}