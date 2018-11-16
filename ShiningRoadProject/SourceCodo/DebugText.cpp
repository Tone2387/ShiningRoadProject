#include "DebugText.h"


namespace{
	//�V�F�[�_�t�@�C����(�p�X���܂�).
	const char SHADER_NAME[] = "Shader\\DebugText.hlsl";

	const char* FILE_PATH = "Data\\Image\\DebugText\\ascii0.png";

	const float DIMENSION		= 12.0f;
	const float WDIMENSION		= 10.0f;
	const float TEX_DIMENSION	= 128.0f;
}

//============================================================
//	�R���X�g���N�^.
//============================================================
clsDebugText::clsDebugText()
{
//	ZeroMemory( this, sizeof( clsDebugText ) );	//������.
	m_pDevice11 = nullptr;		//�f�o�C�X�I�u�W�F�N�g.
	m_pDeviceContext11 = nullptr;	//�f�o�C�X�R���e�L�X�g.

	m_pVertexShader = nullptr;	//���_�V�F�[�_.
	m_pVertexLayout = nullptr;	//���_���C�A�E�g.
	m_pPixelShader = nullptr;		//�s�N�Z���V�F�[�_.
	m_pConstantBuffer = nullptr;	//�R���X�^���g�o�b�t�@.

	for( int i=0; i<100; i++ ){
		m_pVertexBuffer[i] = nullptr;	//���_�o�b�t�@(100��).
	}

	m_pAsciiTexture = nullptr;//�A�X�L�[�e�N�X�`��.
	m_pSampleLinear = nullptr;//�e�N�X�`���̃T���v���[:/�e�N�X�`���Ɋe��t�B���^��������.

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		m_pBlendState[i] = nullptr;
	}

	for( int i=0; i<100; i++ ){
		m_fKerning[i] = 0.0f;
	}
	m_fScale = 1.0f;

	D3DXMatrixIdentity( &m_mView );
	D3DXMatrixIdentity( &m_mProj );
}


//============================================================
//	�f�X�g���N�^.
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


	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		SAFE_RELEASE( m_pBlendState[i] );
	}

	m_pDevice11 = nullptr;		//�f�o�C�X�I�u�W�F�N�g.
	m_pDeviceContext11 = nullptr;	//�f�o�C�X�R���e�L�X�g.

}


//============================================================
//	�������֐�.
//============================================================
HRESULT clsDebugText::Init( ID3D11DeviceContext* pContext,
							DWORD dwWidth, DWORD dwHeight,
							float fSize, D3DXVECTOR4 vColor )
{
	m_fAlpha = vColor.w;
	m_vColor = vColor;
	m_fScale = fSize / 25.0f;

	//95�������J��Ԃ�.
	for( int i=0; i<95; i++ ){
		m_fKerning[i] = 10.0f;
	}
	//�f�o�C�X�R���e�L�X�g���R�s�[.
	m_pDeviceContext11 = pContext;
	m_pDeviceContext11->GetDevice( &m_pDevice11 );

	//�u�����h�X�e�[�g�쐬.
	CreateBlendState();

	//window�T�C�Y.
	m_dwWindowWidth = dwWidth;
	m_dwWindowHeight= dwHeight;

	//�t�H���g���ɃN�A�b�h(��`)�쐬.
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
				D3DXVECTOR3(	  0.0f,	DIMENSION, 0.0f ), D3DXVECTOR2( left, bottom ),		//���_1.
				D3DXVECTOR3(	  0.0f,		 0.0f, 0.0f ), D3DXVECTOR2( left, top ),		//���_2.
				D3DXVECTOR3( DIMENSION, DIMENSION, 0.0f ), D3DXVECTOR2( right, bottom ),	//���_3.
				D3DXVECTOR3( DIMENSION,		 0.0f, 0.0f ), D3DXVECTOR2( right, top )		//���_4.
			};
			D3D11_BUFFER_DESC bd;
			bd.Usage		= D3D11_USAGE_DEFAULT;
			bd.ByteWidth	= sizeof( TextVertex ) * 4;
			bd.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags=0;
			bd.MiscFlags	= 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;
			if( FAILED( m_pDevice11->CreateBuffer(
				&bd, &InitData, &m_pVertexBuffer[cnt]) ) )
			{
				MessageBox( NULL,
					"���_�o�b�t�@�쐬���s(DebugText:Init)",
					"error", MB_OK );
				return E_FAIL;
			}
			cnt ++;
		}
	}

	//�e�N�X�`���p�T���v���쐬.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory( &SamDesc, sizeof( D3D11_SAMPLER_DESC ) );
	//�e�N�X�`���t�B���^�Ɠ\��t�����̎w��.
	SamDesc.Filter	= D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU= D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV= D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW= D3D11_TEXTURE_ADDRESS_WRAP;
	if( FAILED( m_pDevice11->CreateSamplerState(
		&SamDesc, &m_pSampleLinear) ) )
	{
		MessageBox( NULL,
			"�T���v���쐬���s(DebugText:Init)",
			"", MB_OK );
		return E_FAIL;
	}

	//�t�H���g�̃e�N�X�`���쐬.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,
			FILE_PATH,
			NULL, NULL,
			&m_pAsciiTexture, NULL ) ) )
	{
		MessageBox( NULL,
			"�t�H���g�e�N�X�`���쐬���s(DebugText:Init)",
			FILE_PATH, MB_OK );
		return E_FAIL;
	}

	//============================================================
		//hlsl�t�@�C���Ǎ�.
	//============================================================
	ID3DBlob*	pCompileShader = nullptr;
	ID3DBlob*	pErrors	= nullptr;
	if( FAILED( D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL,
		"VS", "vs_5_0", 0, 0,
		NULL, &pCompileShader,
		&pErrors, NULL ) ) )
	{
		MessageBox( NULL, "hlsl�Ǎ����s(VS)", "DebugText:Init", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	if( FAILED( m_pDevice11->CreateVertexShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		NULL, &m_pVertexShader ) ) )
	{
		SAFE_RELEASE( pCompileShader );
		MessageBox( NULL, "�o�[�e�b�N�X�o�b�t�@�쐬���s", "DebugText:Init", MB_OK );
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	//���_�C���v�b�g���C�A�E�g�쐬.
	if( FAILED( m_pDevice11->CreateInputLayout(
		layout, numElements,
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		&m_pVertexLayout ) ) )
	{
		MessageBox( NULL, "���_�C���v�b�g���C�A�E�g�쐬", "DebugText:Init", MB_OK );
		return E_FAIL;
	}

	//�s�N�Z���V�F�[�_�쐬.
	if( FAILED( D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL,
		"PS", "ps_5_0", 0, 0,
		NULL, &pCompileShader,
		&pErrors, NULL ) ) )
	{
		MessageBox( NULL, "hlsl�Ǎ����s(PS)", "DebugText:Init", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );


	if( FAILED( m_pDevice11->CreatePixelShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		NULL, &m_pPixelShader ) ) )
	{
		SAFE_RELEASE( pCompileShader );
		MessageBox( NULL, "�s�N�Z���V�F�[�_�쐬���s", "DebugText:Init", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompileShader );

	//�R���X�^���g�o�b�t�@�쐬.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth	= sizeof( TEXT_CONSTANT_BUFFER );
	cb.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags	= 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if( FAILED(m_pDevice11->CreateBuffer(
		&cb, NULL, &m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "�R���X�^���g�o�b�t�@�쐬", "DebugText:Init", MB_OK );
		return E_FAIL;
	}




	return S_OK;
}


//�u�����h�X�e�[�g�쐬.
HRESULT clsDebugText::CreateBlendState()
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
		if( FAILED( m_pDevice11->CreateBlendState( &blendDesc, &m_pBlendState[i] ) ) ){
			assert( !"�u�����h�X�e�[�g�̍쐬�Ɏ��s" );
			return E_FAIL;
		}
	}

	return S_OK;
}


//============================================================
//	����(�A���t�@�u�����h)�ݒ�̐؂�ւ�.
//============================================================
void clsDebugText::SetBlend( bool isAlpha )
{
	UINT mask = 0xffffffff;	//�}�X�N�l��.

	if( isAlpha ){		
		//�u�����h�X�e�[�g�̐ݒ�.
		m_pDeviceContext11->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_ON ], NULL, mask );
	}
	else{
		m_pDeviceContext11->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_OFF ], NULL, mask );
	}
}


//============================================================
//	�����_�����O�֐�.
//============================================================
void clsDebugText::Render( char* text, int x, int y )
{
	//�r���[�g�����X�t�H�[��.
	D3DXVECTOR3	vEye( 0.0f, 0.0f,-1.0f );
	D3DXVECTOR3	vLook(0.0f, 0.0f, 0.0f );
	D3DXVECTOR3	vUp(  0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH(
		&m_mView, &vEye, &vLook, &vUp );


#if 0
	//�v���W�F�N�V�����g�����X�t�H�[��(�Œ�).
	D3DMATRIX mOtho = {
		2.0f / (float)( m_dwWindowWidth ), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)( m_dwWindowHeight), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
#else
	D3DXMATRIX mOtho;
	D3DXMatrixIdentity( &mOtho );//�P�ʍs��.
	mOtho._11	= 2.0f / (float)( m_dwWindowWidth );
	mOtho._22	= -2.0f / (float)( m_dwWindowHeight );
	mOtho._41	= -1.0f;
	mOtho._42	= 1.0f;
#endif
	m_mProj = mOtho;

	//�v���~�e�B�u�E�g�|���W�[.
	m_pDeviceContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	//���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pDeviceContext11->IASetInputLayout(
		m_pVertexLayout );

	//�g�p����V�F�[�_�̓o�^.
	m_pDeviceContext11->VSSetShader(
		m_pVertexShader, NULL, 0 );
	m_pDeviceContext11->PSSetShader(
		m_pPixelShader, NULL, 0 );

	//���̃R���X�^���g�o�b�t�@���g���V�F�[�_�̓o�^.
	m_pDeviceContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );
	m_pDeviceContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );

	//�e�N�X�`�����V�F�[�_�ɓn��.
	m_pDeviceContext11->PSSetSamplers(
		0, 1, &m_pSampleLinear );
	m_pDeviceContext11->PSSetShaderResources(
		0, 1, &m_pAsciiTexture );


	SetBlend( true );
	//�����������[�v.
	for( UINT i=0; i<strlen( text ); i++ ){
		char font = text[i];
		int index = font - 32;	//�t�H���g�C���f�b�N�X�쐬.

		//�t�H���g�����_�����O.
		RenderFont( index, x, y );

		x += static_cast<int>(m_fKerning[index]);
	}
	SetBlend( false );



}


//============================================================
//�t�H���g�����_�����O�֐�.
//============================================================
void clsDebugText::RenderFont( int FontIndex, int x, int y )
{
	//���[���h�ϊ�.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	D3DXMatrixTranslation(
		&mWorld, (float)x, (float)y,
		-100.0f );	//Z(-100)�Ŏ�O�\���\.

	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE	pData;
	TEXT_CONSTANT_BUFFER		cb;
	if( SUCCEEDED( m_pDeviceContext11->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//���[���h,�r���[,�v���W�F�N�V�����̍����s���n��.
		D3DXMATRIX m = mWorld * m_mView * m_mProj;
		D3DXMatrixTranspose( &m, &m );
		cb.mWVP = m;
		//�J���[��n��.
		cb.vColor = m_vColor;
		//�����x��n��.
		cb.fAlpha.x = m_fAlpha;

		memcpy_s( pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb) );
		m_pDeviceContext11->Unmap(
			m_pConstantBuffer, 0 );
	}
	//�o�[�e�b�N�X�o�b�t�@���Z�b�g.
	UINT stride = sizeof( TextVertex );
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer[FontIndex],
		&stride, &offset );

	//�`��.
//	SetBlend( true );
	m_pDeviceContext11->Draw( 4, 0 );


}