#include "DX9Mesh.h"
#include "OperationString.h"

namespace{
	//�V�F�[�_�[�t�@�C����(�f�B���N�g�����܂�)
	const char SHADER_NAME[] = "Shader\\Mesh.hlsl";//const:��ɏ����ꂽ�ϐ����㏑�������Ȃ�.

	const char sMASK_TEX_NAME[] = "mask";
	const char sMASK_TEX_TYPE[] = ".png";
	const char sMASK_PATH_EMPTY[] = "Data\\Image\\maskEmpty.png";
	//============================================================
	//	�\����.
	//============================================================
	//�R���X�^���g�o�b�t�@�̃A�v�����̒�`(Mesh.hlsl).
	//�V�F�[�_���̃R���X�^���g�o�b�t�@�ƈ�v���Ă���K�v����.
	//hlsl�ƘA�����Ă���(�^�����ق��������V���[�Y).
	struct MESHSHADER_CONSTANT_BUFFER_ZERO
	{
		D3DXMATRIX	mW;			//���[���h(�ʒu)���W�s��.
		D3DXMATRIX	mWVP;		//���[���h,�r���[,�ˉe�̍����ϊ��s��.
		D3DXVECTOR4 vLightDir;	//���C�g����.
		D3DXVECTOR4	vEye;		//�J�����ʒu(���_�ʒu).
		D3DXVECTOR4 vColor;		//�J���[.
	};
	struct MESHSHADER_CONSTANT_BUFFER_FIRST
	{
		D3DXVECTOR4 vAmbient;	//�A���r�G���g.
		D3DXVECTOR4 vDiffuse;	//�f�B�t���[�Y�F.
		D3DXVECTOR4	vSpecular;	//�X�y�L�����F.
	};


	//���_�̍\����.
	struct MeshVertex
	{
		D3DXVECTOR3 vPos;	//���_���W(x,y,z).
		D3DXVECTOR3 vNormal;//�@��(�A�e�v�Z�ɕK�{).
		D3DXVECTOR2 vTex;	//�e�N�X�`�����W.
	};



}

//========================================================
// �R���X�g���N�^.
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
// �f�X�g���N�^.
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
// ������.
//========================================================
HRESULT clsDX9Mesh::Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPSTR fileName )
{
//	m_hWnd = hWnd;
	m_pDevice = pDevice11;
	m_pContext = pContext11;
	LPDIRECT3DDEVICE9	pDevice9 = NULL;	//Dx9�f�o�C�X�I�u�W�F�N�g.

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
//Dx9������.
//========================================================
HRESULT clsDX9Mesh::InitDx9( HWND hWnd, LPDIRECT3DDEVICE9* pOutDevice9, const LPSTR sErrFilePath  )
{
	//�uDirect3D�v�I�u�W�F�N�g�̍쐬.
	LPDIRECT3D9	pD3d = NULL;	//DX9�I�u�W�F�N�g.
	pD3d = Direct3DCreate9( D3D_SDK_VERSION );
	if( pD3d == NULL ){
		MessageBox( NULL, "Dx9�I�u�W�F�N�g�쐬���s", sErrFilePath, MB_OK );
		return E_FAIL;
	}

	//Direct3D�f�o�C�X�I�u�W�F�N�g�̍쐬.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			//�o�b�N�o�b�t�@�̃t�H�[�}�b�g(�f�t�H���g)
	d3dpp.BackBufferCount			= 1;						//�o�b�N�o�b�t�@�̐�.
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	//�X���b�v�G�t�F�N�g(�f�t�H���g)
	d3dpp.Windowed					= true;						//�E�B���h�E���[�h.
	d3dpp.EnableAutoDepthStencil	= true;						//�X�e���V���L��.
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				//�X�e���V���̃t�H�[�}�b�g(16bit)

	//�f�o�C�X�쐬(HAL���[�h:�`��ƒ��_������GPU�ōs��)
	if( FAILED(pD3d->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, pOutDevice9 ) ) )
	{
		//�f�o�C�X�쐬(HAL���[�h:�`���GPU�A���_������CPU�ōs��)
		if( FAILED(pD3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, pOutDevice9 ) ) )
		{
			MessageBox(NULL, "HAL���[�h�Ńf�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", sErrFilePath, MB_OK);

			//�f�o�C�X�쐬(REF���[�h:�`���CPU�A���_������GPU�ōs��)
			if( FAILED(pD3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, pOutDevice9 ) ) )
			{
				//�f�o�C�X�쐬(REF���[�h:�`��ƒ��_������CPU�ōs��)
				if( FAILED(pD3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, pOutDevice9 ) ) )
				{
					MessageBox(NULL, "Direct3D�f�o�C�X�쐬���s", sErrFilePath, MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	SAFE_RELEASE( pD3d );

	return S_OK;
}

//========================================================
//X�t�@�C�����烁�b�V�������[�h����.
//========================================================
HRESULT clsDX9Mesh::LoadXMesh( LPSTR fileName, LPDIRECT3DDEVICE9 pDevice9 )
{
	//�}�e���A���o�b�t�@.
	LPD3DXBUFFER pD3DXMtrlBuffer;

	//���C�ƃ��b�V���̔���̂��߂ɒǉ�.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//�t�@�C����.
		D3DXMESH_SYSTEMMEM,	//�V�X�e���������ɓǂݍ���.
		pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)�}�e���A�����.
		NULL,
		&m_dwNumMaterials,	//(out)�}�e���A����.
		&m_pMeshForRay)))			//(out)���b�V���I�u�W�F�N�g.
	{

		MessageBox(NULL, fileName, "X�t�@�C���ǂݍ��ݎ��s", MB_OK);//�O���t�B�b�J�[�Č�.
		return E_FAIL;
	}

	//X�t�@�C���̃��[�h.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//�t�@�C����.
		D3DXMESH_SYSTEMMEM	//�V�X�e���������ɓǂݍ���.
		| D3DXMESH_32BIT, 
		pDevice9, NULL, 
		&pD3DXMtrlBuffer,	//(out)�}�e���A�����.
		NULL, 
		&m_dwNumMaterials,	//(out)�}�e���A����.
		&m_pMesh)))			//(out)���b�V���I�u�W�F�N�g.
	{

		MessageBox(NULL, fileName, "X�t�@�C���ǂݍ��ݎ��s", MB_OK);//�O���t�B�b�J�[�Č�.
		return E_FAIL;
	}

	D3D11_BUFFER_DESC bd;//Dx11�o�b�t�@�\����.
	D3D11_SUBRESOURCE_DATA InitData;//�������f�[�^.

	//�ǂݍ��񂾏�񂩂�K�v�ȏ��𔲂��o��.
	D3DXMATERIAL* d3dxMaterials
		= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//�}�e���A�������̗̈���m��.
	m_pMaterials = new MY_MATERIAL[m_dwNumMaterials];
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterials];
	//�}�e���A�������J��Ԃ�.
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		m_ppIndexBuffer[i] = nullptr;
		//�}�e���A�����̃R�s�[.
		m_pMaterials[i].Ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_pMaterials[i].Ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_pMaterials[i].Ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_pMaterials[i].Ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//�f�B�t���[�Y.
		m_pMaterials[i].Diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_pMaterials[i].Diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_pMaterials[i].Diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_pMaterials[i].Diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//�X�y�L����.
		m_pMaterials[i].Specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_pMaterials[i].Specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_pMaterials[i].Specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_pMaterials[i].Specular.w = d3dxMaterials[i].MatD3D.Specular.a;

		//(���̖ʂ�)�e�N�X�`�����\���Ă��邩�H.
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			const int iTEX_PATH_SIZE = 128;
			m_bTexture = true;//�e�N�X�`���t���O�𗧂Ă�.

			char path[ iTEX_PATH_SIZE ] = "";
			int path_cnt = lstrlen( fileName );

			//�K�w������O��Ȃ̂Ńt�@�C���𒼉��ɒu���Ǝ�.
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
			//�e�N�X�`���t�@�C�������R�s�[.
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
			//�e�N�X�`���쐬.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice, sTexFilePath,//�e�N�X�`���t�@�C����.
				NULL, NULL,
				&m_pMaterials[i].pTexture, //(out)�e�N�X�`���I�u�W�F�N�g.
				NULL ) ) )
			{
				MessageBox( NULL, sTexFilePath, "�e�N�X�`���쐬���s", MB_OK );
				return E_FAIL;
			}

			//�}�X�N�쐬.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice, sMaskFilePath,//�e�N�X�`���t�@�C����.
				NULL, NULL,
				&m_pMaterials[i].pMask, //(out)�e�N�X�`���I�u�W�F�N�g.
				NULL ) ) )
			{
				if( FAILED( D3DX11CreateShaderResourceViewFromFile(
					m_pDevice, sMASK_PATH_EMPTY,//�e�N�X�`���t�@�C����.
					NULL, NULL,
					&m_pMaterials[i].pMask, //(out)�e�N�X�`���I�u�W�F�N�g.
					NULL ) ) )
				{
					MessageBox( NULL, sTexFilePath, "�}�X�N�쐬���s", MB_OK );
					return E_FAIL;
				}
			}
		}
	}

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	// �C���f�b�N�X�o�b�t�@�̍쐬.
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	//���b�V���̑������𓾂�.
	//�������ŃC���f�b�N�X�o�b�t�@����ׂ����}�e���A�����Ƃ̃C���f�b�N�X�o�b�t�@�𕪗��ł���.
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	//���b�V���̖ʂ���ђ��_�̏��ԕύX�𐧌䂵�A�p�t�H�[�}���X���œK������.
	//D3DXMESHOPT_COMPACT:�ʂ̏��Ԃ�ύX���A�g�p����Ă��Ȃ����_�Ɩʂ��폜����.
	//D3DXMESHOPT_ATTRSORT:�p�t�H�[�}���X���グ��ׁA�ʂ̏��Ԃ�ύX���čœK�����s��.
	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
		0, 0, 0, 0);
	//�����e�[�u���̎擾.
	m_pMesh->GetAttributeTable(NULL, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(
		m_pMesh->GetAttributeTable(pAttrTable,&m_NumAttr)))
	{
		MessageBox( NULL, "�����e�[�u���擾���s", fileName, MB_OK );
		return E_FAIL;
	}

	//���b�N���Ȃ��Ǝ��o���Ȃ�.
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(
		D3DLOCK_READONLY, (void**)&pIndex);

	//�������Ƃ̃C���f�b�N�X�o�b�t�@���쐬.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		m_AttrID[i] = pAttrTable[i].AttribId;

		//Dx9�̃C���f�b�N�X�o�b�t�@����̏���Dx11�̃C���f�b�N�X�o�b�t�@���쐬.
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * pAttrTable[i].FaceCount * 3;	//�ʐ���3�Œ��_��.
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		//�傫�ȃC���f�b�N�X�o�b�t�@���̃I�t�Z�b�g(��3����)
		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];

		if (FAILED(
			m_pDevice->CreateBuffer(
			&bd, &InitData, &m_ppIndexBuffer[i])))
		{
			MessageBox( NULL, "�C���f�b�N�X�o�b�t�@�쐬���s", fileName, MB_OK );
			return E_FAIL;
		}
		//�ʂ̐����R�s�[.
		m_pMaterials[m_AttrID[i]].dwNumFace = pAttrTable[i].FaceCount;
	}
	
	delete[] pAttrTable;//�����e�[�u���̍폜.
	m_pMesh->UnlockIndexBuffer();

	//�s�v�ɂȂ����}�e���A���o�b�t�@�����.
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	// ���_(�o�[�e�b�N�X)�o�b�t�@�̍쐬.
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	//Dx9�̏ꍇ�Amap�ł͂Ȃ����b�N�Ńo�[�e�b�N�X�o�b�t�@����f�[�^�����o��.
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	MeshVertex* pVertex = NULL;
	if (SUCCEEDED(
		pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertex = (MeshVertex*)pVertices;
		//Dx9�̃o�[�e�b�N�X�o�b�t�@����̏��ŁADx11�̃o�[�e�b�N�X�o�b�t�@���쐬.
		bd.Usage = D3D11_USAGE_DEFAULT;
		//���_���i�[����̂ɕK�v�ȃo�C�g��.
		bd.ByteWidth = m_pMesh->GetNumBytesPerVertex() * m_pMesh->GetNumVertices();//���_��.
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pVertex;

		//�e�N�X�`�����W���}�C�i�X�΍�.
		if (m_bTexture)//���e�N�X�`��������Ώ�������.
		{
			//���_�����J��Ԃ�.
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
			MessageBox( NULL, "���_(�o�[�e�b�N�X)�o�b�t�@�쐬���s", fileName, MB_OK );
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);//�o�[�e�b�N�X�o�b�t�@���.

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	// �T���v���[�쐬.
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-
	//�e�N�X�`���p�̃T���v���[�\����.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	//���j�A�t�B���^�[(���`���)
	//POINT:���������e��.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	//���b�s���O���[�h(WMAP:�J��Ԃ�)
	//MIRROR:���]�܂�Ԃ�.
	//CLAMP:���̖͗l���������΂�.
	//BORDER:�ʓr���E�F�����߂�.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//�T���v���[�쐬.
	if (FAILED(
		m_pDevice->CreateSamplerState(
		&SamDesc, &m_pSampleLinear)))//(out)�T���v���[.
	{
		MessageBox(NULL, "�T���v���[�쐬���s", fileName, MB_OK);
		return E_FAIL;
	}


	return S_OK;
}

//========================================================
//HLSL�t�@�C����ǂݍ��݃V�F�[�_���쐬����.
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

	//HLSL����o�[�e�b�N�X�V�F�[�_�̃u���u�̍쐬.
	if (m_bTexture)
	{
		if (FAILED(
			D3DX11CompileFromFile(
			SHADER_NAME, NULL, NULL,
			"VS_Main", "vs_5_0",
			uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl(VS_Main)�ǂݍ��ݎ��s", sErrFilePath, MB_OK);
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
			MessageBox(NULL, "hlsl(VS_NoTex)�ǂݍ��ݎ��s", sErrFilePath, MB_OK);
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�v���쐬.
	if (FAILED(
		m_pDevice->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pVertexShader)))//(out)�o�[�e�b�N�X�V�F�[�_.
	{
		MessageBox(NULL, "vs�쐬���s", sErrFilePath, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = 0;//sizeof(layout) / sizeof(layout[0]);
	if (m_bTexture)
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{
				"POSITION",						//�ʒu.
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI�̃t�H�[�}�b�g (32bit float�^ * 3)
				0,
				0,								//�f�[�^�̊J�n�ʒu.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"NORMAL",						//�@��.
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI�̃t�H�[�}�b�g (32bit float�^ * 3)
				0,
				12,								//�f�[�^�̊J�n�ʒu.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"TEXCOORD",						//�e�N�X�`���̈ʒu.
				0,
				DXGI_FORMAT_R32G32_FLOAT,		//DXGI�̃t�H�[�}�b�g(32bit float�^ * 2)
				0,
				24,								//�e�N�X�`���f�[�^�̊J�n�ʒu.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);//�v�f���Z�o.
		memcpy_s(layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{
				"POSITION",						//�ʒu.
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI�̃t�H�[�}�b�g (32bit float�^ * 3)
				0,
				0,								//�f�[�^�̊J�n�ʒu.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"NORMAL",						//�@��.
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI�̃t�H�[�}�b�g (32bit float�^ * 3)
				0,
				12,								//�f�[�^�̊J�n�ʒu.
				D3D11_INPUT_PER_VERTEX_DATA, 0
			}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);//�v�f���Z�o.
		memcpy_s(layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);
	}
	
	//���_�C���v�b�g���C�A�E�g�쐬.
	if (FAILED(
		m_pDevice->CreateInputLayout(
		layout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		&m_pVertexLayout)))//(out)���_�C���v�b�g���C�A�E�g.
	{
		MessageBox(NULL, "���_�C���v�b�g���C�A�E�g�쐬���s", sErrFilePath, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSL����s�N�Z���V�F�[�_�̃u���u�̍쐬.
	if (m_bTexture)
	{
		if (FAILED(D3DX11CompileFromFile(SHADER_NAME,NULL,NULL,
			"PS_Main","ps_5_0",
			uCompileFlag,0,NULL,&pCompiledShader,&pErrors,NULL)))
		{
			MessageBox(NULL, "hlsl�ǂݍ��ݎ��s(PS_Main)", sErrFilePath, MB_OK);
			return E_FAIL;
		}
	}

	else
	{
		if (FAILED(D3DX11CompileFromFile(SHADER_NAME, NULL, NULL,
			"PS_NoTex", "ps_5_0",
			uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl�ǂݍ��ݎ��s(PS_NoTex)", sErrFilePath, MB_OK);
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�v���쐬.
	if (FAILED(
		m_pDevice->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pPixelShader)))//(out)�s�N�Z���V�F�[�_.
	{
		MessageBox(NULL, "ps�쐬���s", sErrFilePath, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g(�萔)�o�b�t�@�쐬 �V�F�[�_�ɓ���̐��l�𑗂�o�b�t�@.
	//�����ł͕ϊ��s��n���p.
	//�V�F�[�_��World,View,Projection�s���n��.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�R���X�^���g�o�b�t�@���w��.
	cb.ByteWidth = sizeof(MESHSHADER_CONSTANT_BUFFER_ZERO);	//�R���X�^���g�o�b�t�@�̃T�C�Y.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//�������݂ŃA�N�Z�X.
	cb.MiscFlags = 0;									//���̑��̃t���O(���g�p)
	cb.StructureByteStride = 0;									//�\���̂̃T�C�Y(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;					//�g�p���@:���ڂ̏�������.

	//�R���X�^���g�o�b�t�@�쐬.
	if (FAILED(m_pDevice->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer0)))
	{
		MessageBox(NULL, "�R���X�^���g�o�b�t�@�쐬���s", sErrFilePath, MB_OK);
		return E_FAIL;
	}

	//�}�e���A���p�̃R���X�^���g�o�b�t�@.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�R���X�^���g�o�b�t�@���w��.
	cb.ByteWidth = sizeof(MESHSHADER_CONSTANT_BUFFER_FIRST);	//�R���X�^���g�o�b�t�@�̃T�C�Y.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//�������݂ŃA�N�Z�X.
	cb.MiscFlags = 0;									//���̑��̃t���O(���g�p)
	cb.StructureByteStride = 0;									//�\���̂̃T�C�Y(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;					//�g�p���@:���ڂ̏�������.

	//�R���X�^���g�o�b�t�@�쐬.
	if (FAILED(m_pDevice->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer1)))
	{
		MessageBox(NULL, "�R���X�^���g�o�b�t�@�쐬���s", sErrFilePath, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//�����_�����O�p(��DX9MESH����Main���̓�ɑ��݂���̂Œ���)
void clsDX9Mesh::Render( const D3DXMATRIX& mView,	const D3DXMATRIX& mProj,
						 const D3DXVECTOR3& vLight,	const D3DXVECTOR3& vEye,
						 const D3DXVECTOR4& vColor,	const bool isAlpha )
{
	D3DXMATRIX mWorld, mScale, mYaw, mPitch, mRoll,mTrans;

	//���[���h�ϊ�(�\���ʒu��ݒ肷��)
	//�g�k.
	D3DXMatrixScaling(
		&mScale, m_Trans.vScale.x, m_Trans.vScale.y, m_Trans.vScale.z);
	//��].
	D3DXMatrixRotationY(&mYaw, m_Trans.fYaw);//Y����].
	D3DXMatrixRotationX(&mPitch, m_Trans.fPitch);
	D3DXMatrixRotationZ(&mRoll, m_Trans.fRoll);

//	//=================================================================//
//	//�L�����N�^�[�ړ�����(���W�R��)
//	vecAxisX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
//	vecAxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//
//	D3DXVec3TransformCoord(
//		&vecAxisX, //(out)
//		&vecAxisX,
//		&mYaw);//Y����]�s��.
//
//	//Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
//	D3DXVec3TransformCoord(
//		&vecAxisZ, //(out)
//		&vecAxisZ, 
//		&mYaw);//Y����]�s��.
//
//	//�����ɂ���Đi�s������l��ݒ�.
//	/*switch (m_enDir)
//	{
//	case enDirection_Foward://�O�i.
//		//�����������~�i�߂�l(0.1f)
//		m_Trans.vPos += vecAxisZ * 0.1f;
//		break;
//	case enDirection_Backward://���.
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
//	m_enDir = enDirection_Stop;//��~.*/
//	//=================================================================//


	//�ړ�.
	D3DXMatrixTranslation( &mTrans,
		m_Trans.vPos.x, m_Trans.vPos.y, m_Trans.vPos.z );

	//����.
//	mWorld = mScale * mYaw * mPitch * mRoll * mTrans;//�r��.
	mWorld = mScale * mRoll * mPitch * mYaw * mTrans;//���Ƃ���.

	//�g�p����V�F�[�_���Z�b�g.
	m_pContext->VSSetShader( m_pVertexShader, NULL, 0);//���_�V�F�[�_.
	m_pContext->PSSetShader( m_pPixelShader,  NULL, 0);//�s�N�Z���V�F�[�_.

	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	
	//�o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if (SUCCEEDED(
		m_pContext->Map(
		m_pConstantBuffer0, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData)))
	{
		MESHSHADER_CONSTANT_BUFFER_ZERO cb;//�R���X�^���g�o�b�t�@.

		//���[���h�s���n��.
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//���[���h,�r���[(�J����),�v���W�F�N�V����(�ˉe)�s���n��.
		D3DXMATRIX m = mWorld * mView * mProj;

		D3DXMatrixTranspose(&m, &m);//�s���]�u����.
		//���s��̌v�Z���@��DirectX��GPU�ňقȂ邽��.

		cb.mWVP = m;

		//���C�g�̕���.
		cb.vLightDir = (D3DXVECTOR4)vLight;

		//�f�B�t���[�Y�J���[�̐ݒ�.
		//�n�C���C�g�ɐF��t�������Ȃ��ꍇ�͔���ݒ�.
		//cb.vDeffuse = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);

		//�X�y�L������n��.
		//cb.vSpecular = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);

		//�J�����̈ʒu(���_)���V�F�[�_�ɓn��.
		cb.vEye = D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 0.0f);

		//�F.
		cb.vColor = vColor;

		memcpy_s(
			pData.pData,	//�R�s�[��̃o�b�t�@.
			pData.RowPitch,//�R�s�[��̃o�b�t�@�T�C�Y.
			(void*)(&cb),//�R�s�[���̃o�b�t�@.
			sizeof(cb));//�R�s�[����T�C�Y.

		//�o�b�t�@���f�[�^�̏��������I������Unmap.
		m_pContext->Unmap(m_pConstantBuffer0, 0);
	}

	//�R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg����?.
	m_pContext->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer0);//���_�V�F�[�_.

	m_pContext->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer0);//�s�N�Z���V�F�[�_.

	//���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext->IASetInputLayout(m_pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_pContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//���_�o�b�t�@���Z�b�g.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer,
		&stride, &offset);

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬.
	SetBlend( isAlpha );
	
	//�����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@��`��.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�.
		if (m_pMaterials[m_AttrID[i]].dwNumFace == 0){
			continue;
		}
		//�C���f�b�N�X�o�b�t�@���Z�b�g.
		m_pContext->IASetIndexBuffer(
			m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//�}�e���A���̊e�v�f���V�F�[�_�[�ɓn��.
		D3D11_MAPPED_SUBRESOURCE pData1;
		if (SUCCEEDED(
			m_pContext->Map(m_pConstantBuffer1, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData1)))
		{
			MESHSHADER_CONSTANT_BUFFER_FIRST cb;
			//�A���r�G���g,�f�B�t���[�Y,�X�y�L�������V�F�[�_�[�ɓn��.
			cb.vAmbient = m_pMaterials[m_AttrID[i]].Ambient;
			cb.vDiffuse = m_pMaterials[m_AttrID[i]].Diffuse;
			cb.vSpecular = m_pMaterials[m_AttrID[i]].Specular;

			memcpy_s(
				pData1.pData, pData1.RowPitch,
				(void*)&cb, sizeof(MESHSHADER_CONSTANT_BUFFER_FIRST));

			m_pContext->Unmap(m_pConstantBuffer1, 0);
		}

		//���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�[�Ŏg����?.
		m_pContext->VSSetConstantBuffers(
			1, 1, &m_pConstantBuffer1);
		m_pContext->PSSetConstantBuffers(
			1, 1, &m_pConstantBuffer1);

		//�e�N�X�`�����V�F�[�_�[�ɓn��.
		if (m_pMaterials[m_AttrID[i]].pTexture)
		{
			//�e�N�X�`��������Ƃ�.
			m_pContext->PSSetSamplers(
				0, 1, &m_pSampleLinear );
			m_pContext->PSSetShaderResources(
				0, 1, &m_pMaterials[m_AttrID[i]].pTexture );
			//�}�X�N������Ƃ�.
			m_pContext->PSSetShaderResources(
				1, 1, &m_pMaterials[ m_AttrID[i] ].pMask );
		}
		else
		{
			//�e�N�X�`�����Ȃ��Ƃ�.
			ID3D11ShaderResourceView* Nothing[1] = {0};
			m_pContext->PSSetShaderResources(0, 1, Nothing);
		}
		//�v���~�e�B�u(�|���S��)�������_�����O.
		m_pContext->DrawIndexed(
			m_pMaterials[m_AttrID[i]].dwNumFace * 3, 0, 0);
	}
}

//�u�����h�X�e�[�g�쐬.
HRESULT clsDX9Mesh::CreateBlendState()
{
	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬.
	//png�t�@�C�����ɃA���t�@��񂪂���̂ŁA���߂���悤�Ƀu�����h�X�e�[�g�Őݒ肷��.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( D3D11_BLEND_DESC ) );	//������.
	blendDesc.IndependentBlendEnable = false;			//false:RenderTarget[0]�̃����o�[�̂ݎg�p����Btrue:RenderTarget[0�`7]���g�p�ł���(�����_�[�^�[�Q�b�g���ɓƗ������u�����h����).
	blendDesc.AlphaToCoverageEnable = false;			//true:�A���t�@�g�D�J�o���b�W���g�p����.

	//�\���^�C�v
//	blendDesc.RenderTarget[0].BlendEnable = true;					//true:�A���t�@�u�����h���g�p����.
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//�A���t�@�u�����h���w��.
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//�A���t�@�u�����h�̔��]���w��.
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			//ADD�F���Z����.
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		//���̂܂܎g�p.
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//�������Ȃ�.
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//ADD�F���Z����.
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//�S�Ă̐���(RGBA)�ւ̃f�[�^�̊i�[��������.

	bool tmpBlendEnable[ enBLEND_STATE_size ];
	tmpBlendEnable[ enBLEND_STATE_ALPHA_ON ] = true;
	tmpBlendEnable[ enBLEND_STATE_ALPHA_OFF ] = false;

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ )
	{
		blendDesc.RenderTarget[0].BlendEnable = tmpBlendEnable[i];
		if( FAILED( m_pDevice->CreateBlendState( &blendDesc, &m_pBlendState[i] ) ) ){
			assert( !"�u�����h�X�e�[�g�̍쐬�Ɏ��s" );
			return E_FAIL;
		}
	}

	return S_OK;
}


//����(�A���t�@�u�����h)�ݒ�̐؂�ւ�.
void clsDX9Mesh::SetBlend( bool isAlpha )
{
	UINT mask = 0xffffffff;	//�}�X�N�l��.

	if( isAlpha ){		
		//�u�����h�X�e�[�g�̐ݒ�.
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_ON ], NULL, mask );
	}
	else{
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_OFF ], NULL, mask );
	}
}

