#pragma once


#include "SceneBase.h"

#ifdef Tahara
class clsMENU_WINDOW_MISSION_BASE;
#endif//#ifdef Tahara


#include "CameraMission.h"

#include"Player.h"
#include"TestObject.h"
#include"Stage.h"
#include"Sprite.h"
#include"EnemyFactory.h"
#include"FriendFactory.h"


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

#ifdef _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

	clsPlayer* m_pPlayer;
	clsTestObj* m_pTestObj;
	clsCharactor* m_pCamTar;

	bool m_bCamTarChange;
	bool m_bStartFlg;

	std::vector<clsCharactor*> m_v_pFriends;
	std::vector<clsCharactor*> m_v_pEnemys;

	void CreateFriends();
	void CreateEnemys();

	void SetEnemys();//敵味方の作成の後にお互いの敵を認識させる.

	void Collison();
	
	//同キャラのShotが同キャラのBodyに当たる判定を入れるかは処理の兼ね合いで入れる.
	void ColFShottoFBody();
	bool ColFShottoEBody();

	void ColEShottoFBody();
	void ColEShottoEBody();

	//重複時の動作.
	void Duplicate();
	void ColFtoFDuplicate();
	void ColFtoEDuplicate();
	void ColEtoFDuplicate();

	bool AllEnemyDead();

	bool m_bEnemyStop;

	//テスト用モデル( これは消しても良いです、いらないです ).
	void UpdateCamTargetPos(clsCharactor* pChara);
	void GameStart();

	D3DXVECTOR3 m_vCamTargetPos;
	D3DXVECTOR3 m_vLookTargetPos;
	
	//clsCharaStatic* m_pTestChara;

	clsStage* m_pStage;

	float m_fCamMoveSpeed;

	void CreateUI();

	int m_iStartCnt;
	clsUiText* m_pStartText;

	clsSPRITE2D_CENTER* m_pWindowScr;
	clsSPRITE2D_CENTER* m_pWindowScrFilter;

	clsSPRITE2D_CENTER* m_pRaderWindowFront;
	clsSPRITE2D_CENTER* m_pRaderWindowBack;
	clsSPRITE2D_CENTER* m_pRaderPlayerMark;
	std::vector<clsSPRITE2D_CENTER*> m_v_pRaderEnemyMark;

	float m_fRaderSizeW;
	float m_fRaderSizeH;

	float m_fRaderMarkSizeW;
	float m_fRaderMarkSizeH;

	clsSPRITE2D_CENTER* m_pLockWindow;
	clsSPRITE2D_CENTER* m_pLWeaponLockMark;
	clsSPRITE2D_CENTER* m_pRWeaponLockMark;

	clsSPRITE2D_CENTER* m_pLWeaponLockWindow;
	clsSPRITE2D_CENTER* m_pRWeaponLockWindow;

	bool m_bLockStart;

	clsSPRITE2D_CENTER* m_pHitMark;
	int iHitDispTime;

	clsSPRITE2D_CENTER* m_pEnelgy;
	clsSPRITE2D_CENTER* m_pEnelgyFrame;

	clsSPRITE2D_CENTER* m_pCursorFrame;
	clsSPRITE2D_CENTER* m_pCursor;
	float m_fCursorSize;

	clsUiText* m_pHP;
	clsUiText* m_pHPTargetChara;

	clsUiText* m_pBoostOn;

	clsSPRITE2D_CENTER* m_pLBulletMark;
	clsSPRITE2D_CENTER* m_pRBulletMark;

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
#ifdef Tahara
	//メニューの動き.
	void MenuUpdate( enSCENE &enNextScene );
#ifdef RENDER_SCREEN_TEXTURE_
	//ノイズエフェクト.
	void PlayNoise( const HitState& Hitstate );
#endif//#ifdef RENDER_SCREEN_TEXTURE_
	//ポーズメニュー.
	std::unique_ptr< clsMENU_WINDOW_MISSION_BASE > m_upMenu;
	//メニューから受け取った情報を照合する.
	std::vector<unsigned int> m_vecuiInformationDataArray;
	std::unique_ptr< clsSprite2D > m_upPauseDisplayBlack;
#endif//#ifdef Tahara

};