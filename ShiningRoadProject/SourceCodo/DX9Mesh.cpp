#include "DX9Mesh.h"
#include "OperationString.h"

namespace{
	//シェーダーファイル名(ディレクトリを含む)
	const char SHADER_NAME[] = "Shader\\Mesh.hlsl";//const:後に書かれた変数を上書きさせない.

	const char sMASK_TEX_NAME[] = "mask";
	const char sMASK_TEX_TYPE[] = ".png";
	const char sMASK_PATH_EMPTY[] = "Data\\Image\\maskEmpty.png";
	//============================================================
	//	構造体.
	//============================================================
	//コンスタントバッファのアプリ側の定義(Mesh.hlsl).
	//シェーダ内のコンスタントバッファと一致している必要あり.
	//hlslと連動している(疑ったほうがいいシリーズ).
	struct MESHSHADER_CONSTANT_BUFFER_ZERO
	{
		D3DXMATRIX	mW;			//ワールド(位置)座標行列.
		D3DXMATRIX	mWVP;		//ワールド,ビュー,射影の合成変換行列.
		D3DXVECTOR4 vLightDir;	//ライト方向.
		D3DXVECTOR4	vEye;		//カメラ位置(視点位置).
		D3DXVECTOR4 vColor;		//カラー.
	};
	struct MESHSHADER_CONSTANT_BUFFER_FIRST
	{
		D3DXVECTOR4 vAmbient;	//アンビエント.
		D3DXVECTOR4 vDiffuse;	//ディフューズ色.
		D3DXVECTOR4	vSpecular;	//スペキュラ色.
	};


	//頂点の構造体.
	struct MeshVertex
	{
		D3DXVECTOR3 vPos;	//頂点座標(x,y,z).
		D3DXVECTOR3 vNormal;//法線(陰影計算に必須).
		D3DXVECTOR2 vTex;	//テクスチャ座標.
	};



}

//========================================================
// コンストラクタ.
//========================================================
clsDX9Mesh::clsDX9Mesh() 
	:m_pMesh( nullptr )
	,m_pMeshForRay( nullptr )
//	,m_hWnd( nullptr )
	,m_pDevice( nullptr )
	,m_pContext( nullptr )
	,m_pVertexShader( nullptr )
	,m_pVertexLayout( nullptr )
	,m_pPixelShader( nullptr )
	,m_pConstantBuffer0( nullptr )
	,m_pConstantBuffer1( nullptr )
	,m_pVertexBuffer( nullptr )
	,m_ppIndexBuffer( nullptr )
	,m_pSampleLinear( nullptr )
//	,m_pBlendState( nullptr )
{
//	ZeroMemory(this, sizeof(clsDX9Mesh));

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		m_pBlendState[i] = nullptr;
	}

	m_Trans.vScale = {1.0f,1.0f,1.0f};
}

//========================================================
// デストラクタ.
//========================================================
clsDX9Mesh::~clsDX9Mesh()
{
	SAFE_RELEASE( m_pSampleLinear );

	SAFE_RELEASE( m_pConstantBuffer1 );
	SAFE_RELEASE( m_pConstantBuffer0 );

	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pVertexShader );


	if( m_ppIndexBuffer ){
		for( DWORD i=0; i<m_dwNumMaterials; i++ ){
			SAFE_RELEASE( m_ppIndexBuffer[i] );	
		}
		SAFE_DELETE_ARRAY( m_ppIndexBuffer );
	}

	SAFE_DELETE_ARRAY( m_pMaterials );

	SAFE_RELEASE( m_pMesh );
	SAFE_RELEASE( m_pMeshForRay );

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		SAFE_RELEASE( m_pBlendState[i] );
	}

	m_pContext = nullptr;
	m_pDevice = nullptr;
//	m_hWnd = nullptr;

}




//========================================================
// 初期化.
//========================================================
HRESULT clsDX9Mesh::Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPSTR fileName )
{
//	m_hWnd = hWnd;
	m_pDevice = pDevice11;
	m_pContext = pContext11;
	LPDIRECT3DDEVICE9	pDevice9 = NULL;	//Dx9デバイスオブジェクト.

	if( FAILED( CreateBlendState() ) ){
		return E_FAIL;
	}
	if( FAILED( InitDx9( hWnd, &pDevice9, fileName ) ) ){
		return E_FAIL;
	}
	if( FAILED( LoadXMesh( fileName, pDevice9 ) ) ){
		return E_FAIL;
	}
	if( FAILED( InitShader( fileName ) ) ){
		return E_FAIL;
	}

	SAFE_RELEASE( pDevice9 );

	return S_OK;
}


//========================================================
//Dx9初期化.
//========================================================
HRESULT clsDX9Mesh::InitDx9( HWND hWnd, LPDIRECT3DDEVICE9* pOutDevice9, const LPSTR sErrFilePath  )
{
	//「Direct3D」オブジェクトの作成.
	LPDIRECT3D9	pD3d = NULL;	//DX9オブジェクト.
	pD3d = Direct3DCreate9( D3D_SDK_VERSION );
	if( pD3d == NULL ){
		MessageBox( NULL, "Dx9オブジェクト作成失敗", sErrFilePath, MB_OK );
		return E_FAIL;
	}

	//Direct3Dデバイスオブジェクトの作成.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			//バックバッファのフォーマット(デフォルト)
	d3dpp.BackBufferCount			= 1;						//バックバッファの数.
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	//スワップエフェクト(デフォルト)
	d3dpp.Windowed					= true;						//ウィンドウモード.
	d3dpp.EnableAutoDepthStencil	= true;						//ステンシル有効.
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				//ステンシルのフォーマット(16bit)

	//デバイス作成(HALモード:描画と頂点処理をGPUで行う)
	if( FAILED(pD3d->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, pOutDevice9 ) ) )
	{
		//デバイス作成(HALモード:描画はGPU、頂点処理をCPUで行う)
		if( FAILED(pD3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, pOutDevice9 ) ) )
		{
			MessageBox(NULL, "HALモードでデバイスを作成できません\nREFモードで再試行します", sErrFilePath, MB_OK);

			//デバイス作成(REFモード:描画はCPU、頂点処理をGPUで行う)
			if( FAILED(pD3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, pOutDevice9 ) ) )
			{
				//デバイス作成(REFモード:描画と頂点処理をCPUで行う)
				if( FAILED(pD3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, pOutDevice9 ) ) )
				{
					MessageBox(NULL, "Direct3Dデバイス作成失敗", sErrFilePath, MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	SAFE_RELEASE( pD3d );

	return S_OK;
}

//========================================================
//Xファイルからメッシュをロードする.
//========================================================
HRESULT clsDX9Mesh::LoadXMesh( LPSTR fileName, LPDIRECT3DDEVICE9 pDevice9 )
{
	//マテリアルバッファ.
	LPD3DXBUFFER pD3DXMtrlBuffer;

	//レイとメッシュの判定のために追加.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//ファイル名.
		D3DXMESH_SYSTEMMEM,	//システムメモリに読み込み.
		pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)マテリアル情報.
		NULL,
		&m_dwNumMaterials,	//(out)マテリアル数.
		&m_pMeshForRay)))			//(out)メッシュオブジェクト.
	{

		MessageBox(NULL, fileName, "Xファイル読み込み失敗", MB_OK);//グラフィッカー案件.
		return E_FAIL;
	}

	//Xファイルのロード.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//ファイル名.
		D3DXMESH_SYSTEMMEM	//システムメモリに読み込み.
		| D3DXMESH_32BIT, 
		pDevice9, NULL, 
		&pD3DXMtrlBuffer,	//(out)マテリアル情報.
		NULL, 
		&m_dwNumMaterials,	//(out)マテリアル数.
		&m_pMesh)))			//(out)メッシュオブジェクト.
	{

		MessageBox(NULL, fileName, "Xファイル読み込み失敗", MB_OK);//グラフィッカー案件.
		return E_FAIL;
	}

	D3D11_BUFFER_DESC bd;//Dx11バッファ構造体.
	D3D11_SUBRESOURCE_DATA InitData;//初期化データ.

	//読み込んだ情報から必要な情報を抜き出す.
	D3DXMATERIAL* d3dxMaterials
		= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//マテリアル数分の領域を確保.
	m_pMaterials = new MY_MATERIAL[m_dwNumMaterials];
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterials];
	//マテリアル数分繰り返し.
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		m_ppIndexBuffer[i] = nullptr;
		//マテリアル情報のコピー.
		m_pMaterials[i].Ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_pMaterials[i].Ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_pMaterials[i].Ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_pMaterials[i].Ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//ディフューズ.
		m_pMaterials[i].Diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_pMaterials[i].Diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_pMaterials[i].Diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_pMaterials[i].Diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//スペキュラ.
		m_pMaterials[i].Specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_pMaterials[i].Specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_pMaterials[i].Specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_pMaterials[i].Specular.w = d3dxMaterials[i].MatD3D.Specular.a;

		//(その面に)テクスチャが貼られているか？.
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			const int iTEX_PATH_SIZE = 128;
			m_bTexture = true;//テクスチャフラグを立てる.

			char path[ iTEX_PATH_SIZE ] = "";
			int path_cnt = lstrlen( fileName );

			//階層がある前提なのでファイルを直下に置くと死.
			for( int k = path_cnt; k >= 0; k-- )
			{
				if (fileName[k] == '\\')
				{
					for (int j = 0; j <= k; j++)
					{
						path[j] = fileName[j];
					}
					path[k + 1] = '\0';
					break;
				}
			}

			clsOPERATION_STRING OprtStr;
			//テクスチャファイル名をコピー.
			char sTexFilePath[ iTEX_PATH_SIZE ] = "";
			strcpy_s( sTexFilePath,  sizeof( sTexFilePath ),  path );
			strcat_s( sTexFilePath,  sizeof( path ),		  d3dxMaterials[i].pTextureFilename );
			char sMaskFilePath[ iTEX_PATH_SIZE ] = "";
			strcpy_s( sMaskFilePath, sizeof( sMaskFilePath ), path );
			strcat_s( sMaskFilePath, sizeof( sMaskFilePath ), sMASK_TEX_NAME );
			strcpy_s( sMaskFilePath, sizeof( sMaskFilePath ), OprtStr.ConsolidatedNumber( sMaskFilePath, static_cast<int>( i ) ).c_str() );
			strcat_s( sMaskFilePath, sizeof( sMaskFilePath ), sMASK_TEX_TYPE );

//			strcpy_s( sTexFilePath,
//				sizeof( sTexFilePath ),
//				path );sMASK_PATH_EMPTY
//
			//テクスチャ作成.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice, sTexFilePath,//テクスチャファイル名.
				NULL, NULL,
				&m_pMaterials[i].pTexture, //(out)テクスチャオブジェクト.
				NULL ) ) )
			{
				MessageBox( NULL, sTexFilePath, "テクスチャ作成失敗", MB_OK );
				return E_FAIL;
			}

			//マスク作成.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice, sMaskFilePath,//テクスチャファイル名.
				NULL, NULL,
				&m_pMaterials[i].pMask, //(out)テクスチャオブジェクト.
				NULL ) ) )
			{
				if( FAILED( D3DX11CreateShaderResourceViewFromFile(
					m_pDevice, sMASK_PATH_EMPTY,//テクスチャファイル名.
					NULL, NULL,
					&m_pMaterials[i].pMask, //(out)テクスチャオブジェクト.
					NULL ) ) )
				{
					MessageBox( NULL, sTexFilePath, "マスク作成失敗", MB_OK );
					return E_FAIL;
				}
			}
		}
	}

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	// インデックスバッファの作成.
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	//メッシュの属性情報を得る.
	//属性情報でインデックスバッファから細かいマテリアルごとのインデックスバッファを分離できる.
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	//メッシュの面および頂点の順番変更を制御し、パフォーマンスを最適化する.
	//D3DXMESHOPT_COMPACT:面の順番を変更し、使用されていない頂点と面を削除する.
	//D3DXMESHOPT_ATTRSORT:パフォーマンスを上げる為、面の順番を変更して最適化を行う.
	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
		0, 0, 0, 0);
	//属性テーブルの取得.
	m_pMesh->GetAttributeTable(NULL, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(
		m_pMesh->GetAttributeTable(pAttrTable,&m_NumAttr)))
	{
		MessageBox( NULL, "属性テーブル取得失敗", fileName, MB_OK );
		return E_FAIL;
	}

	//ロックしないと取り出せない.
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(
		D3DLOCK_READONLY, (void**)&pIndex);

	//属性ごとのインデックスバッファを作成.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		m_AttrID[i] = pAttrTable[i].AttribId;

		//Dx9のインデックスバッファからの情報でDx11のインデックスバッファを作成.
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * pAttrTable[i].FaceCount * 3;	//面数ｘ3で頂点数.
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		//大きなインデックスバッファ内のオフセット(ｘ3する)
		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];

		if (FAILED(
			m_pDevice->CreateBuffer(
			&bd, &InitData, &m_ppIndexBuffer[i])))
		{
			MessageBox( NULL, "インデックスバッファ作成失敗", fileName, MB_OK );
			return E_FAIL;
		}
		//面の数をコピー.
		m_pMaterials[m_AttrID[i]].dwNumFace = pAttrTable[i].FaceCount;
	}
	
	delete[] pAttrTable;//属性テーブルの削除.
	m_pMesh->UnlockIndexBuffer();

	//不要になったマテリアルバッファを解放.
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	// 頂点(バーテックス)バッファの作成.
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	//Dx9の場合、mapではなくロックでバーテックスバッファからデータを取り出す.
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	MeshVertex* pVertex = NULL;
	if (SUCCEEDED(
		pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertex = (MeshVertex*)pVertices;
		//Dx9のバーテックスバッファからの情報で、Dx11のバーテックスバッファを作成.
		bd.Usage = D3D11_USAGE_DEFAULT;
		//頂点を格納するのに必要なバイト数.
		bd.ByteWidth = m_pMesh->GetNumBytesPerVertex() * m_pMesh->GetNumVertices();//頂点数.
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pVertex;

		//テクスチャ座標がマイナス対策.
		if (m_bTexture)//※テクスチャがあれば処理する.
		{
			//頂点数分繰り返し.
			for (DWORD i = 0; i < m_pMesh->GetNumVertices(); i++)
			{
				if (pVertex[i].vTex.x < 0.0f){
					pVertex[i].vTex.x += 1.0f;
				}
				if (pVertex[i].vTex.y < 0.0f){
					pVertex[i].vTex.y += 1.0f;
				}
			}
		}

		if (FAILED(
			m_pDevice->CreateBuffer(
			&bd, &InitData, &m_pVertexBuffer)))
		{
			MessageBox( NULL, "頂点(バーテックス)バッファ作成失敗", fileName, MB_OK );
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);//バーテックスバッファ解放.

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	// サンプラー作成.
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	//テクスチャ用のサンプラー構造体.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	//リニアフィルター(線形補間)
	//POINT:高速だが粗い.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	//ラッピングモード(WMAP:繰り返し)
	//MIRROR:反転折り返し.
	//CLAMP:橋の模様を引き延ばす.
	//BORDER:別途境界色を決める.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//サンプラー作成.
	if (FAILED(
		m_pDevice->CreateSamplerState(
		&SamDesc, &m_pSampleLinear)))//(out)サンプラー.
	{
		MessageBox(NULL, "サンプラー作成失敗", fileName, MB_OK);
		return E_FAIL;
	}


	return S_OK;
}

//========================================================
//HLSLファイルを読み込みシェーダを作成する.
//HLSL:HIGH-LEVEL-SHADER-LANGUAGE.
//========================================================
HRESULT clsDX9Mesh::InitShader( const LPSTR sErrFilePath  )
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG.

	//HLSLからバーテックスシェーダのブロブの作成.
	if (m_bTexture)
	{
		if (FAILED(
			D3DX11CompileFromFile(
			SHADER_NAME, NULL, NULL,
			"VS_Main", "vs_5_0",
			uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl(VS_Main)読み込み失敗", sErrFilePath, MB_OK);
			return E_FAIL;
		}
	}

	else
	{
		if (FAILED(
			D3DX11CompileFromFile(
			SHADER_NAME, NULL, NULL,
			"VS_NoTex", "vs_5_0",
			uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl(VS_NoTex)読み込み失敗", sErrFilePath, MB_OK);
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if (FAILED(
		m_pDevice->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pVertexShader)))//(out)バーテックスシェーダ.
	{
		MessageBox(NULL, "vs作成失敗", sErrFilePath, MB_OK);
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = 0;//sizeof(layout) / sizeof(layout[0]);
	if (m_bTexture)
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{
				"POSITION",						//位置.
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのフォーマット (32bit float型 * 3)
				0,
				0,								//データの開始位置.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"NORMAL",						//法線.
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのフォーマット (32bit float型 * 3)
				0,
				12,								//データの開始位置.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"TEXCOORD",						//テクスチャの位置.
				0,
				DXGI_FORMAT_R32G32_FLOAT,		//DXGIのフォーマット(32bit float型 * 2)
				0,
				24,								//テクスチャデータの開始位置.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);//要素数算出.
		memcpy_s(layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{
				"POSITION",						//位置.
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのフォーマット (32bit float型 * 3)
				0,
				0,								//データの開始位置.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"NORMAL",						//法線.
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのフォーマット (32bit float型 * 3)
				0,
				12,								//データの開始位置.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);//要素数算出.
		memcpy_s(layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);
	}
	
	//頂点インプットレイアウト作成.
	if (FAILED(
		m_pDevice->CreateInputLayout(
		layout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		&m_pVertexLayout)))//(out)頂点インプットレイアウト.
	{
		MessageBox(NULL, "頂点インプットレイアウト作成失敗", sErrFilePath, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからピクセルシェーダのブロブの作成.
	if (m_bTexture)
	{
		if (FAILED(D3DX11CompileFromFile(SHADER_NAME,NULL,NULL,
			"PS_Main","ps_5_0",
			uCompileFlag,0,NULL,&pCompiledShader,&pErrors,NULL)))
		{
			MessageBox(NULL, "hlsl読み込み失敗(PS_Main)", sErrFilePath, MB_OK);
			return E_FAIL;
		}
	}

	else
	{
		if (FAILED(D3DX11CompileFromFile(SHADER_NAME, NULL, NULL,
			"PS_NoTex", "ps_5_0",
			uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl読み込み失敗(PS_NoTex)", sErrFilePath, MB_OK);
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if (FAILED(
		m_pDevice->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pPixelShader)))//(out)ピクセルシェーダ.
	{
		MessageBox(NULL, "ps作成失敗", sErrFilePath, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);

	//コンスタント(定数)バッファ作成 シェーダに特定の数値を送るバッファ.
	//ここでは変換行列渡し用.
	//シェーダにWorld,View,Projection行列を渡す.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//コンスタントバッファを指定.
	cb.ByteWidth = sizeof(MESHSHADER_CONSTANT_BUFFER_ZERO);	//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//書き込みでアクセス.
	cb.MiscFlags = 0;									//その他のフラグ(未使用)
	cb.StructureByteStride = 0;									//構造体のサイズ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;					//使用方法:直接の書き込み.

	//コンスタントバッファ作成.
	if (FAILED(m_pDevice->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer0)))
	{
		MessageBox(NULL, "コンスタントバッファ作成失敗", sErrFilePath, MB_OK);
		return E_FAIL;
	}

	//マテリアル用のコンスタントバッファ.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//コンスタントバッファを指定.
	cb.ByteWidth = sizeof(MESHSHADER_CONSTANT_BUFFER_FIRST);	//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//書き込みでアクセス.
	cb.MiscFlags = 0;									//その他のフラグ(未使用)
	cb.StructureByteStride = 0;									//構造体のサイズ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;					//使用方法:直接の書き込み.

	//コンスタントバッファ作成.
	if (FAILED(m_pDevice->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer1)))
	{
		MessageBox(NULL, "コンスタントバッファ作成失敗", sErrFilePath, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//レンダリング用(※DX9MESH内とMain内の二つに存在するので注意)
void clsDX9Mesh::Render( const D3DXMATRIX& mView,	const D3DXMATRIX& mProj,
						 const D3DXVECTOR3& vLight,	const D3DXVECTOR3& vEye,
						 const D3DXVECTOR4& vColor,	const bool isAlpha )
{
	D3DXMATRIX mWorld, mScale, mYaw, mPitch, mRoll,mTrans;

	//ワールド変換(表示位置を設定する)
	//拡縮.
	D3DXMatrixScaling(
		&mScale, m_Trans.vScale.x, m_Trans.vScale.y, m_Trans.vScale.z);
	//回転.
	D3DXMatrixRotationY(&mYaw, m_Trans.fYaw);//Y軸回転.
	D3DXMatrixRotationX(&mPitch, m_Trans.fPitch);
	D3DXMatrixRotationZ(&mRoll, m_Trans.fRoll);

//	//=================================================================//
//	//キャラクター移動処理(ラジコン)
//	vecAxisX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
//	vecAxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//
//	D3DXVec3TransformCoord(
//		&vecAxisX, //(out)
//		&vecAxisX,
//		&mYaw);//Y軸回転行列.
//
//	//Z軸ベクトルそのものを回転状態により変換する.
//	D3DXVec3TransformCoord(
//		&vecAxisZ, //(out)
//		&vecAxisZ, 
//		&mYaw);//Y軸回転行列.
//
//	//方向によって進行させる値を設定.
//	/*switch (m_enDir)
//	{
//	case enDirection_Foward://前進.
//		//向かう方向×進める値(0.1f)
//		m_Trans.vPos += vecAxisZ * 0.1f;
//		break;
//	case enDirection_Backward://後退.
//		m_Trans.vPos -= vecAxisZ * 0.1f;
//		break;
//	case Left:
//		m_Trans.vPos -= vecAxisX * 0.1f;
//		break;
//	case Right:
//		m_Trans.vPos += vecAxisX * 0.1f;
//		break;
//
//	case enDirection_LeftTurn:
//
//		break;
//	case enDirection_RightTurn:
//
//		break;
//	}
//	m_enDir = enDirection_Stop;//停止.*/
//	//=================================================================//


	//移動.
	D3DXMatrixTranslation( &mTrans,
		m_Trans.vPos.x, m_Trans.vPos.y, m_Trans.vPos.z );

	//合成.
//	mWorld = mScale * mYaw * mPitch * mRoll * mTrans;//ビル.
	mWorld = mScale * mRoll * mPitch * mYaw * mTrans;//もともと.

	//使用するシェーダをセット.
	m_pContext->VSSetShader( m_pVertexShader, NULL, 0);//頂点シェーダ.
	m_pContext->PSSetShader( m_pPixelShader,  NULL, 0);//ピクセルシェーダ.

	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	
	//バッファ内のデータの書き換え開始時にMap.
	if (SUCCEEDED(
		m_pContext->Map(
		m_pConstantBuffer0, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData)))
	{
		MESHSHADER_CONSTANT_BUFFER_ZERO cb;//コンスタントバッファ.

		//ワールド行列を渡す.
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//ワールド,ビュー(カメラ),プロジェクション(射影)行列を渡す.
		D3DXMATRIX m = mWorld * mView * mProj;

		D3DXMatrixTranspose(&m, &m);//行列を転置する.
		//※行列の計算方法がDirectXとGPUで異なるため.

		cb.mWVP = m;

		//ライトの方向.
		cb.vLightDir = (D3DXVECTOR4)vLight;

		//ディフューズカラーの設定.
		//ハイライトに色を付けたくない場合は白を設定.
		//cb.vDeffuse = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);

		//スペキュラを渡す.
		//cb.vSpecular = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);

		//カメラの位置(視点)をシェーダに渡す.
		cb.vEye = D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 0.0f);

		//色.
		cb.vColor = vColor;

		memcpy_s(
			pData.pData,	//コピー先のバッファ.
			pData.RowPitch,//コピー先のバッファサイズ.
			(void*)(&cb),//コピー元のバッファ.
			sizeof(cb));//コピーするサイズ.

		//バッファ内データの書き換え終了時にUnmap.
		m_pContext->Unmap(m_pConstantBuffer0, 0);
	}

	//コンスタントバッファをどのシェーダで使うか?.
	m_pContext->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer0);//頂点シェーダ.

	m_pContext->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer0);//ピクセルシェーダ.

	//頂点インプットレイアウトをセット.
	m_pContext->IASetInputLayout(m_pVertexLayout);

	//プリミティブ・トポロジーをセット.
	m_pContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//頂点バッファをセット.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer,
		&stride, &offset);

	//アルファブレンド用ブレンドステート作成.
	SetBlend( isAlpha );
	
	//属性の数だけ、それぞれの属性のインデックスバッファを描画.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		//使用されていないマテリアル対策.
		if (m_pMaterials[m_AttrID[i]].dwNumFace == 0){
			continue;
		}
		//インデックスバッファをセット.
		m_pContext->IASetIndexBuffer(
			m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//マテリアルの各要素をシェーダーに渡す.
		D3D11_MAPPED_SUBRESOURCE pData1;
		if (SUCCEEDED(
			m_pContext->Map(m_pConstantBuffer1, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData1)))
		{
			MESHSHADER_CONSTANT_BUFFER_FIRST cb;
			//アンビエント,ディフューズ,スペキュラをシェーダーに渡す.
			cb.vAmbient = m_pMaterials[m_AttrID[i]].Ambient;
			cb.vDiffuse = m_pMaterials[m_AttrID[i]].Diffuse;
			cb.vSpecular = m_pMaterials[m_AttrID[i]].Specular;

			memcpy_s(
				pData1.pData, pData1.RowPitch,
				(void*)&cb, sizeof(MESHSHADER_CONSTANT_BUFFER_FIRST));

			m_pContext->Unmap(m_pConstantBuffer1, 0);
		}

		//このコンスタントバッファをどのシェーダーで使うか?.
		m_pContext->VSSetConstantBuffers(
			1, 1, &m_pConstantBuffer1);
		m_pContext->PSSetConstantBuffers(
			1, 1, &m_pConstantBuffer1);

		//テクスチャをシェーダーに渡す.
		if (m_pMaterials[m_AttrID[i]].pTexture)
		{
			//テクスチャがあるとき.
			m_pContext->PSSetSamplers(
				0, 1, &m_pSampleLinear );
			m_pContext->PSSetShaderResources(
				0, 1, &m_pMaterials[m_AttrID[i]].pTexture );
			//マスクがあるとき.
			m_pContext->PSSetShaderResources(
				1, 1, &m_pMaterials[ m_AttrID[i] ].pMask );
		}
		else
		{
			//テクスチャがないとき.
			ID3D11ShaderResourceView* Nothing[1] = {0};
			m_pContext->PSSetShaderResources(0, 1, Nothing);
		}
		//プリミティブ(ポリゴン)をレンダリング.
		m_pContext->DrawIndexed(
			m_pMaterials[m_AttrID[i]].dwNumFace * 3, 0, 0);
	}
}

//ブレンドステート作成.
HRESULT clsDX9Mesh::CreateBlendState()
{
	//アルファブレンド用ブレンドステート作成.
	//pngファイル内にアルファ情報があるので、透過するようにブレンドステートで設定する.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( D3D11_BLEND_DESC ) );	//初期化.
	blendDesc.IndependentBlendEnable = false;			//false:RenderTarget[0]のメンバーのみ使用する。true:RenderTarget[0～7]が使用できる(レンダーターゲット毎に独立したブレンド処理).
	blendDesc.AlphaToCoverageEnable = false;			//true:アルファトゥカバレッジを使用する.

	//表示タイプ
//	blendDesc.RenderTarget[0].BlendEnable = true;					//true:アルファブレンドを使用する.
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//アルファブレンドを指定.
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//アルファブレンドの反転を指定.
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			//ADD：加算合成.
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		//そのまま使用.
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//何もしない.
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//ADD：加算合成.
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//全ての成分(RGBA)へのデータの格納を許可する.

	bool tmpBlendEnable[ enBLEND_STATE_size ];
	tmpBlendEnable[ enBLEND_STATE_ALPHA_ON ] = true;
	tmpBlendEnable[ enBLEND_STATE_ALPHA_OFF ] = false;

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ )
	{
		blendDesc.RenderTarget[0].BlendEnable = tmpBlendEnable[i];
		if( FAILED( m_pDevice->CreateBlendState( &blendDesc, &m_pBlendState[i] ) ) ){
			assert( !"ブレンドステートの作成に失敗" );
			return E_FAIL;
		}
	}

	return S_OK;
}


//透過(アルファブレンド)設定の切り替え.
void clsDX9Mesh::SetBlend( bool isAlpha )
{
	UINT mask = 0xffffffff;	//マスク値白.

	if( isAlpha ){		
		//ブレンドステートの設定.
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_ON ], NULL, mask );
	}
	else{
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_OFF ], NULL, mask );
	}
}

