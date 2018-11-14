#include "SceneGameOver.h"
#include "MenuWindowGameOverContinue.h"
#include "File.h"
#include "SceneGameOverInformation.h"

using namespace std;

namespace{

	//const char* sMISSION_FAILED_SPRITE_PATH = "Data\\Image\\GameOverUi\\MissionFailed.png";
	//const WHSIZE_FLOAT MISSION_FAILED_SPRITE_SIZE = { 512.0f, 128.0f };
	//const D3DXVECTOR3 vMISSION_FAILED_SPRITE_POS = { static_cast<float>( WND_W/2 ), static_cast<float>( WND_H/2 ), 0.0f };


	const char* sFONT_TEXT_PATH_GAME_OVER = "Data\\Font\\Text\\TextGameOver.csv";

	const char sBLACK_BACK_IMAGE_PATH[] = "Data\\Image\\BlackScreen.png";
	const WHSIZE_FLOAT whBLACK_BACK_SIZE = { 1.0f, 1.0f };
	const D3DXVECTOR3 vBLACK_BACK_SCALE = { WND_W, WND_H, 1.0f };

	//選択.
	const char sSELECT_SPRITE_PATH[] = "Data\\Image\\AssembleUi\\SelectStatus.png";
	const D3DXVECTOR3 vSELECT_SPRITE_SCALE_YES = { 95.0f, 42.0f, 0.0f };
	const D3DXVECTOR3 vSELECT_SPRITE_SCALE_NO  = { 65.0f, 42.0f, 0.0f };
	const float fSELECT_SPRITE_ALPHA = 0.75f;
	const D3DXVECTOR3 vSELECT_SPRITE_POS_YES = { 510.0f, 450.0f, 0.0f };
	const D3DXVECTOR3 vSELECT_SPRITE_POS_NO  = { 720.0f, 450.0f, 0.0f };


	//テキスト.
	const D3DXVECTOR4 vTEXT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	const D3DXVECTOR4 vTEXT_COLOR_GAME_OVER = { 1.0f, 0.0625f, 0.0625f, 1.0f };

}

//================================//
//========== ゲームオーバークラス ==========//
//================================//
clsSCENE_GAME_OVER::clsSCENE_GAME_OVER( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_enTextRenderIndex( enMESSAGE_INDEX_MISSION_FAILD )
{
}

clsSCENE_GAME_OVER::~clsSCENE_GAME_OVER()
{
}

void clsSCENE_GAME_OVER::CreateProduct()
{
	m_wpSound->PlayBGM( enBGM_AMACHA0 );
	
	m_wpFont->Create( sFONT_TEXT_PATH_GAME_OVER );


	m_wpCamera->SetLookPos( { 0.0f, 0.0f, 10.0f } );

//	assert( !m_upBox );
//	m_upBox = make_unique< clsWINDOW_BOX >( m_wpDevice, m_wpContext );

	assert( !m_upBlackBack );
	SPRITE_STATE ss;
	ss.Disp = whBLACK_BACK_SIZE;
	m_upBlackBack = make_unique< clsSprite2D >();
	m_upBlackBack->Create( m_wpDevice, m_wpContext, sBLACK_BACK_IMAGE_PATH, ss );
	m_upBlackBack->SetPos( { 0.0f, 0.0f, 0.0f } );
	m_upBlackBack->SetScale( vBLACK_BACK_SCALE );


	//文字の位置などをファイルから読み込む.
	const char* sFILE_PATH = "Data\\FileData\\Tahara\\GameOverTextTransform.csv";
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	upFile->Open( sFILE_PATH );
	for( int i=0; i<enMESSAGE_INDEX_size; i++ ){
		int iCol = 0;
		m_TextStateArray[i].vPos.x = upFile->GetDataFloat( i, iCol++ );
		m_TextStateArray[i].vPos.y = upFile->GetDataFloat( i, iCol++ );
		m_TextStateArray[i].fScale = upFile->GetDataFloat( i, iCol++ );
	}
	upFile->Close();

	//照合用情報の作成の為のファイルデータ取得.
	const char sTITLE_INFORMATION_DATA_PATH[] = "Data\\FileData\\Tahara\\GameOverMenuInformation.csv";
	upFile->Open( sTITLE_INFORMATION_DATA_PATH );
	//照合用情報の作成.
	m_vecuiInformationDataArray.resize( enINFORMATION_INDEX_size, 0 );
	for( char i=0; i<enINFORMATION_INDEX_size; i++ ){
		const int iCOL = 0;
		assert( static_cast<unsigned int>( i ) < upFile->GetSizeRow() );
		m_vecuiInformationDataArray[i] = static_cast<unsigned int>( upFile->GetDataInt( i, iCOL ) );
	}
	upFile.reset();


	//ゆっくり暗明転.
	const float fDARK_SPD = fBLACK_SCREEN_DEFAULT_SPD * 0.5f;
	m_wpBlackScreen->SetChangeSpd( fDARK_SPD );

}


void clsSCENE_GAME_OVER::UpdateProduct( enSCENE &enNextScene )
{
	if( m_upMenu ){
		MenuUpdate( enNextScene );
	}


//	float fff = 1.0f;
//	if( GetAsyncKeyState( 'W' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, fff, 0.0f } );
//	if( GetAsyncKeyState( 'S' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, -fff, 0.0f } );
//	if( GetAsyncKeyState( 'D' ) & 0x8000 ) m_upMissModel->AddPos( { fff, 0.0f, 0.0f } );
//	if( GetAsyncKeyState( 'A' ) & 0x8000 ) m_upMissModel->AddPos( { -fff, 0.0f, 0.0f } );
//	if( GetAsyncKeyState( 'E' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, 0.0f, fff } );
//	if( GetAsyncKeyState( 'Q' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, 0.0f, -fff } );

	if( !m_wpBlackScreen->GetAlpha() ){
		//箱の開き始め.
		if(!m_upMenu ){
//			m_wpSound->PlaySE( enSE_WIN_APP );
			m_upMenu = make_unique<clsMENU_WINDOW_GAME_OVER_CONTINUE>( m_wpPtrGroup, nullptr, &m_vecuiInformationDataArray );
		}
	}

//	//開ききるまで動かせない.
//	if( !m_isBoxOpened ){
//		return;
//	}
//
//	//コントローラ.
//	if( isPressLeft() ){
//		if(!m_isYes ){
//			m_isYes = true;
//			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
//		}
//	}
//	if( isPressRight() ){
//		if( m_isYes ){
//			m_isYes = false;
//			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
//		}
//	}

//	if( isPressEnter() ){
//		//リトライ.
//		if( m_isYes ){
//			m_wpSound->PlaySE( enSE_ENTER );
//			m_wpSound->PlaySE( enSE_MISSION_START );
//			enNextScene = enSCENE::MISSION;
//		}
//		//あきらめる.
//		else{
//			m_wpSound->PlaySE( enSE_EXIT );
//			m_wpSound->PlaySE( enSE_WIN_DISAPP );
//			//箱を閉じる.
//			const D3DXVECTOR2 vCHANGE_SPD = { 60.0f, 30.0f };
//			m_upBox->SetSizeTarget( { 0.0f, 0.0f, 0.0f } );
//			m_upBox->AddChangeData( 
//				vCHANGE_SPD.x, vCHANGE_SPD.y, 
//				clsWINDOW_BOX::encBEFOR_CHANGE::BOTH );
//			//文字を消す & 赤い文字を出す.
//			float fTmpAlpha = m_fTextAlphaWhite;
//			m_fTextAlphaWhite = m_fTextAlphaRed;
//			m_fTextAlphaRed = fTmpAlpha;
//			//シーン飛ぶ.
//			enNextScene = enSCENE::TITLE;
//		}
//	}
}

//メニューの動き.
void clsSCENE_GAME_OVER::MenuUpdate( enSCENE &enNextScene )
{
	m_upMenu->Update();
	//メニューが何か返してくる.
	unsigned int uiReceiveInformation = m_upMenu->GetInformation();
	if( uiReceiveInformation )
	{
		char cInformationIndex = -1;
		for( char i=0; i<enINFORMATION_INDEX_size; i++ ){
			//有用な情報と合致したなら.
			if( uiReceiveInformation == m_vecuiInformationDataArray[i] ){
				m_upMenu->Close();
				cInformationIndex = i;
			}
		}
		switch( cInformationIndex )
		{
		case enINFORMATION_INDEX_GAME_OVER:
			enNextScene = enSCENE::TITLE;
			m_enTextRenderIndex = enMESSAGE_INDEX_GAME_OVER;
			break;

		case enINFORMATION_INDEX_CONTINUE:
			enNextScene = enSCENE::MISSION;
			m_enTextRenderIndex = enMESSAGE_INDEX_NEVER_GIVE_UP;
			m_wpSound->PlaySE( enSE_MISSION_START );
			break;

		case enINFORMATION_INDEX_ASSEMBLE:
			enNextScene = enSCENE::ASSEMBLE;
			m_enTextRenderIndex = enMESSAGE_INDEX_NEVER_GIVE_UP;
			m_wpSound->PlaySE( enSE_MISSION_START );
			break;

		default:
			assert( !"不正な情報が返されました" );
			break;
		}
	}


	//( 見た目が )消えたら( メモリからも )消える.
	if( m_upMenu->isDeletePermission() ){
		m_upMenu.reset( nullptr );
	}

}





void clsSCENE_GAME_OVER::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
}
void clsSCENE_GAME_OVER::RenderUi()
{
	m_upBlackBack->Render();



//	for( char i=0; i<enMESSAGE_INDEX_size; i++ ){
//		//日本語描画.
//		m_wpFont->SetPos( m_TextStateArray[i].vPos );
//		m_wpFont->SetScale( m_TextStateArray[i].fScale );
//
//		if( i == enMESSAGE_INDEX_MISSION_FAILD ){
//			m_wpFont->SetColor( vTEXT_COLOR );
//			m_wpFont->SetAlpha( m_fTextAlphaWhite );
//		}
//		else if( i == enMESSAGE_INDEX_GAME_OVER ){
//			m_wpFont->SetColor( vTEXT_COLOR_GAME_OVER );
//			m_wpFont->SetAlpha( m_fTextAlphaRed );
//		}
//
//		m_wpFont->Render( i );
//	}

		if( m_enTextRenderIndex == enMESSAGE_INDEX_MISSION_FAILD ){
			m_wpFont->SetColor( vTEXT_COLOR );
		}
		else if( m_enTextRenderIndex == enMESSAGE_INDEX_GAME_OVER ){
			m_wpFont->SetColor( vTEXT_COLOR_GAME_OVER );
		}
		else if( m_enTextRenderIndex == enMESSAGE_INDEX_NEVER_GIVE_UP ){
			m_wpFont->SetColor( vTEXT_COLOR );
		}

		//日本語描画.
		m_wpFont->SetPos( m_TextStateArray[ m_enTextRenderIndex ].vPos );
		m_wpFont->SetScale( m_TextStateArray[ m_enTextRenderIndex ].fScale );
		m_wpFont->SetAlpha( 1.0f );
		m_wpFont->Render( m_enTextRenderIndex );


	if( m_upMenu ){
		m_upMenu->Render();
	}

}


//============================ デバッグテキスト ===========================//
#if _DEBUG
void clsSCENE_GAME_OVER::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.

	sprintf_s( strDbgTxt, 
		"CameraPos : x[%f], y[%f], z[%f]",
		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamLokPos : x[%f], y[%f], z[%f]",
		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

//	sprintf_s( strDbgTxt, 
//		"MisModelPos : x[%f], y[%f], z[%f]",
//		m_upMissModel->GetPos().x, m_upMissModel->GetPos().y, m_upMissModel->GetPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"MisModelBoneNum : L[%f], C[%f], H[%f], A[%f]",
//		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_LEG ), 
//		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_CORE ), 
//		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_HEAD ), 
//		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_ARMS ) );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );


}
#endif //#if _DEBUG
