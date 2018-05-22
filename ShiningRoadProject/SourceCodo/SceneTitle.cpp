#include "SceneTitle.h"


//================================//
//========== タイトルクラス ==========//
//================================//
clsTITLE::clsTITLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup ),
	m_pTestChara( nullptr )
{
}

clsTITLE::~clsTITLE()
{
	//newしたポインタの破棄.
	SAFE_DELETE( m_pTestChara );
}


void clsTITLE::CreateProduct()
{
	//テストモデルの初期化.
	m_pTestChara = new clsCharaStatic;
	m_pTestChara->AttachModel( 
		m_wpResource->GetStaticModels( 
			clsResource::enSTATIC_MODEL::enStaticModel_Ground ) );
	m_pTestChara->Init();
	m_pTestChara->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

}

void clsTITLE::UpdateProduct( enSCENE &nextScene )
{
	//Update関数が機能しているかのテスト用回転.
	m_pTestChara->AddRotation( D3DXVECTOR3( 0.0f, 0.00f, 0.025f ) );


	DebugChangeScene( nextScene );
}

void clsTITLE::RenderProduct()
{
	//Render関数の引数を書きやすくするための変数.
	D3DXVECTOR3 vCamPos = m_wpCamera->GetPos();

	m_pTestChara->Render( m_mView, m_mProj, m_vLight, vCamPos );

}
