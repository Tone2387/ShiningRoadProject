#ifndef SCENE_BASE_H_
#define SCENE_BASE_H_

//ついているとバックバッファではなくいったんテクスチャに描画する.
//#define RENDER_SCREEN_TEXTURE_


#include "Global.h"

#if _DEBUG
#include "DebugText.h"
#endif//#if _DEBUG

#include "CharaStatic.h"

#include "PtrGroup.h"

#include "Sprite2DCenter.h"

#include "UiText.h"

//#include "KeyInput.h"


//================================//
//========== 基底クラス ==========//
//================================//
class clsSCENE_BASE
{
public:
	//引数はリソースなどのポインタのまとめ.
	clsSCENE_BASE( clsPOINTER_GROUP* const ptrGroup );
	virtual ~clsSCENE_BASE();

	//----- 各シーン共通 -----//.
	//シーン作成直後に「SceneManager.cpp」の「SwitchScene」関数内で使用されている.
	void Create();
	//ループ内の処理( 引数を関数内で変更すると今のシーンが破棄され、.
	//				  指定したシーンが生成される ).
	void Update( enSCENE &enNextScene );
	//シーン内のオブジェクトの描画関数のまとめ.
	void Render( 
		ID3D11RenderTargetView* const pBackBuffer_TexRTV,
		ID3D11DepthStencilView* const pDepthStencilView );
	//----- 各シーン共通 -----//.


	//メインまで送り上げる.
	D3DXVECTOR3 GetCameraPos() const;
	D3DXVECTOR3 GetCameraLookPos() const;

protected:

	//----- 各シーンごとの関数 -----//.
	virtual void CreateProduct() = 0;//各シーンのCreate.
	virtual void UpdateProduct( enSCENE &enNextScene ) = 0;//各シーンのUpdate.
	virtual void RenderProduct( const D3DXVECTOR3 &vCamPos ) = 0;//各シーンのRender.
	virtual void RenderUi() = 0;//各シーンの「 UIの 」Render.
	//----- 各シーンごとの関数 -----//.

	//3D座標をスクリーン( 2D )座標へと変換する.
	//dimensions(次元) conversion(変換).
	//戻り値は2D座標.
	D3DXVECTOR3 ConvDimPos( const D3DXVECTOR3 &v3DPos );


	//メニュー操作に使ってね.
	bool isPressRight();
	bool isPressLeft();
	bool isPressUp();
	bool isPressDown();
	bool isPressEnter();
	bool isPressExit();

	//押しっぱなしで動く( trueならスティック有効 ).
	bool isPressHoldRight( bool isWithStick = true );
	bool isPressHoldLeft ( bool isWithStick = true );
	bool isPressHoldUp	 ( bool isWithStick = true );
	bool isPressHoldDown ( bool isWithStick = true );
//	int m_iHoldFreamRight;
//	int m_iHoldFreamLeft;
//	int m_iHoldFreamUp;
//	int m_iHoldFreamDown;
	struct HOLD_STATE
	{
		int iHoldFream;
		int iFirstPush;//最初の一歩.
		HOLD_STATE()
		:iHoldFream( 0 )
		,iFirstPush( 0 )
		{}
	};
	HOLD_STATE m_HoldRight;
	HOLD_STATE m_HoldLeft;
	HOLD_STATE m_HoldUp;
	HOLD_STATE m_HoldDown;

	//----- Render用 -----//.
	//深度テスト(Zテスト)　ON/OFF切替.
	void SetDepth( const bool isOn );
	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;	//プロジェクション行列.
	D3DXVECTOR3		m_vLight;	//ライトの方向.

	//---継承先のRenderProductで使用する---.
	void SetViewPort( 
		D3D11_VIEWPORT* const pVp, 
		const D3DXVECTOR3 &vCamPos, 
		const D3DXVECTOR3 &vCamLookPos,
		const float fWndW, const float fWndH );
	//メインで使っているビューポートのポインタ取得( SetViewPort関数の引数用 ).
	D3D11_VIEWPORT* GetViewPortMainPtr();
	//----- Render用 -----//.



	//デバッグテキストクラス.
#if _DEBUG
//	clsDebugText*	m_upText;
	std::unique_ptr< clsDebugText >	m_upText;
	//デバック゛テキストの表示.
	virtual void RenderDebugText();
	//BGMのチェック.
	void DebugBgm();

#endif//#if _DEBUG


	//基底クラスのポインタは基底クラスで破棄します.
	//派生クラスでは破棄しないでください.

	//以下、消すときdeleteしないでnullしてね( この基底クラスのデストラクタでやっています ).
	//カメラ( 必要ならばこれを消して、シーンごとに異なった機能のカメラを作りましょう ).
	clsPOINTER_GROUP*		m_wpPtrGroup;//エフェクトや効果音を必要とするロボちゃんの為にもprivateにするわけにはいかない.
	ID3D11Device*			m_wpDevice;	//デバイスオブジェクト.
	ID3D11DeviceContext*	m_wpContext;//デバイスコンテキスト.
	clsDxInput*				m_wpDxInput;
	clsXInput*				m_wpXInput;
	clsResource*			m_wpResource;
	clsEffects*				m_wpEffects;
	clsSOUND_MANAGER_BASE*	m_wpSound;
	clsCAMERA_BASE*			m_wpCamera;	
	clsROBO_STATUS_PLAYER*	m_wpRoboStatus;
	clsBLACK_SCREEN*		m_wpBlackScreen;
	clsFont*				m_wpFont;

private:

	HRESULT CreateDepthStencilState();

	//----- Render用 -----//.
	//カメラ関数.
	void Camera();
	//プロジェクション関数.
	void Proj();
	//----- Render用 -----//.

	//デバッグ用シーン切り替え.
	void DebugChangeScene( enSCENE &enNextScene ) const;

private:

	//暗転中に待ってくれるために必要.
	enSCENE m_enNextScene;

	//2DSp用.
	D3D10_VIEWPORT* m_wpViewPort10;
	//分割用( メインのビューポート ).
	D3D11_VIEWPORT* m_wpViewPort11;
	
	//診断用( 今現在使っているビューポート ).
	D3D11_VIEWPORT* m_wpViewPortUsing;

	//奥行on, off.
	ID3D11DepthStencilState* m_pDepthStencilStateOn;
	ID3D11DepthStencilState* m_pDepthStencilStateOff;

#ifdef RENDER_SCREEN_TEXTURE_
	//レンダリングテクスチャ用.
	ID3D11Texture2D*			m_pScreenTex;
	ID3D11RenderTargetView*		m_pScreenRTV;
	ID3D11ShaderResourceView*	m_pScreenSRV;
	ID3D11SamplerState*			m_pScreenSmp;
	ID3D11VertexShader*			m_pScreenVS;
	ID3D11PixelShader*			m_pScreenPS;
	HRESULT CreateScreenTexture();
	HRESULT CreateScreenShaderTexture();
	//Rendertargetをテクスチャにする.
	void SetRenderTargetTexture( ID3D11DepthStencilView* const pDepthStencilView );
	//テクスチャの内容を画面に描画.
	void RenderWindowFromTexture( 
		ID3D11RenderTargetView* const pBackBuffer_TexRTV,
		ID3D11DepthStencilView* const pDepthStencilView );
#endif//#ifdef RENDER_SCREEN_TEXTURE_


};
#endif//#ifndef SCENE_BASE_H_