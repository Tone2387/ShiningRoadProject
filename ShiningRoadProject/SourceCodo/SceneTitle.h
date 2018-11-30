#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_


#include "SceneBase.h"
#include "CameraTitle.h"
#include "Sprite2DCenter.h"

#include "AssembleModel.h"



class clsMENU_WINDOW_TITLE_START_OR_END;

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

	//テキストの明滅.
	void TextAlphaUpdate();


	//メニューの動き.
	void MenuUpdate( enSCENE &enNextScene );


#ifdef _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:


	//タイトルでズンとただずむロボット君.
	clsASSEMBLE_MODEL*	m_pRoboModel;

	//背景.
	std::unique_ptr< clsCharaStatic > m_upBackFloor;
	std::unique_ptr< clsCharaStatic > m_upBackCelling;

	std::unique_ptr< clsSprite2D > m_upLogo;

	std::unique_ptr< clsSprite2D > m_upFlash;

	//push buttonの透明度.
	float	m_fTextAlpha;
	int		m_iTextAlphaStopFrame;
	enum class encTEXT_ALPHA_MODE : UCHAR
	{
		PLUS = 0,
		NEXT_MINUS,//マイナスの準備.
		MINUS,
		NEXT_PLUS
	}	m_encTextAlphaMode;


	std::unique_ptr< clsMENU_WINDOW_TITLE_START_OR_END > m_upMenuBox;

	//メニューから受け取った情報を照合する.
	std::vector<unsigned int> m_vecuiInformationDataArray;


	//テスト用エフェクト.
	::Effekseer::Handle m_ehHibana;

};
#endif//#ifndef SCENE_TITLE_H_