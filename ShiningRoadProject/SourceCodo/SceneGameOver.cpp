#include "SceneGameOver.h"
#include "WindowBox.h"

using namespace std;

//const char* sMISSION_FAILED_SPRITE_PATH = "Data\\Image\\GameOverUi\\MissionFailed.png";
//const WHSIZE_FLOAT MISSION_FAILED_SPRITE_SIZE = { 512.0f, 128.0f };
//const D3DXVECTOR3 vMISSION_FAILED_SPRITE_POS = { static_cast<float>( WND_W/2 ), static_cast<float>( WND_H/2 ), 0.0f };


const char* sFONT_TEXT_PATH_GAME_OVER = "Data\\Font\\Text\\TextGameOver.csv";

const char sBLACK_BACK_IMAGE_PATH[] = "Data\\Image\\BlackScreen.png";
const WHSIZE_FLOAT whBLACK_BACK_SIZE = { 1.0f, 1.0f };
const D3DXVECTOR3 vBLACK_BACK_SCALE = { WND_W, WND_H, 1.0f };


//テキスト.
const D3DXVECTOR3 vTEXT_POS_0 = { 400.0f, 300.0f, 0.0f };
const float		fTEXT_SCALE_0 = 24.0f;

const D3DXVECTOR3 vTEXT_POS_1 = { 600.0f, 280.0f, 0.0f };
const float		fTEXT_SCALE_1 = 32.0f;

const D3DXVECTOR3 vTEXT_POS_2 = { 680.0f, 500.0f, 0.0f };
const float		fTEXT_SCALE_2 = 16.0f;

const D3DXVECTOR3 vTEXT_POS_3 = { 920.0f, 500.0f, 0.0f };

const D3DXVECTOR3 vTEXT_POS_4 = { 400.0f, 200.0f, 0.0f };
const float		fTEXT_SCALE_4 = 36.0f;

const D3DXVECTOR4 vTEXT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
const D3DXVECTOR4 vTEXT_COLOR_GAME_OVER = { 1.0f, 0.0625f, 0.0625f, 1.0f };
const float		fTEXT_ALPHA = 1.0f;
const float		fTEXT_ALPHA_GAME_OVER = 0.0f;


//================================//
//========== ゲームオーバークラス ==========//
//================================//
clsSCENE_GAME_OVER::clsSCENE_GAME_OVER( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
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




}


void clsSCENE_GAME_OVER::UpdateProduct( enSCENE &enNextScene )
{
	m_upBox->Update();

	float fff = 1.0f;
//	if( GetAsyncKeyState( 'W' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, fff, 0.0f } );
//	if( GetAsyncKeyState( 'S' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, -fff, 0.0f } );
//	if( GetAsyncKeyState( 'D' ) & 0x8000 ) m_upMissModel->AddPos( { fff, 0.0f, 0.0f } );
//	if( GetAsyncKeyState( 'A' ) & 0x8000 ) m_upMissModel->AddPos( { -fff, 0.0f, 0.0f } );
//	if( GetAsyncKeyState( 'E' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, 0.0f, fff } );
//	if( GetAsyncKeyState( 'Q' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, 0.0f, -fff } );

	if( isPressEnter() ){
		enNextScene = enSCENE::TITLE;
	}
}

void clsSCENE_GAME_OVER::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
}
void clsSCENE_GAME_OVER::RenderUi()
{
	m_upBlackBack->Render();

	m_upBox->Render();

	//mission failed.
	m_wpFont->SetPos( vTEXT_POS_0 );
	m_wpFont->SetScale( fTEXT_SCALE_0 );
	m_wpFont->SetAlpha( fTEXT_ALPHA );
	m_wpFont->SetColor( vTEXT_COLOR );
	m_wpFont->Render( enMESSAGE_INDEX_MISSION_FAILD );

	//continue.
	m_wpFont->SetPos( vTEXT_POS_1 );
	m_wpFont->SetScale( fTEXT_SCALE_1 );
	m_wpFont->Render( enMESSAGE_INDEX_CONTINUE );

	//yes.
	m_wpFont->SetPos( vTEXT_POS_2 );
	m_wpFont->SetScale( fTEXT_SCALE_2 );
	m_wpFont->Render( enMESSAGE_INDEX_YES );

	//no.
	m_wpFont->SetPos( vTEXT_POS_3 );
	m_wpFont->Render( enMESSAGE_INDEX_NO );

	//game over.
	m_wpFont->SetPos( vTEXT_POS_4 );
	m_wpFont->SetScale( fTEXT_SCALE_4 );
	m_wpFont->SetAlpha( fTEXT_ALPHA_GAME_OVER + 1.0f );
	m_wpFont->SetColor( vTEXT_COLOR_GAME_OVER );
	m_wpFont->Render( enMESSAGE_INDEX_GAME_OVER );


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
