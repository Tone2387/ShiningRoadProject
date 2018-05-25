#include "SceneTitle.h"


//================================//
//========== タイトルクラス ==========//
//================================//
clsSCENE_TITLE::clsSCENE_TITLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup ),
	m_pTestChara( nullptr )
{
}

clsSCENE_TITLE::~clsSCENE_TITLE()
{
	//newしたポインタの破棄.
	SAFE_DELETE( m_pTestChara );
}


void clsSCENE_TITLE::CreateProduct()
{
	//テストモデルの初期化.
	m_pTestChara = new clsCharaStatic;
	m_pTestChara->AttachModel( 
		m_wpResource->GetStaticModels( 
			clsResource::enSTATIC_MODEL::enStaticModel_Ground ) );
	m_pTestChara->Init();
	m_pTestChara->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_pTestChara->SetRotation( { 0.0f, static_cast<FLOAT>(D3DX_PI*0.125 + D3DX_PI), 0.0f } );
	m_pTestChara->SetScale( 0.1 );
}

void clsSCENE_TITLE::UpdateProduct( enSCENE &nextScene )
{
	//Update関数が機能しているかのテスト用回転.
	m_pTestChara->AddRotation( D3DXVECTOR3( 0.0f, 0.00f, 0.025f ) );


	DebugChangeScene( nextScene );
}

void clsSCENE_TITLE::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	m_pTestChara->Render( m_mView, m_mProj, m_vLight, vCamPos );

}
