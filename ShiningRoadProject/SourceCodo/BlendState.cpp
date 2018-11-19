#include "BlendState.h"




clsBLEND_STATE::clsBLEND_STATE(
	ID3D11Device* const pDevice,
	ID3D11DeviceContext* const pContext,
	const bool isAlphaBlend )
{
	assert( pDevice );
	assert( pContext );

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬.
	//png�t�@�C�����ɃA���t�@��񂪂���̂ŁA���߂���悤�Ƀu�����h�X�e�[�g�Őݒ肷��.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( D3D11_BLEND_DESC ) );	//������.
	blendDesc.IndependentBlendEnable = false;			//false:RenderTarget[0]�̃����o�[�̂ݎg�p����Btrue:RenderTarget[0�`7]���g�p�ł���(�����_�[�^�[�Q�b�g���ɓƗ������u�����h����).
	blendDesc.AlphaToCoverageEnable = false;			//true:�A���t�@�g�D�J�o���b�W���g�p����.

	//�\���^�C�v
	blendDesc.RenderTarget[0].BlendEnable = isAlphaBlend;					//true:�A���t�@�u�����h���g�p����.
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//�A���t�@�u�����h���w��.
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//�A���t�@�u�����h�̔��]���w��.
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			//ADD�F���Z����.
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		//���̂܂܎g�p.
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//�������Ȃ�.
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//ADD�F���Z����.
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//�S�Ă̐���(RGBA)�ւ̃f�[�^�̊i�[��������.


	ID3D11BlendState* pBlendState = nullptr;
	if( FAILED( pDevice->CreateBlendState( &blendDesc, &pBlendState ) ) ){
		assert( !"�u�����h�X�e�[�g�̍쐬�Ɏ��s" );
	}


	const UINT mask = 0xffffffff;	//�}�X�N�l��.
	//�u�����h�X�e�[�g�̐ݒ�.
	pContext->OMSetBlendState( pBlendState, NULL, mask );


}


/*
clsBLEND_STATE::clsBLEND_STATE(
	ID3D11Device* const pDevice,
	ID3D11DeviceContext* const pContext )
	:m_pContext( pContext )
	,m_isAlpha( false )
{
	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		m_pBlendState[i] = nullptr;
	}

	if( FAILED( CreateBlendState( pDevice ) ) ){
		assert( !"Can't Create Blend State" );
	}

	SetBlend( !m_isAlpha );
}

clsBLEND_STATE::~clsBLEND_STATE()
{
	m_isAlpha = false;

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		SAFE_RELEASE( m_pBlendState[i] );
	}

	m_pContext = nullptr;
}


//�u�����h�X�e�[�g�쐬.
HRESULT clsBLEND_STATE::CreateBlendState( ID3D11Device* const pDevice )
{
	assert( pDevice );

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
		assert( !m_pBlendState[i] );
		blendDesc.RenderTarget[0].BlendEnable = tmpBlendEnable[i];
		if( FAILED( pDevice->CreateBlendState( &blendDesc, &m_pBlendState[i] ) ) ){
			assert( !"�u�����h�X�e�[�g�̍쐬�Ɏ��s" );
			return E_FAIL;
		}
	}

	return S_OK;
}


void clsBLEND_STATE::SetBlend( const bool isAlpha ) const
{
	//���Ɠ����Ȃ�킴�킴����ł悢.
	if( isAlpha == m_isAlpha ){
		return;
	}

	const UINT mask = 0xffffffff;	//�}�X�N�l��.

	assert( m_pContext );
	//�u�����h�X�e�[�g�̐ݒ�.
	if( isAlpha ){		
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_ON ], NULL, mask );
	}
	else{
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_OFF ], NULL, mask );
	}
}

*/
