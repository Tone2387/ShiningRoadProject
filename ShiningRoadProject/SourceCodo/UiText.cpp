#include "UiText.h"

using namespace std;

namespace{

	//�V�F�[�_�t�@�C����(�p�X���܂�).
	const char SHADER_NAME[] = "Shader\\DebugText.hlsl";

	const char* sFILE_PATH = "Data\\Image\\Font.png";


	const float DIMENSION		= 12.0f;
	const float WDIMENSION		= 10.0f;
	const float TEX_DIMENSION	= 128.0f;

	//�������m�̉��̊��o.
	const float fWIDE_DIS = 6.5f;

	//�E�񂹎��̌W��.
	const float fRIGHT_RATE = 6.75f;

	const float fERROR = -999.0f;
}


//============================================================
//	UI�Ƃ��ĕ���������.
//============================================================
clsUiText::clsUiText()
	:m_vPos( { 0.0f, 0.0f } )
	,m_fAlpha( 1.0f )
{
	m_pVertexShader = nullptr;	//���_�V�F�[�_.
	m_pVertexLayout = nullptr;	//���_���C�A�E�g.
	m_pPixelShader = nullptr;		//�s�N�Z���V�F�[�_.
	m_pConstantBuffer = nullptr;	//�R���X�^���g�o�b�t�@.

	for( int i=0; i<100; i++ ){
		m_pVertexBuffer[i] = nullptr;	//���_�o�b�t�@(100��).
	}

	m_pAsciiTexture = nullptr;//�A�X�L�[�e�N�X�`��.
	m_pSampleLinear = nullptr;//�e�N�X�`���̃T���v���[:/�e�N�X�`���Ɋe��t�B���^��������.

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
clsUiText::~clsUiText()
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



}


//============================================================
//	�������֐�.
//============================================================
HRESULT clsUiText::Create( ID3D11DeviceContext* const pContext,
							const DWORD &dwWidth, const DWORD &dwHeight,
							const float fScale )
{
	m_fScale = fScale;
	m_vColor = { 1.0f, 1.0f, 1.0f, 1.0f };


	//95�������J��Ԃ�.
	for( int i=0; i<95; i++ ){
		m_fKerning[i] = 10.0f;
	}
	//�f�o�C�X�R���e�L�X�g���R�s�[.
	m_wpContext = pContext;
	m_wpContext->GetDevice( &m_wpDevice );

	if( FAILED( CreateBlendState() ) ){
		assert( !"Can't Create BlendState" );
		return E_FAIL;
	}

	//window�T�C�Y.
	m_dwWindowWidth = dwWidth;
	m_dwWindowHeight= dwHeight;

	//�t�H���g���ɃN�A�b�h(��`)�쐬.
	float left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	int cnt = 0;
	for( int k=0; k<10; k++ )
	{
		for( int i=0; i<10; i++ ){
			left	= k * WDIMENSION		;	
			top		= i * DIMENSION			;
			right	= left + m_fKerning[ cnt ];
			bottom	= top + DIMENSION		;

			left	/= TEX_DIMENSION;
			top		/= TEX_DIMENSION;
			right	/= TEX_DIMENSION;
			bottom	/= TEX_DIMENSION;

			TextVertex vertices[] =
			{
				D3DXVECTOR3( 0.0f,		DIMENSION,  0.0f ), D3DXVECTOR2( left, bottom ),	//���_1.
				D3DXVECTOR3( 0.0f,		0.0f,		0.0f ),	D3DXVECTOR2( left, top ),		//���_2.
				D3DXVECTOR3( WDIMENSION,DIMENSION,  0.0f ), D3DXVECTOR2( right, bottom ),	//���_3.
				D3DXVECTOR3( WDIMENSION,0.0f,		0.0f ), D3DXVECTOR2( right, top )		//���_4.
			};
			D3D11_BUFFER_DESC bd;
			bd.Usage		= D3D11_USAGE_DEFAULT;
			bd.ByteWidth	= sizeof( TextVertex ) * 4;
			bd.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags=0;
			bd.MiscFlags	= 0;
			bd.StructureByteStride = 0;//�\���̂̃T�C�Y(���g�p)

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;
			if( FAILED( m_wpDevice->CreateBuffer(
				&bd, &InitData, &m_pVertexBuffer[ cnt ] ) ) )
			{
				MessageBox( NULL,
					"���_�o�b�t�@�쐬���s(UiText:Init)",
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
	if( FAILED( m_wpDevice->CreateSamplerState(
		&SamDesc, &m_pSampleLinear) ) )
	{
		MessageBox( NULL,
			"�T���v���쐬���s(UiText:Init)",
			sFILE_PATH, MB_OK );
		return E_FAIL;
	}

	//�t�H���g�̃e�N�X�`���쐬.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_wpDevice,
		sFILE_PATH,
		NULL, NULL,
		&m_pAsciiTexture, NULL ) ) )
	{
		MessageBox( NULL,
			"�t�H���g�e�N�X�`���쐬���s(UiText:Init)",
			sFILE_PATH, MB_OK );
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
		MessageBox( NULL, "hlsl�Ǎ����s(VS)", SHADER_NAME, MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	if( FAILED( m_wpDevice->CreateVertexShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		NULL, &m_pVertexShader ) ) )
	{
		SAFE_RELEASE( pCompileShader );
		MessageBox( NULL, "�o�[�e�b�N�X�o�b�t�@�쐬���s", SHADER_NAME, MB_OK );
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
	if( FAILED( m_wpDevice->CreateInputLayout(
		layout, numElements,
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		&m_pVertexLayout ) ) )
	{
		MessageBox( NULL, "���_�C���v�b�g���C�A�E�g�쐬", SHADER_NAME, MB_OK );
		return E_FAIL;
	}

	//�s�N�Z���V�F�[�_�쐬.
	if( FAILED( D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL,
		"PS", "ps_5_0", 0, 0,
		NULL, &pCompileShader,
		&pErrors, NULL ) ) )
	{
		MessageBox( NULL, "hlsl�Ǎ����s(PS)", SHADER_NAME, MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );


	if( FAILED( m_wpDevice->CreatePixelShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		NULL, &m_pPixelShader ) ) )
	{
		SAFE_RELEASE( pCompileShader );
		MessageBox( NULL, "�s�N�Z���V�F�[�_�쐬���s", SHADER_NAME, MB_OK );
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

	if( FAILED( m_wpDevice->CreateBuffer(
		&cb, NULL, &m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "�R���X�^���g�o�b�t�@�쐬", SHADER_NAME, MB_OK );
		return E_FAIL;
	}




	return S_OK;
}


//============================================================
//	�����_�����O�֐�.
//============================================================
//�f�t�H���g������true�ɂ���ƉE�[���w����W�ɗ���.
void clsUiText::Render( const enPOS enPos )
{
	float x = m_vPos.x;
	float y = m_vPos.y;

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
	mOtho._11	= 2.0f / static_cast<float>( m_dwWindowWidth );
	mOtho._22	=-2.0f / static_cast<float>( m_dwWindowHeight );
	mOtho._41	=-1.0f;
	mOtho._42	= 1.0f;
#endif
	m_mProj = mOtho;

	//�v���~�e�B�u�E�g�|���W�[.
	m_wpContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	//���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_wpContext->IASetInputLayout(
		m_pVertexLayout );

	//�g�p����V�F�[�_�̓o�^.
	m_wpContext->VSSetShader(
		m_pVertexShader, NULL, 0 );
	m_wpContext->PSSetShader(
		m_pPixelShader, NULL, 0 );

	//���̃R���X�^���g�o�b�t�@���g���V�F�[�_�̓o�^.
	m_wpContext->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );
	m_wpContext->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );

	//�e�N�X�`�����V�F�[�_�ɓn��.
	m_wpContext->PSSetSamplers(
		0, 1, &m_pSampleLinear );
	m_wpContext->PSSetShaderResources(
		0, 1, &m_pAsciiTexture );


	SetBlend( true );

	//���ꂪ�Ȃ��ƕ��s�ړ��̒l��scale�Ɉ���������.
	x /= m_fScale;
	y /= m_fScale;

	//�����������[�v�̏���.
	const char* tmpText = m_sText.c_str();
	UINT textSize = strlen( tmpText );	//������.

	//�E�񂹂Ȃ獶�ɂ��炷.
	switch( enPos )
	{
	case enPOS::RIGHT:
		x -= static_cast<float>( textSize ) * fRIGHT_RATE;
		break;
	case enPOS::MIDDLE:
		const float fHARH = 0.5f;
		x -= static_cast<float>( textSize ) * fRIGHT_RATE * fHARH;
		break;
	}

	//�����������[�v.
	for( UINT i=0; i<textSize; i++ ){
		char font = tmpText[i];
		int index = font - 32;	//�t�H���g�C���f�b�N�X�쐬.

		//�t�H���g�����_�����O.
		RenderFont( index, x, y, static_cast<float>( i ) );

//		x += m_fKerning[index];
		x += fWIDE_DIS;
	}

//	SetBlend( false );



}


//============================================================
//�t�H���g�����_�����O�֐�.
//============================================================
void clsUiText::RenderFont( 
	const int FontIndex, const float x, const float y, const float z ) const
{
	//���[���h�ϊ�.
	D3DXMATRIX mWorld, mTrans, mScale;
	D3DXMatrixIdentity( &mWorld );
	D3DXMatrixIdentity( &mTrans );
	D3DXMatrixIdentity( &mScale );

	//���s�ړ�.
	D3DXMatrixTranslation(
		&mTrans, x, y,
		-100.0f - z );	//Z(-100)�Ŏ�O�\���\.
	//�g�k.
	D3DXMatrixScaling( &mScale, m_fScale, m_fScale, m_fScale );

	//����.
	mWorld = mTrans * mScale;

	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE	pData;
	TEXT_CONSTANT_BUFFER		cb;
	if( SUCCEEDED( m_wpContext->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//���[���h,�r���[,�v���W�F�N�V�����̍����s���n��.
		D3DXMATRIX m = mWorld * m_mView * m_mProj;
		D3DXMatrixTranspose( &m, &m );
		cb.mWVP = m;
		//�J���[��n��.
		cb.vColor = m_vColor;
		//�����x��n��.m_fAlpha
//		cb.fAlpha.x = m_vColor.w;
		cb.fAlpha.x = m_fAlpha;

		memcpy_s( pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb) );
		m_wpContext->Unmap(
			m_pConstantBuffer, 0 );
	}
	//�o�[�e�b�N�X�o�b�t�@���Z�b�g.
	UINT stride = sizeof( TextVertex );
	UINT offset = 0;
	m_wpContext->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer[FontIndex],
		&stride, &offset );


	//�`��.
	m_wpContext->Draw( 4, 0 );



}





