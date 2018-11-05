#ifndef SCENE_ASSEMBLE_H_
#define SCENE_ASSEMBLE_H_


class clsMENU_WINDOW_ASSEMBLE_BASE;

#include "File.h"

#include "SceneBase.h"
#include "CameraAssemble.h"
#include "AssembleModel.h"

#include "AssembleUi.h"

#include "PartsWindowModel.h"


//================================//
//========== 組み換えクラス ==========//
//================================//
class clsSCENE_ASSEMBLE : public clsSCENE_BASE
{
public:

	clsSCENE_ASSEMBLE( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_ASSEMBLE();


private:


	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//「 UIの 」Render.

	//コントローラ操作.
	//カーソル移動.
	void MoveCursorUp();
	void MoveCursorDown();
	void MoveCursorRight();
	void MoveCursorLeft();
	//決定.
	void Enter( enSCENE &enNextScene );
	//キャンセル.
	void Exit();

	//メニューの動き.
	void MenuUpdate( enSCENE &enNextScene );

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

	//スティックの動き( ロボの回転 ).
	void MoveRoboStick();

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



	//パーツ選択中かそれ以外か.
	clsASSEMBLE_UI::enSELECT_MODE m_enSelectMode;

	//パーツウィンドウ用.
	D3D11_VIEWPORT* m_pViewPortPartsWindow;
	//ロボウィンドウ用.
	D3D11_VIEWPORT* m_pViewPortRoboWindow;

	//背景.
	std::unique_ptr< clsSprite2D > m_upBack;

	//お着換えするモデル.
	clsASSEMBLE_MODEL*	m_spAsmModel;

	//選択中パーツ.
	std::unique_ptr< clsPARTS_WINDOW_MODEL > m_upSelectParts;

	//矢印.
	std::unique_ptr< clsSPRITE2D_CENTER > m_upArrow;


	//どの色を変えるかのフラグ.
	clsROBO_STATUS::enCOLOR_GAGE m_enColorGageIndex;


	//UI.
	std::unique_ptr< clsASSEMBLE_UI	>	m_upUI;

	//パーツのステータスを受け取り、このシーン中保持する.
	std::vector< std::shared_ptr< clsFILE > >	m_vecspFile;


	//右スティックの移動.
	D3DXVECTOR3 m_vRoboViewOffsetPos;


	//エフェクト.
	::Effekseer::Handle m_ehHibana;


	//メニュー.
	std::unique_ptr< clsMENU_WINDOW_ASSEMBLE_BASE > m_upMenu;
	//メニューから受け取った情報を照合する.
	std::vector<unsigned int> m_vecuiInformationDataArray;




#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

};



#endif//#ifndef SCENE_ASSEMBLE_H_