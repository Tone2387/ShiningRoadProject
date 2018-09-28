#include "SceneEnding.h"

using namespace std;

const char* sSPRITE_SCROLL_PATH = "Data\\Image\\EndingUi\\SoundWeb.png";
const WHSIZE_FLOAT SPRITE_SCROLL_DISP = { 512.0f, 1024.0f };
const D3DXVECTOR3 vSPRITE_SCROLL_POS_INIT = { static_cast<float>( WND_W / 2 ), static_cast<float>( WND_H ) + SPRITE_SCROLL_DISP.h * 0.5f, 0.0f };

//alphaSprite.
const int iSPRITE_ALPHA_NUM_MAX = 5;
const char* sSPRITE_ALPHA_STAFF_ROLL = "Data\\Image\\EndingUi\\StaffRoll.png"; 
const char* sSPRITE_ALPHA_PROGRAMMER = "Data\\Image\\EndingUi\\Programmer.png"; 
const char* sSPRITE_ALPHA_GRAPHICER = "Data\\Image\\EndingUi\\Graphicer.png"; 
const char* sSPRITE_ALPHA_KYOSTER = "Data\\Image\\EndingUi\\Kyostar.png";
const char* sSPRITE_ALPHA_THANK_YOU = "Data\\Image\\EndingUi\\ThankYouForPlaying.png"; 
const char* sSPRITE_ALPHAS[ iSPRITE_ALPHA_NUM_MAX ] = {
	sSPRITE_ALPHA_STAFF_ROLL, sSPRITE_ALPHA_PROGRAMMER, sSPRITE_ALPHA_GRAPHICER, sSPRITE_ALPHA_KYOSTER, sSPRITE_ALPHA_THANK_YOU
};
const WHSIZE_FLOAT SPRITE_ALPHA_DISP[ iSPRITE_ALPHA_NUM_MAX ] = { 
	512.0f, 128.0f,
	512.0f, 256.0f,
	512.0f, 128.0f,
	512.0f, 128.0f,
	512.0f, 128.0f,
};
const D3DXVECTOR3 vSPRITE_ALPHA_POS_INIT = { static_cast<float>( WND_W / 2 ), static_cast<float>( WND_H / 2 ), 0.0f };



const int iINTERVAL_CNT = 120;
















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
	m_wpSound->PlayBGM( enBGM_MAFIA1 );

	//スクロール.
	SPRITE_STATE ss;
	ss.Disp = SPRITE_SCROLL_DISP;
	m_upSpriteScroll = make_unique< clsSPRITE2D_CENTER >();
	m_upSpriteScroll->Create( m_wpDevice, m_wpContext, sSPRITE_SCROLL_PATH, ss );
	m_upSpriteScroll->SetPos( vSPRITE_SCROLL_POS_INIT );

	//透過.
	m_vupSpriteAlpha.resize( iSPRITE_ALPHA_NUM_MAX );
	for( unsigned int i=0; i<m_vupSpriteAlpha.size(); i++ ){
		ss.Disp = SPRITE_ALPHA_DISP[i];
		m_vupSpriteAlpha[i] = make_unique< clsSPRITE2D_CENTER >();
		m_vupSpriteAlpha[i]->Create( m_wpDevice, m_wpContext, sSPRITE_ALPHAS[i], ss );
		m_vupSpriteAlpha[i]->SetPos( vSPRITE_ALPHA_POS_INIT );
		m_vupSpriteAlpha[i]->SetAlpha( 0.0f );
	}


	m_upStage = make_unique< clsStage >( m_wpPtrGroup );

	m_wpCamera->SetPos( { 0.0f, 500.0f, -1.0f } );
	m_wpCamera->SetLookPos( { 0.0f, 0.0f, 0.0f } );
}

void clsSCENE_ENDING::UpdateProduct( enSCENE &enNextScene )
{
	m_iIntervalCnt ++;

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


	if( m_iIntervalCnt >= iINTERVAL_CNT )
	{
		const float fSCROLL_SPD = -1.0f;
		const int iOFFSET_SCROLL = -1;
		const int iOFFSET_END = -1;
		float fAlpha = 0.25f * 0.25f * 0.25f;

		//終わり.
		if( m_uiSpriteCnt == m_vupSpriteAlpha.size() ){
		}
		//スクロール.
		else if( m_isScroll ){
			m_upSpriteScroll->AddPos( { 0.0f, fSCROLL_SPD, 0.0f } );
			//スクロール終了.
			if( m_upSpriteScroll->GetPos().y < -SPRITE_SCROLL_DISP.h * 0.5f ){
				m_iIntervalCnt = 0;
				m_isScroll = false;
			}
		}
		//透過.
		else{
			if( !m_isSpriteAlphaAppear ){
				fAlpha *= -1.0f;
			}

			//透過値変更.
			if( !m_vupSpriteAlpha[ m_uiSpriteCnt ]->AddAlpha( fAlpha ) ){
				m_iIntervalCnt = 0;
				//消す.
				if( m_isSpriteAlphaAppear ){
					m_isSpriteAlphaAppear = false;
					if( m_uiSpriteCnt == m_vupSpriteAlpha.size() + iOFFSET_END ){ 
						m_isSpriteAlphaAppear = true;
					}
				}
				//次へ.
				else{
					m_isSpriteAlphaAppear = true;
					m_uiSpriteCnt ++;
					//スクロールに行く.
					if( m_uiSpriteCnt == m_vupSpriteAlpha.size() + iOFFSET_SCROLL ){ 
						m_isScroll = true;
					}
				}
			}
		}
	}



	if( isPressEnter() ){
		enNextScene = enSCENE::TITLE;
		m_wpRoboStatus->SaveHeroData();
	}
}

void clsSCENE_ENDING::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	m_upStage->Render( m_mView, m_mProj, m_vLight, vCamPos );
}

void clsSCENE_ENDING::RenderUi()
{
	for( unsigned int i=0; i<m_vupSpriteAlpha.size(); i++  ){
		assert( m_vupSpriteAlpha[i] );
		m_vupSpriteAlpha[i]->Render();
	}

	assert( m_upSpriteScroll );
	m_upSpriteScroll->Render();
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
