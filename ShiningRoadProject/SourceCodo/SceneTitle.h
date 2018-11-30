#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_


#include "SceneBase.h"
#include "CameraTitle.h"
#include "Sprite2DCenter.h"

#include "AssembleModel.h"



class clsMENU_WINDOW_TITLE_START_OR_END;

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


	//�^�C�g���ŃY���Ƃ������ރ��{�b�g�N.
	clsASSEMBLE_MODEL*	m_pRoboModel;

	//�w�i.
	std::unique_ptr< clsCharaStatic > m_upBackFloor;
	std::unique_ptr< clsCharaStatic > m_upBackCelling;

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