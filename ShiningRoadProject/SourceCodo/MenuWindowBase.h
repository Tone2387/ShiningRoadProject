#ifndef MENU_WINDOW_BASE_H_
#define MENU_WINDOW_BASE_H_


class clsPOINTER_GROUP;
class clsUiText;
class clsFont;
class clsXInput;
class clsDxInput;
class clsSOUND_MANAGER_MENUWINDOW;

#include "WindowBox.h"




//�����j���[�̊��N���X.
class clsMENU_WINDOW_BASE : public clsWINDOW_BOX
{
public:

	clsMENU_WINDOW_BASE(	
		const HWND hWnd,
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec,
		const char* sWindowName );
	virtual ~clsMENU_WINDOW_BASE();

	//�V�[���N���X�Ə��̂���������ϐ��̌^�y�э쐬�֐�.
	using INFORMATION_MENU_DATA_ARRAY = std::vector<unsigned int>;
	//�����֐��͂��̊��N���X����protected:���ɂ���A�p����̊e�V�[��Base�N���X�ŁA���̃R�����g�̌`�Ń��b�v����.
	//���̓��e�́A�Q�Ƃ���Information�̃t�@�C���p�X���w�肷�邾��.
//	static INFORMATION_MENU_DATA_ARRAY CreateInformation( 
//		INFORMATION_MENU_DATA_ARRAY* const InformationDataArray,
//		const int iInformationSize );



public:



	void Update() final;

	void Render() final;

	//���̃E�B���h�E����n�߂Đe�E�B���h�E�ɑ����Ԃ�.
	void Close( const float fCloseSpdRate = 8.0f );

	//���̃��j���[�E�B���h�E��delete�͂���if���̒��Ŏg���܂��傤.
	bool isDeletePermission();

	//���̍����0�Ƃ��č��W��^����.
	D3DXVECTOR3 SetPosFromWindow( const D3DXVECTOR2& vPos );


	//�I��悵�����ʂ�Ԃ�.
	unsigned int GetInformation(){
		auto ReturnInformation = m_uiInformation;
		if( m_pNextWindow ){
			ReturnInformation = m_pNextWindow->GetInformation();
		}
		m_uiInformation = 0;
		return ReturnInformation;
	}


protected:

	//===== �p����̃R���X�g���N�^�Ŏg�� =====//.
	void Open( const D3DXVECTOR2& vSize );
	//===== �p����̃R���X�g���N�^�Ŏg�� =====//.


	//���̑���f���o��( ���������Ȃ�true ).
	bool CreateNextWindow( clsMENU_WINDOW_BASE** ppOutNextWindow );

	//���̃E�B���h�E�𑀍삷��悤�ɂȂ�Ȃ�true,�����Ȃ�false.
	void Operation( const bool isOperation ){ m_isOperation = isOperation; };


	//����.
	bool SelectUp	( bool isWithStick = true );
	bool SelectDown	( bool isWithStick = true );
	bool SelectRight( bool isWithStick = true );
	bool SelectLeft	( bool isWithStick = true );
	bool SelectEnter();
	bool SelectExit();

	//�V�[���N���X�Ə��̂���������ϐ��̌^�y�э쐬�֐��̎����֐�.
	static INFORMATION_MENU_DATA_ARRAY CreateInformationProduct( 
		INFORMATION_MENU_DATA_ARRAY* const InformationDataArray,
		const int iInformationSize,
		const char* sInformationDataPath );

protected:

	//�I����.
	int m_iSelectNum;
	//���j���[���J�������̂ɕԂ��l.
	unsigned int m_uiInformation;
	//�V�[�����������Ă���, ���̉�������\�����̐��l�̔z��.
	std::vector<unsigned int>*		m_pInformationVec;

	//����.
	clsFont* m_wpFont;
	std::vector< std::unique_ptr< clsUiText > > m_vecupUiText;
	//�I���J�[�\��.
	std::unique_ptr< clsSprite2D > m_upCursor;

	//����.
	clsXInput*	m_wpXInput;
	clsDxInput* m_wpDInput;


	//���ɊJ����.
	clsMENU_WINDOW_BASE* m_pNextWindow;


	//�q���̂��߂ɕK�v.
	clsPOINTER_GROUP*	m_pPtrGroup;

	HWND m_hWnd;

	//���ʉ�.
	std::unique_ptr< clsSOUND_MANAGER_MENUWINDOW > m_upSound;

private:
	virtual void UpdateProduct() = 0;
	virtual void RenderProduct() = 0;
	virtual bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) = 0;

	void SetColor( const D3DXVECTOR3& vColor ) final;
	void SetTextAlpha( const float& fAlpha );

	//�ŏ���( �������̏o��T�C�Y�� ).
	bool isMinimum();

private:

	//�J�[�\���ړ��ɕK�v.
	struct HOLD_STATE
	{
		int iHoldFream;
		int iFirstPush;//�ŏ��̈��.
		HOLD_STATE()
		:iHoldFream( 0 )
		,iFirstPush( 0 )
		{}
	};
	HOLD_STATE m_HoldRight;
	HOLD_STATE m_HoldLeft;
	HOLD_STATE m_HoldUp;
	HOLD_STATE m_HoldDown;





	//���̑����J������.
	clsMENU_WINDOW_BASE* m_pParentWindow;


	//true�Ȃ炱�̑��𓮂�����.
	bool m_isOperation;
	
	//���̑������\��.
	bool m_isClose;

};

#endif//#ifndef MENU_WINDOW_BASE_H_