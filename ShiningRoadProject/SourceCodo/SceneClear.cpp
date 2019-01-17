#include "SceneClear.h"



clsSCENE_CLEAR::clsSCENE_CLEAR( clsPOINTER_GROUP* const ptrGroup )
	:clsSCENE_BASE( ptrGroup )
{
}

clsSCENE_CLEAR::~clsSCENE_CLEAR()
{
}


void clsSCENE_CLEAR::CreateProduct()
{
	

}



void clsSCENE_CLEAR::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
}

void clsSCENE_CLEAR::RenderUi()
{

}




//============================ デバッグテキスト ===========================//
#ifdef _DEBUG
void clsSCENE_CLEAR::RenderDebugText()
{
	//NULLチェック.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.

	sprintf_s( strDbgTxt, 
		"CameraPos : x[%f], y[%f], z[%f]",
		m_wpCamera->GetPos().x, m_wpCamera->GetPos().y, m_wpCamera->GetPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamLokPos : x[%f], y[%f], z[%f]",
		m_wpCamera->GetLookPos().x, m_wpCamera->GetLookPos().y, m_wpCamera->GetLookPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	iTxtY += iOFFSET;

//	sprintf_s( strDbgTxt, 
//		"PlayerPos : x[%f], y[%f], z[%f]",
//		m_upPlayer->GetPosition().x, m_upPlayer->GetPosition().y, m_upPlayer->GetPosition().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"EnemyPos : x[%f], y[%f], z[%f]",
//		m_upEnemy->GetPosition().x, m_upEnemy->GetPosition().y, m_upEnemy->GetPosition().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	iTxtY += iOFFSET;
//
//	sprintf_s( strDbgTxt, 
//		"Frame : [%f]",
//		m_fMovieFrame );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//
//	iTxtY += iOFFSET;
//
//	switch( m_iCut )
//	{
//	case clsSCENE_TAKEOFF::enCUT_START:
//		sprintf_s( strDbgTxt,"Cut : enCUT_START" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_RED_1:
//		sprintf_s( strDbgTxt,"Cut : enCUT_RED_1" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_RED_2:
//		sprintf_s( strDbgTxt,"Cut : enCUT_RED_2" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_RED_3:
//		sprintf_s( strDbgTxt,"Cut : enCUT_RED_3" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_LIA_OPEN:
//		sprintf_s( strDbgTxt,"Cut : enCUT_LIA_OPEN" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_ENEMY_APP:
//		sprintf_s( strDbgTxt,"Cut : enCUT_ENEMY_APP" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_PLAYER_UP:
//		sprintf_s( strDbgTxt,"Cut : enCUT_PLAYER_UP" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_PLAYER_ROAD:
//		sprintf_s( strDbgTxt,"Cut : enCUT_PLAYER_ROAD" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_PLAYER_APP:
//		sprintf_s( strDbgTxt,"Cut : enCUT_PLAYER_APP" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_ENCOUNT:
//		sprintf_s( strDbgTxt,"Cut : enCUT_ENCOUNT" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_ENEMY_LANDING:
//		sprintf_s( strDbgTxt,"Cut : enCUT_ENEMY_LANDING" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_VS:
//		sprintf_s( strDbgTxt,"Cut : enCUT_VS" );
//		break;
//	case clsSCENE_TAKEOFF::enCUT_END:
//		sprintf_s( strDbgTxt,"Cut : enCUT_END" );
//		break;
//	}
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

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
#endif //#ifdef _DEBUG
