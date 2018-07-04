#pragma once

//#include "Global.h"
#include "Sprite2DCenter.h"
#include <vector>

class clsRENDER_AT_START_UP
{
public:
	clsRENDER_AT_START_UP(
		ID3D11Device*			 const pDevice,
		ID3D11DeviceContext*	 const pContext,
		IDXGISwapChain*			 const pSwapChain,
		ID3D11RenderTargetView*	 const pBackBuffer_TexRTV,
		ID3D11DepthStencilView*	 const pBackBuffer_DSTexDSV,
		ID3D11DepthStencilState* const pDepthStencilState );
	~clsRENDER_AT_START_UP();

	//ループ処理.
	void Loop();

	//終了処理.
	void End();

private:

	//描画.
	void Render();

	//深度テスト(Zテスト)ON/OFF切替.
	void SetDepth( bool isOn );

	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vupRogo;

	bool						m_bEnd;//trueになったら終了.

	ID3D11Device*				m_wpDevice;
	ID3D11DeviceContext*		m_wpContext;			//デバイスコンテキスト.
	IDXGISwapChain*				m_wpSwapChain;			//スワップチェーン.
	ID3D11RenderTargetView*		m_wpBackBuffer_TexRTV;	//レンダーターゲットビュー.
	ID3D11DepthStencilView*		m_wpBackBuffer_DSTexDSV;//デプスステンシルビュー.
	ID3D11DepthStencilState*	m_wpDepthStencilState;	//深度(Z)テスト設定.
};


