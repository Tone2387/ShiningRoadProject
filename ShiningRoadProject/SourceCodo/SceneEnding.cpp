#include "SceneEnding.h"
#include "File.h"
using namespace std;

namespace{

//つけていると背景に真ん中がわかるものを出す.
//#define CENTER_SPRITE_RENDER
#ifdef CENTER_SPRITE_RENDER
	unique_ptr<clsSprite2D> g_upTex;
#endif//#ifdef CENTER_SPRITE_RENDER

	const int iINTERVAL_CNT = 120;


	const char* sSTAFF_ROLL_STATUS_DATA_PATH = "Data\\FileData\\Tahara\\EndingStaffRollTransform.csv";
	const char* sFONT_TEXT_PATH_ENDING = "Data\\Font\\Text\\TextEnding.csv";

	const string sSCROLL_START_NUM_TEXT = "ー　フリー音源　使用サイト　ー";

	//.
	const int iPOS_X_FILE_DATA_INDEX = 0;
	const int iPOS_Y_FILE_DATA_INDEX = 1;
	const int iSCALE_FILE_DATA_INDEX = 2;
	const int iALPHA_FILE_DATA_INDEX = 3;


	//黒背景.
	const char sBLACK_BACK_PATH[] = "Data\\Image\\BlackScreen.png";
	const WHSIZE_FLOAT BLACK_BACK_SIZE = { 1.0f, 1.0f };
	const D3DXVECTOR3 vBLACK_BACK_SCALE = { static_cast<float>( WND_W ), static_cast<float>( WND_H ), 0.0f };
	//ロゴ.
	const char sLOGO_PATH[] = "Data\\Image\\TitleUi\\TitleLogo.png";
	const WHSIZE_FLOAT LOGO_SIZE = { 960.0f, 640.0f };
	const D3DXVECTOR3 vLOGO_POS = { static_cast<float>( WND_W ) * 0.5f, static_cast<float>( WND_H ) * 0.5f, 0.0f };
	const D3DXVECTOR3 vLOGO_COLOR = { 1.0f, 1.0f, 1.0f };
	const float fLOGO_ALPHA = 0.375f;


	struct STAFF_TEXT_RENDER_NUM
	{
		unsigned int uiIndex;//何番の時に.
		unsigned int uiNum;	//いくつ同時に描画する?.
	};

	const int iSTAFF_TEXT_RENDER_NUM_MAX = 3;
	const STAFF_TEXT_RENDER_NUM STAFF_ROLL_PROGRAMR = { 1, 3 };
	const STAFF_TEXT_RENDER_NUM STAFF_ROLL_GRAPHICR = { 4, 2 };
	const STAFF_TEXT_RENDER_NUM STAFF_ROLL_SPECIAL =  { 6, 2 };
	const STAFF_TEXT_RENDER_NUM STAFF_TEXT_NUM_ARRAY[ iSTAFF_TEXT_RENDER_NUM_MAX ]=
	{ STAFF_ROLL_PROGRAMR, STAFF_ROLL_GRAPHICR, STAFF_ROLL_SPECIAL };

	//const float fSCROLL_END_POS_Y = 1;
}

//================================//
//========== エンディングクラス ==========//
//================================//
clsSCENE_ENDING::clsSCENE_ENDING( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_uiSpriteCnt( 0 )
	,m_isSpriteAlphaAppear( true )
	,m_iIntervalCnt( 0 )
	,m_isScroll( false )
	,m_isCanGoTitle( false )
	,m_uiRenderTextNum( 0 )
{
#ifdef CENTER_SPRITE_RENDER	
	SPRITE_STATE ss;
	ss.Disp = { WND_W, WND_H };
	g_upTex = make_unique<clsSprite2D>();
	g_upTex->Create( m_wpDevice, m_wpContext, "Data\\Image\\sima.png", ss );
	g_upTex->SetPos( { 0.0f, 0.0f, 0.0f } );
#endif//#ifdef CENTER_SPRITE_RENDER
}

clsSCENE_ENDING::~clsSCENE_ENDING()
{
}

void clsSCENE_ENDING::CreateProduct()
{
	m_wpFont->Create( sFONT_TEXT_PATH_ENDING );

	m_wpSound->PlayBGM( enBGM_NOVA0 );

	clsFILE File;
	File.Open( sSTAFF_ROLL_STATUS_DATA_PATH );

	//行数を調べる.
	int iTextNum = m_wpFont->GetTextRow();
	assert( iTextNum > 0 );

	//スクロール開始番号を調べる.
	int iAlphaSize = -1;
	for( int i=0; i<iTextNum; i++ ){
		if( m_wpFont->GetText( i ) == sSCROLL_START_NUM_TEXT ){
			iAlphaSize = i;
			break;
		}
	}
	assert( iAlphaSize >= 0 );

	int iScrollSize = iTextNum - iAlphaSize;

	//スクロール.
	m_vecupTextStateScroll.resize( iScrollSize );
	for( unsigned int i=0; i<m_vecupTextStateScroll.size(); i++ ){
		m_vecupTextStateScroll[i] = make_unique< TEXT_STATE >();
		m_vecupTextStateScroll[i]->vPos.x = File.GetDataFloat( static_cast<int>( i ) + iAlphaSize, iPOS_X_FILE_DATA_INDEX );
		m_vecupTextStateScroll[i]->vPos.y = File.GetDataFloat( static_cast<int>( i ) + iAlphaSize, iPOS_Y_FILE_DATA_INDEX );
		m_vecupTextStateScroll[i]->fScale = File.GetDataFloat( static_cast<int>( i ) + iAlphaSize, iSCALE_FILE_DATA_INDEX );
		m_vecupTextStateScroll[i]->fAlpha = File.GetDataFloat( static_cast<int>( i ) + iAlphaSize, iALPHA_FILE_DATA_INDEX );
	}

	//最後に表示する番号.
	m_iGoScrollIndex = iAlphaSize - 1;

	//透過.
	m_vecupTextStateAlpha.resize( iAlphaSize );
	for( unsigned int i=0; i<m_vecupTextStateAlpha.size(); i++ ){
		m_vecupTextStateAlpha[i] = make_unique< TEXT_STATE >();
		m_vecupTextStateAlpha[i]->vPos.x = File.GetDataFloat( static_cast<int>( i ), iPOS_X_FILE_DATA_INDEX );
		m_vecupTextStateAlpha[i]->vPos.y = File.GetDataFloat( static_cast<int>( i ), iPOS_Y_FILE_DATA_INDEX );
		m_vecupTextStateAlpha[i]->fScale = File.GetDataFloat( static_cast<int>( i ), iSCALE_FILE_DATA_INDEX );
		m_vecupTextStateAlpha[i]->fAlpha = File.GetDataFloat( static_cast<int>( i ), iALPHA_FILE_DATA_INDEX );
	}

	File.Close();

//	m_upStage = make_unique< clsStage >( m_wpPtrGroup );

	//背景.
	SPRITE_STATE ss;
	ss.Disp = BLACK_BACK_SIZE;
	assert( !m_upBack );
	m_upBack = make_unique< clsSprite2D >();
	m_upBack->Create( m_wpDevice, m_wpContext, sBLACK_BACK_PATH, ss );
	m_upBack->SetPos( { 0.0f, 0.0f, 0.0f } );
	m_upBack->SetScale( vBLACK_BACK_SCALE );


	//ロゴ.
	ss.Disp = LOGO_SIZE;
	assert( !m_upLogo );
	m_upLogo = make_unique< clsSPRITE2D_CENTER >();	
	m_upLogo->Create( m_wpDevice, m_wpContext, sLOGO_PATH, ss );
	m_upLogo->SetPos( vLOGO_POS );
	m_upLogo->SetColor( vLOGO_COLOR );
	m_upLogo->SetAlpha( fLOGO_ALPHA );




	m_wpCamera->SetPos( { 0.0f, 500.0f, -1.0f } );
	m_wpCamera->SetLookPos( { 0.0f, 0.0f, 0.0f } );
}

void clsSCENE_ENDING::UpdateProduct( enSCENE &enNextScene )
{
	//ボタンを押していると加速させる.
	bool isAccel = false;
	if( m_wpXInput->isPressStay( XINPUT_B ) ||
		GetAsyncKeyState( VK_SPACE ) & 0x8000 )
	{
		isAccel = true;
	}

	const float fACCEL_RATE = 50.0f;
	int iIntervalCnt = 1;
	if( isAccel ){
		iIntervalCnt *= static_cast<int>( fACCEL_RATE );
	}

	m_iIntervalCnt += iIntervalCnt;


	if( m_iIntervalCnt >= iINTERVAL_CNT ){
		const int iOFFSET_SCROLL = -1;
		const int iOFFSET_END = -1;
		float fScrollSpd = -1.0f;
		float fAlpha = 0.25f * 0.25f * 0.25f;

		//ボタン押したら加速.
		if( isAccel ){
			fScrollSpd *= fACCEL_RATE;
			fAlpha *= fACCEL_RATE;
		}

		//終わり.
		if( m_uiSpriteCnt == m_vecupTextStateAlpha.size() ){
		}
		//スクロール.
		else if( m_isScroll ){
			for( unsigned int i=0; i<m_vecupTextStateScroll.size(); i++ ){
				m_vecupTextStateScroll[i]->vPos.y += fScrollSpd;
			}
			//スクロール終了.
			const unsigned int uiSCROLL_LAST_INDEX = m_vecupTextStateScroll.size() - 1;
			const float fSCROLL_END_POS = m_vecupTextStateScroll[ uiSCROLL_LAST_INDEX ]->fScale * -2.0f;
			if( m_vecupTextStateScroll[ uiSCROLL_LAST_INDEX ]->vPos.y < 
				fSCROLL_END_POS )
			{
				m_iIntervalCnt = 0;
				m_isScroll = false;
			}
		}
		//透過.
		else{
			//消すときは反転.
			if( !m_isSpriteAlphaAppear ){
				fAlpha *= -1.0f;
			}

			//スタッフロールの同時表示数セット.
			const unsigned int uiRENDER_TEXT_NUM_DEFFULT = 1;
			m_uiRenderTextNum = uiRENDER_TEXT_NUM_DEFFULT;
			//複数同時表示の為.
			for( int i=0; i<iSTAFF_TEXT_RENDER_NUM_MAX; i++ ){
				if( m_uiSpriteCnt == STAFF_TEXT_NUM_ARRAY[i].uiIndex ){
					m_uiRenderTextNum = STAFF_TEXT_NUM_ARRAY[i].uiNum;
					break;
				}
			}

			bool bAddAlphaReturn;
			for( unsigned int i=0; i<m_uiRenderTextNum; i++ ){
				bAddAlphaReturn = AddAlphaState( m_vecupTextStateAlpha[ m_uiSpriteCnt + i ].get(), fAlpha );
			}

			//透過値変更.AddAlphaState//->fAlpha + fAlpha 
			if( !bAddAlphaReturn ){
				m_iIntervalCnt = 0;
				//出し終わったので次は消す.
				if( m_isSpriteAlphaAppear ){
					m_isSpriteAlphaAppear = false;
					//終われるようにする( サンキューの描画完了 ).
					if( m_uiSpriteCnt == m_vecupTextStateAlpha.size() + iOFFSET_END ){ 
						m_isSpriteAlphaAppear = true;
						m_isCanGoTitle = true;
					}
				}
				//消し終わったので次は出す.
				else{
					m_isSpriteAlphaAppear = true;
					m_uiSpriteCnt += m_uiRenderTextNum;
					//スクロールに行く.
					if( m_uiSpriteCnt == m_iGoScrollIndex ){ 
						m_isScroll = true;
						m_iIntervalCnt = iINTERVAL_CNT;//消えてすぐにスクロールして欲しいから.
					}
				}
			}
		}
	}



	if( ( m_wpXInput->isPressEnter( XINPUT_START ) || GetAsyncKeyState( VK_RETURN ) & 0x1 ) ||
		( ( m_wpXInput->isPressEnter( XINPUT_B ) ||	GetAsyncKeyState( VK_SPACE ) & 0x1 ) && m_isCanGoTitle ) )
	{
		enNextScene = enSCENE::TITLE;
		m_wpRoboStatus->SaveHeroData();
	}


#ifdef _DEBUG
	float fff = 50.0f;
	if( isPressRight() ){
		m_wpCamera->AddPos( { fff, 0.0f, 0.0f } );
	}
	if( isPressLeft() ){
		m_wpCamera->AddPos( { -fff, 0.0f, 0.0f } );
	}
	if( isPressUp() ){
		m_wpCamera->AddPos( { 0.0f, 0.0f, fff } );
	}
	if( isPressDown() ){
		m_wpCamera->AddPos( { 0.0f, 0.0f, -fff } );
	}
//	if( isPressEnter() ){
//		m_wpCamera->AddPos( { 0.0f, fff, -0.0f } );
//	}
	if( isPressExit() ){
		m_wpCamera->AddPos( { 0.0f, -fff, -0.0f } );
	}
#endif//#indef _DEBUG

}

void clsSCENE_ENDING::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
//	m_upStage->Render( m_mView, m_mProj, m_vLight, vCamPos );
}

void clsSCENE_ENDING::RenderUi()
{
	assert( m_upBack );
	m_upBack->Render();

	assert( m_upLogo );
	m_upLogo->Render();

#ifdef CENTER_SPRITE_RENDER
	g_upTex->Render();
#endif//#ifdef CENTER_SPRITE_RENDER

	int iTextIndex = 0;

	for( unsigned int i=0; i<m_vecupTextStateAlpha.size(); i++  ){
		assert( m_vecupTextStateAlpha[i] );
		m_wpFont->SetPos(	m_vecupTextStateAlpha[i]->vPos );
		m_wpFont->SetScale(	m_vecupTextStateAlpha[i]->fScale );
		m_wpFont->SetAlpha(	m_vecupTextStateAlpha[i]->fAlpha );
		m_wpFont->Render( iTextIndex );
		iTextIndex ++;
	}

	for( unsigned int i=0; i<m_vecupTextStateScroll.size(); i++ ){
		assert( m_vecupTextStateScroll[i] );
		m_wpFont->SetPos(	m_vecupTextStateScroll[i]->vPos );
		m_wpFont->SetScale(	m_vecupTextStateScroll[i]->fScale );
		m_wpFont->SetAlpha(	m_vecupTextStateScroll[i]->fAlpha );
		m_wpFont->Render( iTextIndex );
		iTextIndex ++;
	}
}

bool clsSCENE_ENDING::AddAlphaState( TEXT_STATE* const pTextState, const float fAlpha )
{
	pTextState->fAlpha += fAlpha;

	if( pTextState->fAlpha > 1.0f ){
		pTextState->fAlpha = 1.0f;
	}
	else if( pTextState->fAlpha < 0.0f ){
		pTextState->fAlpha = 0.0f;
	}
	else{
		return true;
	}
	return false;
}

//============================ デバッグテキスト ===========================//
#if _DEBUG
void clsSCENE_ENDING::RenderDebugText()
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


	//dbgtxty += 10;
	//if( m_pBgm[0]->IsStopped() ){
	//	sprintf_s( strDbgTxt, "Stopped" );
	//	m_pText->Render( strDbgTxt, 0, dbgtxty );
	//}
	//if( m_pBgm[0]->IsPlaying() ){
	//	sprintf_s( strDbgTxt, "Playingn" );
	//	m_pText->Render( strDbgTxt, 0, dbgtxty );
	//}
}
#endif //#if _DEBUG
