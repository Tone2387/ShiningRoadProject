#include "MenuWindowAssembleColorChange.h"
#include "PtrGroup.h"

#include "AssembleModel.h"

using namespace std;

namespace{
	const float fCOLOR_GAGE_SIZE_BASE = 32.0f;

	const WHSIZE_FLOAT BONE_SIZE = { fCOLOR_GAGE_SIZE_BASE * 15.0f, fCOLOR_GAGE_SIZE_BASE };
	const WHSIZE_FLOAT GAGE_SIZE = { fCOLOR_GAGE_SIZE_BASE,			fCOLOR_GAGE_SIZE_BASE };
}

clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE::clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE(
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	std::vector<unsigned int>* const pInformationVec,
	clsASSEMBLE_MODEL* const pAssembleModel )
	:clsMENU_WINDOW_ASSEMBLE_BASE( pPtrGroup, pParentWindow, pInformationVec )
	,m_wpAssembleModel( pAssembleModel )
{
	//この窓のサイズ.
	const D3DXVECTOR2 vTHIS_WINDOW_SIZE = { 678.0f, 500.0f };
	Open( vTHIS_WINDOW_SIZE );

	//骨.
	m_vecupColorBone.resize( clsROBO_STATUS::enCOLOR_GAGE_size );
	const char sCOLOR_GAGE_BONE_PATH[] = "Data\\Image\\AssembleUi\\ColorGageBone.png";
	SPRITE_STATE ss;
	ss.Disp = BONE_SIZE;

	for( unsigned int i=0; i<m_vecupColorBone.size(); i++ ){
		m_vecupColorBone[i] = make_unique<clsSprite2D>();
		m_vecupColorBone[i]->Create(
			pPtrGroup->GetDevice(), pPtrGroup->GetContext(), sCOLOR_GAGE_BONE_PATH, ss );
	}


	//ゲージ本体.
	m_vecupColorGage.resize( clsROBO_STATUS::enCOLOR_GAGE_size );
	const char sCOLOR_GAGE_PATH[] = "Data\\Image\\AssembleUi\\ColorGage.png";
	ss.Disp = GAGE_SIZE;
	const float fGAGE_ALPHA = 0.5f;
	const unsigned int uiCOLOR_MAX = 3; 
	const D3DXVECTOR3 vGAGE_COLOR_ARRAY[3] = {
		D3DXVECTOR3( 1.0f, 0.0f, 0.0f ),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f ),
	}; 

	unsigned int uiColorIndex = 0;
	for( unsigned int i=0; i<m_vecupColorGage.size(); i++ ){
		m_vecupColorGage[i] = make_unique<clsSprite2D>();
		m_vecupColorGage[i]->Create(
			pPtrGroup->GetDevice(), pPtrGroup->GetContext(), sCOLOR_GAGE_PATH, ss );

		m_vecupColorGage[i]->SetAlpha( fGAGE_ALPHA );
		m_vecupColorGage[i]->SetColor( vGAGE_COLOR_ARRAY[ uiColorIndex ++ ] );
		if( uiColorIndex >= uiCOLOR_MAX ){
			uiColorIndex = 0;
		}
	}

}

clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE::~clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE()
{
	m_wpAssembleModel = nullptr;
}




void clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE::UpdateProduct()
{
	if( SelectUp() ){
		m_iSelectNum --;
		if( m_iSelectNum < 0 ){
			m_iSelectNum = 0;
		}
		else{
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		}
	}
	else if( SelectDown() ){
		m_iSelectNum ++;
		if( m_iSelectNum >= enSELECT_NUM_size ){
			m_iSelectNum = enSELECT_NUM_size - 1;
		}
		else{
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
		}
	}

	if( SelectRight() ){
		//音はメインに任せる.
		switch( m_iSelectNum )
		{
		case enSELECT_NUM_1_R:
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_1_R_INCREMENT ];
			break;
		case enSELECT_NUM_1_G:
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_1_G_INCREMENT ];
			break;
		case enSELECT_NUM_1_B:
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_1_B_INCREMENT ];
			break;

		case enSELECT_NUM_2_R:
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_2_R_INCREMENT ];
			break;
		case enSELECT_NUM_2_G:
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_2_G_INCREMENT ];
			break;
		case enSELECT_NUM_2_B:
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_2_B_INCREMENT ];
			break;
		}
	}
	else if( SelectLeft() ){
		//音はメインに任せる.
		switch( m_iSelectNum )
		{
		case enSELECT_NUM_1_R:
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_1_R_DECREMENT ];
			break;																	
		case enSELECT_NUM_1_G:														
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_1_G_DECREMENT ];
			break;																	
		case enSELECT_NUM_1_B:														
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_1_B_DECREMENT ];
			break;																	
																					
		case enSELECT_NUM_2_R:														
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_2_R_DECREMENT ];
			break;																	
		case enSELECT_NUM_2_G:														
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_2_G_DECREMENT ];
			break;																	
		case enSELECT_NUM_2_B:														
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_COLOR_2_B_DECREMENT ];
			break;
		}
	}

	if( SelectEnter() ){
		if( m_iSelectNum == enSELECT_NUM_BACK ){
			m_wpSound->PlaySE( enSE_ENTER );
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_CLOSE_WINDOW ];
		}
	}

	if( SelectExit() ){
		m_wpSound->PlaySE( enSE_EXIT );
		m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_CLOSE_WINDOW ];
	}

}

void clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE::RenderProduct()
{
	int iTextRow;
//	const int iYES_INDEX = 4;
//	const int iQUESTION_INDEX = 1;
//	
//	//yes.
//	const D3DXVECTOR2 vPOS_YES_LOCAL = { 150.0f, 250.0f };
//	const D3DXVECTOR3 vPOS_YES = SetPosFromWindow( vPOS_YES_LOCAL );
//	const float fSCALE = 24;
//	int iTextRow = iYES_INDEX;
//	m_wpFont->SetPos( vPOS_YES );
//	m_wpFont->SetScale( fSCALE );
//	m_wpFont->Render( iTextRow ++ );
//
//	//no.
//	const D3DXVECTOR2 vPOS_NO_LOCAL = { 370.0f, 250.0f };
//	const D3DXVECTOR3 vPOS_NO = SetPosFromWindow( vPOS_NO_LOCAL );
//	m_wpFont->SetPos( vPOS_NO );
//	m_wpFont->Render( iTextRow ++ );
//


	//画像.
	const D3DXVECTOR2 vBONE_POS_LOCAL = { 180.0f, 100.0f };
	const D3DXVECTOR3 vBONE_POS = SetPosFromWindow( vBONE_POS_LOCAL );
	const float fBONE_OFFSET_Y = fCOLOR_GAGE_SIZE_BASE + 20.0f;
	const float fBONE_OFFSET_Y_2 =  24.0f;//パーツのベースと装甲の合間の追加オフセット.

	for( unsigned int i=0; i<m_vecupColorBone.size(); i++ ){
		m_vecupColorBone[i]->SetPos( vBONE_POS );
		m_vecupColorBone[i]->AddPos( { 0.0f, fBONE_OFFSET_Y * static_cast<float>( i ), 0.0f } );
		if( i >= clsROBO_STATUS::enCOLOR_GAGE_ARMOR_R ){
			m_vecupColorBone[i]->AddPos( { 0.0f, fBONE_OFFSET_Y_2, 0.0f } );
		}
		m_vecupColorBone[i]->Render();
	}

	for( unsigned int i=0; i<m_vecupColorGage.size(); i++ ){
		m_vecupColorGage[i]->SetPos( m_vecupColorBone[i]->GetPos() );
		m_vecupColorGage[i]->SetScale( { 
			m_wpAssembleModel->GetColorGradation( 
				static_cast<clsROBO_STATUS::enCOLOR_GAGE>( i ) ) * ( BONE_SIZE.w / GAGE_SIZE.w ),
			1.0f, 
			1.0f } );
		m_vecupColorGage[i]->Render();
	}

	//----- Text -----//.
	//RGB.
	const float fSCALE_RGB = 24.0f;
	const int iRGB_INDEX = 8;
	iTextRow = iRGB_INDEX;
	m_wpFont->SetScale( fSCALE_RGB );
	for( unsigned int i=0; i<m_vecupColorGage.size(); i++ ){
		const D3DXVECTOR3 vPOS_AGB = { m_vecupColorBone[i]->GetPos().x, m_vecupColorBone[i]->GetPos().y, 0.0f };
//		const D3DXVECTOR3 vPOS_AGB = SetPosFromWindow( vPOS_AGB_LOCAL );
		m_wpFont->SetPos( vPOS_AGB );
		const D3DXVECTOR3 vADD_POS = { -50.0f, 0.0f, 0.0f };
		m_wpFont->AddPos( vADD_POS );
		m_wpFont->Render( iTextRow ++ );
		if( i >= clsROBO_STATUS::enCOLOR_GAGE_ARMOR_R ){
			iTextRow = iRGB_INDEX;
		}
	}


	///カーソル移動.
	if( m_iSelectNum == enSELECT_NUM_BACK ){
		const D3DXVECTOR3 vCURSOR_SCALE = { fCOLOR_GAGE_SIZE_BASE, fCOLOR_GAGE_SIZE_BASE, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
//		m_upCursor->SetPos( m_vecupColorBone[ m_iSelectNum ]->GetPos() );
	}
	else{
		const D3DXVECTOR3 vCURSOR_SCALE = { 24.0f*2.1f, fCOLOR_GAGE_SIZE_BASE, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
		m_upCursor->SetPos( m_vecupColorBone[ m_iSelectNum ]->GetPos() );
	}
	m_upCursor->Render();

}

bool clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE::CreateNextWindowProduct( 
	clsMENU_WINDOW_BASE** ppOutNextWindow, 
	clsMENU_WINDOW_BASE* const pParentWindow )
{


	return false;
}






