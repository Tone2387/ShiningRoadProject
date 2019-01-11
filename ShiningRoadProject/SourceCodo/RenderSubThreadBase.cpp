#include "RenderSubThreadBase.h"






clsRENDER_SUB_THREAD_BASE::clsRENDER_SUB_THREAD_BASE(
	ID3D11Device*			const pDevice,
	ID3D11DeviceContext*	const pContext,
	IDXGISwapChain*			const pSwapChain,
	ID3D11RenderTargetView*	const pBackBuffer_TexRTV,
	ID3D11DepthStencilView*	const pBackBuffer_DSTexDSV )
	:m_wpDevice(pDevice)
	,m_wpContext( pContext )
	,m_wpSwapChain( pSwapChain )
	,m_wpBackBuffer_TexRTV( pBackBuffer_TexRTV )
	,m_wpBackBuffer_DSTexDSV( pBackBuffer_DSTexDSV )
	,m_pDepthStencilStateOn( nullptr )
	,m_pDepthStencilStateOff( nullptr )
	,m_bEnd( false )
{
	//���̃N���X�̏��������ɊD�F��ʂ��o��̂�h��.
	Render( false );

//	clsBLEND_STATE* pBlend = &clsSINGLETON<clsBLEND_STATE>::GetInstance();
//	pBlend->Create( m_wpDevice, m_wpContext );

	CreateDepthStencilState();
}

clsRENDER_SUB_THREAD_BASE::~clsRENDER_SUB_THREAD_BASE()
{
	End();

	SAFE_RELEASE( m_pDepthStencilStateOff );
	SAFE_RELEASE( m_pDepthStencilStateOn );
	m_wpBackBuffer_DSTexDSV = nullptr;
	m_wpBackBuffer_TexRTV = nullptr;	
	m_wpSwapChain = nullptr;			
	m_wpContext = nullptr;		
	m_wpDevice = nullptr;
}




void clsRENDER_SUB_THREAD_BASE::Update()
{
	UpdateProduct();
	Render();
}

//�N�����̕`��.
void clsRENDER_SUB_THREAD_BASE::Render( bool isLoop ) const
{
	//��ʂ̃N���A.
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//�N���A�F(RGBA��)(0.0f~1.0f).
	//�J���[�o�b�N�o�b�t�@.
	m_wpContext->ClearRenderTargetView(
		m_wpBackBuffer_TexRTV, ClearColor );
	//�f�v�X�X�e���V���r���[�o�b�N�o�b�t�@.
	m_wpContext->ClearDepthStencilView(
		m_wpBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );

	SetDepth( false );	//Z�e�X�g:OFF.

	if( isLoop ){
		RenderProduct();
	}

	//�����_�����O���ꂽ�C���[�W��\��.
	m_wpSwapChain->Present( 0, 0 );

}




HRESULT clsRENDER_SUB_THREAD_BASE::CreateDepthStencilState()
{
	assert( !m_pDepthStencilStateOn );
	assert( !m_pDepthStencilStateOff );

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc,
		sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	//ON.
	depthStencilDesc.DepthEnable = true;
	m_wpDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilStateOn );

	//OFF.
	depthStencilDesc.DepthEnable = false;
	m_wpDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilStateOff );


	return S_OK;
}


//�[�x�e�X�g(Z�e�X�g)ON/OFF�ؑ�.
void clsRENDER_SUB_THREAD_BASE::SetDepth( bool isOn ) const
{
	if( isOn ){
		m_wpContext->OMSetDepthStencilState(
			m_pDepthStencilStateOn, 1 );
	}
	else{
		m_wpContext->OMSetDepthStencilState(
			m_pDepthStencilStateOff, 1 );
	}
}

