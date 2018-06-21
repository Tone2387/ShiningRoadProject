#include "Game.h"

using namespace std;

//�N�����̏����V�[��.
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

//new����Ɏg��.
void clsGAME::Create()
{ 
	ASSERT_IF_NOT_NULL( m_spDxInput );
	m_spDxInput = new clsDxInput;
	m_spDxInput->initDI(m_hWnd);

	ASSERT_IF_NOT_NULL( m_spXInput );
	m_spXInput = new clsXInput;

	m_pSound = new clsSOUND_MANAGER( m_hWnd );
	m_pSound->PlayBGM( 0 );			//�N�����Đ�.

	m_pResource = new clsResource;
	m_pResource->Create( m_hWnd, m_wpDevice, m_wpContext );

	m_pEffect = new clsEffects;
	m_pEffect->Create( m_wpDevice, m_wpContext );

	m_spRoboStatus = new clsROBO_STATUS;

	//�Ó].
	SPRITE_STATE ss;
	ss.Anim = { 0.0f, 0.0f };
	ss.Disp = { WND_W, WND_H };
	m_spBlackScreen = new clsBLACK_SCREEN;
	m_spBlackScreen->Create( m_wpDevice, m_wpContext,
		"Data\\Image\\BlackScreen.png", ss );

	//�����̃|�C���^�̏W����.
	m_pPtrGroup = new clsPOINTER_GROUP( 
		m_wpDevice, m_wpContext, 
		m_wpViewPort, m_wpDepthStencilState,
		m_spDxInput, m_spXInput,
		m_pResource, m_pEffect, m_pSound,
		m_spRoboStatus, m_spBlackScreen );


	//�t�@�N�g���̍쐬.
	ASSERT_IF_NOT_NULL( m_pSceneFactory );
	m_pSceneFactory = new clsSCENE_FACTORY( m_pPtrGroup );
	ASSERT_IF_NOT_NULL( m_pCameraFactory );
	m_pCameraFactory = new clsFACTORY_CAMERA;

	//�ŏ��̃V�[���̓^�C�g�����w�肷��.
	SwitchScene( START_UP_SCENE );

}

//���t���[���g��.
void clsGAME::Update()
{ 
	//�R���g���[�����͏��X�V.
	ASSERT_IF_NULL( m_spDxInput );
	m_spDxInput->UpdataInputState();

	ASSERT_IF_NULL( m_spXInput );
	m_spXInput->UpdateStatus();

	//�V�[��������Ă���Ȃ�.
	ASSERT_IF_NULL( m_pScene );

	//���̃V�[���͉�?�t���O.
	enSCENE enNextScene = enSCENE::NOTHING;

	m_pScene->Update( enNextScene );

	//�t���O�ɕύX������΃V�[���ύX.
	if( enNextScene != enSCENE::NOTHING ){
		SwitchScene( enNextScene );
	}
}

//���t���[���g��.
void clsGAME::Render()
{ 
	ASSERT_IF_NULL( m_pScene );
	m_pScene->Render(); 
}


//�V�[���؂�ւ�.
void clsGAME::SwitchScene( const enSCENE enNextScene )
{
	//�������Ȃ��Ȃ炻�̂܂�( �V�[���ύX���s��Ȃ� ).
	if( enNextScene == enSCENE::NOTHING ){
		return;
	}

	//���̃V�[����������.
	SAFE_DELETE( m_pScene );
	SAFE_DELETE( m_spCamera );

	//�w���ǂ���̃V�[�������.
	//�܂��̓J����.
	m_spCamera = m_pCameraFactory->Create( enNextScene );
	m_pPtrGroup->UpdateCameraPtr( m_spCamera );

	//���҂����˂̃V�[���{��.
	m_pScene = m_pSceneFactory->Create( enNextScene );
	m_pScene->Create();

	//���]�J�n.
	m_spBlackScreen->GetBright();

//BGM�Đ�.//( �e�N���X�̃R���X�g���N�^�ł�� ).
m_pSound->PlayBGM( (int)enNextScene );
}





//���b�v�֐�.
D3DXVECTOR3 clsGAME::GetCameraPos() const
{
	return m_pScene->GetCameraPos();
}
D3DXVECTOR3 clsGAME::GetCameraLookPos() const
{
	return m_pScene->GetCameraLookPos();
}



