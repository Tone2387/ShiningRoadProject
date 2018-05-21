#pragma once

#include "FactoryScene.h"
#include "FactoryCamera.h"
#include "DxInput.h"



//ゲームの動きはこのクラスの中で行う.
class clsGAME
{
public:
	//引数はシーンの初期化に必要なもの.
	clsGAME( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext );
	~clsGAME();

	//new直後に使う.
	void Create();
	//毎フレーム使う.
	void Update();
	//毎フレーム使う.
	void Render(
		const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight );


	//引数のシーンに切り替え.
	void SwitchScene( const enSCENE enNextScene );

	//ラップ関数.
	D3DXVECTOR3 GetCameraPos() const;
	D3DXVECTOR3 GetCameraLookPos() const;


private:

	HWND m_hWnd;
	ID3D11Device*			m_wpDevice;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_wpContext;	//デバイスコンテキスト.

	//ジョイスティック.
	clsDxInput*		m_spDxInput;

	//リソースクラス.
	clsResource*	m_pResource;
	//エフェクト.
	clsEffects*		m_pEffect;
	//サウンド.
	clsSOUND_MANAGER* m_pSound;


	//シーンクラス.
	clsSCENE_BASE*		m_pScene;
	//シーンのファクトリ.
	clsSCENE_FACTORY* m_pSceneFactory;

	//カメラ.
	clsCAMERA_BASE*	m_spCamera;
	//カメラファクトリ.
	clsFACTORY_CAMERA* m_pCameraFactory;


	//消すときdeleteしないでnullしてね.
	clsPOINTER_GROUP* m_pPtrGroup; 
};


