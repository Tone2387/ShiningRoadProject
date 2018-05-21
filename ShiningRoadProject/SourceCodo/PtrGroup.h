#pragma once

#include "DxInput.h"
#include "Resource.h"
#include "Effects.h"
#include "SoundManager.h"
#include "Camera.h"


//========== 元の姿 ===========//.
////シーンの初期化に必要なもののまとめ.
//struct clsPOINTER_GROUP
//{
//	//リソース.
//	clsResource* pResource;
//	//エフェクトクラス.
//	clsEffects*	pEffects;
//	//BGM,SE.
//	clsSOUND_MANAGER*	pSound;
//};
//========== 元の姿 ===========//.


class clsPOINTER_GROUP
{
public:
	//これらは絶対変わらないのでコンストラクタで初期化する.
	clsPOINTER_GROUP( 
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext,
		clsDxInput* const pInput,
		clsResource* const pResource,
		clsEffects* const pEffects,
		clsSOUND_MANAGER* const pSound );
	~clsPOINTER_GROUP();

	//カメラのアドレス取得.
	void UpdateCameraPtr( clsCAMERA_BASE* const pCamera );

	//----- ポインターを渡す -----//.
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	//ジョイスティック.
	clsDxInput* GetDxInput() const;
	//リソース.
	clsResource* GetResource() const;
	//エフェクトクラス.
	clsEffects*	GetEffects() const;
	//BGM,SE.
	clsSOUND_MANAGER*	GetSound() const;
	//カメラ.
	clsCAMERA_BASE*	GetCamera() const;

private:

	ID3D11Device*			m_wpDevice;
	ID3D11DeviceContext*	m_wpContext;

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

};

