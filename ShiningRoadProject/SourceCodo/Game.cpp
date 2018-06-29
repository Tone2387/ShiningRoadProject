#include "Game.h"

using namespace std;

const char* cBLACK_FILE_NAME = "Data\\Image\\BlackScreen.png";


//起動時の初期シーン.
#define START_UP_SCENE enSCENE::TITLE
//シーン初期化時の初期BGM番号.
const int iINIT_SCENE_BGM_NO = 0;

const unsigned char cSTART_UP_MUSIC_NO = 0;

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
		,m_spResource( nullptr )
		,m_spEffect( nullptr )
		,m_spSound( nullptr )
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
	SAFE_DELETE( m_spEffect );
	SAFE_DELETE( m_spResource );
	SAFE_DELETE( m_spSound );
	m_upSoundFactory.reset();
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

	m_upSoundFactory = make_unique<clsFACTORY_SOUND_MANAGER>();

	m_spSound = m_upSoundFactory->Create( START_UP_SCENE, m_hWnd );
	m_spSound->Create();
	m_spSound->PlayBGM( cSTART_UP_MUSIC_NO );//起動音再生.

	m_spResource = new clsResource;
	m_spResource->Create( m_hWnd, m_wpDevice, m_wpContext );

	m_spEffect = new clsEffects;
	m_spEffect->Create( m_wpDevice, m_wpContext );

	m_spRoboStatus = new clsROBO_STATUS;

	//暗転.
	SPRITE_STATE ss;
	m_spBlackScreen = new clsBLACK_SCREEN;
	m_spBlackScreen->Create( m_wpDevice, m_wpContext,
		cBLACK_FILE_NAME, ss );

	//引数のポインタの集合体.
	m_pPtrGroup = new clsPOINTER_GROUP( 
		m_wpDevice, m_wpContext, 
		m_wpViewPort, m_wpDepthStencilState,
		m_spDxInput, m_spXInput,
		m_spResource, m_spEffect, m_spSound,
		m_spRoboStatus, m_spBlackScreen );


	//ファクトリの作成.
	ASSERT_IF_NOT_NULL( m_pSceneFactory );
	m_pSceneFactory = new clsSCENE_FACTORY( m_pPtrGroup );
	ASSERT_IF_NOT_NULL( m_pCameraFactory );
	m_pCameraFactory = new clsFACTORY_CAMERA;

	//最初のシーンはタイトルを指定する.
	SwitchScene( START_UP_SCENE, true );

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
void clsGAME::SwitchScene( const enSCENE enNextScene, const bool bStartUp )
{
	//何もしないならそのまま( シーン変更を行わない ).
	if( enNextScene == enSCENE::NOTHING ){
		return;
	}

	//今のシーンを消して.
	SAFE_DELETE( m_pScene );
	SAFE_DELETE( m_spCamera );

	//起動時は無視.
	if( !bStartUp ){
		SAFE_DELETE( m_spSound );
		//指示どうりのシーンを作る.
		//サウンド.
		m_spSound = m_upSoundFactory->Create( enNextScene, m_hWnd );
		m_spSound->Create();
		m_pPtrGroup->UpdateSoundPtr( m_spSound );
	}

	//カメラ.
	m_spCamera = m_pCameraFactory->Create( enNextScene );
	m_pPtrGroup->UpdateCameraPtr( m_spCamera );

	//お待ちかねのシーン本体.
	m_pScene = m_pSceneFactory->Create( enNextScene );

	//起動時は無視.
	if( !bStartUp ){
		m_spSound->PlayBGM( iINIT_SCENE_BGM_NO );//BGM再生.
	}

	m_pScene->Create();//シーン初期化.

	//明転開始.
	m_spBlackScreen->GetBright();
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



