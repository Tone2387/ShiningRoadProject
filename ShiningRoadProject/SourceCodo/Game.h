#pragma once

#include "FactoryScene.h"
#include "FactoryCamera.h"
#include "FactorySoundManager.h"


#include "DxInput.h"
#include "CXInput.h"

#include "BlackScreen.h"

#include <memory>

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
		D3D11_VIEWPORT* const pViewPort11 );
	~clsGAME();

	//new直後に使う.
	void Create();
	//毎フレーム使う.
	void Update();
	//毎フレーム使う.
	void Render( 
		ID3D11RenderTargetView *pBackBuffer_TexRTV,
		ID3D11DepthStencilView *pBackBuffer_DSTexDSV );


	//ラップ関数.
	D3DXVECTOR3 GetCameraPos() const;
	D3DXVECTOR3 GetCameraLookPos() const;


private:

	//引数のシーンに切り替え.
	void SwitchScene( const enSCENE enNextScene, const bool bStartUp = false );

	//シーンクラス.
	clsSCENE_BASE*		m_upScene;
	//シーンのファクトリ.
	std::unique_ptr< clsSCENE_FACTORY > m_upSceneFactory;

	//カメラ.
	clsCAMERA_BASE*	m_spCamera;
	//カメラファクトリ.
	std::unique_ptr< clsFACTORY_CAMERA > m_upCameraFactory;

	//サウンドのファクトリ.
	std::unique_ptr< clsFACTORY_SOUND_MANAGER > m_upSoundFactory;
	
	//フォント.
	clsFont*			m_spFont;


	//シーン初期化に必要なポインタのまとめ.
	clsPOINTER_GROUP* m_spPtrGroup; 


	//消すときdeleteしないでnullしてね.
	HWND					m_hWnd;
	ID3D11Device*			m_wpDevice;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_wpContext;	//デバイスコンテキスト.
	//2DSp用.
	D3D10_VIEWPORT* m_wpViewPort10;
	//分割用.
	D3D11_VIEWPORT* m_wpViewPort11;

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
	clsROBO_STATUS*		m_spRoboStatus;

	//暗幕を作ろう.
	clsBLACK_SCREEN*	m_spBlackScreen;

};


