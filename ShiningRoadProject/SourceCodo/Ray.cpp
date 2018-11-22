#include "Ray.h"

namespace{

	//シェーダファイル名(ディレクトリも含む)(\\).
	const char SHADER_NAME[] = "Shader\\Ray.hlsl";
	//エントリーポイント名(ディレクトリも含む)(\\).
	const char ENT_VER_NAME[] = "VS";
	const char ENT_PIX_NAME[] = "PS";
}


clsRay::clsRay()
{
//	ZeroMemory( this, sizeof( clsRay ) );
	m_pVertexShader = nullptr;	
	m_pVertexLayout = nullptr;	
	m_pPixelShader = nullptr;		
	m_pConstantBuffer = nullptr;	

	m_pVertexBuffer = nullptr;	
}

clsRay::~clsRay()
{
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );
}


//============================================================
//	初期化.
//============================================================
HRESULT clsRay::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_wpDevice = pDevice11;
	m_wpContext = pContext11;

	if( FAILED( InitShader() ) ){
		return E_FAIL;
	}
	if( FAILED( InitModel() ) ){
		return E_FAIL;
	}

	return S_OK;
}


//============================================================
//描画.
//============================================================
void clsRay::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj ) const
{
	//ワールド行列.
	D3DXMATRIX	mWorld;

	//回転行列、移動行列.
	D3DXMATRIX mRot, mTran;

	//回転行列作成(ゲーム、ステージ次第).
	D3DXMatrixRotationY( &mRot, m_Ray.fYaw );


	//平行移動作成.
	D3DXMatrixTranslation( &mTran,
		m_Ray.vPos.x, m_Ray.vPos.y, m_Ray.vPos.z );

	//ワールド行列作成.
	mWorld = mRot * mTran;

	//使用するシェーダの登録.
	m_wpContext->VSSetShader( m_pVertexShader, NULL, 0 );
	m_wpContext->PSSetShader( m_pPixelShader,  NULL, 0 );


	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cd;	//コンスタントバッファ.
	//バッファ内のデータの書き方開始時にmap.
	if( SUCCEEDED(
		m_wpContext->Map(
			m_pConstantBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//ワールド,カメラ,プロジェクション行列を渡す.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose( &m, &m );	//行列を転置する.
					//行列の計算方法がDirectXとGPUで異なるため.
		cd.mWVP = m;

		//カラー.
		cd.vColor = D3DXVECTOR4 ( 1.0f, 0.0f, 1.0f, 1.0f );



		memcpy_s( pData.pData, pData.RowPitch,
			(void*)( &cd ), sizeof( cd ) );

		m_wpContext->Unmap( m_pConstantBuffer, 0 );
	}

	//このコンスタントバッファをどのシェーダで使うか?.
	m_wpContext->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );
	m_wpContext->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );

	//頂点バッファをセット.
	UINT stride = sizeof( MODEL_VERTEX );	//データの間隔.
	UINT offset = 0;
	m_wpContext->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset );

	//頂点インプットレイアウトをセット.
	m_wpContext->IASetInputLayout( m_pVertexLayout );

	//プリミティブ・トポロジーをセット.
	m_wpContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

	//プリミティブをレンダリング.
	m_wpContext->Draw( 2, 0 );

}


//============================================================
//	HLSLファイルを読み込みシェーダを作成する.
//	HLSL:HIGE-LEVEL-SHADER-LANGUAGE.
//============================================================
HRESULT clsRay::InitShader()
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
			SHADER_NAME,	//シェーダファイル名(HLSLファイル).
			NULL,			//マクロ定義の配列へのポインタ(未使用).
			NULL,			//インクルードファイルを扱うインターフェースへのポインタ(未使用).
			ENT_VER_NAME,		//シェーダエントリーポイント関数の名前.
			"vs_5_0",		//シェーダのモデルを指定する文字列(プロファイル).
			uCompileFlag,	//シェーダコンパイルフラグ.
			0,				//エフェクトコンパイルフラグ(未使用).
			NULL,			//スレッドポンプインターフェースへのポインタ(未使用).
			&pCompiledShader,//ブロブを格納するメモリへのポインタ.
			&pErrors,		//エラーと警告一覧を格納するメモリへのポインタ.
			NULL ) ) )		//戻り値へのポインタ(未使用).
	{
		MessageBox( NULL, "hlsl(vs)読み込み失敗", "clsRay::InitShader", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if( FAILED(
		m_wpDevice->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pVertexShader) ) )//(out)バーテックスシェーダ.
	{
		MessageBox( NULL, "vs作成失敗", "clsRay::InitShader", MB_OK );
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{
			"POSITION",						//位置.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのフォーマット(32bit float型*3).
			0,
			0,								//データの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
	};
	//頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );//.

	//頂点インプットレイアウトの作成.
	if( FAILED(
		m_wpDevice->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ) ) )//(out)頂点インプットレイアウト.
	{
		MessageBox( NULL, "頂点インプットレイアウト作成失敗", "clsRay::InitShader", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );



	//HLSLからピクセルシェーダのブロブを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//シェーダファイル名(HLSLファイル).
			NULL,
			NULL,
			ENT_PIX_NAME,		//シェーダエントリーポイント関数の名前.
			"ps_5_0",		//シェーダのモデルを指定する文字列(プロファイル).
			uCompileFlag,	//シェーダコンパイルフラグ.
			0,
			NULL,
			&pCompiledShader,//ブロブを格納するメモリへのポインタ.
			&pErrors,
			NULL ) ) )
	{
		MessageBox( NULL, "hlsl(ps)読み込み失敗", "clsRay::InitShader", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if( FAILED(
		m_wpDevice->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pPixelShader ) ) )//(out)ピクセルシェーダ.
	{
		MessageBox( NULL, "ps作成失敗", "clsRay::InitShader", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );//ブロブ解放.


	//コンスタントバッファ(定数)バッファ作成　シェーダに特定の数値を送るバッファ.
	//ここでは変換行列渡し用.
	//シェーダにWorld,View,Projection行列を渡す.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;;			//コンストバッファを指定.
	cb.ByteWidth	= sizeof( SHADER_CONSTANT_BUFFER );//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//書き込みでアクセス.
	cb.MiscFlags	= 0;					//その他のフラグ(未使用).
	cb.StructureByteStride	= 0;			//構造体のサイズ(未使用)
	cb.Usage		= D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

	//コンスタントバッファ作成.
	if( FAILED(
		m_wpDevice->CreateBuffer(
			&cb,
			NULL,
			&m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "コンスタントバッファ作成失敗", "clsRay::InitShader", MB_OK );
		return E_FAIL;
	}


	return S_OK;
}


//============================================================
//	レイ初期化.
//============================================================
HRESULT clsRay::InitModel()
{
	//レイのバーテックスバッファ作成.
	D3D11_BUFFER_DESC	bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage		= D3D11_USAGE_DEFAULT;
	bd.ByteWidth	= sizeof( MODEL_VERTEX ) * 2;//2点分.
	bd.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags	= 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_Ray.vPoint;//レイの座標をセット.
	if( FAILED(
		m_wpDevice->CreateBuffer(
			&bd, &InitData, &m_pVertexBuffer ) ) )
	{
		ERR_MSG( "頂点バッファ作成失敗", "clsRay::InitLine" );
		return E_FAIL;
	}


	return S_OK;
}



