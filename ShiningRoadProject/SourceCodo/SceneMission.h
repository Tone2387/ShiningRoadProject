#pragma once

#include "SceneBase.h"
#include "CameraMission.h"

#include"Player.h"
#include"TestObject.h"
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
	void RenderUi() final;//「 UIの 」Render.

#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

	clsPlayer* m_pPlayer;
	clsTestObj* m_pTestObj;
	clsCharactor* m_pCamTar;

	bool m_bCamTarChange;

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

	bool AllEnemyDead();

	bool m_bEnemyStop;

	//テスト用モデル( これは消しても良いです、いらないです ).

	void UpdateCamTargetPos(clsCharactor* pChara);

	D3DXVECTOR3 m_vCamTargetPos;
	D3DXVECTOR3 m_vLookTargetPos;
	
	//clsCharaStatic* m_pTestChara;

	clsStage* m_pStage;

	float m_fCamMoveSpeed;

	void CreateUI();

	clsSPRITE2D_CENTER* m_pRaderWindow;
	std::vector<clsSPRITE2D_CENTER*> m_v_pRaderEnemyMark;

	float m_fRaderSizeW;
	float m_fRaderSizeH;

	float m_fRaderMarkSizeW;
	float m_fRaderMarkSizeH;

	float m_fRaderDis;


	clsSPRITE2D_CENTER* m_pEnelgy;
	clsSPRITE2D_CENTER* m_pEnelgyFrame;

	clsUiText* m_pHP;

	clsUiText* m_pLBulletNum;
	clsUiText* m_pRBulletNum;

	clsUiText* m_pLimitTime;

	clsPlayer* CreatePlayer();
	clsTestObj* CreateEnemy();

	//基底クラスに以下があります.
	//カメラ.
	//リソース.
	//エフェクト.
	//サウンド.

};

