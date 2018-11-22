#include "ScreenTexture.h"

#include "SoundManagerNoise.h"
#include "Sprite2D.h"	

#include <random>

using namespace std;

namespace{

	struct SCREEN_TEXTURE_CONSTANT_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				//ワールド行列.
		ALIGN16	D3DXVECTOR4	vColor;			//アルファ値(透過で使用する)
		ALIGN16 D3DXVECTOR2 vViewPort;	//ビューポート幅.
		ALIGN16 float		fPulse;			//パルス.
		ALIGN16 float		fPulseOffset;	
		ALIGN16 int			iBlock;			//ブロックの分割数.
		ALIGN16 int			iSeed;			//ブロックのseed値.
		ALIGN16 float		isfNega;
	};

	//頂点の構造体.
	struct SCREEN_VERTEX
	{
		D3DXVECTOR3 vPos;	//位置.
		D3DXVECTOR2 vTex;	//テクスチャ座標.
	};

	enum enSE_WEAK : int{
		enSE_WEAK_A = 0,
		enSE_WEAK_B,

		enSE_WEAK_size
	};
	enum enSE_STRONG : int{
		enSE_STRONG_A = enSE_WEAK_size,
		enSE_STRONG_B,
		enSE_STRONG_C,
		enSE_STRONG_D,
		enSE_STRONG_E,

		enSE_STRONG_size
	};

	const char sSHADER_NAME[] = "Shader\\Screen.hlsl";


	const float fPULSE_INIT = 1.0f;
	const float fPULSE_OFFSET_INIT = -62000.0f;
	const float fPULSE_OFFSET_ADD = 0.75f;
	const int iBLOCK_INIT = 256;
}


clsSCREEN_TEXTURE::clsSCREEN_TEXTURE(
	const HWND hWnd,
	ID3D11DeviceContext* const pContext )
	:m_wpContext( pContext )
	,m_wpDevice( nullptr )
	,m_pTexture( nullptr )
	,m_pRenderTargetView( nullptr )
	,m_pShaderResourceView( nullptr )
	,m_pSamplerState( nullptr )
	,m_pVertexShader( nullptr )
	,m_pDefaultPS( nullptr )
	,m_pNoisePS( nullptr )
	,m_pConstantBuffer( nullptr )
	,m_isNoise( false )
	,m_iBlock( iBLOCK_INIT )
	,m_iSeed( 0 )
	,m_fPulse( fPULSE_INIT )
	,m_fPulseOffset( fPULSE_OFFSET_INIT )
	,m_fPulseOffsetAdd( fPULSE_OFFSET_ADD )
	,m_isNega( false )
	,m_vColor( 1.0f, 1.0f, 1.0f, 1.0f )
{
	assert( m_wpContext );
	m_wpContext->GetDevice( &m_wpDevice );

	if( FAILED( CreateTexture() ) ){
		ERR_MSG( "描画先テクスチャ作成失敗", "" );
	}
	if( FAILED( CreateShader() ) ){
		ERR_MSG( "描画先テクスチャシェーダー作成失敗", "" );
	}
	if( FAILED( CreateConstantBuffer() ) ){
		ERR_MSG( "描画先テクスチャバッファ作成失敗", "" );
	}

	//サウンド作成.
	m_upSound = make_unique< clsSOUND_MANAGER_NOISE >( hWnd );
	assert( m_upSound );
	m_upSound->Create();
}

clsSCREEN_TEXTURE::~clsSCREEN_TEXTURE()
{

	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pNoisePS );
	SAFE_RELEASE( m_pDefaultPS );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pSamplerState );
	SAFE_RELEASE( m_pShaderResourceView );
	SAFE_RELEASE( m_pRenderTargetView );
	SAFE_RELEASE( m_pTexture );



	m_wpContext = nullptr;
	m_wpDevice = nullptr;
}



HRESULT clsSCREEN_TEXTURE::CreateTexture()
{
	//テクスチャ.
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.Format				= DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width				= WND_W;
	texDesc.Height				= WND_H;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.MiscFlags			= 0;
	
	HRESULT hr = m_wpDevice->CreateTexture2D( &texDesc, nullptr, &m_pTexture );
	if( FAILED( hr ) ){
		ERR_MSG( "スクリーンテクスチャ作成失敗", "" );
		assert( !"スクリーンテクスチャ作成失敗" );
		return hr;
	}

	//レンダーターゲットビュー.
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Buffer.ElementOffset					= 0;
	rtvDesc.Buffer.ElementWidth						= 0;
	rtvDesc.Buffer.FirstElement						= 0;
	rtvDesc.Buffer.NumElements						= 0;
	rtvDesc.Texture1D.MipSlice						= 0;
	rtvDesc.Texture1DArray.ArraySize				= 0;
	rtvDesc.Texture1DArray.FirstArraySlice			= 0;
	rtvDesc.Texture1DArray.MipSlice					= 0;
	rtvDesc.Texture2D.MipSlice						= 0;
	rtvDesc.Texture2DArray.ArraySize				= 0;
	rtvDesc.Texture2DArray.FirstArraySlice			= 0;
	rtvDesc.Texture2DArray.MipSlice					= 0;
	rtvDesc.Texture2DMS.UnusedField_NothingToDefine	= 0;
	rtvDesc.Texture2DMSArray.ArraySize				= 0;
	rtvDesc.Texture2DMSArray.FirstArraySlice		= 0;
	rtvDesc.Texture3D.FirstWSlice					= 0;
	rtvDesc.Texture3D.MipSlice						= 0;
	rtvDesc.Texture3D.WSize							= 0;
	
	hr = m_wpDevice->CreateRenderTargetView( m_pTexture, &rtvDesc, &m_pRenderTargetView );
	if( FAILED( hr ) ){
		ERR_MSG( "スクリーンレンダーターゲットビュー作成失敗", "" );
		assert( !"スクリーンレンダーターゲットビュー作成失敗" );
		return hr;
	}

	//シェーダーリソースビュー.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof( srvDesc ) );
	srvDesc.Format				= rtvDesc.Format;
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels	= 1;

	hr = m_wpDevice->CreateShaderResourceView( m_pTexture, &srvDesc, &m_pShaderResourceView );
	if( FAILED( hr ) ){
		ERR_MSG( "スクリーンシェーダーリソースビュー作成失敗", "" );
		assert( !"スクリーンシェーダーリソースビュー作成失敗" );
		return hr;
	}

	//サンプラーステート.
	D3D11_SAMPLER_DESC smpDesc;
	ZeroMemory( &smpDesc, sizeof( smpDesc ) );
	smpDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	smpDesc.MinLOD			= 0;
	smpDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = m_wpDevice->CreateSamplerState( &smpDesc, &m_pSamplerState );
	if( FAILED( hr ) ){
		ERR_MSG( "スクリーンサンプラーステート作成失敗", "" );
		assert( !"スクリーンサンプラーステート作成失敗" );
		return hr;
	}

	return S_OK;
}

HRESULT clsSCREEN_TEXTURE::CreateShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;

	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION; 
#endif//#ifdef _DEBUG


	//HLSLからバーテックスシェーダのブロブを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			sSHADER_NAME,	//シェーダファイル名(HLSLファイル).
			NULL,			//マクロ定義の配列へのポインタ(未使用).
			NULL,			//インクルードファイルを扱うインターフェースへのポインタ(未使用).
			"VS_Main",			//シェーダエントリーポイント関数の名前.
			"vs_5_0",		//シェーダのモデルを指定する文字列(プロファイル).
			uCompileFlag,	//シェーダコンパイルフラグ.
			0,				//エフェクトコンパイルフラグ(未使用).
			NULL,			//スレッドポンプインターフェースへのポインタ(未使用).
			&pCompiledShader,//ブロブを格納するメモリへのポインタ.
			&pErrors,		//エラーと警告一覧を格納するメモリへのポインタ.
			NULL ) ) )		//戻り値へのポインタ(未使用).
	{
		MessageBox( NULL, "hlsl(vs)読み込み失敗", "エラー", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if( FAILED(
		m_wpDevice->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pVertexShader ) ) )//(out)バーテックスシェーダ.
	{
		MessageBox( NULL, "vs作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}

	SAFE_RELEASE( pCompiledShader );




	//----- 通常 -----//.
	//HLSLからピクセルシェーダのブロブを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			sSHADER_NAME,	//シェーダファイル名(HLSLファイル).
			NULL,
			NULL,
			"PS_Main",			//シェーダエントリーポイント関数の名前.
			"ps_5_0",		//シェーダのモデルを指定する文字列(プロファイル).
			uCompileFlag,	//シェーダコンパイルフラグ.
			0,
			NULL,
			&pCompiledShader,//ブロブを格納するメモリへのポインタ.
			&pErrors,
			NULL ) ) )
	{
		MessageBox( NULL, "hlsl(ps)読み込み失敗", "エラー", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if( FAILED(
		m_wpDevice->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pDefaultPS ) ) )//(out)ピクセルシェーダ.
	{
		MessageBox( NULL, "通常ps作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}


	//----- ノイズ -----//.
	//HLSLからピクセルシェーダのブロブを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			sSHADER_NAME,	//シェーダファイル名(HLSLファイル).
			NULL,
			NULL,
			"PS_Noise",			//シェーダエントリーポイント関数の名前.
			"ps_5_0",		//シェーダのモデルを指定する文字列(プロファイル).
			uCompileFlag,	//シェーダコンパイルフラグ.
			0,
			NULL,
			&pCompiledShader,//ブロブを格納するメモリへのポインタ.
			&pErrors,
			NULL ) ) )
	{
		MessageBox( NULL, "hlsl(ps)読み込み失敗", "エラー", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if( FAILED(
		m_wpDevice->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pNoisePS ) ) )//(out)ピクセルシェーダ.
	{
		MessageBox( NULL, "ノイズps作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );//ブロブ解放.

	return S_OK;
}

HRESULT clsSCREEN_TEXTURE::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//コンスタントバッファを指定.
	cb.ByteWidth = sizeof( SCREEN_TEXTURE_CONSTANT_BUFFER );//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//書き込みでアクセス.
	cb.MiscFlags = 0;//その他のフラグ(未使用)
	cb.StructureByteStride = 0;//構造体のサイズ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;//使用方法:直接書き込み.

	//コンスタントバッファ作成.
	if( FAILED( m_wpDevice->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "コンスタントバッファ作成失敗", "ScreenTexture", MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

//Rendertargetをテクスチャにする.
void clsSCREEN_TEXTURE::SetRenderTargetTexture( ID3D11DepthStencilView* const pDepthStencilView ) const
{
	if( !pDepthStencilView )	return;

	//レンダーターゲットをテクスチャに.
	float clearcolor[] = { 2.5f, 0.125f, 0.125f, 1.0f };
	m_wpContext->OMSetRenderTargets( 1, &m_pRenderTargetView, pDepthStencilView );
	m_wpContext->ClearRenderTargetView( m_pRenderTargetView, clearcolor );
	m_wpContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}


//ノイズの更新.
void clsSCREEN_TEXTURE::NoiseUpdate()
{
	m_upSound->UpdateLoop();

	//ポストエフェクト.
	m_iSeed ++;
	const int iSEED_MAX = 32000;
	if( m_iSeed >= iSEED_MAX ){
		m_iSeed = 0;
	}
	m_fPulseOffset += m_fPulseOffsetAdd;
	const float fPULSE_OFFSET_MAX = -fPULSE_OFFSET_INIT;
	if( m_fPulseOffset >= fPULSE_OFFSET_MAX ){
		m_fPulseOffset = fPULSE_OFFSET_INIT;
	}

	//音再生中なら再生中フラグを立て続け、停止中なら寝かせる.
	if( !m_SeFlagWeak.isCanPlay )
	{
		for( int i=enSE_WEAK_A; i<enSE_WEAK_size; i++ )
		{
			if( m_upSound->IsPlayingSE( i ) ){
				goto PLAYING_SE_WEAK;//再生中である.
			}
		}
		m_SeFlagWeak.isCanPlay = false;
	}
PLAYING_SE_WEAK:;

	if( !m_SeFlagStrong.isCanPlay )
	{
		for( int i=enSE_STRONG_A; i<enSE_STRONG_size; i++ )
		{
			if( m_upSound->IsPlayingSE( i ) ){
				goto PLAYING_SE_STRONG;
			}
		}
		m_SeFlagStrong.isCanPlay = false;
	}
PLAYING_SE_STRONG:;


}


//テクスチャの内容を画面に描画.
void clsSCREEN_TEXTURE::RenderWindowFromTexture( 
	ID3D11RenderTargetView* const pBackBuffer_TexRTV,
	ID3D11DepthStencilView* const pDepthStencilView ) const
{
	if( !pBackBuffer_TexRTV )	return;
	if( !pDepthStencilView )	return;

	float clearcolor[] = { 1.5f, 0.5f, 0.5f, 1.0f };
	m_wpContext->OMSetRenderTargets( 1, &pBackBuffer_TexRTV, pDepthStencilView );
	m_wpContext->ClearRenderTargetView( pBackBuffer_TexRTV, clearcolor );
	m_wpContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	ID3D11Buffer* pBuffer;
	//板ポリ(四角形)の頂点を作成.
	float tmpw = WND_W;
	float tmph = WND_H;
	SCREEN_VERTEX vertices[] = 
	{
#if 1
		//頂点座標(x,y,z).					//UV座標( u, v ).
		D3DXVECTOR3( 0.0f,  tmph,  0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),//頂点1(左下).
		D3DXVECTOR3( 0.0f,  0.0f,  0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),//頂点2(左上).
		D3DXVECTOR3( tmpw,  tmph,  0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),//頂点3(右下).
		D3DXVECTOR3( tmpw,  0.0f,  0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),//頂点4(右上).
#else
		//頂点座標(x,y,z).					//UV座標( u, v ).
		D3DXVECTOR3( tmpw,  0.0f,  0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),//頂点4(右上).
		D3DXVECTOR3( tmpw,  tmph,  0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),//頂点3(右下).
		D3DXVECTOR3( 0.0f,  tmph,  0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),//頂点1(左下).

		D3DXVECTOR3( tmpw,  0.0f,  0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),//頂点4(右上).
		D3DXVECTOR3( 0.0f,  tmph,  0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),//頂点1(左下).
		D3DXVECTOR3( 0.0f,  0.0f,  0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),//頂点2(左上).
#endif
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof( vertices ) / sizeof( vertices[0] );

	//バッファ構造体.
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage				= D3D11_USAGE_DEFAULT;				//使用法(デフォルト).
	bd.ByteWidth			= sizeof( SCREEN_VERTEX ) * uVerMax;	//頂点サイズ(頂点*4).
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;			//頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;								//CPUからはアクセスしない.
	bd.MiscFlags			= 0;								//その他のフラグ(未使用).
	bd.StructureByteStride	= 0;								//構造体サイズ(未使用).

	//サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem	= vertices;	//板ポリの頂点をセット.
	InitData.SysMemPitch= 0;
	InitData.SysMemSlicePitch = 0;

	//頂点バッファの作成.
	if( FAILED(
		m_wpDevice->CreateBuffer(
			&bd, &InitData, &pBuffer ) ) )
	{
		ERR_MSG( "バッファ作成失敗", "" );
		assert( !"バッファ作成失敗" );
		return ;
	}

	m_wpContext->VSSetShader( m_pVertexShader, NULL, 0 );
	if( m_isNoise ){
		m_wpContext->PSSetShader( m_pNoisePS, NULL, 0 );
	}
	else{
		m_wpContext->PSSetShader( m_pDefaultPS, NULL, 0 );
	}

	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_wpContext->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		SCREEN_TEXTURE_CONSTANT_BUFFER cb;
		D3DXMATRIX m;
		D3DXMatrixIdentity( &m );
	
		cb.mW = m;
		cb.vColor = m_vColor;
		cb.vViewPort.x = WND_W;
		cb.vViewPort.y = WND_H;

		cb.fPulse = m_fPulse;
		cb.fPulseOffset = m_fPulseOffset;
		cb.iBlock = m_iBlock;
		cb.iSeed  = m_iSeed;

		const float fNEGA = 1.0f;
		if( m_isNega )	cb.isfNega = fNEGA;
		else			cb.isfNega = 0.0f;
	
		memcpy_s( pData.pData, pData.RowPitch,
			(void*)( &cb ), sizeof( cb ) );
		m_wpContext->Unmap( m_pConstantBuffer, 0 );
	}

	//このコンスタントバッファをどのシェーダで使うか？.
	m_wpContext->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
	m_wpContext->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );

	//テクスチャをシェーダに渡す.
	m_wpContext->PSSetShaderResources(	0, 1, &m_pShaderResourceView );
	m_wpContext->PSSetSamplers(			0, 1, &m_pSamplerState );

	//頂点バッファをセット.
	UINT stride = sizeof( SCREEN_VERTEX );//データ間隔.
	uint32_t offset = 0;
	m_wpContext->IASetVertexBuffers( 0, 1, &pBuffer, &stride, &offset );
	m_wpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
//	m_wpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	m_wpContext->Draw( uVerMax, 0 );

}




//効果音再生.
bool clsSCREEN_TEXTURE::PlaySeStrong()
{
	if( m_SeFlagStrong.isCanPlay ) return false;

	int iContinueNo = m_SeFlagStrong.iContinueNo;
	bool isPlay;

	if( m_SeFlagStrong.isContinue ){
		isPlay = m_upSound->PlaySE( m_SeFlagStrong.iContinueNo );
	}
	else{
		isPlay = PlaySeProduct( enSE_STRONG_A, enSE_STRONG_size, &iContinueNo );
	}


	if( isPlay ){
		m_SeFlagStrong.isCanPlay	= true;
		m_SeFlagStrong.isContinue	= true;
		m_SeFlagStrong.iContinueNo	= iContinueNo;
	}

	return isPlay;
}

bool clsSCREEN_TEXTURE::PlaySeWeak()
{
	if( m_SeFlagStrong.isCanPlay )	return false;
	if( m_SeFlagWeak.isCanPlay )	return false;

	int iContinueNo = m_SeFlagWeak.iContinueNo;
	bool isPlay;

	if( m_SeFlagWeak.isContinue ){
		isPlay = m_upSound->PlaySE( m_SeFlagWeak.iContinueNo );
	}
	else{
		isPlay = PlaySeProduct( enSE_WEAK_A, enSE_WEAK_size, &iContinueNo );
	}


	if( isPlay ){
		m_SeFlagWeak.isCanPlay	= true;
		m_SeFlagWeak.isContinue	= true;
		m_SeFlagWeak.iContinueNo= iContinueNo;
	}

	return isPlay;
}


bool clsSCREEN_TEXTURE::PlaySeProduct( 
	const int iMin, const int iSize, int* const outSeNo )
{
	//ランダムでノイズ音再生.
	mt19937 mt{ std::random_device{}() };

	int iMax = iSize - 1;
	if( iMax < iMin ){
		iMax = iMin;
	}

	uniform_int_distribution<int> dist( iMin, iMax );

	*outSeNo = dist( mt );

	const bool NOISE_LOOP = true;
	return m_upSound->PlaySE( *outSeNo, NOISE_LOOP );
}

void clsSCREEN_TEXTURE::StopSe()
{
	//音.
	m_upSound->StopAllSound();

	m_SeFlagStrong.isCanPlay = false;
	m_SeFlagWeak.isCanPlay	 = false;

	m_SeFlagStrong.isContinue = false;
	m_SeFlagWeak.isContinue	 = false;

}
