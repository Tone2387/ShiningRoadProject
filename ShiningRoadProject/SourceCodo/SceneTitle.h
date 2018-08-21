#pragma once

#include "SceneBase.h"
#include "CameraTitle.h"
#include "Sprite2DCenter.h"

#include "AssembleModel.h"


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

	std::unique_ptr< clsSPRITE2D_CENTER > m_upLogo;

	//テスト用エフェクト.
	::Effekseer::Handle m_ehHibana;

};
