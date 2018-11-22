#include "Common.h"

clsCommon::clsCommon()
{
	m_pDevice = nullptr;
	m_pContext = nullptr;
	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		m_pBlendState[i] = nullptr;
	}

//	if( FAILED( CreateBlendState() ) ){
//		assert( !"Can't Create Blend State" );
//	}

};
clsCommon::~clsCommon()
{
	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		SAFE_RELEASE( m_pBlendState[i] );
	}

	//�����ł͊J�����Ȃ�.
	m_pContext = nullptr;
	m_pDevice = nullptr;
};

//�u�����h�X�e�[�g�쐬.
HRESULT clsCommon::CreateBlendState()
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

void clsCommon::SetBlend( const bool isAlpha )
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

