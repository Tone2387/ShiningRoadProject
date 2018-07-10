#pragma once

#include "DxInput.h"
#include "CXInput.h"
#include "Resource.h"
#include "Effects.h"
#include "SoundManagerBase.h"
#include "Camera.h"
#include "RoboStatus.h"
#include "BlackScreen.h"

class clsPOINTER_GROUP
{
public:
	//これらは絶対変わらないのでコンストラクタで初期化する.
	clsPOINTER_GROUP( 
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext,
		D3D10_VIEWPORT* const pViewPort,
		ID3D11DepthStencilState* const pDepthState,
		clsDxInput* const pDxInput,
		clsXInput* const pXInput,
		clsResource* const pResource,
		clsEffects* const pEffects,
		clsSOUND_MANAGER_BASE* const pSound,
		clsROBO_STATUS* const pRoboStatus,
		clsBLACK_SCREEN* const pBlack );
	~clsPOINTER_GROUP();

	//サウンドのアドレス取得.
	void UpdateSoundPtr( clsSOUND_MANAGER_BASE* const pSound );
	//カメラのアドレス取得.
	void UpdateCameraPtr( clsCAMERA_BASE* const pCamera );

	//----- ポインターを渡す -----//.
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	D3D10_VIEWPORT* GetViewPort() const;
	ID3D11DepthStencilState* GetDepthState() const;
	clsDxInput* GetDxInput() const;
	clsXInput* GetXInput() const;
	clsResource* GetResource() const;
	clsEffects*	GetEffects() const;
	clsSOUND_MANAGER_BASE*	GetSound() const;
	clsCAMERA_BASE*	GetCamera() const;
	clsROBO_STATUS* GetRoboStatus() const;
	clsBLACK_SCREEN* GetBlackScreen() const;

private:
	//デバイスオブジェクト.
	ID3D11Device*			m_wpDevice;
	ID3D11DeviceContext*	m_wpContext;
	//2DSp用.
	D3D10_VIEWPORT* m_wpViewPort;
	//深度(Z)テスト設定.
	ID3D11DepthStencilState* m_wpDepthStencilState;

	//ジョイスティック.
	clsDxInput*		m_wpDxInput;
	clsXInput*		m_wpXInput;
	//リソース.
	clsResource* m_wpResource;
	//エフェクトクラス.
	clsEffects*	m_wpEffects;
	//BGM,SE.
	clsSOUND_MANAGER_BASE*	m_wpSound;

	//カメラ( こいつだけシーン切り替えごとに変わる ).
	clsCAMERA_BASE*		m_wpCamera;

	//( シーンをまたぐ )ロボットのステータス.
	clsROBO_STATUS*		m_wpRoboStatus;

	//暗転用黒スプライト.
	clsBLACK_SCREEN*	m_wpBlackScreen;
};

