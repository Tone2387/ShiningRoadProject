#ifndef RENDER_AT_START_UP_H_
#define RENDER_AT_START_UP_H_

class clsBLEND_STATE;

//#include "Global.h"
#include "RenderSubThreadBase.h"
#include "Sprite2DCenter.h"
#include "LineBox.h"
#include "UiText.h"
#include "BlackScreen.h"
#include <vector>
#include <string>

class clsRENDER_AT_START_UP : public clsRENDER_SUB_THREAD_BASE
{
public:
	clsRENDER_AT_START_UP(
		ID3D11Device*			const pDevice,
		ID3D11DeviceContext*	const pContext,
		IDXGISwapChain*			const pSwapChain,
		ID3D11RenderTargetView*	const pBackBuffer_TexRTV,
		ID3D11DepthStencilView*	const pBackBuffer_DSTexDSV );
	~clsRENDER_AT_START_UP();

	//ループ処理.
	void Loop() override;

	//ロードが終わった時に使う.
	void FinishLoad() override;

private:
	
	void UpdateProduct() override;
	void RenderProduct() const override;

	enum class enMODE
	{
		LINE_V,		//外枠が縦に大きくなる.
		LINE_H,		//外枠が横に大きくなる.

		GAGE_H,		//ゲージの枠が横に大きくなる.
		GAGE_V,		//ゲージの枠が縦に大きくなる.

		GAGE_MOVE,	//ゲージが動く.

		COMPLETE,	//ロード完了.
	}	m_enMode;

	//switch文の中身.
	void BiggerLineBoxV();//外枠が縦に大きくなる.
	void BiggerLineBoxH();//外枠が横に大きくなる.
	void BiggerGageBoxH();//ゲージの枠が横に大きくなる.
	void BiggerGageBoxV();//ゲージの枠が縦に大きくなる.
	void UpdateLoadMsg();//ゲージが動く.

	void Complete();	//ロード完了後.


private:

	int m_iTimer;//モードの、待機時間.


	std::unique_ptr< clsLINE_BOX > m_upLineBox;
	std::unique_ptr< clsLINE_BOX > m_upGageBox;

	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupGage;//ゲージ本体.

	std::unique_ptr< clsBLACK_SCREEN > m_upBlack;

	//ロゴ.
	std::unique_ptr< clsSPRITE2D_CENTER > m_upLogo;

	//ロード中メッセージ.
	std::unique_ptr< clsUiText > m_upText;
	std::string m_sLodeMsg;



#ifdef _DEBUG
	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupDebugImage;
#endif//#ifdef _DEBUG

};


#endif//#ifndef RENDER_AT_START_UP_H_