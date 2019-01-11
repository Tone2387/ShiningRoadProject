#ifndef RENDER_AT_LOAD_TIME_H_
#define RENDER_AT_LOAD_TIME_H_

class clsBLEND_STATE;

#include "RenderSubThreadBase.h"
#include "LineBox.h"
#include "Sprite2DCenter.h"
#include <vector>

class clsRENDER_AT_LOAD_TIME : public clsRENDER_SUB_THREAD_BASE
{
public:
	clsRENDER_AT_LOAD_TIME(
		ID3D11Device*			const pDevice,
		ID3D11DeviceContext*	const pContext,
		IDXGISwapChain*			const pSwapChain,
		ID3D11RenderTargetView*	const pBackBuffer_TexRTV,
		ID3D11DepthStencilView*	const pBackBuffer_DSTexDSV );
	~clsRENDER_AT_LOAD_TIME();

//	//ロードが終わった時に使う.
	void FinishLoad() override;

private:
	
	void UpdateProduct() override;
	void RenderProduct() const override;

private:

	std::unique_ptr< clsLINE_BOX > m_upGageBox;//箱.
	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupGage;//ゲージ本体.

	bool m_bTrigger;

};



#endif//#ifndef RENDER_AT_LOAD_TIME_H_