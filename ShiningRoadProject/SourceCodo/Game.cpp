#include "Game.h"

using namespace std;

//起動時の初期シーン.
#define START_UP_SCENE enSCENE::TITLE


clsGAME::clsGAME( 
	const HWND hWnd, 
	ID3D11Device* const pDevice, 
	ID3D11DeviceContext* const pContext,
	D3D10_VIEWPORT* const pViewPort,
	ID3D11DepthStencilState* const pDepthState  )
		:m_hWnd( hWnd )
		,m_wpDevice( pDevice )
		,m_wpContext( pContext )
		,m_wpViewPort( pViewPort )
		,m_wpDepthStencilState( pDepthState )
		,m_pPtrGroup( nullptr )
		,m_spDxInput( nullptr )
		,m_spXInput( nullptr )
		,m_pResource( nullptr )
		,m_pEffect( nullptr )
		,m_pSound( nullptr )
		,m_pScene( nullptr )
		,m_pSceneFactory( nullptr )
		,m_spCamera( nullptr )
		,m_pCameraFactory( nullptr )
		,m_spRoboStatus( nullptr )
		,m_spBlackScreen( nullptr )
{

}

clsGAME::~clsGAME()
{
	SAFE_DELETE( m_pScene );
	SAFE_DELETE( m_spCamera );
	SAFE_DELETE( m_pCameraFactory );
	SAFE_DELETE( m_pSceneFactory );
	SAFE_DELETE( m_pPtrGroup );
	SAFE_DELETE( m_spBlackScreen );
	SAFE_DELETE( m_spRoboStatus );
	SAFE_DELETE( m_pEffect );
	SAFE_DELETE( m_pResource );
	SAFE_DELETE( m_pSound );
	SAFE_DELETE( m_spXInput );
//	if( m_spXInput != nullptr ){
//		m_spXInput->EndProc();
//		XInputEnable( false );
//		delete m_spXInput;
//		m_spXInput = nullptr;
//	}
	SAFE_DELETE( m_spDxInput );

	m_wpDepthStencilState = nullptr;
	m_wpViewPort = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
	m_hWnd = nullptr;
}

//new直後に使う.
void clsGAME::Create()
{ 
	ASSERT_IF_NOT_NULL( m_spDxInput );
	m_spDxInput = new clsDxInput;
	m_spDxInput->initDI(m_hWnd);

	ASSERT_IF_NOT_NULL( m_spXInput );
	m_spXInput = new clsXInput;

	m_pSound = new clsSOUND_MANAGER( m_hWnd );
	m_pSound->PlayBGM( 0 );			//起動音再生.

	m_pResource = new clsResource;
	m_pResource->Create( m_hWnd, m_wpDevice, m_wpContext );

	m_pEffect = new clsEffects;
	m_pEffect->Create( m_wpDevice, m_wpContext );

	m_spRoboStatus = new clsROBO_STATUS;

	//暗転.
	SPRITE_STATE ss;
	ss.Anim = { 0.0f, 0.0f };
	ss.Disp = { WND_W, WND_H };
	m_spBlackScreen = new clsBLACK_SCREEN;
	m_spBlackScreen->Create( m_wpDevice, m_wpContext,
		"Data\\Image\\BlackScreen.png", ss );

	//引数のポインタの集合体.
	m_pPtrGroup = new clsPOINTER_GROUP( 
		m_wpDevice, m_wpContext, 
		m_wpViewPort, m_wpDepthStencilState,
		m_spDxInput, m_spXInput,
		m_pResource, m_pEffect, m_pSound,
		m_spRoboStatus, m_spBlackScreen );


	//ファクトリの作成.
	ASSERT_IF_NOT_NULL( m_pSceneFactory );
	m_pSceneFactory = new clsSCENE_FACTORY( m_pPtrGroup );
	ASSERT_IF_NOT_NULL( m_pCameraFactory );
	m_pCameraFactory = new clsFACTORY_CAMERA;

	//最初のシーンはタイトルを指定する.
	SwitchScene( START_UP_SCENE );

}

//毎フレーム使う.
void clsGAME::Update()
{ 
	//コントローラ入力情報更新.
	ASSERT_IF_NULL( m_spDxInput );
	m_spDxInput->UpdataInputState();

	ASSERT_IF_NULL( m_spXInput );
	m_spXInput->UpdateStatus();

	//シーンが作られているなら.
	ASSERT_IF_NULL( m_pScene );

	//次のシーンは何?フラグ.
	enSCENE enNextScene = enSCENE::NOTHING;

	m_pScene->Update( enNextScene );

	//フラグに変更があればシーン変更.
	if( enNextScene != enSCENE::NOTHING ){
		SwitchScene( enNextScene );
	}
}

//毎フレーム使う.
void clsGAME::Render()
{ 
	ASSERT_IF_NULL( m_pScene );
	m_pScene->Render(); 
}


//シーン切り替え.
void clsGAME::SwitchScene( const enSCENE enNextScene )
{
	//何もしないならそのまま( シーン変更を行わない ).
	if( enNextScene == enSCENE::NOTHING ){
		return;
	}

	//今のシーンを消して.
	SAFE_DELETE( m_pScene );
	SAFE_DELETE( m_spCamera );

	//指示どうりのシーンを作る.
	//まずはカメラ.
	m_spCamera = m_pCameraFactory->Create( enNextScene );
	m_pPtrGroup->UpdateCameraPtr( m_spCamera );

	//お待ちかねのシーン本体.
	m_pScene = m_pSceneFactory->Create( enNextScene );
	m_pScene->Create();

	//明転開始.
	m_spBlackScreen->GetBright();

//BGM再生.//( 各クラスのコンストラクタでやる ).
m_pSound->PlayBGM( (int)enNextScene );
}





//ラップ関数.
D3DXVECTOR3 clsGAME::GetCameraPos() const
{
	return m_pScene->GetCameraPos();
}
D3DXVECTOR3 clsGAME::GetCameraLookPos() const
{
	return m_pScene->GetCameraLookPos();
}



