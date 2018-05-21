#include "SceneMission.h"


//================================//
//========== ミッション中クラス ==========//
//================================//
clsMISSION::clsMISSION( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup ),
	m_pTestRobo( nullptr ),
	m_pTestChara( nullptr )
{
}

clsMISSION::~clsMISSION()
{
	SAFE_DELETE( m_pTestChara );
	SAFE_DELETE( m_pTestRobo );
}

//生成時に一度だけ通る処理.
void clsMISSION::Create()
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
void clsMISSION::Update( enSCENE &nextScene )
{
	//nullならassert.
	ASSERT_IF_NULL( m_pTestRobo );
	m_pTestRobo->Update();



	//エンディングに行く場合は以下のようにする.
	if( !"クリア条件を満たすとここを通る" ){
		nextScene = enSCENE::ENDING;
	}

	//デバッグ用シーン変更( キーボードのF9～12が対応している )すべてのシーンのUpdateの一番下に置く.
	DebugChangeScene( nextScene );
}

//描画.
void clsMISSION::Render(
	const D3DXMATRIX &mView, 
	const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight ) const
{
	//Render関数の引数を書きやすくするための変数.
	D3DXVECTOR3 vCamPos = m_wpCamera->GetPos();

	m_pTestChara->Render( mView, mProj, vLight, vCamPos );
	m_pTestRobo->Render( mView, mProj, vLight, vCamPos );
}

