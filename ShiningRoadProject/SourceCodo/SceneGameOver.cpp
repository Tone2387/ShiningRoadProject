#include "SceneGameOver.h"

using namespace std;


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
	m_wpCamera->SetLookPos( { 0.0f, 0.0f, 10.0f } );

	assert( !m_upMissModel );
	m_upMissModel = make_unique< clsMISSION_MODEL >();
	m_upMissModel->Create( m_wpResource, m_wpRoboStatus );
}


void clsSCENE_GAME_OVER::UpdateProduct( enSCENE &enNextScene )
{
	float fff = 1.0f;
	if( GetAsyncKeyState( 'W' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, fff, 0.0f } );
	if( GetAsyncKeyState( 'S' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, -fff, 0.0f } );
	if( GetAsyncKeyState( 'D' ) & 0x8000 ) m_upMissModel->AddPos( { fff, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'A' ) & 0x8000 ) m_upMissModel->AddPos( { -fff, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'E' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, 0.0f, fff } );
	if( GetAsyncKeyState( 'Q' ) & 0x8000 ) m_upMissModel->AddPos( { 0.0f, 0.0f, -fff } );

	if( isPressEnter() ){
		enNextScene = enSCENE::TITLE;
	}
}

void clsSCENE_GAME_OVER::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	assert( m_upMissModel );
	m_upMissModel->Render( m_mView, m_mProj, m_vLight, vCamPos );
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
