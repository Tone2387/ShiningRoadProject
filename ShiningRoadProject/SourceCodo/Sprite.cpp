#include "Sprite.h"

namespace{
	//シェーダファイル名(ディレクトリも含む)(\\).
	const char SHADER_NAME[] = "Shader\\Mesh.hlsl";
}

//============================================================
//コンストラクタ.
//============================================================
clsSprite::clsSprite()
{
	//初期化.
//	ZeroMemory( this, sizeof( clsSprite ) );
	m_pVertexShader		= nullptr;	
	m_pVertexLayout		= nullptr;	
	m_pPixelShader		= nullptr;		
	m_pConstantBuffer	= nullptr;
	m_pVertexBuffer		= nullptr;
	m_pTexture			= nullptr;		
	m_pSampleLinear		= nullptr;

	m_vSplit = { 1.0f, 1.0f };
}

//============================================================
//デストラクタ.
//============================================================
clsSprite::~clsSprite()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );
}

//============================================================
//初期化.
//============================================================
HRESULT clsSprite::Create( ID3D11Device* const pDevice11,
	ID3D11DeviceContext* const pContext11,
	const char* sTexName )
{
	m_pDevice11 = pDevice11;
	m_pDeviceContext11 = pContext11;

	if( FAILED( CreateBlendState() ) ){
		return E_FAIL;
	}
	//シェーダ作成.
	if( FAILED( InitShader() ) ){
		return E_FAIL;
	}
	//板ポリゴン作成.
	if( FAILED( InitModel( sTexName ) ) ){
		return E_FAIL;
	}





	return S_OK;
}

//============================================================
//	HLSLファイルを読み込みシェーダを作成する.
//	HLSL:HIGE-LEVEL-SHADER-LANGUAGE.
//============================================================
HRESULT clsSprite::InitShader()
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
			"VS_Ita",			//シェーダエントリーポイント関数の名前.
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
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pVertexShader) ) )//(out)バーテックスシェーダ.
	{
		MessageBox( NULL, "vs作成失敗", "エラー", MB_OK );
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
		{
			"TEXCOORD",					//テクスチャ位置.
			0,
			DXGI_FORMAT_R32G32_FLOAT,	//DXGIのフォーマット(32bit float型*2).
			0,
			12,							//テクスチャデータの開始位置(頂点データがfloat型*3=12biteなので+12).
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );//.

	//頂点インプットレイアウトの作成.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ) ) )//(out)頂点インプットレイアウト.
	{
		MessageBox( NULL, "頂点インプットレイアウト作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );





	//HLSLからピクセルシェーダのブロブを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//シェーダファイル名(HLSLファイル).
			NULL,
			NULL,
			"PS_Ita",			//シェーダエントリーポイント関数の名前.
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
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pPixelShader ) ) )//(out)ピクセルシェーダ.
	{
		MessageBox( NULL, "ps作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );//ブロブ解放.


	//コンスタントバッファ(定数)バッファ作成　シェーダに特定の数値を送るバッファ.
	//ここでは変換行列渡し用.
	//シェーダにWorld,View,Projection行列を渡す.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;;			//コンストバッファを指定.
	cb.ByteWidth	= sizeof( SPRITESHADER_CONSTANT_BUFFER );//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//書き込みでアクセス.
	cb.MiscFlags	= 0;					//その他のフラグ(未使用).
	cb.StructureByteStride	= 0;			//構造体のサイズ(未使用)
	cb.Usage		= D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

	//コンスタントバッファ作成.
	if( FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			NULL,
			&m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "コンスタントバッファ作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	モデル作成.
//============================================================
HRESULT clsSprite::InitModel( const char* sTexName )
{
//	const float itaW = 0.5075f;
	const float itaW = 0.5f;
	float w, h;
//	w = h = ( 1.0f / 8.0f );
	w = h = 1.0f;

	//板ポリ(四角形)の頂点を作成.
	SpriteVertex vertices[] = 
	{
		//頂点座標(x,y,z).					//UV座標( u, v ).
		D3DXVECTOR3(-itaW,-itaW, 0.0f ),	D3DXVECTOR2( 0.0f,    h ),	//頂点1.
		D3DXVECTOR3(-itaW, itaW, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),	//頂点2.
		D3DXVECTOR3( itaW,-itaW, 0.0f ),	D3DXVECTOR2(	w,	  h ),	//頂点3.
		D3DXVECTOR3( itaW, itaW, 0.0f ),	D3DXVECTOR2(	w, 0.0f )	//頂点4.
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;		//使用法(デフォルト).
	bd.ByteWidth		= sizeof( SpriteVertex ) * uVerMax;//頂点サイズ(頂点*4).
	bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;	//頂点バッファとして扱う.
	bd.CPUAccessFlags	= 0;						//CPUからはアクセスしない.
	bd.MiscFlags		= 0;						//その他のフラグ(未使用).
	bd.StructureByteStride	= 0;					//構造体サイズ(未使用).

	//サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem	= vertices;	//板ポリの頂点をセット.

	//頂点バッファの作成.
	if( FAILED(
		m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_pVertexBuffer ) ) )
	{
		MessageBox( NULL, "頂点バッファ作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}


	//頂点バッファをセット.
	UINT stride = sizeof( SpriteVertex );//データ間隔.
	UINT offset = 0;//開始位置.
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, 
		&m_pVertexBuffer, &stride, &offset );


	//テクスチャ用のサンプラー構造体.
	D3D11_SAMPLER_DESC	SamDesc;
	ZeroMemory( &SamDesc, sizeof( SamDesc ) );
	//リニアフィルター(線型補間).
	//	POINT:高速だが粗い.
	SamDesc.Filter	= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//ラッピングモード.
	//	WRAP:繰り返し.
	//	MIRROR	:反転繰り返し.
	//	CLAMP	:端の模様を引き延ばす.
	//	BORDER	;別途境界色を決める.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;


	//サンプラー作成.
	if( FAILED(
		m_pDevice11->CreateSamplerState(
			&SamDesc, &m_pSampleLinear ) ) )//(out)サンプラー.
	{
		MessageBox( NULL, "サンプラ作成失敗", "エラー", MB_OK );
		return E_FAIL;
	}

	//テクスチャ作成.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,		//リソースを使用するデバイスへのポインタ.
//			"Data\\Image\\MissonUI\\Lockon.png",	//ファイル名(パスも必要).
			sTexName,
			NULL, NULL,
			&m_pTexture,	//(out)テクスチャ.
			NULL ) ) )
	{
		MessageBox( NULL, "テクスチャ作成失敗w", "InitModel()", MB_OK );
		return E_FAIL;
	}


	return S_OK;
}

//============================================================
//描画(レンダリング)(※DX9MESH内とMain内で2つ存在するので注意).
//============================================================
void clsSprite::Render( const D3DXMATRIX& mView, const D3DXMATRIX& mProj, const D3DXVECTOR3 &vEye, bool isBillBoard )
{
	//ワールド行列.
	D3DXMATRIX	mWorld, mScale, mTrans, mYaw, mPitch, mRoll;
	//ワールド変換(表示位置を設定する).
	D3DXMatrixIdentity( &mWorld );	//初期化:単位行列作成.

	//拡縮.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );

	//平行移動.
	D3DXMatrixTranslation( &mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z );

	D3DXMatrixRotationZ( &mRoll,	m_vRot.z );
	D3DXMatrixRotationY( &mYaw,		m_vRot.y );
	D3DXMatrixRotationX( &mPitch,	m_vRot.x );

	//合算.
	mWorld = mScale * mYaw * mPitch * mRoll * mTrans;

	//使用するシェーダの登録.
	m_pDeviceContext11->VSSetShader( m_pVertexShader, NULL, 0 );
	m_pDeviceContext11->PSSetShader( m_pPixelShader,  NULL, 0 );

	if( isBillBoard ){
		//ビルボード用.
		D3DXMATRIX CancelRotation = mView;//ビュー(カメラ)行列.
		CancelRotation._41 =
			CancelRotation._42 = CancelRotation._43 = 0;//xyzを0にする.
		//CancelRotationの逆行列を求める.
		D3DXMatrixInverse( &CancelRotation, NULL, &CancelRotation );
		mWorld = CancelRotation * mWorld;
	}


	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITESHADER_CONSTANT_BUFFER cd;	//コンスタントバッファ.
	//バッファ内のデータの書き方開始時にmap.
	if( SUCCEEDED(
		m_pDeviceContext11->Map(
			m_pConstantBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//ワールド,カメラ,プロジェクション行列を渡す.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose( &m, &m );	//行列を転置する.
					//行列の計算方法がDirectXとGPUで異なるため.
		cd.mWVP = m;

		//カラー.
		D3DXVECTOR4 vColor( 1.0f, 1.0f, 1.0f, 1.0f );
		cd.vColor = vColor;
		D3DXVECTOR4 vSplit( 1.0f, 1.0f, 1.0f, 1.0f );
		cd.vSplit = vSplit;
		cd.vSplit.x = m_vSplit.x;
		cd.vSplit.y = m_vSplit.y;

		//テクスチャ座標.
		D3DXVECTOR4 texUV( 0.0f, 0.0f, 0.0f, 0.0f );
//		if( m_AnimCount % 8 == 0 ){
//			texUV.x += 1.0f / 8.0f;
//			m_AnimCount = 0;
//			if( texUV.x >= 1.0f ){
//				texUV.x = 0.0f;
//				texUV.y += 1.0f / 8.0f;
//			}
//		}
//		m_AnimCount ++;

		cd.vUV = texUV;



		memcpy_s( pData.pData, pData.RowPitch,
			(void*)( &cd ), sizeof( cd ) );

		m_pDeviceContext11->Unmap( m_pConstantBuffer, 0 );
	}

	//このコンスタントバッファをどのシェーダで使うか?.
	m_pDeviceContext11->VSSetConstantBuffers(
		2, 1, &m_pConstantBuffer );
	m_pDeviceContext11->PSSetConstantBuffers(
		2, 1, &m_pConstantBuffer );

	//頂点バッファをセット.
	UINT stride = sizeof( SpriteVertex );
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset );

	//頂点インプットレイアウトをセット.
	m_pDeviceContext11->IASetInputLayout( m_pVertexLayout );

	//プリミティブ・トポロジーをセット.
	m_pDeviceContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャをシェーダに渡す.
	m_pDeviceContext11->PSSetSamplers(
		0, 1, &m_pSampleLinear );	//サンプラーをセット.
	m_pDeviceContext11->PSSetShaderResources(
		0, 1, &m_pTexture );		//テクスチャをシェーダに渡す.

	//アルファブレンド用ブレンドステート作成&設定.
	SetBlend( true );


	//プリミティブをレンダリング.
	m_pDeviceContext11->Draw( 4, 0 );

	//アルファブレンドを無効にする.
	SetBlend( false );

}



D3DXVECTOR3 clsSprite::GetPos()
{
	return m_vPos;
}

void clsSprite::SetPos( const D3DXVECTOR3& vPos )
{
	m_vPos = vPos;
}

void clsSprite::AddPos( const D3DXVECTOR3& vPos )
{
	m_vPos += vPos;
}


D3DXVECTOR3 clsSprite::GetRot()
{
	return m_vRot;
}

void clsSprite::SetRot( const D3DXVECTOR3& vRot )
{
	m_vRot = vRot;
}

void clsSprite::AddRot( const D3DXVECTOR3& vRot )
{
	m_vRot += vRot;
}


D3DXVECTOR3 clsSprite::GetScale()
{
	return m_vScale;
}

void clsSprite::SetScale( const D3DXVECTOR3& vScale )
{
	m_vScale = vScale;
}

void clsSprite::AddScale( const D3DXVECTOR3& vScale )
{
	m_vScale += vScale;
}

