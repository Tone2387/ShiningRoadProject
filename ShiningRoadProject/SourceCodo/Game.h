#ifndef GAME_H_
#define GAME_H_

class clsSCENE_BASE;
class clsCAMERA_BASE;
class clsFont;
class clsPOINTER_GROUP;
class clsDxInput;
class clsXInput;
class clsResource;
class clsEffects;
class clsSOUND_MANAGER_BASE;
class clsROBO_STATUS_PLAYER;
class clsBLACK_SCREEN;

#include "Global.h"


//ゲームの動きはこのクラスの中で行う.
class clsGAME
{
public:
	//引数はシーンの初期化に必要なもの.
	clsGAME( 
		const HWND hWnd, 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext,
		D3D10_VIEWPORT* const pViewPort10,
		D3D11_VIEWPORT* const pViewPort11, 
		IDXGISwapChain* const pSwapChain,
		ID3D11RenderTargetView* const pBackBuffer_TexRTV,
		ID3D11DepthStencilView* const pBackBuffer_DSTexDSV );
	~clsGAME();

	//new直後に使う.
	void Create();
	//毎フレーム使う( 戻り値は、正常ならtrue, 終了ならfalse ).
	bool Update();
	//毎フレーム使う.
	void Render() const;




private:

	//引数のシーンに切り替え.
	void SwitchScene( const enSCENE enNextScene );

private:

	//シーンクラス.
	std::unique_ptr< clsSCENE_BASE > m_upScene;

	//カメラ.
	clsCAMERA_BASE*	m_spCamera;
	
	//フォント.
	clsFont*			m_spFont;

	//シーン初期化に必要なポインタのまとめ.
	clsPOINTER_GROUP* m_spPtrGroup; 


	HWND					m_hWnd;
	ID3D11Device*			m_wpDevice;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_wpContext;	//デバイスコンテキスト.
	//2DSp用.
	D3D10_VIEWPORT* m_wpViewPort10;
	//分割用.
	D3D11_VIEWPORT* m_wpViewPort11;


	IDXGISwapChain*			m_wpSwapChain;		//スワップチェーン.
	ID3D11RenderTargetView*	m_wpBackBuffer_TexRTV;//レンダーターゲットビュー.
	ID3D11DepthStencilView*	m_wpBackBuffer_DSTexDSV;//デプスステンシルビュー.


	//ジョイスティック.
	clsDxInput*		m_spDxInput;
	clsXInput*		m_spXInput;

	//リソースクラス.
	clsResource*	m_spResource;
	//エフェクト.
	clsEffects*		m_spEffect;
	//サウンド.
	clsSOUND_MANAGER_BASE* m_spSound;

	//( シーンをまたぐ )ロボットのステータス.
	clsROBO_STATUS_PLAYER* m_spRoboStatus;

	//暗幕を作ろう.
	clsBLACK_SCREEN* m_spBlackScreen;

};


#endif//#ifndef GAME_H_