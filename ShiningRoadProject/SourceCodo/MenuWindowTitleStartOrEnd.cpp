#include "MenuWindowTitleStartOrEnd.h"
#include "PtrGroup.h"
#include "SoundManagerMenuWindow.h"
#include "CFont.h"
//#include "UiText.h"


namespace{

	const int iSELECT_NUM_YES_INDEX = 0;

}




clsMENU_WINDOW_TITLE_START_OR_END::clsMENU_WINDOW_TITLE_START_OR_END(		
	const HWND hWnd,
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* pParentWindow,
		std::vector<unsigned int>* const pInformationVec )
		:clsMENU_WINDOW_TITLE_BASE( hWnd, pPtrGroup, pParentWindow, pInformationVec, "MenuWindowTitleStartOrEnd" )
{
	//この窓のサイズ.
	const D3DXVECTOR2 vTHIS_WINDOW_SIZE = { 200.0f, 150.0f };
	Open( vTHIS_WINDOW_SIZE );
}

clsMENU_WINDOW_TITLE_START_OR_END::~clsMENU_WINDOW_TITLE_START_OR_END()
{
}





void clsMENU_WINDOW_TITLE_START_OR_END::UpdateProduct()
{
	const int iSELECT_NUM_MAX = 1;//選択肢の最大は何?.

	if( SelectUp() ){
		m_iSelectNum --;
		if( m_iSelectNum < 0 ){
			m_iSelectNum = 0;
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_CURSOL );
		}
	}

	if( SelectDown() ){
		m_iSelectNum ++;
		if( m_iSelectNum > iSELECT_NUM_MAX ){
			m_iSelectNum = iSELECT_NUM_MAX;
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_CURSOL );
		}
	}

	if( SelectEnter() ){
		if( m_iSelectNum ){
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_EXIT );
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_GAME_END ];
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_ENTER );
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_NEXT_SCENE ];
		}
	}

	if( SelectExit() ){
		m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_WIN_DISAPP );
		m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_CLOSE_MENU ];
	}


}

void clsMENU_WINDOW_TITLE_START_OR_END::RenderProduct()
{
	//始める.
	const D3DXVECTOR2 vPOS_START_LOCAL = { 45.0f, 30.0f };
	const D3DXVECTOR3 vPOS_START = SetPosFromWindow( vPOS_START_LOCAL );
	const float fSCALE = 24;
	int iTextRow = 1;
	m_wpFont->SetPos( vPOS_START );
	m_wpFont->SetScale( fSCALE );
	m_wpFont->Render( iTextRow ++ );

	//終わる.
	const D3DXVECTOR2 vPOS_EXIT_LOCAL = { 45.0f, 80.0f };
	const D3DXVECTOR3 vPOS_EXIT = SetPosFromWindow( vPOS_EXIT_LOCAL );
	m_wpFont->SetPos( vPOS_EXIT );
	m_wpFont->Render( iTextRow ++ );

	///カーソル移動.
	if( m_iSelectNum == iSELECT_NUM_YES_INDEX ){
		const D3DXVECTOR3 vCURSOR_SCALE = { 24.0f*5.0f, 32.0f, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
		m_upCursor->SetPos( vPOS_START );
	}
	else{
		const D3DXVECTOR3 vCURSOR_SCALE = { 24.0f*4.0f, 32.0f, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
		m_upCursor->SetPos( vPOS_EXIT );
	}


}

