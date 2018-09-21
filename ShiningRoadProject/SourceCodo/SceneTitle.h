#pragma once

#include "SceneBase.h"
#include "CameraTitle.h"
#include "Sprite2DCenter.h"

#include "AssembleModel.h"


class clsWINDOW_BOX;

//================================//
//========== タイトルクラス ==========//
//================================//
class clsSCENE_TITLE : public clsSCENE_BASE
{
public:
	clsSCENE_TITLE( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_TITLE();


private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//「 UIの 」Render.


#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

	//タイトルでズンとただずむロボット君.
	clsASSEMBLE_MODEL*	m_pRoboModel;

	//背景.
	std::unique_ptr< clsCharaStatic > m_upBack;

	std::unique_ptr< clsSprite2D > m_upLogo;

	std::unique_ptr< clsSprite2D > m_upFlash;



	std::unique_ptr< clsUiText >	m_upPlessStart;
	bool							m_isDispPlessStart;

	//メッセボックス.
	std::unique_ptr< clsWINDOW_BOX > m_upBox;


	//テスト用エフェクト.
	::Effekseer::Handle m_ehHibana;
	//音の引数.
	enum enBGM : int
	{
		enBGM_MAOU1 = 0,
		enBGM_MAOU3,
	};

	enum enSE : int
	{
		enSE_CURSOL_MOVE = 0,
		enSE_ENTER,
		enSE_EXIT,
		enSE_BOMBER,
		enSE_GUITAR
	};

};
