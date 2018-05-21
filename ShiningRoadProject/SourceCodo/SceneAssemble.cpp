#include "SceneAssemble.h"

#define TEST_TEX_PASS "Data\\Load\\LoadBack.png"


//================================//
//========== 組み換えクラス ==========//
//================================//
clsASSEMBLE::clsASSEMBLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup ),
	m_pTestChara( nullptr ),
	m_pParts( nullptr ),
	m_pSprite( nullptr )
{
}

clsASSEMBLE::~clsASSEMBLE()
{
	SAFE_DELETE( m_pSprite );
	SAFE_DELETE( m_pParts );
	SAFE_DELETE( m_pTestChara );
}

void clsASSEMBLE::Create()
{
	m_pTestChara = new clsCharaStatic;
	m_pTestChara->AttachModel( 
		m_wpResource->GetStaticModels( 
		clsResource::enSTATIC_MODEL::enStaticModel_Ground ) );
	m_pTestChara->Init();
	m_pTestChara->SetPosition( D3DXVECTOR3( 1.0f, 0.0f, 50.0f ) );

	m_pParts = new clsCharaStatic;
	m_pParts->AttachModel( 
		m_wpResource->GetStaticModels( 
		clsResource::enSTATIC_MODEL::enStaticModel_Enemy ) );
	m_pParts->Init();
	m_pParts->SetPosition( D3DXVECTOR3( -2.0f, 1.0f, 0.0f ) );


	m_pSprite = new clsSPRITE2D_CENTER;
	SPRITE_STATE tmpSs;
	m_pSprite->Create( 
		m_wpDevice, m_wpContext, 
		TEST_TEX_PASS, tmpSs );
	m_pSprite->SetPos( { WND_W*0.5f, WND_H*0.5f, 0.0f } );
}

void clsASSEMBLE::Update( enSCENE &nextScene )
{
	m_pParts->AddRotationY( 0.01f);

	m_pSprite->AddRot( { 0.0f, 0.0f, 0.025f } );


	DebugChangeScene( nextScene );
}

void clsASSEMBLE::Render(
	const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight ) const

{
	//Render関数の引数を書きやすくするための変数.
	D3DXVECTOR3 vCamPos = m_wpCamera->GetPos();

	m_pParts->Render( mView, mProj, vLight, vCamPos );
	m_pTestChara->Render( mView, mProj, vLight, vCamPos, D3DXVECTOR4(0.5f,2.0f,0.5f,0.75f), true );

	m_pSprite->Render();
}






