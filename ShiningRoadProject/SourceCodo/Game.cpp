//ロード画面の描画.
//#define RENDER_LOAD_SCREEN_


#include "Game.h"
#include "FactoryScene.h"
#include "FactoryCamera.h"
#include "FactorySoundManager.h"
#include "DxInput.h"
#include "CXInput.h"
#include "BlackScreen.h"
#include "RoboStatusPlayer.h"
#include "PtrGroup.h"
#include "Resource.h"
#include "SceneBase.h"
#include "CFont.h"
#ifdef RENDER_LOAD_SCREEN_
#include "RenderAtLoadTime.h"
#endif//#ifdef RENDER_LOAD_SCREEN_
#include <thread>


using namespace std;

//起動時の初期シーン.
#define START_UP_SCENE enSCENE::TITLE

namespace{


}


clsGAME::clsGAME( 
	const HWND hWnd, 
	ID3D11Device* const pDevice, 
	ID3D11DeviceContext* const pContext,
	D3D10_VIEWPORT* const pViewPort10,
	D3D11_VIEWPORT* const pViewPort11, 
	IDXGISwapChain* const pSwapChain,
	ID3D11RenderTargetView* const pBackBuffer_TexRTV,
	ID3D11DepthStencilView* const pBackBuffer_DSTexDSV )
		:m_hWnd( hWnd )
		,m_wpDevice( pDevice )
		,m_wpContext( pContext )
		,m_wpViewPort10( pViewPort10 )
		,m_wpViewPort11( pViewPort11 )
		,m_wpSwapChain( pSwapChain )
		,m_wpBackBuffer_TexRTV( pBackBuffer_TexRTV )
		,m_wpBackBuffer_DSTexDSV( pBackBuffer_DSTexDSV )
		,m_spPtrGroup( nullptr )
		,m_spDxInput( nullptr )
		,m_spXInput( nullptr )
		,m_spResource( nullptr )
		,m_spEffect( nullptr )
		,m_spSound( nullptr )
		,m_spCamera( nullptr )
		,m_spRoboStatus( nullptr )
		,m_spBlackScreen( nullptr )
		,m_spFont( nullptr )
{
	assert( !m_spResource );
	m_spResource = new clsResource;
	m_spResource->Create( m_hWnd, m_wpDevice, m_wpContext );

}

clsGAME::~clsGAME()
{
	m_upScene.reset( nullptr );
	SAFE_DELETE( m_spCamera );
	SAFE_DELETE( m_spPtrGroup );
	SAFE_DELETE( m_spFont );
	SAFE_DELETE( m_spBlackScreen );
	SAFE_DELETE( m_spRoboStatus );
	SAFE_DELETE( m_spEffect );
	SAFE_DELETE( m_spSound );
	SAFE_DELETE( m_spXInput );
	SAFE_DELETE( m_spDxInput );
	SAFE_DELETE( m_spResource );

	m_wpBackBuffer_DSTexDSV = nullptr;
	m_wpBackBuffer_TexRTV = nullptr;
	m_wpSwapChain = nullptr;


	m_wpViewPort11 = nullptr;
	m_wpViewPort10 = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
	m_hWnd = nullptr;
}

//new直後に使う.
void clsGAME::Create()
{ 
	assert( !m_spDxInput );
	m_spDxInput = new clsDxInput;
	m_spDxInput->initDI(m_hWnd);

	assert( !m_spXInput );
	m_spXInput = new clsXInput;

	assert( !m_spEffect );
	m_spEffect = new clsEffects;
	m_spEffect->Create( m_wpDevice, m_wpContext );

	assert( !m_spRoboStatus );
	m_spRoboStatus = new clsROBO_STATUS_PLAYER;

	//暗転.
	const char* cBLACK_FILE_NAME = "Data\\Image\\BlackScreen.png";
	SPRITE_STATE ss;
	assert( !m_spBlackScreen );
	m_spBlackScreen = new clsBLACK_SCREEN;
	m_spBlackScreen->Create( 
		m_wpDevice, m_wpContext,
		cBLACK_FILE_NAME, ss );

	assert( !m_spFont );
	m_spFont = new clsFont( m_wpDevice, m_wpContext );

	//引数のポインタの集合体.
	assert( !m_spPtrGroup );
	m_spPtrGroup = new clsPOINTER_GROUP( 
		m_wpDevice, m_wpContext, 
		m_wpViewPort10, m_wpViewPort11,
		m_spDxInput, m_spXInput,
		m_spResource, 
		m_spEffect, 
		m_spSound,
		m_spRoboStatus, 
		m_spBlackScreen,
		m_spFont );



	//最初のシーンはタイトルを指定する.
	SwitchScene( START_UP_SCENE );

}

//毎フレーム使う.
bool clsGAME::Update()
{ 
	if( !m_upScene ){
		//閉じる.
		return false;
	}

	//コントローラ入力情報更新.
	assert( m_spXInput );
	m_spXInput->UpdateStatus();
	assert( m_spDxInput );
	m_spDxInput->UpdataInputState();

	//次のシーンは何?のフラグ.
	enSCENE enNextScene = enSCENE::NOTHING;

	//シーンが作られているなら.
	m_upScene->Update( enNextScene );

	//フラグに変更があればシーン変更.
	if( enNextScene != enSCENE::NOTHING ){
		SwitchScene( enNextScene );
	}
//SwitchScene( g_Scene );

	return true;
}

//毎フレーム使う.
void clsGAME::Render() const
{ 
	//画面の初期化色.
//	const float fCleaColorArray[4] = { 0.5f, 0.25f, 2.0f, 1.0f };//クリア色(RGBA順)(0.0f~1.0f).
	const float fCleaColorArray[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//クリア色(RGBA順)(0.0f~1.0f).

	assert( m_wpContext );
	//画面のクリア.
	m_wpContext->ClearRenderTargetView(
		m_wpBackBuffer_TexRTV, fCleaColorArray );
	//デプスステンシルビューバックバッファ.
	m_wpContext->ClearDepthStencilView(
		m_wpBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );

	//シーンの描画.
	if( m_upScene ){
		m_upScene->Render( m_wpBackBuffer_TexRTV, m_wpBackBuffer_DSTexDSV ); 
	}

}


//シーン切り替え.
void clsGAME::SwitchScene( const enSCENE enNextScene )
{
	//何もしないならそのまま( シーン変更を行わない ).
	if( enNextScene == enSCENE::NOTHING ){
		return;
	}

#ifdef RENDER_LOAD_SCREEN_
	//起動中の描画.
	unique_ptr< clsRENDER_AT_LOAD_TIME > upRenderAtLoad =
		make_unique< clsRENDER_AT_LOAD_TIME >( 
			m_wpDevice,
			m_wpContext,
			m_wpSwapChain,
			m_wpBackBuffer_TexRTV,
			m_wpBackBuffer_DSTexDSV );

	//別スレッドで描画.
	thread thStartUpRender( &clsRENDER_AT_LOAD_TIME::Loop, upRenderAtLoad.get() );
#endif//#ifdef RENDER_LOAD_SCREEN_

	//今のシーンを消して.
	m_upScene.reset( nullptr );
	SAFE_DELETE( m_spCamera );
	SAFE_DELETE( m_spSound );

	//サウンド.
	unique_ptr< clsFACTORY_SOUND_MANAGER > upSoundFactory = make_unique<clsFACTORY_SOUND_MANAGER>();
	m_spSound = upSoundFactory->Create( enNextScene, m_hWnd );
	if( m_spSound ){
		m_spSound->Create();
	}
	m_spPtrGroup->UpdateSoundPtr( m_spSound );

	//カメラ.
	//ファクトリの作成.
	unique_ptr< clsFACTORY_CAMERA > upCameraFactory = make_unique< clsFACTORY_CAMERA >();
	m_spCamera = upCameraFactory->Create( enNextScene );
	m_spPtrGroup->UpdateCameraPtr( m_spCamera );

	//お待ちかねのシーン本体.
	unique_ptr< clsFACTORY_SCENE > upSceneFactory = make_unique< clsFACTORY_SCENE >();
	m_upScene.reset( upSceneFactory->Create( enNextScene, m_spPtrGroup ) );

	if( m_upScene ){
		m_upScene->Create( m_hWnd );//シーン初期化.
	}

#ifdef RENDER_LOAD_SCREEN_
	//ロード画面は必要なくなったので閉じる.
	upRenderAtLoad->FinishLoad();
	thStartUpRender.join();
	upRenderAtLoad.reset();
#endif//#ifdef RENDER_LOAD_SCREEN_

	//明転開始.
	m_spBlackScreen->GetBright();
}





