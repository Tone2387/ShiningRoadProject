#include "MenuWindowGameOverContinue.h"
#include "PtrGroup.h"
#include "SoundManagerMenuWindow.h"
#include "CFont.h"

//���j���[���Ăяo������.
#include "MenuWindowGameOverDoAssemble.h"

namespace{

	const int iSELECT_NUM_YES_INDEX = 0;
	//���̑��̃T�C�Y.
	const D3DXVECTOR2 vTHIS_WINDOW_SIZE = { 600.0f, 400.0f };

}


clsMENU_WINDOW_GAME_OVER_CONTINUE::clsMENU_WINDOW_GAME_OVER_CONTINUE(
	const HWND hWnd,
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	std::vector<unsigned int>* const pInformationVec )
	:clsMENU_WINDOW_GAME_OVER_BASE(
		hWnd, pPtrGroup, pParentWindow,
		pInformationVec, 
		vTHIS_WINDOW_SIZE,
		"MenuWindowGameoverContinue" )
{
}

clsMENU_WINDOW_GAME_OVER_CONTINUE::~clsMENU_WINDOW_GAME_OVER_CONTINUE()
{
}




void clsMENU_WINDOW_GAME_OVER_CONTINUE::UpdateProduct() 
{
	const int iSELECT_NUM_MAX = 1;//�I�����̍ő�͉�?.

	if( SelectLeft() ){
		m_iSelectNum --;
		if( m_iSelectNum < 0 ){
			m_iSelectNum = 0;
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_CURSOL );
		}
	}

	if( SelectRight() ){
		m_iSelectNum ++;
		if( m_iSelectNum > iSELECT_NUM_MAX ){
			m_iSelectNum = iSELECT_NUM_MAX;
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_CURSOL );
		}
	}

	if( SelectEnter() ){
		if( m_iSelectNum == iSELECT_NUM_YES_INDEX ){
			//�����J��.
			//�������݂���Ȃ���Ȃ�.
			if( m_pNextWindow ){}
			else if( CreateNextWindow( &m_pNextWindow ) ){
				m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_ENTER );
				const D3DXVECTOR3 vADD_POS = D3DXVECTOR3( 20.0f, 20.0f, 0.0f );
				m_pNextWindow->SetPos( GetPos() + vADD_POS );
				Operation( false );
			}
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_EXIT );
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_GAME_OVER ];
		}
	}

	if( SelectExit() ){
		m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_EXIT );
		m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_GAME_OVER ];
	}

}

void clsMENU_WINDOW_GAME_OVER_CONTINUE::RenderProduct() 
{
	//yes.
	const D3DXVECTOR2 vPOS_YES_LOCAL = { 150.0f, 250.0f };
	const D3DXVECTOR3 vPOS_YES = SetPosFromWindow( vPOS_YES_LOCAL );
	const float fSCALE = 24;
	int iTextRow = 3;
	m_wpFont->SetPos( vPOS_YES );
	m_wpFont->SetScale( fSCALE );
	m_wpFont->Render( iTextRow ++ );

	//no.
	const D3DXVECTOR2 vPOS_NO_LOCAL = { 370.0f, 250.0f };
	const D3DXVECTOR3 vPOS_NO = SetPosFromWindow( vPOS_NO_LOCAL );
	m_wpFont->SetPos( vPOS_NO );
	m_wpFont->Render( iTextRow ++ );

	//�R���e�B�j���[?.
	const D3DXVECTOR2 vPOS_CONTINUE_LOCAL = { 75.0f, 70.0f };
	const D3DXVECTOR3 vPOS_CONTINUE = SetPosFromWindow( vPOS_CONTINUE_LOCAL );
	const float fSCALE_CONTINUE = 36;
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


bool clsMENU_WINDOW_GAME_OVER_CONTINUE::CreateNextWindowProduct( 
	clsMENU_WINDOW_BASE** ppOutNextWindow, 
	clsMENU_WINDOW_BASE* const pParentWindow ) 
{
//	*ppOutNextWindow = new clsMENU_WINDOW_GAME_OVER_CONTINUE( m_hWnd, m_pPtrGroup, pParentWindow, m_pInformationVec );
	*ppOutNextWindow = new clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE( m_hWnd, m_pPtrGroup, pParentWindow, m_pInformationVec );

	if( *ppOutNextWindow ){
		return true;
	}

	return false;
}




