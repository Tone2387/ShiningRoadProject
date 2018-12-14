#include "MenuWindowMissionPause.h"
#include "SoundManagerMenuWindow.h"
#include "CFont.h"



namespace{

	//��ʏ�ɏo�Ă郁�j���[�̂�YES�̑I��ԍ�.
	const int iSELECT_NUM_YES_INDEX = 0;

}



clsMENU_WINDOW_MISSION_PAUSE::clsMENU_WINDOW_MISSION_PAUSE(
	const HWND hWnd,
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	std::vector<unsigned int>* const pInformationVec )
	:clsMENU_WINDOW_MISSION_BASE( 
		hWnd, pPtrGroup, pParentWindow, pInformationVec, 
		"MenuWindowAssembleBackTitle" )
{
	//���̑��̃T�C�Y.
	const D3DXVECTOR2 vTHIS_WINDOW_SIZE = { 600.0f, 340.0f };
	Open( vTHIS_WINDOW_SIZE );
}

clsMENU_WINDOW_MISSION_PAUSE::~clsMENU_WINDOW_MISSION_PAUSE()
{
}




void clsMENU_WINDOW_MISSION_PAUSE::UpdateProduct()
{
	const int iSELECT_NUM_MAX = 1;//�I�����̍ő�͉�?.

	//�I�������E.
	if( SelectLeft( false ) ){
		m_iSelectNum --;
		if( m_iSelectNum < 0 ){
			m_iSelectNum = 0;
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_CURSOL );
		}
	}
	if( SelectRight( false ) ){
		m_iSelectNum ++;
		if( m_iSelectNum > iSELECT_NUM_MAX ){
			m_iSelectNum = iSELECT_NUM_MAX;
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_CURSOL );
		}
	}

	if( SelectEnter() ){
		//�~�b�V��������.
		if( m_iSelectNum == iSELECT_NUM_YES_INDEX ){
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_ENTER );
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_MISSION_FAILED ];
		}
		//�|�[�Y����.
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_EXIT );
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_WINDOW_CLOSE ];
		}
	}
	//�|�[�Y����.
	if( SelectExit() ){
		m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_EXIT );
		m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_WINDOW_CLOSE ];
	}


}

void clsMENU_WINDOW_MISSION_PAUSE::RenderProduct()
{
	const int iYES_INDEX = 1;
	const int iQUESTION_INDEX = 0;

	//yes.
	const D3DXVECTOR2 vPOS_YES_LOCAL = { 180.0f, 250.0f };
	const D3DXVECTOR3 vPOS_YES = SetPosFromWindow( vPOS_YES_LOCAL );
	const float fSCALE = 24;
	int iTextRow = iYES_INDEX;
	m_wpFont->SetPos( vPOS_YES );
	m_wpFont->SetScale( fSCALE );
	m_wpFont->Render( iTextRow ++ );

	//no.
	const D3DXVECTOR2 vPOS_NO_LOCAL = { 400.0f, 250.0f };
	const D3DXVECTOR3 vPOS_NO = SetPosFromWindow( vPOS_NO_LOCAL );
	m_wpFont->SetPos( vPOS_NO );
	m_wpFont->Render( iTextRow ++ );


	//�u�~�b�V������������܂���?�v.
	const D3DXVECTOR2 vPOS_CONTINUE_LOCAL = { 70.0f, 70.0f };
	const D3DXVECTOR3 vPOS_CONTINUE = SetPosFromWindow( vPOS_CONTINUE_LOCAL );
	const float fSCALE_CONTINUE = 36;
	iTextRow = iQUESTION_INDEX;
	m_wpFont->SetPos( vPOS_CONTINUE );
	m_wpFont->SetScale( fSCALE_CONTINUE );
	m_wpFont->Render( iTextRow ++ );



	///�J�[�\���ړ�.
	if( m_iSelectNum == iSELECT_NUM_YES_INDEX ){
		const D3DXVECTOR3 vCURSOR_SCALE = { 24.0f*3.0f, 32.0f, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
		m_upCursor->SetPos( vPOS_YES );
	}
	else{
		const D3DXVECTOR3 vCURSOR_SCALE = { 24.0f*2.1f, 32.0f, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
		m_upCursor->SetPos( vPOS_NO );
	}

}
