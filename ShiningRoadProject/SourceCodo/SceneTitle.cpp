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


void clsTITLE::Create()
{
	//テストモデルの初期化.
	m_pTestChara = new clsCharaStatic;
	m_pTestChara->AttachModel( 
		m_wpResource->GetStaticModels( 
			clsResource::enSTATIC_MODEL::enStaticModel_Ground ) );
	m_pTestChara->Init();
	m_pTestChara->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

}

void clsTITLE::Update( enSCENE &nextScene )
{
	//Update関数が機能しているかのテスト用回転.
	m_pTestChara->AddRotation( D3DXVECTOR3( 0.0f, 0.00f, 0.025f ) );


	DebugChangeScene( nextScene );
}

void clsTITLE::Render(
	const D3DXMATRIX &mView, 
	const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight ) const
{
	//Render関数の引数を書きやすくするための変数.
	D3DXVECTOR3 vCamPos = m_wpCamera->GetPos();

	m_pTestChara->Render( mView, mProj, vLight, vCamPos );

}
