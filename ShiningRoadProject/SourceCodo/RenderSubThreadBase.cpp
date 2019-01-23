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
	//このクラスの初期化時に灰色画面が出るのを防ぐ.
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

//起動中の描画.
void clsRENDER_SUB_THREAD_BASE::Render( bool isLoop ) const
{
	//画面のクリア.
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//クリア色(RGBA順)(0.0f~1.0f).
	//カラーバックバッファ.
	m_wpContext->ClearRenderTargetView(
		m_wpBackBuffer_TexRTV, ClearColor );
	//デプスステンシルビューバックバッファ.
	m_wpContext->ClearDepthStencilView(
		m_wpBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );

	SetDepth( false );	//Zテスト:OFF.

	if( isLoop ){
		RenderProduct();
	}

	//レンダリングされたイメージを表示.
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


//深度テスト(Zテスト)ON/OFF切替.
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

