#ifndef ASSENBLE_UI_H_
#define ASSENBLE_UI_H_

//#include "Global.h"
#include "DxInput.h"
#include "CXInput.h"

#include "Sprite2DCenter.h"
#include "UiText.h"
#include "File.h"
#include "CFont.h"
#include "AssembleModel.h"
#include <vector>
class clsWINDOW_BOX;


class clsASSEMBLE_UI
{
public:
	clsASSEMBLE_UI( clsFont* const pFont );
	~clsASSEMBLE_UI();

	//�e�p�[�cUI���󂯎��p.
	using PARTS_NUM_DATA = std::vector< int >; 

	//�p�[�c�I�𒆂�����ȊO��.
	enum class enSELECT_MODE : UCHAR
	{
		PARTS = 0,
		STATUS,
		MISSION_START,
		TITLE_BACK,
		COLOR_CHANGE,
	}	m_enSelectMode;

	void Create( 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext,
		PARTS_NUM_DATA data );//�e�p�[�cUI�̐��m��p.

	void Input(
		const clsXInput* const pXInput,
		const clsDxInput* const pDxInput );

	//�������̓f�[�^����A�X�e�[�^�X�ȊO�̃f�[�^�̐��B��O�����̓p�[�c���̔ԍ�.
	void Update( 
		enSELECT_MODE enSelect,
		std::shared_ptr< clsFILE > const spFile = nullptr,
		clsASSEMBLE_MODEL* const pModel = nullptr,
		const int iPartsType = 0,	//�p�[�c���.
		const int iPartsNum = 0,	//�p�[�c�ԍ�.
		const int iStatusCutNum = 0 );//�X�e�[�^�X����Ȃ��f�[�^�̐�.	

	void Render( 
		enSELECT_MODE enSelect, 
		const int iPartsType, 
		const int iPartsNum ) const;//�I�𒆃p�[�c�ԍ�.

	void RenderPartsState( 
		enSELECT_MODE enSelect, 
		const int iPartsType, 
		const int iPartsNum ) const;//�I�𒆃p�[�c�ԍ�.

	
	//�������̍s�w��.
	int SetReadLinePartsComment(
		const int iPartsType );	//�p�[�c���.


	//�X�e�[�^�X�E�B���h�E���B��.
	void SwitchDispStatusComment();
	//�X�e�[�^�Xcomment�̐؂�ւ�����.
	bool isCanSwitchStatusComment();

	//�X�e�[�^�X�ڍׂƃp�[�c�I���̐؂�ւ�.
	void SwitchStatusComment();
	//�w��.
	void SetStatusComment( const enSELECT_MODE enMode );
	//.
	void AddStatusCommentNo( const bool isPlus );
	//�S�������C��.
	//�I���������Ɏ����čs�������̒���.
	void AddCommentNoForChangePartsType( const int iPartsType );

	//�p�[�c�������̕\����������0�ɖ߂�.
	void InitReadNumPartsComment(){ m_iReadNumPartsComment = 0; }

#if _DEBUG
	//�f�o�b�O�e�L�X�g�p.
	D3DXVECTOR3 GetUiPos();
#endif//#if _DEBUG

private:


	D3DXVECTOR4 GetStatusColor( 
		const int iBefore, const int iAfter,
		const int iPartsType, const int iStatusNum ) const;


	//�����Ă����Ȃ���߂�.
	void StatusNumKeepRange();
	//�����Ă����Ȃ烋�[�v����.
	void StatusNumLoopRange();

private:

	//�p�[�c���ڐ�.
	enum enPARTS_TYPE : int
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,
		WEAPON_L,
		WEAPON_R,
	
		enPARTS_TYPE_SIZE
	};

	//�p�[�c�\���p�̃E�B���h�E.
	D3D11_VIEWPORT m_ViewPortPartsWindow;

	std::vector< std::unique_ptr< clsSprite2D > >	m_vecupPartsType;		//�p�[�c�J�e�S��.
	std::unique_ptr< clsSprite2D >					m_upPartsTypeSelect;//�I��( ������ ).

	std::vector< std::unique_ptr< clsSprite2D > >	m_vecupPartsIconArray[ enPARTS_TYPE_SIZE ];	//�e�p�[�c��UI.
	std::unique_ptr< clsSprite2D >					m_upPartsNumSelect;	//�I��( ������ ).

	std::unique_ptr< clsSprite2D >					m_upStatusWindow;	//�X�e�[�^�X���\�������.

	std::unique_ptr< clsSprite2D >					m_upPartsWindow;	//�p�[�c�̒P�̃��f���\�������.
	std::unique_ptr< clsSprite2D >					m_upRoboWindow;		//���{�S�̂��\�������.

	std::unique_ptr< clsSprite2D > m_upHeader;//��ʏ㕔�̑�.
	std::unique_ptr< clsSprite2D > m_upFooter;//��ʉ����̑�.

	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupArrow;//���.

	std::unique_ptr< clsUiText > m_upHeaderText;//�w�b�_�[����.

	std::vector< std::unique_ptr< clsUiText > > m_vecupStatusText;	//�X�e�[�^�X����( ���ږ� ).
	std::vector< std::unique_ptr< clsUiText > > m_vecupStatusNumText;	//�����Ă���p�[�c�̃X�e�[�^�X�l.
	std::vector< std::unique_ptr< clsUiText > > m_vecupStatusNumTextNow;	//���݂̑����̃X�e�[�^�X�l.


	std::unique_ptr< clsUiText > m_upPartsNameText;//�p�[�c��.

	std::unique_ptr< clsSprite2D > m_upButton;


	//�u���̕���͉E�r?����Ƃ����r?�v.
	std::unique_ptr< clsWINDOW_BOX > m_upWndBox;//���E�ǂ����������Ă��锠.


	//�X�e�[�^�X�̐�( �s�� ).
	int m_iStatusNum;
	//�X�e�[�^�X�̖��O���i�[���Ă���.
	std::vector< std::string > m_vecsStatusNameBoxArray[ enPARTS_TYPE_SIZE ];

	//�X�e�[�^�X�̕\���t���O.
	bool	m_isDispStatus;

	//�I�����Ă���X�e�[�^�X��No.
	int		m_iStatusCommentNo;
	//���������UI.
	std::unique_ptr< clsSprite2D > m_upSelectStatus;
	//�����p�t���O.
	bool	m_bStatusCommentOffset;

	//���{��t�H���g.
	clsFont*	m_wpFont;
	//�p�[�c�A�X�e�[�^�X�̓��{��������̓ǂݍ��ݍs�w��.
	int m_iReadLinePartsComment;
	//���������������ڂ܂ŕ\�����邩.
	int m_iReadNumPartsComment;


#if _DEBUG
	std::unique_ptr< clsSprite2D > m_upDegine;
#endif//#if _DEBUG

};

#endif//#ifndef ASSENBLE_UI_H_