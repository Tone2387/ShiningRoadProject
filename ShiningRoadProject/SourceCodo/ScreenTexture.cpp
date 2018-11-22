#include "ScreenTexture.h"

#include "SoundManagerNoise.h"
#include "Sprite2D.h"	

#include <random>

using namespace std;

namespace{

	struct SCREEN_TEXTURE_CONSTANT_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				//���[���h�s��.
		ALIGN16	D3DXVECTOR4	vColor;			//�A���t�@�l(���߂Ŏg�p����)
		ALIGN16 D3DXVECTOR2 vViewPort;	//�r���[�|�[�g��.
		ALIGN16 float		fPulse;			//�p���X.
		ALIGN16 float		fPulseOffset;	
		ALIGN16 int			iBlock;			//�u���b�N�̕�����.
		ALIGN16 int			iSeed;			//�u���b�N��seed�l.
		ALIGN16 float		isfNega;
	};

	//���_�̍\����.
	struct SCREEN_VERTEX
	{
		D3DXVECTOR3 vPos;	//�ʒu.
		D3DXVECTOR2 vTex;	//�e�N�X�`�����W.
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
		ERR_MSG( "�`���e�N�X�`���쐬���s", "" );
	}
	if( FAILED( CreateShader() ) ){
		ERR_MSG( "�`���e�N�X�`���V�F�[�_�[�쐬���s", "" );
	}
	if( FAILED( CreateConstantBuffer() ) ){
		ERR_MSG( "�`���e�N�X�`���o�b�t�@�쐬���s", "" );
	}

	//�T�E���h�쐬.
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
	//�e�N�X�`��.
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
		ERR_MSG( "�X�N���[���e�N�X�`���쐬���s", "" );
		assert( !"�X�N���[���e�N�X�`���쐬���s" );
		return hr;
	}

	//�����_�[�^�[�Q�b�g�r���[.
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
		ERR_MSG( "�X�N���[�������_�[�^�[�Q�b�g�r���[�쐬���s", "" );
		assert( !"�X�N���[�������_�[�^�[�Q�b�g�r���[�쐬���s" );
		return hr;
	}

	//�V�F�[�_�[���\�[�X�r���[.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof( srvDesc ) );
	srvDesc.Format				= rtvDesc.Format;
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels	= 1;

	hr = m_wpDevice->CreateShaderResourceView( m_pTexture, &srvDesc, &m_pShaderResourceView );
	if( FAILED( hr ) ){
		ERR_MSG( "�X�N���[���V�F�[�_�[���\�[�X�r���[�쐬���s", "" );
		assert( !"�X�N���[���V�F�[�_�[���\�[�X�r���[�쐬���s" );
		return hr;
	}

	//�T���v���[�X�e�[�g.
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
		ERR_MSG( "�X�N���[���T���v���[�X�e�[�g�쐬���s", "" );
		assert( !"�X�N���[���T���v���[�X�e�[�g�쐬���s" );
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


	//HLSL����o�[�e�b�N�X�V�F�[�_�̃u���u���쐬.
	if( FAILED(
		D3DX11CompileFromFile(
			sSHADER_NAME,	//�V�F�[�_�t�@�C����(HLSL�t�@�C��).
			NULL,			//�}�N����`�̔z��ւ̃|�C���^(���g�p).
			NULL,			//�C���N���[�h�t�@�C���������C���^�[�t�F�[�X�ւ̃|�C���^(���g�p).
			"VS_Main",			//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
			"vs_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����(�v���t�@�C��).
			uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
			0,				//�G�t�F�N�g�R���p�C���t���O(���g�p).
			NULL,			//�X���b�h�|���v�C���^�[�t�F�[�X�ւ̃|�C���^(���g�p).
			&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors,		//�G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
			NULL ) ) )		//�߂�l�ւ̃|�C���^(���g�p).
	{
		MessageBox( NULL, "hlsl(vs)�ǂݍ��ݎ��s", "�G���[", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�v���쐬.
	if( FAILED(
		m_wpDevice->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pVertexShader ) ) )//(out)�o�[�e�b�N�X�V�F�[�_.
	{
		MessageBox( NULL, "vs�쐬���s", "�G���[", MB_OK );
		return E_FAIL;
	}

	SAFE_RELEASE( pCompiledShader );




	//----- �ʏ� -----//.
	//HLSL����s�N�Z���V�F�[�_�̃u���u���쐬.
	if( FAILED(
		D3DX11CompileFromFile(
			sSHADER_NAME,	//�V�F�[�_�t�@�C����(HLSL�t�@�C��).
			NULL,
			NULL,
			"PS_Main",			//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
			"ps_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����(�v���t�@�C��).
			uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
			0,
			NULL,
			&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors,
			NULL ) ) )
	{
		MessageBox( NULL, "hlsl(ps)�ǂݍ��ݎ��s", "�G���[", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�v���쐬.
	if( FAILED(
		m_wpDevice->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pDefaultPS ) ) )//(out)�s�N�Z���V�F�[�_.
	{
		MessageBox( NULL, "�ʏ�ps�쐬���s", "�G���[", MB_OK );
		return E_FAIL;
	}


	//----- �m�C�Y -----//.
	//HLSL����s�N�Z���V�F�[�_�̃u���u���쐬.
	if( FAILED(
		D3DX11CompileFromFile(
			sSHADER_NAME,	//�V�F�[�_�t�@�C����(HLSL�t�@�C��).
			NULL,
			NULL,
			"PS_Noise",			//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
			"ps_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����(�v���t�@�C��).
			uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
			0,
			NULL,
			&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors,
			NULL ) ) )
	{
		MessageBox( NULL, "hlsl(ps)�ǂݍ��ݎ��s", "�G���[", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�v���쐬.
	if( FAILED(
		m_wpDevice->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pNoisePS ) ) )//(out)�s�N�Z���V�F�[�_.
	{
		MessageBox( NULL, "�m�C�Yps�쐬���s", "�G���[", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );//�u���u���.

	return S_OK;
}

HRESULT clsSCREEN_TEXTURE::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�R���X�^���g�o�b�t�@���w��.
	cb.ByteWidth = sizeof( SCREEN_TEXTURE_CONSTANT_BUFFER );//�R���X�^���g�o�b�t�@�̃T�C�Y.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//�������݂ŃA�N�Z�X.
	cb.MiscFlags = 0;//���̑��̃t���O(���g�p)
	cb.StructureByteStride = 0;//�\���̂̃T�C�Y(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;//�g�p���@:���ڏ�������.

	//�R���X�^���g�o�b�t�@�쐬.
	if( FAILED( m_wpDevice->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "�R���X�^���g�o�b�t�@�쐬���s", "ScreenTexture", MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

//Rendertarget���e�N�X�`���ɂ���.
void clsSCREEN_TEXTURE::SetRenderTargetTexture( ID3D11DepthStencilView* const pDepthStencilView ) const
{
	if( !pDepthStencilView )	return;

	//�����_�[�^�[�Q�b�g���e�N�X�`����.
	float clearcolor[] = { 2.5f, 0.125f, 0.125f, 1.0f };
	m_wpContext->OMSetRenderTargets( 1, &m_pRenderTargetView, pDepthStencilView );
	m_wpContext->ClearRenderTargetView( m_pRenderTargetView, clearcolor );
	m_wpContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}


//�m�C�Y�̍X�V.
void clsSCREEN_TEXTURE::NoiseUpdate()
{
	m_upSound->UpdateLoop();

	//�|�X�g�G�t�F�N�g.
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

	//���Đ����Ȃ�Đ����t���O�𗧂đ����A��~���Ȃ�Q������.
	if( !m_SeFlagWeak.isCanPlay )
	{
		for( int i=enSE_WEAK_A; i<enSE_WEAK_size; i++ )
		{
			if( m_upSound->IsPlayingSE( i ) ){
				goto PLAYING_SE_WEAK;//�Đ����ł���.
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


//�e�N�X�`���̓��e����ʂɕ`��.
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
	//�|��(�l�p�`)�̒��_���쐬.
	float tmpw = WND_W;
	float tmph = WND_H;
	SCREEN_VERTEX vertices[] = 
	{
#if 1
		//���_���W(x,y,z).					//UV���W( u, v ).
		D3DXVECTOR3( 0.0f,  tmph,  0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),//���_1(����).
		D3DXVECTOR3( 0.0f,  0.0f,  0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),//���_2(����).
		D3DXVECTOR3( tmpw,  tmph,  0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),//���_3(�E��).
		D3DXVECTOR3( tmpw,  0.0f,  0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),//���_4(�E��).
#else
		//���_���W(x,y,z).					//UV���W( u, v ).
		D3DXVECTOR3( tmpw,  0.0f,  0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),//���_4(�E��).
		D3DXVECTOR3( tmpw,  tmph,  0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),//���_3(�E��).
		D3DXVECTOR3( 0.0f,  tmph,  0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),//���_1(����).

		D3DXVECTOR3( tmpw,  0.0f,  0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),//���_4(�E��).
		D3DXVECTOR3( 0.0f,  tmph,  0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),//���_1(����).
		D3DXVECTOR3( 0.0f,  0.0f,  0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),//���_2(����).
#endif
	};
	//�ő�v�f�����Z�o����.
	UINT uVerMax = sizeof( vertices ) / sizeof( vertices[0] );

	//�o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage				= D3D11_USAGE_DEFAULT;				//�g�p�@(�f�t�H���g).
	bd.ByteWidth			= sizeof( SCREEN_VERTEX ) * uVerMax;	//���_�T�C�Y(���_*4).
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;			//���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;								//CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;								//���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;								//�\���̃T�C�Y(���g�p).

	//�T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem	= vertices;	//�|���̒��_���Z�b�g.
	InitData.SysMemPitch= 0;
	InitData.SysMemSlicePitch = 0;

	//���_�o�b�t�@�̍쐬.
	if( FAILED(
		m_wpDevice->CreateBuffer(
			&bd, &InitData, &pBuffer ) ) )
	{
		ERR_MSG( "�o�b�t�@�쐬���s", "" );
		assert( !"�o�b�t�@�쐬���s" );
		return ;
	}

	m_wpContext->VSSetShader( m_pVertexShader, NULL, 0 );
	if( m_isNoise ){
		m_wpContext->PSSetShader( m_pNoisePS, NULL, 0 );
	}
	else{
		m_wpContext->PSSetShader( m_pDefaultPS, NULL, 0 );
	}

	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
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

	//���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�����H.
	m_wpContext->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
	m_wpContext->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );

	//�e�N�X�`�����V�F�[�_�ɓn��.
	m_wpContext->PSSetShaderResources(	0, 1, &m_pShaderResourceView );
	m_wpContext->PSSetSamplers(			0, 1, &m_pSamplerState );

	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof( SCREEN_VERTEX );//�f�[�^�Ԋu.
	uint32_t offset = 0;
	m_wpContext->IASetVertexBuffers( 0, 1, &pBuffer, &stride, &offset );
	m_wpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
//	m_wpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	m_wpContext->Draw( uVerMax, 0 );

}




//���ʉ��Đ�.
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
	//�����_���Ńm�C�Y���Đ�.
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
	//��.
	m_upSound->StopAllSound();

	m_SeFlagStrong.isCanPlay = false;
	m_SeFlagWeak.isCanPlay	 = false;

	m_SeFlagStrong.isContinue = false;
	m_SeFlagWeak.isContinue	 = false;

}
