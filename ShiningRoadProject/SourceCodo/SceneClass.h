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
	void Create();
	//ループ内の処理( 引数を関数内で変更すると今のシーンが破棄され、.
	//				  指定したシーンが生成される ).
	void Update( enSCENE &nextScene );
	//シーン内のオブジェクトの描画関数のまとめ.
	void Render();



	//メインまで送り上げる.
	D3DXVECTOR3 GetCameraPos() const;
	D3DXVECTOR3 GetCameraLookPos() const;

protected:

	virtual void CreateProduct() = 0;//各シーンのCreate.
	virtual void UpdateProduct( enSCENE &nextScene ) = 0;//各シーンのUpdate.
	virtual void RenderProduct() = 0;//各シーンのRender.

	//3D座標をスクリーン( 2D )座標へと変換する.
	//dimensions(次元) conversion(変換).
	//戻り値は2D座標.
	D3DXVECTOR3 ConvDimPos( const D3DXVECTOR3 &v3DPos );


	//デバッグ用シーン切り替え.
	//すべてのシーンの一番下に置いておく.
	void DebugChangeScene( enSCENE &nextScene ) const;


	//----- Render用 -----//.
	//カメラ関数.
	void Camera();
	//プロジェクション関数.
	void Proj();
	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;	//プロジェクション行列.
	D3DXVECTOR3		m_vLight;	//ライトの方向.
	//----- Render用 -----//.


//基底クラスのポインタは基底クラスで破棄します.
//派生クラスでは破棄しないでください.



	//以下、消すときdeleteしないでnullしてね( この基底クラスのデストラクタでやっています ).
	//カメラ( 必要ならばこれを消して、シーンごとに異なった機能のカメラを作りましょう ).
	ID3D11Device*			m_wpDevice;	//デバイスオブジェクト.
	ID3D11DeviceContext*	m_wpContext;//デバイスコンテキスト.
	D3D10_VIEWPORT*			m_wpViewPort;//2DSp用.
	clsPOINTER_GROUP*	m_wpPtrGroup;
	clsDxInput*			m_wpDxInput;
	clsResource*		m_wpResource;
	clsEffects*			m_wpEffects;
	clsSOUND_MANAGER*	m_wpSound;
	clsCAMERA_BASE*		m_wpCamera;


};
