#include "SceneMission.h"


//================================//
//========== ミッション中クラス ==========//
//================================//
clsSCENE_MISSION::clsSCENE_MISSION( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup ),
	m_pTestRobo( nullptr ),
	m_pTestChara( nullptr )
{
}

clsSCENE_MISSION::~clsSCENE_MISSION()
{
	SAFE_DELETE( m_pTestChara );
	SAFE_DELETE( m_pTestRobo );
}

//生成時に一度だけ通る処理.
void clsSCENE_MISSION::CreateProduct()
{
	m_pTestRobo = new clsRobo;
	m_pTestRobo->RoboInit( nullptr, nullptr, nullptr, m_wpPtrGroup );//4つ目の引数は効果音やエフェクトを出すために追加しました.

	D3DXVECTOR3 tmpVec3 = { 0.0f, -2.0f, 0.0f };
	m_pTestRobo->SetPosition( tmpVec3 );
	m_pTestRobo->SetScale( 0.25f );

	//テストモデルの初期化.
	m_pTestChara = new clsCharaStatic;
	m_pTestChara->AttachModel( 
		m_wpResource->GetStaticModels( 
			clsResource::enSTATIC_MODEL::enStaticModel_Enemy ) );
	m_pTestChara->Init();
	m_pTestChara->SetPosition( D3DXVECTOR3( -2.0f, 0.0f, 0.0f ) );
	m_pTestChara->SetRotation( D3DXVECTOR3( 0.0f, 0.0f, D3DX_PI*0.5 ) );

}

//毎フレーム通る処理.
void clsSCENE_MISSION::UpdateProduct( enSCENE &nextScene )
{
	//nullならassert.
	ASSERT_IF_NULL( m_pTestRobo );
	m_pTestRobo->Update();



	//エンディングに行く場合は以下のようにする.
	if( !"クリア条件を満たすとここを通る" ){
		nextScene = enSCENE::ENDING;
	}
}

//描画.
void clsSCENE_MISSION::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
	m_pTestChara->Render( m_mView, m_mProj, m_vLight, vCamPos );
	m_pTestRobo->Render( m_mView, m_mProj, m_vLight, vCamPos );
}

