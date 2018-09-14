#pragma once

#include "File.h"

#include "SceneBase.h"
#include "CameraAssemble.h"
#include "AssembleModel.h"

#include "AssembleUi.h"

#include "PartsWindowModel.h"
class clsWINDOW_BOX;


//================================//
//========== 組み換えクラス ==========//
//================================//
class clsSCENE_ASSEMBLE : public clsSCENE_BASE
{
public:

	clsSCENE_ASSEMBLE( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_ASSEMBLE();


private:


	//どのパーツを選んでるの?.
	struct PARTS_SELECT
	{
		short Type;	//パーツの種類( 脚、コア等 ).
		short Num[clsASSEMBLE_MODEL::ENUM_SIZE];	//パーツ番号.

		PARTS_SELECT()
		:Num()
		{
			Type = 0;
		}
	}m_PartsSelect;


	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//「 UIの 」Render.

	//コントローラ操作.
	//カーソル移動.
	void MoveCursor();//カーソル移動の共通動作.
	void MoveCursorUp();
	void MoveCursorDown();
	void MoveCursorRight();
	void MoveCursorLeft();
	//決定.
	void Enter( enSCENE &enNextScene );
	//キャンセル.
	void Exit();


	//メッセボックス出現.
	void AppearMessageBox( const clsASSEMBLE_UI::enSELECT_MODE encMode );
	//メッセボックス消す.
	void DisAppearMessageBox();
	//パーツ変更.
	void AssembleParts();
	//ステータスの表示非表示切替.
	void SwitchDispStatus();
	//パーツ選択とステータス選択の切り替え.
	void ChangePartsSelect();
	void ChangeStatusSelect();
	//出撃.
	void MissionStart( enSCENE &enNextScene );
	//タイトルに戻る.
	void TitleBack( enSCENE &enNextScene );



	//範囲内に収める( パーツの選択肢がオーバーしないようにする ).
	//minはその数値より小さくならない、maxはそれ以上にはならない.
	// min <= t < max.
	template< class T, class MIN, class MAX >
	T LoopRange( T t, const MIN min, const MAX max ) const;

	//メッセボックスが閉じているならtrue.
	bool isMessageBoxClose();


#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

	//パーツ選択中かそれ以外か.
	clsASSEMBLE_UI::enSELECT_MODE m_enSelectMode;

	//パーツウィンドウ用.
	D3D11_VIEWPORT* m_pViewPortPartsWindow;
	//ロボウィンドウ用.
	D3D11_VIEWPORT* m_pViewPortRoboWindow;

	//背景.
	std::unique_ptr< clsSprite2D > m_upBack;

	//お着換えするモデル.
	clsASSEMBLE_MODEL*	m_pAsmModel;

	//選択中パーツ.
	clsPARTS_WINDOW_MODEL* m_pSelectParts;

	//矢印.
	std::unique_ptr< clsSPRITE2D_CENTER > m_upArrow;


	//メッセボックス.
	std::unique_ptr< clsWINDOW_BOX >	m_upBox;
	//メッセの行数を表す.
	int									m_iMessageNum;

	//メッセボックスの選択肢.
	std::unique_ptr< clsSPRITE2D_CENTER >	m_upYesNo;
	bool									m_isMessageBoxYes;

	//UI.
	clsASSEMBLE_UI*		m_pUI;

	std::vector< std::shared_ptr< clsFILE > >	m_vspFile;


//	clsSPRITE2D_CENTER* m_pSprite;
//	clsCharaStatic* m_pTestChara;
//	clsCharaStatic* m_pParts;

	//操作可能ならtrue.
	bool m_isCanControl;
		

	//エフェクト.
	::Effekseer::Handle m_ehHibana;


	//音の引数.
	enum enBGM : int
	{
		ASSEMBLE = 0,
	};

	enum enSE : int
	{
		CURSOL_MOVE = 0,
		ENTER,
		EXIT,
		MISSION_START,
		WIN_APP,
		WIN_DISAPP,
	};

};



