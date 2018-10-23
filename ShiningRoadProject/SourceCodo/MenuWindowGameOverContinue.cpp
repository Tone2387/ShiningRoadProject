#include "MenuWindowGameOverContinue.h"
#include "PtrGroup.h"
//#include "UiText.h"
#include "MenuWindowGameOverDoAssemble.h"


namespace{
	//渡す情報の添え字.
	const char INFORMATION__INDEX_GAME_OVER	= 0;
	const char INFORMATION__INDEX_CONTINUE	= 1;
	const char INFORMATION__INDEX_ASSEMBLE	= 2;
}


clsMENU_WINDOW_GAME_OVER_CONTINUE::clsMENU_WINDOW_GAME_OVER_CONTINUE(
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	unsigned int* const pInformationArray )
	:clsMENU_WINDOW_BASE( pPtrGroup, pParentWindow, pInformationArray )
{
	//この窓のサイズ.
	const D3DXVECTOR2 vTHIS_WINDOW_SIZE = { 600.0f, 400.0f };
	Open( vTHIS_WINDOW_SIZE );
}

clsMENU_WINDOW_GAME_OVER_CONTINUE::~clsMENU_WINDOW_GAME_OVER_CONTINUE()
{
}




void clsMENU_WINDOW_GAME_OVER_CONTINUE::UpdateProduct() 
{
	const int iSELECT_NUM_MAX = 1;//選択肢の最大は何?.

	if( SelectLeft() ){
		m_iSelectNum --;
		if( m_iSelectNum < 0 ){
			m_iSelectNum = 0;
		}

	}

	if( SelectRight() ){
		m_iSelectNum ++;
		if( m_iSelectNum > iSELECT_NUM_MAX ){
			m_iSelectNum = iSELECT_NUM_MAX;
		}
	}

	if( SelectEnter() ){
		if( m_iSelectNum == 0 ){
//			m_uiInformation = m_puiInformationDataArray[ INFORMATION__INDEX_CONTINUE ];
			//次を開く.
			if( CreateNextWindow( &m_pNextWindow ) ){
				const D3DXVECTOR3 vADD_POS = D3DXVECTOR3( 20.0f, 20.0f, 0.0f );
				m_pNextWindow->SetPos( GetPos() + vADD_POS );
				Operation( false );
			}
		}
		else if( m_iSelectNum == 1 ){
			m_uiInformation = m_puiInformationDataArray[ INFORMATION__INDEX_GAME_OVER ];
		}
	}

	if( SelectExit() ){
		m_uiInformation = m_puiInformationDataArray[ INFORMATION__INDEX_GAME_OVER ];
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

	//コンティニュー?.
	const D3DXVECTOR2 vPOS_CONTINUE_LOCAL = { 150.0f, 70.0f };
	const D3DXVECTOR3 vPOS_CONTINUE = SetPosFromWindow( vPOS_CONTINUE_LOCAL );
	const float fSCALE_CONTINUE = 36;
	m_wpFont->SetPos( vPOS_CONTINUE );
	m_wpFont->SetScale( fSCALE_CONTINUE );
	m_wpFont->Render( iTextRow ++ );

	///カーソル移動.
	if( m_iSelectNum == 0 ){
		const D3DXVECTOR3 vCURSOR_SCALE = { 24.0f*3.0f, 32.0f, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
		m_upCursor->SetPos( vPOS_YES );
	}
	else if( m_iSelectNum == 1 ){
		const D3DXVECTOR3 vCURSOR_SCALE = { 24.0f*2.1f, 32.0f, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
		m_upCursor->SetPos( vPOS_NO );
	}


}


bool clsMENU_WINDOW_GAME_OVER_CONTINUE::CreateNextWindowProduct( 
	clsMENU_WINDOW_BASE** ppOutNextWindow, 
	clsMENU_WINDOW_BASE* const pParentWindow ) 
{
//	*ppOutNextWindow = new clsMENU_WINDOW_GAME_OVER_CONTINUE( m_pPtrGroup, pParentWindow, m_puiInformationArray );
	*ppOutNextWindow = new clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE( m_pPtrGroup, pParentWindow, m_puiInformationArray );

	if( *ppOutNextWindow ){
		return true;
	}

	return false;
}




