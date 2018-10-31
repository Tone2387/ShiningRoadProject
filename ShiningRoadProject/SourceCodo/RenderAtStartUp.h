#ifndef RENDER_AT_START_UP_H_
#define RENDER_AT_START_UP_H_

//#include "Global.h"
#include "Sprite2DCenter.h"
#include "LineBox.h"
#include "UiText.h"
#include "BlackScreen.h"
#include <vector>
#include <string>

class clsRENDER_AT_START_UP
{
public:
	clsRENDER_AT_START_UP(
		ID3D11Device*			 const pDevice,
		ID3D11DeviceContext*	 const pContext,
		IDXGISwapChain*			 const pSwapChain,
		ID3D11RenderTargetView*	 const pBackBuffer_TexRTV,
		ID3D11DepthStencilView*	 const pBackBuffer_DSTexDSV );
	~clsRENDER_AT_START_UP();

	//ループ処理.
	void Loop();

	//ロードが終わった時に使う.
	void FinishLoad();

private:
	HRESULT CreateDepthStencilState();

	void Update();

	//描画.
	void Render( bool isLoop = true );



	//深度テスト(Zテスト)ON/OFF切替.
	void SetDepth( bool isOn );
	

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


	//終了させるための処理.
	void End();

	int m_iTimer;//モードの、待機時間.

#ifdef _DEBUG
	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupRogo;
#endif//#ifdef _DEBUG

	std::unique_ptr< clsLINE_BOX > m_upLineBox;
	std::unique_ptr< clsLINE_BOX > m_upGageBox;

	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupGage;//ゲージ本体.

	std::unique_ptr< clsBLACK_SCREEN > m_upBlack;

	//ロード中メッセージ.
	std::unique_ptr< clsUiText > m_upText;
	std::string m_sLodeMsg;

	bool						m_bEnd;//trueになったら終了.

	ID3D11Device*				m_wpDevice;
	ID3D11DeviceContext*		m_wpContext;			//デバイスコンテキスト.
	IDXGISwapChain*				m_wpSwapChain;			//スワップチェーン.
	ID3D11RenderTargetView*		m_wpBackBuffer_TexRTV;	//レンダーターゲットビュー.
	ID3D11DepthStencilView*		m_wpBackBuffer_DSTexDSV;//デプスステンシルビュー.
	ID3D11DepthStencilState*	m_pDepthStencilStateOn;	//深度(Z)テスト設定.
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;	//深度(Z)テスト設定.

};


#endif//#ifndef RENDER_AT_START_UP_H_