#include "Sprite2D.h"

//シェーダファイル名(ディレクトリも含む)
const char SHADER_NAME[] = "Shader\\Sprite2D.hlsl";

//コンストラクタ.
clsSprite2D::clsSprite2D() :
	m_pVertexShader( nullptr ),	
	m_pVertexLayout( nullptr ),	
	m_pPixelShader( nullptr ),	
	m_pConstantBuffer( nullptr ),
	m_pVertexBuffer( nullptr ),	
	m_pTexture( nullptr ),		
	m_pSampleLinear( nullptr )	
{
//	ZeroMemory( this, sizeof( clsSprite2D ) );


	//アルファ値の設定.
	m_fAlpha = 1.0f;	//0:透明, 1:不透明.
	m_vPos = m_vScale = { 1.0f, 1.0f, 0.0f };
	m_fPatternNo = { 0.0f, 0.0f };
}

//デストラクタ.
clsSprite2D::~clsSprite2D()
{
	Release();
}

//初期化.
HRESULT clsSprite2D::Create(
	ID3D11Device* const pDevice11, ID3D11DeviceContext* const pContext11,
	LPSTR const fileName, const SPRITE_STATE ss )
{
	m_pDevice11 = pDevice11;
	m_pDeviceContext11 = pContext11;

	m_SState = ss;

	if( FAILED( InitShader() ))
	{
		return E_FAIL;
	}
	if( FAILED( InitModel( ss )))
	{
		return E_FAIL;
	}
	if( FAILED( CreateTexture( fileName, &m_pTexture )))
	{
		return E_FAIL;
	}

	return S_OK;
}

//======================================
//	HLSLファイルを読み込みシェーダを作成する.
//	HLSL:HIGH-LEVEL-SHADER-LANGUAGE.
//======================================
HRESULT clsSprite2D::InitShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからバーテックスシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME,	//シェーダファイル名(HLSLファイル)
		NULL,			//マクロ定義の配列へのポインタ(未使用)
		NULL,			//インクルードファイルを扱うインターフェイスへのポインタ(未使用)
		"VS_Main",		//シェーダエントリーポイント関数の名前.
		"vs_5_0",		//シェーダのモデルを指定する文字列(プロファイル)
		uCompileFlag,	//シェーダコンパイルフラグ.
		0,				//エフェクトコンパイルフラグ(未使用)
		NULL,			//スレッドポンプインターフェイスへのポインタ(未使用)
		&pCompiledShader,//ブロブを格納するメモリへのポインタ.
		&pErrors,		//エラーと警告一覧を格納するメモリへのポインタ.
		NULL)))		//戻り値へのポインタ(未使用)
	{
		MessageBox(NULL, "hlsl読込失敗", "エラー", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pVertexShader)))//(out)バーテックスシェーダ.
	{
		MessageBox(NULL, "vs作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",				//位置.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,//DXGIのフォーマット(32bit float型*3)
			0,
			0,							//データの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",		//テクスチャ位置.
			0,
			DXGI_FORMAT_R32G32_FLOAT,	//DXGIのフォーマット(32bit float型*2)
			0,
			12,							//テクスチャデータの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトの作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
		layout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		&m_pVertexLayout)))//(out)頂点インプットレイアウト.
	{
		MessageBox(NULL, "頂点インプットレイアウト作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);


	//HLSLからピクセルシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME,	//シェーダファイル名(HLSLファイル)
		NULL,
		NULL,
		"PS_Main",		//シェーダエントリーポイント関数の名前.
		"ps_5_0",		//シェーダのモデルを指定する文字列(プロファイル)
		uCompileFlag,	//シェーダコンパイルフラグ.
		0,
		NULL,
		&pCompiledShader,//ブロブを格納するメモリへのポインタ.
		&pErrors,
		NULL)))
	{
		MessageBox(NULL, "hlsl(ps)読込失敗", "エラー", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pPixelShader)))//(out)ピクセルシェーダ.
	{
		MessageBox(NULL, "ps作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);//ブロブ解放.



	//コンスタント(定数)バッファ作成 シェーダに特定の数値を送るバッファ.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//コンスタントバッファを指定.
	cb.ByteWidth = sizeof(SPRITE2D_CONSTANT_BUFFER);//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//書き込みでアクセス.
	cb.MiscFlags = 0;//その他のフラグ(未使用)
	cb.StructureByteStride = 0;//構造体のサイズ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;//使用方法:直接書き込み.

	//コンスタントバッファ作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer)))
	{
		MessageBox(NULL, "コンスタントバッファ作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}



//================================================
//	モデル作成.
//================================================
HRESULT clsSprite2D::InitModel( SPRITE_STATE ss )
{
	float fW = ss.Disp.w;	//表示スプライト幅.
	float fH = ss.Disp.h;	//表示スプライト高さ.
	float fU = 1.0f/ ss.Anim.w;	//１コマ当たりの幅.
	float fV = 1.0f/ ss.Anim.h;	//１コマ当たりの高さ.

	//板ポリ(四角形)の頂点を作成.
	SpriteVertex vertices[] =
	{
		//頂点座標(x,y,z)				UV座標(u,v)
		D3DXVECTOR3( 0.0f,   fH, 0.0f), D3DXVECTOR2(0.0f,   fV),//頂点1(左下).
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),//頂点2(左上).
		D3DXVECTOR3(   fW,   fH, 0.0f), D3DXVECTOR2(  fU,   fV),//頂点3(右下).
		D3DXVECTOR3(   fW, 0.0f, 0.0f), D3DXVECTOR2(  fU, 0.0f)	//頂点4(右上).
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;//使用方(デフォルト)
	bd.ByteWidth = sizeof(SpriteVertex) * uVerMax;//頂点サイズ(頂点×4)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//頂点バッファとして扱う.
	bd.CPUAccessFlags = 0;	//CPUからはアクセスしない.
	bd.MiscFlags = 0;	//その他のフラグ(未使用)
	bd.StructureByteStride = 0;//構造体のサイズ(未使用)

	//サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;//板ポリの頂点をセット.

	//頂点バッファの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox(NULL, "頂点バッファ作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}


	//頂点バッファをセット.
	UINT stride = sizeof(SpriteVertex);//データ間隔.
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer, &stride, &offset);


	//テクスチャ用のサンプラー構造体.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	//リニアフィルタ-(線形補間)
	//	POINT:高速だが粗い.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//ラッピングモード.
	//	WRAP  :繰り返し
	//	MIRROR:反転繰り返し.
	//	CLAMP :端の模様を引き伸ばす.
	//	BORDER:別途境界色を決める.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//サンプラー作成.
	if (FAILED(
		m_pDevice11->CreateSamplerState(
		&SamDesc, &m_pSampleLinear)))//(out)サンプラー.
	{
		MessageBox(NULL, "サンプラ作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//================================================
//	テクスチャ作成.
//================================================
HRESULT clsSprite2D::CreateTexture(
	LPSTR fileName,
	ID3D11ShaderResourceView** pTex )
{
	//テクスチャ作成.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,	//リソースを使用するデバイスへのポインタ.
		fileName,		//ファイル名.
		NULL, NULL,
		pTex,			//(out)テクスチャ.
		NULL )))
	{
		ERR_MSG( fileName, "clsSprite2D::CreateTexture()" );
		return E_FAIL;
	}

	return S_OK;
}


//板ポリゴンを画面にレンダリング.
void clsSprite2D::Render()
{
	//ワールド行列.
	D3DXMATRIX	mWorld, mTrans, mScale;

	//ワールド変換(表示位置を設定する)
	D3DXMatrixIdentity( &mWorld );	//初期化:単位行列作成.
	D3DXMatrixIdentity( &mTrans );	//初期化:単位行列作成.
	D3DXMatrixIdentity( &mScale );	//初期化:単位行列作成.

	//拡縮.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );


	//平行移動.
	D3DXMatrixTranslation(&mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z);

	//合成.
	mWorld = mTrans * mScale;

	//使用するシェーダの登録.
	m_pDeviceContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext11->PSSetShader(m_pPixelShader, NULL, 0);


	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITE2D_CONSTANT_BUFFER cd;	//コンスタントバッファ.
	//バッファ内のデータの書き方開始時にmap.
	if (SUCCEEDED(
		m_pDeviceContext11->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド行列を渡す.
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose(&m, &m);	//行列を転置する.					
		//行列の計算方法がDirectXとGPUで異なる為.
		cd.mW = m;

		//ビューポートの幅、高さを渡す.
		cd.fViewPortWidth = WND_W;
		cd.fViewPortHeight= WND_H;

		//アルファ値を渡す.
		cd.fAlpha = m_fAlpha;

		//UV座標.
		//１コマ当たりの割合にコマ番号を掛けて位置を設定する.
		cd.vUV.x = ( 1.0f / m_SState.Anim.w ) * m_fPatternNo.x;
		cd.vUV.y = ( 1.0f / m_SState.Anim.h ) * m_fPatternNo.y;

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cd), sizeof(cd));

		m_pDeviceContext11->Unmap(m_pConstantBuffer, 0);
	}

	//このコンスタントバッファをどのシェーダで使うか？.
	m_pDeviceContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);
	m_pDeviceContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);

	//頂点バッファをセット.
	UINT stride = sizeof(SpriteVertex);	//データの間隔.
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット.
	m_pDeviceContext11->IASetInputLayout(m_pVertexLayout);

	//プリミティブ・トポロジーをセット.
	m_pDeviceContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャをシェーダに渡す.
	m_pDeviceContext11->PSSetSamplers(
		0, 1, &m_pSampleLinear);	//サンプラ-をセット.
	m_pDeviceContext11->PSSetShaderResources(
		0, 1, &m_pTexture);		//テクスチャをシェーダに渡す.

	//アルファブレンド用ブレンドステート作成＆設定.
	SetBlend(true);

	//プリミティブをレンダリング.
	m_pDeviceContext11->Draw(4, 0);

	//アルファブレンドを無効にする.
	SetBlend(false);
}


void clsSprite2D::Release()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );


//	m_pVertexShader = nullptr;
//	m_pVertexLayout = nullptr;
//	m_pPixelShader = nullptr;	
//	m_pConstantBuffer = nullptr;
//	m_pVertexBuffer = nullptr;
//	m_pTexture = nullptr;
//	m_pSampleLinear = nullptr;
}



void clsSprite2D::SetPos( const D3DXVECTOR3 &vPos )
{
	m_vPos = vPos;
}

void clsSprite2D::SetScale( const D3DXVECTOR3 &vScale )
{
	m_vScale = vScale;
}

void clsSprite2D::SetScale( const float &fScale, const bool withZ )
{
	m_vScale.x = fScale;
	m_vScale.y = fScale;

	if( withZ ) return;
	m_vScale.z = fScale;
}

void clsSprite2D::SetAlpha( const float fAlpha )
{
	m_fAlpha = fAlpha;
}
bool clsSprite2D::AddAlpha( const float fAlpha )
{
	//範囲内.
	bool isWithInRange = true;

	m_fAlpha += fAlpha;

	//範囲確認.
	if( m_fAlpha < 0.0f ){
		m_fAlpha = 0.0f;
		isWithInRange = false;
	}
	else if( m_fAlpha > 1.0 ){
		m_fAlpha = 1.0f;
		isWithInRange = false;
	}

	return isWithInRange;
}


D3DXVECTOR3 clsSprite2D::GetPos() const
{
	return m_vPos;
}
D3DXVECTOR3 clsSprite2D::GetScale() const
{
	return m_vScale;
}



void clsSprite2D::AddPos( const D3DXVECTOR3 &vPos )
{
	m_vPos += vPos;
}

void clsSprite2D::AddScale( const D3DXVECTOR3 &vScale )
{
	m_vScale.x *= vScale.x;
	m_vScale.y *= vScale.y;
	m_vScale.z *= vScale.z;
}

void clsSprite2D::AddScale( const float &fScale, const bool withZ )
{
	m_vScale.x *= fScale;
	m_vScale.y *= fScale;

	if( withZ ) return;
	m_vScale.z *= fScale;
}
