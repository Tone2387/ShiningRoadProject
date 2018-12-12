#include "Sprite.h"

#include "OperationString.h"

using namespace std;

namespace{
	const char sMASK_PATH_EMPTY[] = "Data\\Image\\maskEmpty.png";

	//�V�F�[�_���̃R���X�^���g�o�b�t�@�ƈ�v���Ă���K�v����.
	struct SPRITESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX	mWVP;		//���[���h,�r���[,�ˉe�̍����ϊ��s��.
		D3DXVECTOR4	vColor;		//�J���[(RGBA�̌^�ɍ��킹��) : �e�N�X�`���̏ォ��F���ڂ���(�Ԃ��ۂ��������邽�߂Ƃ�).
		D3DXVECTOR4	vUV;		//UV���W.
		D3DXVECTOR4 vSplit;//������?.
	};

	//�}�X�N�̍ő喇��.
	const int iMASK_MAX = 1;
}

//============================================================
//�R���X�g���N�^.
//============================================================
clsSprite::clsSprite()
	:m_fAlpha( 1.0f )
{
	//������.
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
//�f�X�g���N�^.
//============================================================
clsSprite::~clsSprite()
{
	for( unsigned int i=0; i<m_vecpMask.size(); i++ ){
		SAFE_RELEASE( m_vecpMask[i] );
	}
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );
}

//============================================================
//������.
//============================================================
HRESULT clsSprite::Create( ID3D11Device* const pDevice11,
	ID3D11DeviceContext* const pContext11,
	const char* sTexName )
{
	m_wpDevice = pDevice11;
	m_wpContext = pContext11;

	if( FAILED( CreateBlendState() ) ){
		return E_FAIL;
	}
	//�V�F�[�_�쐬.
	if( FAILED( InitShader( sTexName ) ) ){
		return E_FAIL;
	}
	//�|���S���쐬.
	if( FAILED( InitModel( sTexName ) ) ){
		return E_FAIL;
	}
	//�}�X�N�쐬.
	if( FAILED( CreateMask( sTexName ) ) ){
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	HLSL�t�@�C����ǂݍ��݃V�F�[�_���쐬����.
//	HLSL:HIGE-LEVEL-SHADER-LANGUAGE.
//============================================================
HRESULT clsSprite::InitShader( const char* sErrFileName )
{
	//�V�F�[�_�t�@�C����(�f�B���N�g�����܂�)(\\).
	const char SHADER_NAME[] = "Shader\\Mesh.hlsl";

	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;

	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION; 
#endif//#ifdef _DEBUG


	//HLSL����o�[�e�b�N�X�V�F�[�_�̃u���u���쐬.
	if( FAILED( D3DX11CompileFromFile(
		SHADER_NAME,	//�V�F�[�_�t�@�C����(HLSL�t�@�C��).
		NULL,			//�}�N����`�̔z��ւ̃|�C���^(���g�p).
		NULL,			//�C���N���[�h�t�@�C���������C���^�[�t�F�[�X�ւ̃|�C���^(���g�p).
		"VS_Ita",			//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
		"vs_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����(�v���t�@�C��).
		uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
		0,				//�G�t�F�N�g�R���p�C���t���O(���g�p).
		NULL,			//�X���b�h�|���v�C���^�[�t�F�[�X�ւ̃|�C���^(���g�p).
		&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
		&pErrors,		//�G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		NULL ) ) )		//�߂�l�ւ̃|�C���^(���g�p).
	{
		MessageBox( NULL, "hlsl(vs)�ǂݍ��ݎ��s", sErrFileName, MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�v���쐬.
	if( FAILED( m_wpDevice->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		NULL,
		&m_pVertexShader) ) )//(out)�o�[�e�b�N�X�V�F�[�_.
	{
		MessageBox( NULL, "vs�쐬���s", sErrFileName, MB_OK );
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{
			"POSITION",						//�ʒu.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI�̃t�H�[�}�b�g(32bit float�^*3).
			0,
			0,								//�f�[�^�̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",					//�e�N�X�`���ʒu.
			0,
			DXGI_FORMAT_R32G32_FLOAT,	//DXGI�̃t�H�[�}�b�g(32bit float�^*2).
			0,
			12,							//�e�N�X�`���f�[�^�̊J�n�ʒu(���_�f�[�^��float�^*3=12bite�Ȃ̂�+12).
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );//.

	//���_�C���v�b�g���C�A�E�g�̍쐬.
	if( FAILED( m_wpDevice->CreateInputLayout(
		layout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		&m_pVertexLayout ) ) )//(out)���_�C���v�b�g���C�A�E�g.
	{
		MessageBox( NULL, "���_�C���v�b�g���C�A�E�g�쐬���s", sErrFileName, MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );





	//HLSL����s�N�Z���V�F�[�_�̃u���u���쐬.
	if( FAILED( D3DX11CompileFromFile(
		SHADER_NAME,	//�V�F�[�_�t�@�C����(HLSL�t�@�C��).
		NULL,
		NULL,
		"PS_Ita",			//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
		"ps_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����(�v���t�@�C��).
		uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
		0,
		NULL,
		&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
		&pErrors,
		NULL ) ) )
	{
		MessageBox( NULL, "hlsl(ps)�ǂݍ��ݎ��s", sErrFileName, MB_OK );
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


	//�R���X�^���g�o�b�t�@(�萔)�o�b�t�@�쐬�@�V�F�[�_�ɓ���̐��l�𑗂�o�b�t�@.
	//�����ł͕ϊ��s��n���p.
	//�V�F�[�_��World,View,Projection�s���n��.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;;			//�R���X�g�o�b�t�@���w��.
	cb.ByteWidth	= sizeof( SPRITESHADER_CONSTANT_BUFFER );//�R���X�^���g�o�b�t�@�̃T�C�Y.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//�������݂ŃA�N�Z�X.
	cb.MiscFlags	= 0;					//���̑��̃t���O(���g�p).
	cb.StructureByteStride	= 0;			//�\���̂̃T�C�Y(���g�p)
	cb.Usage		= D3D11_USAGE_DYNAMIC;	//�g�p���@:���ڏ�������.

	//�R���X�^���g�o�b�t�@�쐬.
	if( FAILED( m_wpDevice->CreateBuffer(
		&cb,
		NULL,
		&m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "�R���X�^���g�o�b�t�@�쐬���s", sErrFileName, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	���f���쐬.
//============================================================
HRESULT clsSprite::InitModel( const char* sTexName )
{
	const float itaW = 0.5f;
	float w, h;
//	w = h = ( 1.0f / 8.0f );
	w = h = 1.0f;

	//�|��(�l�p�`)�̒��_���쐬.
	SpriteVertex vertices[] = 
	{
		//���_���W(x,y,z).					//UV���W( u, v ).
		D3DXVECTOR3(-itaW,-itaW, 0.0f ),	D3DXVECTOR2( 0.0f,    h ),	//���_1.
		D3DXVECTOR3(-itaW, itaW, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),	//���_2.
		D3DXVECTOR3( itaW,-itaW, 0.0f ),	D3DXVECTOR2(	w,	  h ),	//���_3.
		D3DXVECTOR3( itaW, itaW, 0.0f ),	D3DXVECTOR2(	w, 0.0f )	//���_4.
	};
	//�ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//�o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;		//�g�p�@(�f�t�H���g).
	bd.ByteWidth		= sizeof( SpriteVertex ) * uVerMax;//���_�T�C�Y(���_*4).
	bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;	//���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags	= 0;						//CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags		= 0;						//���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;					//�\���̃T�C�Y(���g�p).

	//�T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem	= vertices;	//�|���̒��_���Z�b�g.

	//���_�o�b�t�@�̍쐬.
	if( FAILED( m_wpDevice->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) )
	{
		MessageBox( NULL, "���_�o�b�t�@�쐬���s", sTexName, MB_OK );
		return E_FAIL;
	}


	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof( SpriteVertex );//�f�[�^�Ԋu.
	UINT offset = 0;//�J�n�ʒu.
	m_wpContext->IASetVertexBuffers(
		0, 1, 
		&m_pVertexBuffer, &stride, &offset );


	//�e�N�X�`���p�̃T���v���[�\����.
	D3D11_SAMPLER_DESC	SamDesc;
	ZeroMemory( &SamDesc, sizeof( SamDesc ) );
	//���j�A�t�B���^�[(���^���).
	//	POINT:���������e��.
	SamDesc.Filter	= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//���b�s���O���[�h.
	//	WRAP:�J��Ԃ�.
	//	MIRROR	:���]�J��Ԃ�.
	//	CLAMP	:�[�̖͗l���������΂�.
	//	BORDER	;�ʓr���E�F�����߂�.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;


	//�T���v���[�쐬.
	if( FAILED( m_wpDevice->CreateSamplerState(
		&SamDesc, &m_pSampleLinear ) ) )//(out)�T���v���[.
	{
		MessageBox( NULL, "�T���v���쐬���s", sTexName, MB_OK );
		return E_FAIL;
	}

	//�e�N�X�`���쐬.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_wpDevice,		//���\�[�X���g�p����f�o�C�X�ւ̃|�C���^.
		sTexName,
		NULL, NULL,
		&m_pTexture,	//(out)�e�N�X�`��.
		NULL ) ) )
	{
		MessageBox( NULL, "�e�N�X�`���쐬���sw", sTexName, MB_OK );
		return E_FAIL;
	}


	return S_OK;
}

//�}�X�N�쐬.
HRESULT clsSprite::CreateMask( const char* sTexName )
{
	//----- �}�X�N�쐬 -----//.
	//�e�N�X�`���p�X����e�N�摜��������.
	const char sTYPE[] = ".png";
	char sTmpMaskPath[ 128 ] = "";
	strcpy_s( sTmpMaskPath, sizeof( sTmpMaskPath ), sTexName );
	char* pcType = strstr( sTmpMaskPath, sTYPE );
	*pcType = '\0';

	//�}�X�N�摜���A��.
	const char sMASK_NAME[] = "Mask";
	strcat_s( sTmpMaskPath, sizeof( sTmpMaskPath ), sMASK_NAME );
	
	clsOPERATION_STRING OprtStr;
	m_vecpMask.resize( iMASK_MAX, nullptr );

	for( unsigned int i=0; i<m_vecpMask.size(); i++ ){
		//�}�X�N�ԍ��A��.
		string sMaskPath = OprtStr.ConsolidatedNumber( sTmpMaskPath, i );
		sMaskPath += sTYPE;

		//�}�X�N�쐬.
		if( FAILED( D3DX11CreateShaderResourceViewFromFile(
			m_wpDevice,		//���\�[�X���g�p����f�o�C�X�ւ̃|�C���^.
			sMaskPath.c_str(),
			NULL, NULL,
			&m_vecpMask[i],		//(out)�}�X�N�e�N�X�`��.
			NULL ) ) )
		{
			//�쐬���s������^�����̃}�X�N�쐬.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_wpDevice,		//���\�[�X���g�p����f�o�C�X�ւ̃|�C���^.
				sMASK_PATH_EMPTY,
				NULL, NULL,
				&m_vecpMask[i],		//(out)�}�X�N�e�N�X�`��.
				NULL ) ) )
			{
				MessageBox( NULL, sMaskPath.c_str(), sTexName, MB_OK );
				return E_FAIL;		
			}
		}
	}

	return S_OK;
}


//============================================================
//�`��(�����_�����O)(��DX9MESH����Main����2���݂���̂Œ���).
//============================================================
void clsSprite::Render( 
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3 &vEye, 
	const D3DXVECTOR4& vColor,
	bool isBillBoard )
{
	//���[���h�s��.
	D3DXMATRIX	mWorld, mScale, mTrans, mYaw, mPitch, mRoll;
	//���[���h�ϊ�(�\���ʒu��ݒ肷��).
	D3DXMatrixIdentity( &mWorld );	//������:�P�ʍs��쐬.

	//�g�k.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );

	//���s�ړ�.
	D3DXMatrixTranslation( &mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z );

	D3DXMatrixRotationZ( &mRoll,	m_vRot.z );
	D3DXMatrixRotationY( &mYaw,		m_vRot.y );
	D3DXMatrixRotationX( &mPitch,	m_vRot.x );

	//���Z.
	mWorld = mScale * mYaw * mPitch * mRoll * mTrans;

	//�g�p����V�F�[�_�̓o�^.
	m_wpContext->VSSetShader( m_pVertexShader, NULL, 0 );
	m_wpContext->PSSetShader( m_pPixelShader,  NULL, 0 );

	if( isBillBoard ){
		//�r���{�[�h�p.
		D3DXMATRIX CancelRotation = mView;//�r���[(�J����)�s��.
		CancelRotation._41 =
			CancelRotation._42 = CancelRotation._43 = 0;//xyz��0�ɂ���.
		//CancelRotation�̋t�s������߂�.
		D3DXMatrixInverse( &CancelRotation, NULL, &CancelRotation );
		mWorld = CancelRotation * mWorld;
	}


	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITESHADER_CONSTANT_BUFFER cd;	//�R���X�^���g�o�b�t�@.
	//�o�b�t�@���̃f�[�^�̏������J�n����map.
	if( SUCCEEDED( m_wpContext->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//���[���h,�J����,�v���W�F�N�V�����s���n��.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose( &m, &m );	//�s���]�u����.
					//�s��̌v�Z���@��DirectX��GPU�ňقȂ邽��.
		cd.mWVP = m;

		//�J���[.
		cd.vColor = vColor;
		cd.vColor.w = m_fAlpha;

		//����.
		D3DXVECTOR4 vSplit( 1.0f, 1.0f, 1.0f, 1.0f );
		cd.vSplit = vSplit;
		cd.vSplit.x = m_vSplit.x;
		cd.vSplit.y = m_vSplit.y;

		//�e�N�X�`�����W.
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

		m_wpContext->Unmap( m_pConstantBuffer, 0 );
	}

	//���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg����?.
	m_wpContext->VSSetConstantBuffers(
		2, 1, &m_pConstantBuffer );
	m_wpContext->PSSetConstantBuffers(
		2, 1, &m_pConstantBuffer );

	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof( SpriteVertex );
	UINT offset = 0;
	m_wpContext->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset );

	//���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_wpContext->IASetInputLayout( m_pVertexLayout );

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_wpContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`�����V�F�[�_�ɓn��.
	m_wpContext->PSSetSamplers(
		0, 1, &m_pSampleLinear );	//�T���v���[���Z�b�g.
	m_wpContext->PSSetShaderResources(
		0, 1, &m_pTexture );		//�e�N�X�`�����V�F�[�_�ɓn��.

	for( unsigned int i=0; i<m_vecpMask.size(); i++ ){
		m_wpContext->PSSetShaderResources(
			i + 1, 1, &m_vecpMask[i] );		//�}�X�N���V�F�[�_�ɓn��.
	}

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬&�ݒ�.
	SetBlend( true );


	//�v���~�e�B�u�������_�����O.
	m_wpContext->Draw( 4, 0 );

//	//�A���t�@�u�����h�𖳌��ɂ���.
//	SetBlend( false );

}





