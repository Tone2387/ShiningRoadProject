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


using namespace std;

namespace{

	const char* cBLACK_FILE_NAME = "Data\\Image\\BlackScreen.png";


	//�N�����̏����V�[��.
	#define START_UP_SCENE enSCENE::TITLE
	//�^�C�g���̑O�ɃA�Z���u���V�[����ǂݍ���ŁA�X�e�[�^�X����ɓ����.
	#define GET_STATUS_DATA_INIT_SCENE enSCENE::ASSEMBLE

	const unsigned char cSTART_UP_MUSIC_NO = 0;

	//��ʂ̏������F.
	const float g_fClearColor[4] = { 0.5f, 0.25f, 2.0f, 1.0f };//�N���A�F(RGBA��)(0.0f~1.0f).

}


clsGAME::clsGAME( 
	const HWND hWnd, 
	ID3D11Device* const pDevice, 
	ID3D11DeviceContext* const pContext,
	D3D10_VIEWPORT* const pViewPort10,
	D3D11_VIEWPORT* const pViewPort11 )
		:m_hWnd( hWnd )
		,m_wpDevice( pDevice )
		,m_wpContext( pContext )
		,m_wpViewPort10( pViewPort10 )
		,m_wpViewPort11( pViewPort11 )
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

	m_wpViewPort11 = nullptr;
	m_wpViewPort10 = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
	m_hWnd = nullptr;
}

//new����Ɏg��.
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

	//�Ó].
	SPRITE_STATE ss;
	assert( !m_spBlackScreen );
	m_spBlackScreen = new clsBLACK_SCREEN;
	m_spBlackScreen->Create( 
		m_wpDevice, m_wpContext,
		cBLACK_FILE_NAME, ss );

	assert( !m_spFont );
	m_spFont = new clsFont ( m_wpDevice, m_wpContext );

	//�����̃|�C���^�̏W����.
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



	//�^�C�g���̑O�ɃA�Z���u���V�[����ǂݍ���ŁA�X�e�[�^�X����ɓ����.
	SwitchScene( GET_STATUS_DATA_INIT_SCENE, true );
	//�ŏ��̃V�[���̓^�C�g�����w�肷��.
	SwitchScene( START_UP_SCENE );

}

//���t���[���g��.
bool clsGAME::Update()
{ 
	if( !m_upScene ){
		//����.
		return false;
	}

	//�R���g���[�����͏��X�V.
	assert( m_spDxInput );
	m_spDxInput->UpdataInputState();

	assert( m_spXInput );
	m_spXInput->UpdateStatus();

	//���̃V�[���͉�?�̃t���O.
	enSCENE enNextScene = enSCENE::NOTHING;

	//�V�[��������Ă���Ȃ�.
	m_upScene->Update( enNextScene );

	//�t���O�ɕύX������΃V�[���ύX.
	if( enNextScene != enSCENE::NOTHING ){
		SwitchScene( enNextScene );
	}

	return true;
}

//���t���[���g��.
void clsGAME::Render(		
	ID3D11RenderTargetView* const pBackBuffer_TexRTV,
	ID3D11DepthStencilView* const pBackBuffer_DSTexDSV ) const
{ 
	assert( m_wpContext );
	//��ʂ̃N���A.
	m_wpContext->ClearRenderTargetView(
		pBackBuffer_TexRTV, g_fClearColor );
	//�f�v�X�X�e���V���r���[�o�b�N�o�b�t�@.
	m_wpContext->ClearDepthStencilView(
		pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );

	//�V�[���̕`��.
	if( m_upScene ){
		m_upScene->Render( pBackBuffer_TexRTV, pBackBuffer_DSTexDSV ); 
	}

}


//�V�[���؂�ւ�.
void clsGAME::SwitchScene( const enSCENE enNextScene, const bool bStartUp )
{
	//�������Ȃ��Ȃ炻�̂܂�( �V�[���ύX���s��Ȃ� ).
	if( enNextScene == enSCENE::NOTHING ){
		return;
	}

	//���̃V�[����������.
//	SAFE_DELETE( m_upScene );
	SAFE_DELETE( m_spCamera );
	SAFE_DELETE( m_spSound );

	//�T�E���h.

	if( !bStartUp ){
		unique_ptr< clsFACTORY_SOUND_MANAGER > upSoundFactory = make_unique<clsFACTORY_SOUND_MANAGER>();
		m_spSound = upSoundFactory->Create( enNextScene, m_hWnd );
		if( m_spSound ){
			m_spSound->Create();
		}
	}
	m_spPtrGroup->UpdateSoundPtr( m_spSound );

	//�J����.
	//�t�@�N�g���̍쐬.
	unique_ptr< clsFACTORY_CAMERA > upCameraFactory = make_unique< clsFACTORY_CAMERA >();
	m_spCamera = upCameraFactory->Create( enNextScene );
	m_spPtrGroup->UpdateCameraPtr( m_spCamera );

	//���҂����˂̃V�[���{��.
	unique_ptr< clsFACTORY_SCENE > upSceneFactory = make_unique< clsFACTORY_SCENE >();
	m_upScene.reset( upSceneFactory->Create( enNextScene, m_spPtrGroup ) );

	if( m_upScene ){
		m_upScene->Create( m_hWnd );//�V�[��������.
	}


	//���]�J�n.
	m_spBlackScreen->GetBright();
}





