#include "MenuWindowGameOverDoAssemble.h"
#include "PtrGroup.h"
//#include "UiText.h"


namespace{

	const int iSELECT_NUM_YES_INDEX = 0;

}



clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE::clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE(
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	std::vector<unsigned int>* const pInformationVec )
	:clsMENU_WINDOW_GAME_OVER_BASE( pPtrGroup, pParentWindow, pInformationVec )
{
	//この窓のサイズ.
	const D3DXVECTOR2 vTHIS_WINDOW_SIZE = { 600.0f, 400.0f };
	Open( vTHIS_WINDOW_SIZE );

}

clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE::~clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE()
{
}

void clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE::UpdateProduct()
{
	const int iSELECT_NUM_MAX = 1;//選択肢の最大は何?.

	if( SelectLeft() ){
		m_iSelectNum --;
		if( m_iSelectNum < 0 ){
			m_iSelectNum = 0;
		}
		else{
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		}
	}

	if( SelectRight() ){
		m_iSelectNum ++;
		if( m_iSelectNum > iSELECT_NUM_MAX ){
			m_iSelectNum = iSELECT_NUM_MAX;
		}
		else{
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		}
	}

	if( SelectEnter() ){
		m_wpSound->PlaySE( enSE_MISSION_START );
		m_wpSound->PlaySE( enSE_ENTER );
		//アセンブルしなおす.
		if( m_iSelectNum == iSELECT_NUM_YES_INDEX ){
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_ASSEMBLE ];
		}
		//そのまま戦う.
		else{
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_CONTINUE ];
		}
	}

	if( SelectExit() ){
		m_wpSound->PlaySE( enSE_EXIT );
		Close();
	}

}

void clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE::RenderProduct()
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

	//組みなおす?.
	const D3DXVECTOR2 vPOS_ASSEMBLE_LOCAL = { 35.0f, 70.0f };
	const D3DXVECTOR3 vPOS_ASSEMBLE = SetPosFromWindow( vPOS_ASSEMBLE_LOCAL );
	const float fSCALE_ASSEMBLE = 36;
	m_wpFont->SetPos( vPOS_ASSEMBLE );
	m_wpFont->SetScale( fSCALE_ASSEMBLE );
	m_wpFont->Render( ++ iTextRow );

	///カーソル移動.
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

bool clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE::CreateNextWindowProduct( 
	clsMENU_WINDOW_BASE** ppOutNextWindow, 
	clsMENU_WINDOW_BASE* const pParentWindow )
{

	return false;
}

