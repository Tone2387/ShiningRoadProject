#pragma once

#include "SceneBase.h"
#include "CameraMission.h"


#include"TestObject.h"


//================================//
//========== ミッション中クラス ==========//
//================================//
class clsSCENE_MISSION : public clsSCENE_BASE
{
public:
	//引数はリソースなどのポインタのまとめ( 引数は基底クラスで取り扱っているので特に意識しなくて大丈夫です ).
	clsSCENE_MISSION( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_MISSION();

//	//「SceneManager.cpp」の「SwitchScene」関数内でシーン作成直後に使用される.
//	void Create() final;
//
//	//ループ内の処理( 引数のフラグをを関数内で変更すると今のシーンが破棄され、.
//	//				  指定したシーンが生成される ).
//	//このフラグの詳細は「SceneClass.h」の「enSCENE」を見てください.
//	void Update( enSCENE &enNextScene ) final;
//
//	//シーン内のオブジェクトの描画関数のまとめ.
//	void Render() final;

private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;

#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

	//m_pPlayer
	//m_vpFrends
	//m_vpEnemys

	//テスト用モデル( これは消しても良いです、いらないです ).
	clsTestObj* m_pTestRobo;
	//clsCharaStatic* m_pTestChara;

	clsDX9Mesh* m_pStage;

	clsCAMERA_MISSION* m_pCam;

	//基底クラスに以下があります.
	//カメラ.
	//リソース.
	//エフェクト.
	//サウンド.

};

