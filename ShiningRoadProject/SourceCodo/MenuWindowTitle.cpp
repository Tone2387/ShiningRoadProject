#include "MenuWindowTitle.h"



namespace{
	const char INFORMATION__INDEX_GAME_END		= 0;
	const char INFORMATION__INDEX_NEXT_SCENE	= 1;
}

clsMENU_WINDOW_TITLE::clsMENU_WINDOW_TITLE(		
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* pParentWindow,
		unsigned int* const pInformationArray )
		:clsMENU_WINDOW_BASE( pPtrGroup, pParentWindow, pInformationArray )
{
	//この窓のサイズ.
	D3DXVECTOR2 vTHIS_WINDOW_SIZE = { 300.0f, 200.0f };
	Open( vTHIS_WINDOW_SIZE );
}

clsMENU_WINDOW_TITLE::~clsMENU_WINDOW_TITLE()
{
}





void clsMENU_WINDOW_TITLE::UpdateProduct()
{
	const int iSELECT_NUM_MAX = 1;//選択肢の最大は何?.

	if( SelectUp() ){
		m_iSelectNum --;
		if( m_iSelectNum < 0 ){
			m_iSelectNum = 0;
		}

	}

	if( SelectDown() ){
		m_iSelectNum ++;
		if( m_iSelectNum > iSELECT_NUM_MAX ){
			m_iSelectNum = iSELECT_NUM_MAX;
		}
	}

	if( SelectEnter() ){
		if( m_iSelectNum ){
			m_uiInformation = m_puiInformationDataArray[ INFORMATION__INDEX_GAME_END ];
		}
		else{
			m_uiInformation = m_puiInformationDataArray[ INFORMATION__INDEX_NEXT_SCENE ];
		}
	}

	if( SelectExit() ){
		m_uiInformation = m_puiInformationDataArray[ INFORMATION__INDEX_GAME_END ];
	}

}

void clsMENU_WINDOW_TITLE::RenderProduct()
{

}

bool clsMENU_WINDOW_TITLE::CreateNextWindowProduct( 
	clsMENU_WINDOW_BASE** ppOutNextWindow, 
	clsMENU_WINDOW_BASE* const pParentWindow )
{

	return false;
}




