#pragma once

#include "SceneBase.h"
#include "CameraMission.h"

#include"Player.h"
#include"Stage.h"

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

	clsPlayer* m_pPlayer;

	std::vector<clsCharactor*> m_v_pFriends;
	std::vector<clsCharactor*> m_v_pEnemys;

	void CreateFriends();
	void CreateEnemys();

	void Collison();
	
	//同キャラのShotが同キャラのBodyに当たる判定を入れるかは処理の兼ね合いで入れる.
	void ColFShottoFBody();
	void ColFShottoEBody();

	void ColEShottoFBody();
	void ColEShottoEBody();

	//テスト用モデル( これは消しても良いです、いらないです ).
	
	//clsCharaStatic* m_pTestChara;

	clsStage* m_pStage;

	float m_fCamMoveSpeed;


	std::vector<clsSPRITE2D_CENTER*> m_v_pRader;
	std::vector<clsSPRITE2D_CENTER*> m_v_pLockOn;
	std::vector<clsUiText*> m_v_pBulletNum;


	//基底クラスに以下があります.
	//カメラ.
	//リソース.
	//エフェクト.
	//サウンド.

};

