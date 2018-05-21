#pragma once

#include "Global.h"

#include "CharaStatic.h"

#include "PtrGroup.h"

#include "Sprite2DCenter.h"



//================================//
//========== 基底クラス ==========//
//================================//
class clsSCENE_BASE
{
public:
	//引数はリソースなどのポインタのまとめ.
	clsSCENE_BASE( clsPOINTER_GROUP* const ptrGroup );
	virtual ~clsSCENE_BASE();

	//シーン作成直後に「SceneManager.cpp」の「SwitchScene」関数内で使用されている.
	virtual void Create() = 0;
	//ループ内の処理( 引数を関数内で変更すると今のシーンが破棄され、.
	//				  指定したシーンが生成される ).
	virtual void Update( enSCENE &nextScene ) = 0;
	//シーン内のオブジェクトの描画関数のまとめ.
	virtual void Render(
		const D3DXMATRIX &mView, 
		const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight ) const = 0;



	//メインまで送り上げる.
	D3DXVECTOR3 GetCameraPos() const;
	D3DXVECTOR3 GetCameraLookPos() const;

protected:

	//デバッグ用シーン切り替え.
	//すべてのシーンの一番下に置いておく.
	void DebugChangeScene( enSCENE &nextScene ) const;

//基底クラスのポインタは基底クラスで破棄します.
//派生クラスでは破棄しないでください.



	//以下、消すときdeleteしないでnullしてね( この基底クラスのデストラクタでやっています ).
	//カメラ( 必要ならばこれを消して、シーンごとに異なった機能のカメラを作りましょう ).
	ID3D11Device*			m_wpDevice;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_wpContext;//デバイスコンテキスト.
	clsPOINTER_GROUP*	m_wpPtrGroup;
	clsDxInput*			m_wpDxInput;
	clsResource*		m_wpResource;
	clsEffects*			m_wpEffects;
	clsSOUND_MANAGER*	m_wpSound;
	clsCAMERA_BASE*		m_wpCamera;


};
