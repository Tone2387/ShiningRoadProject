#include "SceneClass.h"
#include <Windows.h>


//ライト方向.
const D3DXVECTOR3 vLIGHT_DIR = { 0.0f, 0.01f, 0.02f };
//カメラのより具合.
const float fZOOM = static_cast<float>( D3DX_PI / 4.0 );
//描画限界距離.
const float fRENDER_LIMIT = 150.0f;


//================================//
//========== 基底クラス ==========//
//================================//
clsSCENE_BASE::clsSCENE_BASE( clsPOINTER_GROUP* const ptrGroup ) :
	m_wpDevice( ptrGroup->GetDevice() ),
	m_wpContext( ptrGroup->GetContext() ),
	m_wpViewPort( ptrGroup->GetViewPort() ),
	m_wpPtrGroup( ptrGroup ),
	m_wpDxInput( ptrGroup->GetDxInput() ),
	m_wpResource( ptrGroup->GetResource() ),
	m_wpEffects( ptrGroup->GetEffects() ),
	m_wpSound( ptrGroup->GetSound() ),
	m_wpCamera( ptrGroup->GetCamera() )
{
	D3DXMatrixIdentity( &m_mView );
	D3DXMatrixIdentity( &m_mProj );	
	m_vLight = vLIGHT_DIR;
}

clsSCENE_BASE::~clsSCENE_BASE()
{
	m_wpCamera = nullptr;
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
	m_wpDxInput = nullptr;
	m_wpPtrGroup = nullptr;
	m_wpViewPort = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
}


//シーン作成直後に「SceneManager.cpp」の「SwitchScene」関数内で使用されている.
void clsSCENE_BASE::Create()
{
	CreateProduct();
}

//ループ内の処理( 引数を関数内で変更すると今のシーンが破棄され、.
//				  指定したシーンが生成される ).
void clsSCENE_BASE::Update( enSCENE &nextScene )
{
	UpdateProduct( nextScene );

	DebugChangeScene( nextScene );
}

//シーン内のオブジェクトの描画関数のまとめ.
void clsSCENE_BASE::Render()
{
	//カメラ関数.
	Camera();
	//プロジェクション関数.
	Proj();

	RenderProduct();
}


D3DXVECTOR3 clsSCENE_BASE::GetCameraPos() const
{
	ASSERT_IF_NULL( m_wpCamera );
	return m_wpCamera->GetPos();
}
D3DXVECTOR3 clsSCENE_BASE::GetCameraLookPos() const
{
	ASSERT_IF_NULL( m_wpCamera );
	return m_wpCamera->GetLookPos();
}


//3D座標をスクリーン( 2D )座標へと変換する.dimensions(次元) conversion(変換)
D3DXVECTOR3 clsSCENE_BASE::ConvDimPos( const D3DXVECTOR3 &v3DPos )
{
	D3DXVECTOR3 v2DPos;
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	D3DXVec3Project( &v2DPos, &v3DPos, m_wpViewPort, &m_mProj, &m_mView, &mWorld );
	return v2DPos;
}


//デバッグ用シーン切り替え.
void clsSCENE_BASE::DebugChangeScene( enSCENE &nextScene ) const
{
	if( GetAsyncKeyState( VK_F5 ) & 0x1 ){
		nextScene = enSCENE::TITLE;
	}
	else if( GetAsyncKeyState( VK_F6 ) & 0x1 ){
		nextScene = enSCENE::ASSEMBLE;
	}
	else if( GetAsyncKeyState( VK_F7  ) & 0x1 ){
		nextScene = enSCENE::MISSION;
	}
	else if( GetAsyncKeyState( VK_F8  ) & 0x1 ){
		nextScene = enSCENE::ENDING;
	}
}

//============================================================
//	カメラ関数.
//============================================================
void clsSCENE_BASE::Camera()
{
	//ビュー(カメラ)変換.
	D3DXVECTOR3 vUpVec	( 0.0f, 1.0f, 0.0f );	//上方位置.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&GetCameraPos(), &GetCameraLookPos(), &vUpVec );

}
//============================================================
//	プロジェクション関数.
//============================================================
void clsSCENE_BASE::Proj()
{
	//プロジェクション(射影行列)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,			//(out)プロジェクション計算結果.
		fZOOM,	//y方向の視野(ラジアン指定)数字を大きくしたら視野が狭くなる.
		static_cast<FLOAT>( WND_W ) / static_cast<FLOAT>( WND_H ),//アスペクト比(幅/高さ).
		0.1f,				//近いビュー平面のz値.
		fRENDER_LIMIT );	//遠いビュー平面のz値.
}
