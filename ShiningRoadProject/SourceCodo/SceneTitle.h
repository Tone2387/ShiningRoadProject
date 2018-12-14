#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_

class clsASSEMBLE_MODEL;
class clsMENU_WINDOW_TITLE_START_OR_END;

class clsStage;

#include "SceneBase.h"



//================================//
//========== �^�C�g���N���X ==========//
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
	void RenderUi() final;//�u UI�� �vRender.

	//�e�L�X�g�̖���.
	void TextAlphaUpdate();


	//���j���[�̓���.
	void MenuUpdate( enSCENE &enNextScene );


#ifdef _DEBUG
	//�f�o�b�N�J�e�L�X�g�̕\��.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:

	//���̈���.
	enum enBGM : int
	{
		enBGM_MAOU1 = 0,
		enBGM_MAOU3,
		enBGM_AMACHA0,
		enBGM_NOVA2,
	};

	enum enSE : int
	{
		enSE_CURSOL_MOVE = 0,
		enSE_ENTER,
		enSE_EXIT,

		enSE_BOMBER,

		enSE_WIN_APP,
		enSE_WIN_DISAPP
	};

	//�^�C�g���ŃY���Ƃ������ރ��{�b�g�N.
	std::unique_ptr< clsASSEMBLE_MODEL >	m_upRoboModel;

	//�w�i.
	std::unique_ptr< clsStage > m_upBack;

	std::unique_ptr< clsSprite2D > m_upLogo;

	std::unique_ptr< clsSprite2D > m_upFlash;

	//push button�̓����x.
	float	m_fTextAlpha;
	int		m_iTextAlphaStopFrame;
	enum class encTEXT_ALPHA_MODE : UCHAR
	{
		PLUS = 0,
		NEXT_MINUS,//�}�C�i�X�̏���.
		MINUS,
		NEXT_PLUS
	}	m_encTextAlphaMode;


	std::unique_ptr< clsMENU_WINDOW_TITLE_START_OR_END > m_upMenuBox;

	//���j���[����󂯎���������ƍ�����.
	std::vector<unsigned int> m_vecuiInformationDataArray;


	//�e�X�g�p�G�t�F�N�g.
	::Effekseer::Handle m_ehHibana;

};
#endif//#ifndef SCENE_TITLE_H_