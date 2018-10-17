#include "SceneGameOver.h"
#include "WindowBox.h"
#include "File.h"

using namespace std;

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


//================================//
//========== ゲームオーバークラス ==========//
//================================//
clsSCENE_GAME_OVER::clsSCENE_GAME_OVER( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_fTextAlphaWhite( 1.0f )
	,m_fTextAlphaRed( 0.0f )
	,m_bCanBoxOpen( false )
	,m_isBoxOpened( false )
	,m_isYes( true )
{
}

clsSCENE_GAME_OVER::~clsSCENE_GAME_OVER()
{
}

void clsSCENE_GAME_OVER::CreateProduct()
{
//	m_wpSound->PlayBGM( enBGM_MUS0 );
	
	m_wpFont->Create( sFONT_TEXT_PATH_GAME_OVER );

	m_wpCamera->SetLookPos( { 0.0f, 0.0f, 10.0f } );

	assert( !m_upBox );
	m_upBox = make_unique< clsWINDOW_BOX >( m_wpDevice, m_wpContext );

	assert( !m_upBlackBack );
	SPRITE_STATE ss;
	ss.Disp = whBLACK_BACK_SIZE;
	m_upBlackBack = make_unique< clsSprite2D >();
	m_upBlackBack->Create( m_wpDevice, m_wpContext, sBLACK_BACK_IMAGE_PATH, ss );
	m_upBlackBack->SetPos( { 0.0f, 0.0f, 0.0f } );
	m_upBlackBack->SetScale( vBLACK_BACK_SCALE );

	//選択肢を示す.
	m_upSelect = make_unique< clsSprite2D >();
	m_upSelect->Create( m_wpDevice, m_wpContext, sSELECT_SPRITE_PATH, ss  );
	m_upSelect->SetPos( vSELECT_SPRITE_POS_YES );
	m_upSelect->SetScale( vSELECT_SPRITE_SCALE_YES );
	m_upSelect->SetAlpha( fSELECT_SPRITE_ALPHA );

	//文字の位置などをファイルから読み込む.
	const char* sFILE_PATH = "Data\\FileData\\GameOverTextTransform.csv";
	clsFILE File;
	File.Open( sFILE_PATH );
	for( int i=0; i<enMESSAGE_INDEX_size; i++ ){
		int iCol = 0;
		m_TextStateArray[i].vPos.x = File.GetDataFloat( i, iCol++ );
		m_TextStateArray[i].vPos.y = File.GetDataFloat( i, iCol++ );
		m_TextStateArray[i].fScale = File.GetDataFloat( i, iCol++ );
	}


	//ゆっくり暗明転.
	const float fDARK_SPD = fBLACK_SCREEN_DEFAULT_SPD * 0.5f;
	m_wpBlackScreen->SetChangeSpd( fDARK_SPD );

}


void clsSCENE_GAME_OVER::UpdateProduct( enSCENE &enNextScene )
{
	m_upBox->Update();

//	float fff = 1.0f;
//	if( GetAsyncKeyState( 'W' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, fff, 0.0f } );
//	if( GetAsyncKeyState( 'S' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, -fff, 0.0f } );
//	if( GetAsyncKeyState( 'D' ) & 0x8000 ) m_upMissModel->AddPos( { fff, 0.0f, 0.0f } );
//	if( GetAsyncKeyState( 'A' ) & 0x8000 ) m_upMissModel->AddPos( { -fff, 0.0f, 0.0f } );
//	if( GetAsyncKeyState( 'E' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, 0.0f, fff } );
//	if( GetAsyncKeyState( 'Q' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, 0.0f, -fff } );

	if( !m_wpBlackScreen->GetAlpha() ){
		//箱の開き始め.
		if(!m_bCanBoxOpen ){
			m_bCanBoxOpen = true;
			m_wpSound->PlaySE( enSE_WIN_APP );
			//箱を開く.
			const D3DXVECTOR3 vTARGET_SIZE = { 600.0f, 400.0f, 1.0f };
			const D3DXVECTOR2 vCHANGE_SPD = { 60.0f, 30.0f };
			m_upBox->SetSizeTarget( vTARGET_SIZE );
			m_upBox->AddChangeData( 
				vCHANGE_SPD.x, vCHANGE_SPD.y, 
				clsWINDOW_BOX::encBEFOR_CHANGE::WIDTH );
		}
		//開き始めた後.
		else if( m_upBox->isStopChange() ){
			m_isBoxOpened = true;
		}
	}

	//開ききるまで動かせない.
	if( !m_isBoxOpened ){
		return;
	}

	//コントローラ.
	if( isPressLeft() ){
		if(!m_isYes ){
			m_isYes = true;
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
			m_upSelect->SetPos( vSELECT_SPRITE_POS_YES );
			m_upSelect->SetScale( vSELECT_SPRITE_SCALE_YES );
		}
	}
	if( isPressRight() ){
		if( m_isYes ){
			m_isYes = false;
			m_wpSound->PlaySE( enSE_CURSOL_MOVE );
			m_upSelect->SetPos( vSELECT_SPRITE_POS_NO );
			m_upSelect->SetScale( vSELECT_SPRITE_SCALE_NO );
		}
	}

	if( isPressEnter() ){
		//リトライ.
		if( m_isYes ){
			m_wpSound->PlaySE( enSE_ENTER );
			m_wpSound->PlaySE( enSE_MISSION_START );
			enNextScene = enSCENE::MISSION;
		}
		//あきらめる.
		else{
			m_wpSound->PlaySE( enSE_EXIT );
			m_wpSound->PlaySE( enSE_WIN_DISAPP );
			//箱を閉じる.
			const D3DXVECTOR2 vCHANGE_SPD = { 60.0f, 30.0f };
			m_upBox->SetSizeTarget( { 0.0f, 0.0f, 0.0f } );
			m_upBox->AddChangeData( 
				vCHANGE_SPD.x, vCHANGE_SPD.y, 
				clsWINDOW_BOX::encBEFOR_CHANGE::BOTH );
			//文字を消す & 赤い文字を出す.
			float fTmpAlpha = m_fTextAlphaWhite;
			m_fTextAlphaWhite = m_fTextAlphaRed;
			m_fTextAlphaRed = fTmpAlpha;
			//選択を消す.
			m_upSelect->SetAlpha( 0.0f );
			//シーン飛ぶ.
			enNextScene = enSCENE::TITLE;
		}
	}
}

void clsSCENE_GAME_OVER::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
}
void clsSCENE_GAME_OVER::RenderUi()
{
	m_upBlackBack->Render();


	m_wpFont->SetColor( vTEXT_COLOR );
	m_wpFont->SetAlpha( m_fTextAlphaWhite );

	for( char i=0; i<enMESSAGE_INDEX_size; i++ ){
		//この範囲は映さないこともある.
		if( enMESSAGE_INDEX_CONTINUE <= i && i <= enMESSAGE_INDEX_NO ){
			//まだBOXが開ききっていないなら次.
			if( !m_isBoxOpened ){
				continue;
			}
		}

		//色は文字の後ろ.
		if( enMESSAGE_INDEX_CONTINUE == i ){
			m_upSelect->Render();
		}

		//日本語描画.
		m_wpFont->SetPos( m_TextStateArray[i].vPos );
		m_wpFont->SetScale( m_TextStateArray[i].fScale );
		if( i == enMESSAGE_INDEX_GAME_OVER ){
			m_wpFont->SetColor( vTEXT_COLOR_GAME_OVER );
			m_wpFont->SetAlpha( m_fTextAlphaRed );
		}
		m_wpFont->Render( i );

		//一つ目の上に箱を置く.
		if( i == enMESSAGE_INDEX_MISSION_FAILD ){
			//箱.
			m_upBox->Render();
		}
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

	sprintf_s( strDbgTxt, 
		"MisModelPos : x[%f], y[%f], z[%f]",
		m_upMissModel->GetPos().x, m_upMissModel->GetPos().y, m_upMissModel->GetPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"MisModelBoneNum : L[%f], C[%f], H[%f], A[%f]",
		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_LEG ), 
		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_CORE ), 
		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_HEAD ), 
		(float)m_upMissModel->GetColNum( clsMISSION_MODEL::enCOL_PARTS_ARMS ) );
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
