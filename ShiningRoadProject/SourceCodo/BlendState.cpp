#include "BlendState.h"


clsBLEND_STATE::clsBLEND_STATE()
	:m_wpDevice( nullptr )
	,m_wpContext( nullptr )
	,m_isAlpha( false )
	,m_isUse( false )
{

	int i=0;
}
//clsBLEND_STATE::clsBLEND_STATE(
//	ID3D11Device* const pDevice,
//	ID3D11DeviceContext* const pContext )
//	:m_wpDevice( pDevice )
//	,m_wpContext( pContext )
//	,m_isAlpha( false )
//{
//	//�������ƕʂ̂��̂�u���Ȃ��ƍ��Ȃ�.
//	SetBlend( true );
//}

clsBLEND_STATE::~clsBLEND_STATE()
{
	m_wpDevice = nullptr;
	m_wpContext = nullptr;
}

void clsBLEND_STATE::Create(
	ID3D11Device* const pDevice,
	ID3D11DeviceContext* const pContext )
{
	m_wpDevice = pDevice;
	m_wpContext= pContext;
	//�������ƕʂ̂��̂�u���Ȃ��ƍ��Ȃ�.
	SetBlend( !m_isAlpha );
}


void clsBLEND_STATE::SetBlend( const bool isAlpha, const bool isCreateAbsolute )
{
	if( m_isUse ){ 
		return; 
	}
	m_isUse = true;

	//���ꂪtrue�Ȃ�ⓚ���p�Ńu�����h�X�e�[�g�쐬.
	if( isCreateAbsolute ){
	}
	//���Ɠ����Ȃ�킴�킴����ł悢.
	else if( m_isAlpha == isAlpha ){
		return;
	}

	m_isAlpha = isAlpha;

	ID3D11BlendState* pBlendState = nullptr;
	if( FAILED( CreateBlendState( &pBlendState ) ) ){
		assert( !"Can't Create Blend State" );
	}

	//�u�����h�X�e�[�g�̐ݒ�.
	const UINT mask = 0xffffffff;	//�}�X�N�l��.
	m_wpContext->OMSetBlendState( pBlendState, NULL, mask );

	SAFE_RELEASE( pBlendState );

	m_isUse = false;
}

//�u�����h�X�e�[�g�쐬.
HRESULT clsBLEND_STATE::CreateBlendState( ID3D11BlendState** ppBlendState )
{
	D3D11_BLEND_DESC blendDesc;
	CreateBlendDesc( &blendDesc );

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬.
	//png�t�@�C�����ɃA���t�@��񂪂���̂ŁA���߂���悤�Ƀu�����h�X�e�[�g�Őݒ肷��.
	if( FAILED( m_wpDevice->CreateBlendState( &blendDesc, ppBlendState ) ) ){
		assert( !"�u�����h�X�e�[�g�̍쐬�Ɏ��s" );
		return E_FAIL;
	}

	return S_OK;
}

//�u�����h�f�X�N�쐬.
D3D11_BLEND_DESC clsBLEND_STATE::CreateBlendDesc( D3D11_BLEND_DESC* const pBlendDesc )
{
	assert( pBlendDesc );

	ZeroMemory( pBlendDesc, sizeof( D3D11_BLEND_DESC ) );	//������.
	pBlendDesc->IndependentBlendEnable	= false;			//false:RenderTarget[0]�̃����o�[�̂ݎg�p����Btrue:RenderTarget[0�`7]���g�p�ł���(�����_�[�^�[�Q�b�g���ɓƗ������u�����h����).
	pBlendDesc->AlphaToCoverageEnable	= false;			//true:�A���t�@�g�D�J�o���b�W���g�p����.

	//�\���^�C�v
	pBlendDesc->RenderTarget[0].BlendEnable				= true;							//true:�A���t�@�u�����h���g�p����.
	pBlendDesc->RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;		//�A���t�@�u�����h���w��.
	pBlendDesc->RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;	//�A���t�@�u�����h�̔��]���w��.
	pBlendDesc->RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;			//ADD�F���Z����.
	pBlendDesc->RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;				//���̂܂܎g�p.
	pBlendDesc->RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;				//�������Ȃ�.
	pBlendDesc->RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;			//ADD�F���Z����.
	pBlendDesc->RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;	//�S�Ă̐���(RGBA)�ւ̃f�[�^�̊i�[��������.


//	switch( m_encBrendFlg )
//	{
//	case encBLEND_FLAG::ALPHA:
//		pBlendDesc->RenderTarget[0].BlendEnable = static_cast<BOOL>( true );
//		break;
//	case encBLEND_FLAG::NOT_ALPHA:
//		pBlendDesc->RenderTarget[0].BlendEnable = static_cast<BOOL>( false );
//		break;
//	default:
//		break;
//	}

	pBlendDesc->RenderTarget[0].BlendEnable = static_cast<BOOL>( m_isAlpha );

	return *pBlendDesc;
}