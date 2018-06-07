#pragma once

#include "DxInput.h"
#include "Resource.h"
#include "Effects.h"
#include "SoundManager.h"
#include "Camera.h"
#include "RoboStatus.h"




class clsPOINTER_GROUP
{
public:
	//これらは絶対変わらないのでコンストラクタで初期化する.
	clsPOINTER_GROUP( 
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext,
		D3D10_VIEWPORT* const pViewPort,
		ID3D11DepthStencilState* const pDepthState,
		clsDxInput* const pInput,
		clsResource* const pResource,
		clsEffects* const pEffects,
		clsSOUND_MANAGER* const pSound,
		clsROBO_STATUS* const pRoboStatus );
	~clsPOINTER_GROUP();

	//カメラのアドレス取得.
	void UpdateCameraPtr( clsCAMERA_BASE* const pCamera );

	//----- ポインターを渡す -----//.
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	D3D10_VIEWPORT* GetViewPort() const;
	ID3D11DepthStencilState* GetDepthState() const;
	clsDxInput* GetDxInput() const;
	clsResource* GetResource() const;
	clsEffects*	GetEffects() const;
	clsSOUND_MANAGER*	GetSound() const;
	clsCAMERA_BASE*	GetCamera() const;
	clsROBO_STATUS* GetRoboStatus() const;

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
	//リソース.
	clsResource* m_wpResource;
	//エフェクトクラス.
	clsEffects*	m_wpEffects;
	//BGM,SE.
	clsSOUND_MANAGER*	m_wpSound;

	//カメラ( こいつだけシーン切り替えごとに変わる ).
	clsCAMERA_BASE*		m_wpCamera;

	//( シーンをまたぐ )ロボットのステータス.
	clsROBO_STATUS*		m_wpRoboStatus;
};

