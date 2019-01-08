#include "MenuWindowAssembleColorChange.h"
#include "PtrGroup.h"
#include "SoundManagerMenuWindow.h"
#include "CFont.h"

#include "AssembleModel.h"

using namespace std;

namespace{
	const float fCOLOR_GAGE_SIZE_BASE = 32.0f;

	const WHSIZE_FLOAT BONE_SIZE = { fCOLOR_GAGE_SIZE_BASE * 15.0f, fCOLOR_GAGE_SIZE_BASE };
	const WHSIZE_FLOAT GAGE_SIZE = { fCOLOR_GAGE_SIZE_BASE,			fCOLOR_GAGE_SIZE_BASE };

	//���̑��̃T�C�Y.
	const D3DXVECTOR2 vTHIS_WINDOW_SIZE = { 678.0f, 500.0f };
}

clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE::clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE(
	const HWND hWnd,
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* const pParentWindow,
	std::vector<unsigned int>* const pInformationVec,
	clsASSEMBLE_MODEL* const pAssembleModel )
	:clsMENU_WINDOW_ASSEMBLE_BASE( 
		hWnd, pPtrGroup, pParentWindow, 
		pInformationVec, 
		vTHIS_WINDOW_SIZE,
		"MenuWindowAssembleColorChange" )
	,m_wpAssembleModel( pAssembleModel )
{

	//��.
	m_vecupColorBone.resize( clsROBO_STATUS::enCOLOR_GAGE_size );
	const char sCOLOR_GAGE_BONE_PATH[] = "Data\\Image\\AssembleUi\\ColorGageBone.png";
	SPRITE_STATE ss;
	ss.Disp = BONE_SIZE;

	for( unsigned int i=0; i<m_vecupColorBone.size(); i++ ){
		m_vecupColorBone[i] = make_unique<clsSprite2D>();
		m_vecupColorBone[i]->Create(
			pPtrGroup->GetDevice(), pPtrGroup->GetContext(), sCOLOR_GAGE_BONE_PATH, ss );
	}


	//�Q�[�W�{��.
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
	if( SelectUp( false ) ){
		m_iSelectNum --;
		if( m_iSelectNum < 0 ){
			m_iSelectNum = 0;
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_CURSOL );
		}
	}
	else if( SelectDown( false ) ){
		m_iSelectNum ++;
		if( m_iSelectNum >= enSELECT_NUM_size ){
			m_iSelectNum = enSELECT_NUM_size - 1;
		}
		else{
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_CURSOL );
		}
	}

	if( SelectRight( false ) ){
		//���̓��C���ɔC����.
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
	else if( SelectLeft( false ) ){
		//���̓��C���ɔC����.
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
			m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_ENTER );
			m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_CLOSE_WINDOW ];
		}
	}

	if( SelectExit() ){
		m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_EXIT );
		m_uiInformation = ( *m_pInformationVec )[ enINFORMATION_INDEX_CLOSE_WINDOW ];
	}

}

void clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE::RenderProduct()
{


	//�Q�[�W�摜.
	const D3DXVECTOR2 vBONE_POS_LOCAL = { 180.0f, 100.0f };
	const D3DXVECTOR3 vBONE_POS = SetPosFromWindow( vBONE_POS_LOCAL );
	const float fBONE_OFFSET_Y = fCOLOR_GAGE_SIZE_BASE + 20.0f;
	const float fBONE_OFFSET_Y_2 =  24.0f;//�p�[�c�̃x�[�X�Ƒ��b�̍��Ԃ̒ǉ��I�t�Z�b�g.
	//��.
	for( unsigned int i=0; i<m_vecupColorBone.size(); i++ ){
		m_vecupColorBone[i]->SetPos( vBONE_POS );
		m_vecupColorBone[i]->AddPos( { 0.0f, fBONE_OFFSET_Y * static_cast<float>( i ), 0.0f } );
		if( i >= clsROBO_STATUS::enCOLOR_GAGE_ARMOR_R ){
			m_vecupColorBone[i]->AddPos( { 0.0f, fBONE_OFFSET_Y_2, 0.0f } );
		}
		m_vecupColorBone[i]->Render();
	}
	//�Q�[�W�{��.
	for( unsigned int i=0; i<m_vecupColorGage.size(); i++ ){
		m_vecupColorGage[i]->SetPos( m_vecupColorBone[i]->GetPos() );
		//����̃��{�̐F�ɍ��킹�ăQ�[�W�����ɐL�k������.
		m_vecupColorGage[i]->SetScale( { 
			m_wpAssembleModel->GetColorGradation( 
				static_cast<clsROBO_STATUS::enCOLOR_GAGE>( i ) ) * ( BONE_SIZE.w / GAGE_SIZE.w ),
			1.0f, 
			1.0f } );
		m_vecupColorGage[i]->Render();
	}


	//�J�[�\���ׂ̈̍��W�z��.
	D3DXVECTOR3 vCURSOL_POS_ARRAY[ enSELECT_NUM_size ];


	//----- Text���W�����肷�� -----//.
	//RGB.
	const float fSCALE_RGB = 24.0f;
	const int iRGB_INDEX = 9;
	for( unsigned int i=0; i<m_vecupColorGage.size(); i++ ){
		const D3DXVECTOR3 vADD_POS = { -50.0f, 0.0f, 0.0f };
		const D3DXVECTOR3 vPOS_AGB = { m_vecupColorBone[i]->GetPos().x, m_vecupColorBone[i]->GetPos().y, 0.0f };

		//�u R,G,B,R,G,B �v�̍��W���o����.
		vCURSOL_POS_ARRAY[i] = vADD_POS + vPOS_AGB;
	}

	//�u�߂�v�̍��W���o����.
	if( enSELECT_NUM_BACK > 0 ){
		vCURSOL_POS_ARRAY[ enSELECT_NUM_BACK ] = vCURSOL_POS_ARRAY[ enSELECT_NUM_BACK - 1 ];
		vCURSOL_POS_ARRAY[ enSELECT_NUM_BACK ] += D3DXVECTOR3( 0.0f, fBONE_OFFSET_Y, 0.0f );
	}

	///�J�[�\���ړ�.
	m_upCursor->SetPos( vCURSOL_POS_ARRAY[ m_iSelectNum ] );
	//�u�߂�v.
	if( m_iSelectNum == enSELECT_NUM_BACK ){
		const D3DXVECTOR3 vCURSOR_SCALE = { 24.0f*2.1f, fCOLOR_GAGE_SIZE_BASE, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
		const D3DXVECTOR3 vCURSOR_SCALE_ADD = { 1.0625f, 1.25f, 0.0f };
		m_upCursor->AddScale( vCURSOR_SCALE_ADD );
		const D3DXVECTOR3 vCURSOL_ADD_POS = { 0.0f, -5.0f, 0.0f };
		m_upCursor->AddPos( vCURSOL_ADD_POS );
	}
	else{
		const D3DXVECTOR3 vCURSOR_SCALE = { fCOLOR_GAGE_SIZE_BASE, fCOLOR_GAGE_SIZE_BASE, 0.0f };
		m_upCursor->SetScale( vCURSOR_SCALE );
		const D3DXVECTOR3 vCURSOR_SCALE_ADD = { 0.875f, 0.975f, 0.0f };
		m_upCursor->AddScale( vCURSOR_SCALE_ADD );
	}
	m_upCursor->Render();

	//----- Text�`�� -----//.
	//RGB.
	int iTextRow = iRGB_INDEX;
	m_wpFont->SetScale( fSCALE_RGB );
	for( unsigned int i=0; i<m_vecupColorGage.size(); i++ ){
		//R, G, B, �̕\�������J��Ԃ�.
		if( iTextRow >= iRGB_INDEX + clsROBO_STATUS::enCOLOR_GAGE_ARMOR_R ){
			iTextRow =  iRGB_INDEX;
		}
	
		m_wpFont->SetPos( vCURSOL_POS_ARRAY[i] );
		m_wpFont->Render( iTextRow ++ );
	}

	//�߂�( RGB�̉��i ).
	m_wpFont->SetPos( vCURSOL_POS_ARRAY[ enSELECT_NUM_BACK ] );
	m_wpFont->Render( iTextRow );



	//----- ��������color1, 2 -----//.
	//���j���[�̃^�C�g��.
	const D3DXVECTOR2 vTITLE_POS_LOCAL = { 130.0f, 20.0f };
	const D3DXVECTOR3 vTITLE_POS = SetPosFromWindow( vTITLE_POS_LOCAL );
	m_wpFont->SetPos( vTITLE_POS );

	const float fTITLE_TEXT_SCALE = 36;
	m_wpFont->SetScale( fTITLE_TEXT_SCALE );

	const int iMENU_TITLE_TEXT_INDEX = 4;
	m_wpFont->Render( iMENU_TITLE_TEXT_INDEX );

	//color1.
	const D3DXVECTOR2 vCOLOR1_POS_LOCAL = { 20.0f, 85.0f };
	const D3DXVECTOR3 vCOLOR1_POS = SetPosFromWindow( vCOLOR1_POS_LOCAL );
	m_wpFont->SetPos( vCOLOR1_POS );

	const float fCOLOR1_TEXT_SCALE = 18;
	m_wpFont->SetScale( fCOLOR1_TEXT_SCALE );

	int iColorTextIndex = 7;
	m_wpFont->Render( iColorTextIndex ++ );

	//color2.
	const D3DXVECTOR3 vCOLOR2_POS_ADD = { 0.0f, 180.0f, 0.0f };
	m_wpFont->AddPos( vCOLOR2_POS_ADD );
	m_wpFont->Render( iColorTextIndex ++ );

}

