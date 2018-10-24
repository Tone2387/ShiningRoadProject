#ifndef SCENE_ASSEMBLE_H_
#define SCENE_ASSEMBLE_H_


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
		,Type( 0 )
		{}
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


	//メッセボックス出現.//引数は開きたい窓.
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

	//色替え( 左右キーを押された ).
	void AddRoboColor( const bool isIncrement );

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
//	std::unique_ptr< clsSPRITE2D_CENTER >	m_upYesNo;
	bool									m_isMessageBoxYes;

	//色の棒.
	
	clsROBO_STATUS::enCOLOR_GAGE m_enColorGageIndex;

	clsSPRITE2D_CENTER* m_pColorGagesBone[ clsROBO_STATUS::enCOLOR_GAGE_size ];
	clsSprite2D* m_pColorGages[ clsROBO_STATUS::enCOLOR_GAGE_size ];
//	unsigned int m_uiColorChangeNum;//ローカル変数.
	std::unique_ptr< clsUiText > m_upColorTexts[ clsROBO_STATUS::enCOLOR_GAGE_size ];//RとかGとか書いてる.
	std::unique_ptr< clsUiText > m_upColorNumText;//色1と色2とか書く.
	std::unique_ptr< clsSPRITE2D_CENTER > m_upSelectColor;//選択中の色を表す.


	//UI.
	clsASSEMBLE_UI*		m_pUI;

	std::vector< std::shared_ptr< clsFILE > >	m_vspFile;



	//操作可能ならtrue.
	bool m_isCanControl;

	//右スティックでモデルに近づく距離.
	float m_fDistanceAssembleModel;
		

	//エフェクト.
	::Effekseer::Handle m_ehHibana;


	//音の引数.
	enum enBGM : int
	{
		enBGM_RENGOKU0 = 0,
		enBGM_MAOU0,
		enBGM_MAOU2,
	};

	enum enSE : int
	{
		enSE_CURSOL_MOVE = 0,
		enSE_ENTER,
		enSE_EXIT,
		enSE_MISSION_START,
		enSE_WIN_APP,
		enSE_WIN_DISAPP,
	};


#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

};



#endif//#ifndef SCENE_ASSEMBLE_H_