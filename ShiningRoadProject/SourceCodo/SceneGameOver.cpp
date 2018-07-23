#include "SceneGameOver.h"

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

}


void clsSCENE_GAME_OVER::UpdateProduct( enSCENE &enNextScene )
{

	if( isPressEnter() ){
		enNextScene = enSCENE::TITLE;
	}
}

void clsSCENE_GAME_OVER::RenderProduct( const D3DXVECTOR3 &vCamPos )
{

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
