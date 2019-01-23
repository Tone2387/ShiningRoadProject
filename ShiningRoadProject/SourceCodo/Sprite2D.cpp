#include "Sprite2D.h"
#include "BlendState.h"

namespace{

}


//�R���X�g���N�^.
clsSprite2D::clsSprite2D() :
	m_pVertexShader( nullptr ),	
	m_pVertexLayout( nullptr ),	
	m_pPixelShader( nullptr ),	
	m_pConstantBuffer( nullptr ),
	m_pVertexBuffer( nullptr ),	
	m_pTexture( nullptr ),		
	m_pSampleLinear( nullptr )
	,m_vPos( 0.0f, 0.0f, 0.0f )
	,m_vScale( 1.0f, 1.0f, 1.0f )
	,m_vColor( { 1.0f, 1.0f, 1.0f, 1.0f } )
{
//	ZeroMemory( this, sizeof( clsSprite2D ) );


	m_fPatternNo = { 0.0f, 0.0f };
}

//�f�X�g���N�^.
clsSprite2D::~clsSprite2D()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );
}

//������.
HRESULT clsSprite2D::Create(
	ID3D11Device* const pDevice11, ID3D11DeviceContext* const pContext11,
	const char* fileName, const SPRITE_STATE& ss )
{
	m_wpDevice = pDevice11;
	m_wpContext = pContext11;

	m_SState = ss;

	if( FAILED( InitShader( fileName ) ) ){
		return E_FAIL;
	}
	if( FAILED( InitModel( ss, fileName ) ) ){
		return E_FAIL;
	}
	if( FAILED( CreateTexture( fileName, &m_pTexture ) ) ){
		return E_FAIL;
	}

	return S_OK;
}

//======================================
//	HLSL�t�@�C����ǂݍ��݃V�F�[�_���쐬����.
//	HLSL:HIGH-LEVEL-SHADER-LANGUAGE.
//======================================
HRESULT clsSprite2D::InitShader( const char* sErrFileName )
{
	//�V�F�[�_�t�@�C����(�f�B���N�g�����܂�)
	const char SHADER_NAME[] = "Shader\\Sprite2D.hlsl";

	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSL����o�[�e�b�N�X�V�F�[�_�̃u���u���쐬.
	if( FAILED(	D3DX11CompileFromFile(
		SHADER_NAME,	//�V�F�[�_�t�@�C����(HLSL�t�@�C��)
		NULL,			//�}�N����`�̔z��ւ̃|�C���^(���g�p)
		NULL,			//�C���N���[�h�t�@�C���������C���^�[�t�F�C�X�ւ̃|�C���^(���g�p)
		"VS_Main",		//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
		"vs_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����(�v���t�@�C��)
		uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
		0,				//�G�t�F�N�g�R���p�C���t���O(���g�p)
		NULL,			//�X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^(���g�p)
		&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
		&pErrors,		//�G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		NULL ) ) )		//�߂�l�ւ̃|�C���^(���g�p)
	{
		MessageBox(NULL, "hlsl�Ǎ����s", sErrFileName, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�v���쐬.
	if( FAILED(	m_wpDevice->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pVertexShader ) ) )//(out)�o�[�e�b�N�X�V�F�[�_.
	{
		MessageBox(NULL, "vs�쐬���s", sErrFileName, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",				//�ʒu.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,//DXGI�̃t�H�[�}�b�g(32bit float�^*3)
			0,
			0,							//�f�[�^�̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",		//�e�N�X�`���ʒu.
			0,
			DXGI_FORMAT_R32G32_FLOAT,	//DXGI�̃t�H�[�}�b�g(32bit float�^*2)
			0,
			12,							//�e�N�X�`���f�[�^�̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	//���_�C���v�b�g���C�A�E�g�̍쐬.
	if( FAILED(	m_wpDevice->CreateInputLayout(
		layout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		&m_pVertexLayout ) ) )//(out)���_�C���v�b�g���C�A�E�g.
	{
		MessageBox(NULL, "���_�C���v�b�g���C�A�E�g�쐬���s", sErrFileName, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );


	//HLSL����s�N�Z���V�F�[�_�̃u���u���쐬.
	if( FAILED(	D3DX11CompileFromFile(
		SHADER_NAME,	//�V�F�[�_�t�@�C����(HLSL�t�@�C��)
		NULL,
		NULL,
		"PS_Main",		//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
		"ps_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����(�v���t�@�C��)
		uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
		0,
		NULL,
		&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
		&pErrors,
		NULL ) ) )
	{
		MessageBox( NULL, "hlsl(ps)�Ǎ����s", sErrFileName, MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�v���쐬.
	if( FAILED( m_wpDevice->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pPixelShader ) ) )//(out)�s�N�Z���V�F�[�_.
	{
		MessageBox( NULL, "ps�쐬���s", sErrFileName, MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );//�u���u���.



	//�R���X�^���g(�萔)�o�b�t�@�쐬 �V�F�[�_�ɓ���̐��l�𑗂�o�b�t�@.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�R���X�^���g�o�b�t�@���w��.
	cb.ByteWidth = sizeof(SPRITE2D_CONSTANT_BUFFER);//�R���X�^���g�o�b�t�@�̃T�C�Y.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//�������݂ŃA�N�Z�X.
	cb.MiscFlags = 0;//���̑��̃t���O(���g�p)
	cb.StructureByteStride = 0;//�\���̂̃T�C�Y(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;//�g�p���@:���ڏ�������.

	//�R���X�^���g�o�b�t�@�쐬.
	if( FAILED(	m_wpDevice->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "�R���X�^���g�o�b�t�@�쐬���s", sErrFileName, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}



//================================================
//	���f���쐬.
//================================================
HRESULT clsSprite2D::InitModel( const SPRITE_STATE& ss, const char* sErrFileName )
{
	float fW = ss.Disp.w;	//�\���X�v���C�g��.
	float fH = ss.Disp.h;	//�\���X�v���C�g����.
	float fU = 1.0f/ ss.Anim.w;	//�P�R�}������̕�.
	float fV = 1.0f/ ss.Anim.h;	//�P�R�}������̍���.

	//�|��(�l�p�`)�̒��_���쐬.
	SpriteVertex vertices[] =
	{
		//���_���W(x,y,z)				UV���W(u,v)
		D3DXVECTOR3( 0.0f,   fH, 0.0f), D3DXVECTOR2(0.0f,   fV),//���_1(����).
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),//���_2(����).
		D3DXVECTOR3(   fW,   fH, 0.0f), D3DXVECTOR2(  fU,   fV),//���_3(�E��).
		D3DXVECTOR3(   fW, 0.0f, 0.0f), D3DXVECTOR2(  fU, 0.0f)	//���_4(�E��).
	};
	//�ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//�o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;//�g�p��(�f�t�H���g)
	bd.ByteWidth = sizeof( SpriteVertex ) * uVerMax;//���_�T�C�Y(���_�~4)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags = 0;	//CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags = 0;	//���̑��̃t���O(���g�p)
	bd.StructureByteStride = 0;//�\���̂̃T�C�Y(���g�p)

	//�T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;//�|���̒��_���Z�b�g.

	//���_�o�b�t�@�̍쐬.
	if( FAILED( m_wpDevice->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) )
	{
		MessageBox( NULL, "���_�o�b�t�@�쐬���s", sErrFileName, MB_OK );
		return E_FAIL;
	}


	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof( SpriteVertex );//�f�[�^�Ԋu.
	UINT offset = 0;
	m_wpContext->IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer, &stride, &offset );


	//�e�N�X�`���p�̃T���v���[�\����.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory( &SamDesc, sizeof( SamDesc ) );
	//���j�A�t�B���^-(���`���)
	//	POINT:���������e��.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//���b�s���O���[�h.
	//	WRAP  :�J��Ԃ�
	//	MIRROR:���]�J��Ԃ�.
	//	CLAMP :�[�̖͗l�������L�΂�.
	//	BORDER:�ʓr���E�F�����߂�.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//�T���v���[�쐬.
	if( FAILED( m_wpDevice->CreateSamplerState(
		&SamDesc, &m_pSampleLinear ) ) )//(out)�T���v���[.
	{
		MessageBox( NULL, "�T���v���쐬���s", sErrFileName, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

//================================================
//	�e�N�X�`���쐬.
//================================================
HRESULT clsSprite2D::CreateTexture(
	const char* const fileName,
	ID3D11ShaderResourceView** pTex )
{
	//�e�N�X�`���쐬.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_wpDevice,	//���\�[�X���g�p����f�o�C�X�ւ̃|�C���^.
		fileName,		//�t�@�C����.
		NULL, NULL,
		pTex,			//(out)�e�N�X�`��.
		NULL ) ) )
	{
		ERR_MSG( fileName, "clsSprite2D::CreateTexture()" );
		return E_FAIL;
	}

	return S_OK;
}


//�|���S������ʂɃ����_�����O.
void clsSprite2D::Render()
{
	//���[���h�s��.
	D3DXMATRIX	mWorld, mTrans, mScale;

	//���[���h�ϊ�(�\���ʒu��ݒ肷��)
	D3DXMatrixIdentity( &mWorld );	//������:�P�ʍs��쐬.
	D3DXMatrixIdentity( &mTrans );	//������:�P�ʍs��쐬.
	D3DXMatrixIdentity( &mScale );	//������:�P�ʍs��쐬.

	//�g�k.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );


	//���s�ړ�.
	D3DXMatrixTranslation( &mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z );

	//����.
	mWorld = mScale * mTrans;

	//�g�p����V�F�[�_�̓o�^.
	m_wpContext->VSSetShader( m_pVertexShader, NULL, 0 );
	m_wpContext->PSSetShader( m_pPixelShader,  NULL, 0 );


	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITE2D_CONSTANT_BUFFER cd;	//�R���X�^���g�o�b�t�@.
	//�o�b�t�@���̃f�[�^�̏������J�n����map.
	if( SUCCEEDED( m_wpContext->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//���[���h�s���n��.
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose( &m, &m );	//�s���]�u����.					
		//�s��̌v�Z���@��DirectX��GPU�ňقȂ��.
		cd.mW = m;

		//�r���[�|�[�g�̕��A������n��.
		cd.fViewPortWidth = WND_W;
		cd.fViewPortHeight= WND_H;

		//�A���t�@�l��n��.
		cd.vColor = m_vColor;

		//UV���W.
		//�P�R�}������̊����ɃR�}�ԍ����|���Ĉʒu��ݒ肷��.
		cd.vUV.x = ( 1.0f / m_SState.Anim.w ) * m_fPatternNo.x;
		cd.vUV.y = ( 1.0f / m_SState.Anim.h ) * m_fPatternNo.y;

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)( &cd ), sizeof( cd ) );

		m_wpContext->Unmap( m_pConstantBuffer, 0 );
	}

	//���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�����H.
	m_wpContext->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );
	m_wpContext->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );

	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof( SpriteVertex );	//�f�[�^�̊Ԋu.
	UINT offset = 0;
	m_wpContext->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset );

	//���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_wpContext->IASetInputLayout( m_pVertexLayout );

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_wpContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//�e�N�X�`�����V�F�[�_�ɓn��.
	m_wpContext->PSSetSamplers(
		0, 1, &m_pSampleLinear );	//�T���v��-���Z�b�g.
	m_wpContext->PSSetShaderResources(
		0, 1, &m_pTexture );		//�e�N�X�`�����V�F�[�_�ɓn��.

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬���ݒ�.
	m_psinBlend->SetBlend( true );

	//�v���~�e�B�u�������_�����O.
	m_wpContext->Draw( 4, 0 );

//	//�A���t�@�u�����h�𖳌��ɂ���.
//	SetBlend(false);
}





void clsSprite2D::SetScale( const float &fScale, const bool withZ )
{
	m_vScale.x = fScale;
	m_vScale.y = fScale;

	if( withZ ) return;
	m_vScale.z = fScale;
}

bool clsSprite2D::AddAlpha( const float fAlpha )
{
	//�͈͓�.
	bool isWithInRange = true;

	m_vColor.w += fAlpha;

	//�͈͊m�F.
	if( m_vColor.w < 0.0f ){
		m_vColor.w = 0.0f;
		isWithInRange = false;
	}
	else if( m_vColor.w > 1.0 ){
		m_vColor.w = 1.0f;
		isWithInRange = false;
	}

	return isWithInRange;
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

void clsSprite2D::SetAnim( const POINTFLOAT& anim )
{
	m_fPatternNo = anim;
}



void clsSprite2D::SetColor( const D3DXVECTOR3& vColor )
{
	m_vColor.x = vColor.x;
	m_vColor.y = vColor.y;
	m_vColor.z = vColor.z;
}

D3DXVECTOR3 clsSprite2D::GetColor()
{
	D3DXVECTOR3 vReturn;

	vReturn.x = m_vColor.x;
	vReturn.y = m_vColor.y;
	vReturn.z = m_vColor.z;

	return vReturn;
}
