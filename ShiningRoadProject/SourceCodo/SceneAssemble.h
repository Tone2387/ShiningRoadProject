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
//========== �g�݊����N���X ==========//
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
	void RenderUi() final;//�u UI�� �vRender.

	//�R���g���[������.
	//�J�[�\���ړ�.
	void MoveCursorUp();
	void MoveCursorDown();
	void MoveCursorRight();
	void MoveCursorLeft();
	//����.
	void Enter( enSCENE &enNextScene )const;
	//�L�����Z��.
	void Exit();

	//���j���[�̓���.
	void MenuUpdate( enSCENE &enNextScene );

	//���b�Z�{�b�N�X�o��.//�����͊J��������.
	void AppearMessageBox( const clsASSEMBLE_UI::enSELECT_MODE encMode );
	//���b�Z�{�b�N�X����.
	void DisAppearMessageBox();
	//�p�[�c�ύX.
	void AssembleParts()const;
	//�X�e�[�^�X�̕\����\���ؑ�.
	void SwitchDispStatus();
	//�p�[�c�I���ƃX�e�[�^�X�I���̐؂�ւ�.
	void ChangePartsSelect();
	void ChangeStatusSelect();
	//�o��.
	void MissionStart( enSCENE &enNextScene );
	//�^�C�g���ɖ߂�.
	void TitleBack( enSCENE &enNextScene )const;



	//�͈͓��Ɏ��߂�( �p�[�c�̑I�������I�[�o�[���Ȃ��悤�ɂ��� ).
	//min�͂��̐��l��菬�����Ȃ�Ȃ��Amax�͂���ȏ�ɂ͂Ȃ�Ȃ�.
	// min <= t < max.
	template< class T, class MIN, class MAX >
	T LoopRange( T t, const MIN min, const MAX max ) const;

	//���b�Z�{�b�N�X�����Ă���Ȃ�true.
	bool isMessageBoxClose()const;

	//�F�ւ�( ���E�L�[�������ꂽ ).
	void AddRoboColor( const bool isIncrement )const;

	//�X�e�B�b�N�̓���( ���{�̉�] ).
	void MoveRoboStick();


#ifdef _DEBUG
	//�f�o�b�N�J�e�L�X�g�̕\��.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:


	//�ǂ̃p�[�c��I��ł��?.
	struct PARTS_SELECT
	{
		short Type;	//�p�[�c�̎��( �r�A�R�A�� ).
		short Num[ clsASSEMBLE_MODEL::ENUM_SIZE ];	//�p�[�c�ԍ�.

		PARTS_SELECT()
		:Num()
		,Type( 0 )
		{}
	}m_PartsSelect;



	//�p�[�c�I�𒆂�����ȊO��.
	clsASSEMBLE_UI::enSELECT_MODE m_enSelectMode;

	//�p�[�c�E�B���h�E�p.
	D3D11_VIEWPORT* m_pViewPortPartsWindow;
	//���{�E�B���h�E�p.
	D3D11_VIEWPORT* m_pViewPortRoboWindow;

	//�w�i.
	std::unique_ptr< clsSprite2D > m_upBack;

	//�����������郂�f��.
	clsASSEMBLE_MODEL*	m_spAsmModel;

	//�I�𒆃p�[�c.
	std::unique_ptr< clsPARTS_WINDOW_MODEL > m_upSelectParts;

	//���.
	std::unique_ptr< clsSPRITE2D_CENTER > m_upArrow;


	//�ǂ̐F��ς��邩�̃t���O.
	clsROBO_STATUS::enCOLOR_GAGE m_enColorGageIndex;


	//UI.
	std::unique_ptr< clsASSEMBLE_UI	>	m_upUI;

	//�p�[�c�̃X�e�[�^�X���󂯎��A���̃V�[�����ێ�����.
	std::vector< std::shared_ptr< clsFILE > >	m_vecspFile;


	//�E�X�e�B�b�N�̈ړ�.
	D3DXVECTOR3 m_vRoboViewOffsetPos;


	//�G�t�F�N�g.
	::Effekseer::Handle m_ehHibana;


	//���j���[.
	std::unique_ptr< clsMENU_WINDOW_ASSEMBLE_BASE > m_upMenu;
	//���j���[����󂯎���������ƍ�����.
	std::vector<unsigned int> m_vecuiInformationDataArray;


};



#endif//#ifndef SCENE_ASSEMBLE_H_