#include "SceneEnding.h"
#include "File.h"
using namespace std;


const int iINTERVAL_CNT = 120;


const char* sSTAFF_ROLL_STATUS_DATA_PATH = "Data\\FileData\\EndingStaffRollTransform.csv";
const char* sFONT_TEXT_PATH_ENDING = "Data\\Font\\Text\\TextEnding.csv";

const string sSCROLL_START_NUM_TEXT = "フリー音源　使用サイト";


const int iPOS_X_FILE_DATA_INDEX = 0;
const int iPOS_Y_FILE_DATA_INDEX = 1;
const int iSCALE_FILE_DATA_INDEX = 2;
const int iALPHA_FILE_DATA_INDEX = 3;


//const float fSCROLL_END_POS_Y = 1;


//================================//
//========== エンディングクラス ==========//
//================================//
clsSCENE_ENDING::clsSCENE_ENDING( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_uiSpriteCnt( 0 )
	,m_isSpriteAlphaAppear( true )
	,m_iIntervalCnt( 0 )
	,m_isScroll( false )
{
}

clsSCENE_ENDING::~clsSCENE_ENDING()
{
}

void clsSCENE_ENDING::CreateProduct()
{
	m_wpFont->Create( sFONT_TEXT_PATH_ENDING );

	m_wpSound->PlayBGM( enBGM_MAFIA1 );

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

	int iScrollSize = iTextNum - iAlphaSize;

	//スクロール.
	m_vupTextStateScroll.resize( iScrollSize );
	for( unsigned int i=0; i<m_vupTextStateScroll.size(); i++ ){
		m_vupTextStateScroll[i] = make_unique< TEXT_STATE >();
		m_vupTextStateScroll[i]->vPos.x = File.GetDataFloat( static_cast<int>( i ) + iAlphaSize, iPOS_X_FILE_DATA_INDEX );
		m_vupTextStateScroll[i]->vPos.y = File.GetDataFloat( static_cast<int>( i ) + iAlphaSize, iPOS_Y_FILE_DATA_INDEX );
		m_vupTextStateScroll[i]->fScale = File.GetDataFloat( static_cast<int>( i ) + iAlphaSize, iSCALE_FILE_DATA_INDEX );
		m_vupTextStateScroll[i]->fAlpha = File.GetDataFloat( static_cast<int>( i ) + iAlphaSize, iALPHA_FILE_DATA_INDEX );
	}

	//最後に表示する番号.
	m_iGoScrollIndex = iAlphaSize - 1;

	//透過.
	m_vupTextStateAlpha.resize( iAlphaSize );
	for( unsigned int i=0; i<m_vupTextStateAlpha.size(); i++ ){
		m_vupTextStateAlpha[i] = make_unique< TEXT_STATE >();
		m_vupTextStateAlpha[i]->vPos.x = File.GetDataFloat( static_cast<int>( i ), iPOS_X_FILE_DATA_INDEX );
		m_vupTextStateAlpha[i]->vPos.y = File.GetDataFloat( static_cast<int>( i ), iPOS_Y_FILE_DATA_INDEX );
		m_vupTextStateAlpha[i]->fScale = File.GetDataFloat( static_cast<int>( i ), iSCALE_FILE_DATA_INDEX );
		m_vupTextStateAlpha[i]->fAlpha = File.GetDataFloat( static_cast<int>( i ), iALPHA_FILE_DATA_INDEX );
	}

	File.Close();

	m_upStage = make_unique< clsStage >( m_wpPtrGroup );

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
	const float fACCEL_RATE = 5.0f;
	int iIntervalCnt = 1;
	if( isAccel ){
		iIntervalCnt *= static_cast<int>( fACCEL_RATE );
	}


	m_iIntervalCnt += iIntervalCnt;

	bool isCanGoTitle = false;

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
		if( m_uiSpriteCnt == m_vupTextStateAlpha.size() ){
			isCanGoTitle = true;
		}
		//スクロール.
		else if( m_isScroll ){
			for( unsigned int i=0; i<m_vupTextStateScroll.size(); i++ ){
				m_vupTextStateScroll[i]->vPos.y += fScrollSpd;
			}
			//スクロール終了.
			const unsigned int uiSCROLL_LAST_INDEX = m_vupTextStateScroll.size() - 1;
			const float fSCROLL_END_POS = m_vupTextStateScroll[ uiSCROLL_LAST_INDEX ]->fScale * -2.0f;
			if( m_vupTextStateScroll[ uiSCROLL_LAST_INDEX ]->vPos.y < 
				fSCROLL_END_POS )
			{
				m_iIntervalCnt = 0;
				m_isScroll = false;
			}
		}
		//透過.
		else{
			if( !m_isSpriteAlphaAppear ){
				fAlpha *= -1.0f;
			}

			//透過値変更.AddAlphaState//->fAlpha + fAlpha 
			if( !AddAlphaState( m_vupTextStateAlpha[ m_uiSpriteCnt ].get(), fAlpha ) ){
				m_iIntervalCnt = 0;
				//消す.
				if( m_isSpriteAlphaAppear ){
					m_isSpriteAlphaAppear = false;
					if( m_uiSpriteCnt == m_vupTextStateAlpha.size() + iOFFSET_END ){ 
						m_isSpriteAlphaAppear = true;
					}
				}
				//次へ.
				else{
					m_isSpriteAlphaAppear = true;
					m_uiSpriteCnt ++;
					//スクロールに行く.
					if( m_uiSpriteCnt == m_iGoScrollIndex ){ 
						m_isScroll = true;
					}
				}
			}
		}
	}



	if( ( m_wpXInput->isPressEnter( XINPUT_START ) || GetAsyncKeyState( VK_RETURN ) & 0x1 ) ||
		( ( m_wpXInput->isPressEnter( XINPUT_B ) ||	GetAsyncKeyState( VK_SPACE ) & 0x1 ) && isCanGoTitle ) )
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
	m_upStage->Render( m_mView, m_mProj, m_vLight, vCamPos );
}

void clsSCENE_ENDING::RenderUi()
{
	int iTextIndex = 0;

	for( unsigned int i=0; i<m_vupTextStateAlpha.size(); i++  ){
		assert( m_vupTextStateAlpha[i] );
		m_wpFont->SetPos(	m_vupTextStateAlpha[i]->vPos );
		m_wpFont->SetScale(	m_vupTextStateAlpha[i]->fScale );
		m_wpFont->SetAlpha(	m_vupTextStateAlpha[i]->fAlpha );
		m_wpFont->Render( iTextIndex );
		iTextIndex ++;
	}

	for( unsigned int i=0; i<m_vupTextStateScroll.size(); i++ ){
		assert( m_vupTextStateScroll[i] );
		m_wpFont->SetPos(	m_vupTextStateScroll[i]->vPos );
		m_wpFont->SetScale(	m_vupTextStateScroll[i]->fScale );
		m_wpFont->SetAlpha(	m_vupTextStateScroll[i]->fAlpha );
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
