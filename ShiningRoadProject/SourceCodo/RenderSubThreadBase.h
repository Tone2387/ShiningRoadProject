#ifndef RENDER_SUB_THREAD_BASE_H_
#define RENDER_SUB_THREAD_BASE_H_


#include "Global.h"

class clsRENDER_SUB_THREAD_BASE
{
public:
	clsRENDER_SUB_THREAD_BASE(
		ID3D11Device*			const pDevice,
		ID3D11DeviceContext*	const pContext,
		IDXGISwapChain*			const pSwapChain,
		ID3D11RenderTargetView*	const pBackBuffer_TexRTV,
		ID3D11DepthStencilView*	const pBackBuffer_DSTexDSV );
	~clsRENDER_SUB_THREAD_BASE();

	//ループ処理.
	virtual void Loop() = 0;

	//ロードが終わった時に使う.
	//継承先のこの中のどこかでEnd()を使う.
	virtual void FinishLoad() = 0;

protected:

	//継承先のこの中のどこかでEnd()を使う.
	void Update();

	//継承先の内容.
	virtual void UpdateProduct() = 0;
	virtual void RenderProduct() const = 0;


	//終了させるための処理.
	//継承先のUpdate()かFinishLoad()内のどこかで使う.
	void End(){
		m_bEnd = true;//このフラグがtrueになればこのクラスのループは終わる.
	};

	bool isEnd(){
		return m_bEnd;
	}

private:


	//描画.
	void Render( bool isLoop = true ) const;


	HRESULT CreateDepthStencilState();

	//深度テスト(Zテスト)ON/OFF切替.
	void SetDepth( bool isOn ) const;
	

private:


	bool						m_bEnd;//trueになったら終了.

	ID3D11Device*				m_wpDevice;
	ID3D11DeviceContext*		m_wpContext;			//デバイスコンテキスト.
	IDXGISwapChain*				m_wpSwapChain;			//スワップチェーン.
	ID3D11RenderTargetView*		m_wpBackBuffer_TexRTV;	//レンダーターゲットビュー.
	ID3D11DepthStencilView*		m_wpBackBuffer_DSTexDSV;//デプスステンシルビュー.
	ID3D11DepthStencilState*	m_pDepthStencilStateOn;	//深度(Z)テスト設定.
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;	//深度(Z)テスト設定.

};


#endif//RENDER_SUB_THREAD_BASE_H_