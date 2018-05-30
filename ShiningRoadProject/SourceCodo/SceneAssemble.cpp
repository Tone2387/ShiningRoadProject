#include "SceneAssemble.h"

#define TEST_TEX_PASS "Data\\Load\\LoadBack.png"


//================================//
//========== 組み換えクラス ==========//
//================================//
clsSCENE_ASSEMBLE::clsSCENE_ASSEMBLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup ),
	m_pAsmModel( nullptr )
{
}

clsSCENE_ASSEMBLE::~clsSCENE_ASSEMBLE()
{
	SAFE_DELETE( m_pAsmModel );
}

void clsSCENE_ASSEMBLE::CreateProduct()
{

//	m_pTestChara = new clsCharaStatic;
//	m_pTestChara->AttachModel( 
//		m_wpResource->GetStaticModels( 
//		clsResource::enSTATIC_MODEL::enStaticModel_Ground ) );
//	m_pTestChara->Init();
//	m_pTestChara->SetPosition( D3DXVECTOR3( 1.0f, 0.0f, 50.0f ) );
//
//	m_pParts = new clsCharaStatic;
//	m_pParts->AttachModel( 
//		m_wpResource->GetStaticModels( 
//		clsResource::enSTATIC_MODEL::enStaticModel_Enemy ) );
//	m_pParts->Init();
//	m_pParts->SetPosition( D3DXVECTOR3( -2.0f, 1.0f, 0.0f ) );
//
//
//	m_pSprite = new clsSPRITE2D_CENTER;
//	SPRITE_STATE tmpSs;
//	m_pSprite->Create( 
//		m_wpDevice, m_wpContext, 
//		TEST_TEX_PASS, tmpSs );
//	m_pSprite->SetPos( { WND_W*0.5f, WND_H*0.5f, 0.0f } );

	m_pAsmModel = new clsASSEMBLE_MODEL;
	m_pAsmModel->Create();
}

void clsSCENE_ASSEMBLE::UpdateProduct( enSCENE &nextScene )
{
	if( GetAsyncKeyState( VK_SPACE ) & 0x1 ){
//		m_pAsmModel->AttachModel( enPARTS::LEG,
//			m_wpResource->GetSkinModels( 
//				static_cast<clsResource::enSKIN_MODEL>( clsResource::enLegModel0 ) ) );
//		
//		m_pAsmModel->AttachModel( enPARTS::CORE,
//			m_wpResource->GetSkinModels( 
//				static_cast<clsResource::enSKIN_MODEL>( clsResource::enCoreModel0 ) ) );
//		
//		m_pAsmModel->AttachModel( enPARTS::HEAD,
//			m_wpResource->GetSkinModels( 
//				static_cast<clsResource::enSKIN_MODEL>( clsResource::enHeadModel0 ) ) );
//		
//		m_pAsmModel->AttachModel( enPARTS::ARM_L,
//			m_wpResource->GetSkinModels( 
//				static_cast<clsResource::enSKIN_MODEL>( clsResource::enArmLModel0 ) ) );
//		
//		m_pAsmModel->AttachModel( enPARTS::ARM_R,
//			m_wpResource->GetSkinModels( 
//				static_cast<clsResource::enSKIN_MODEL>( clsResource::enArmRModel0 ) ) );
//		
//		m_pAsmModel->AttachModel( enPARTS::WEAPON_L,
//			m_wpResource->GetSkinModels( 
//				static_cast<clsResource::enSKIN_MODEL>( clsResource::enWeaponModel0 ) ) );
//		
//		m_pAsmModel->AttachModel( enPARTS::WEAPON_R,
//			m_wpResource->GetSkinModels( 
//				static_cast<clsResource::enSKIN_MODEL>( clsResource::enWeaponModel0 ) ) );
	}


	m_pAsmModel->UpDate();
}

void clsSCENE_ASSEMBLE::RenderProduct( const D3DXVECTOR3 &vCamPos ) const
{
//	m_pSprite->SetPos( ConvDimPos( m_pParts->GetPosition() ) );
////	ConvDimPos( m_pSprite->GetPos(), m_pParts->GetPosition() );
//
//	m_pParts->Render( m_mView, m_mProj, m_vLight, vCamPos );
//	m_pTestChara->Render( m_mView, m_mProj, m_vLight, vCamPos, 
//		D3DXVECTOR4(0.5f,2.0f,0.5f,0.75f), true );
//
//	m_pSprite->Render();
	m_pAsmModel->Render( m_mView, m_mProj, m_vLight, vCamPos );
}



//============================ デバッグテキスト ===========================//
#if _DEBUG
void clsSCENE_ASSEMBLE::RenderDebugText()
{
	//NULLチェック.
	ASSERT_IF_NULL( m_upText );

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



