#include "SceneBase.h"
#include <Windows.h>


//���C�g����.
const D3DXVECTOR3 vLIGHT_DIR = { 0.005f, 0.01f, -2.0f };
//�J�����̂��.
const float fZOOM = static_cast<float>( D3DX_PI / 4.0 );
//�`����E����.
const float fRENDER_LIMIT = 640.0f;//150.0f.


//================================//
//========== ���N���X ==========//
//================================//
clsSCENE_BASE::clsSCENE_BASE( clsPOINTER_GROUP* const ptrGroup )
	:m_wpPtrGroup( ptrGroup )
	,m_wpDevice(		m_wpPtrGroup->GetDevice() )
	,m_wpContext(		m_wpPtrGroup->GetContext() )
	,m_wpViewPort(		m_wpPtrGroup->GetViewPort() )
	,m_wpDxInput(		m_wpPtrGroup->GetDxInput() )
	,m_wpXInput(		m_wpPtrGroup->GetXInput() )
	,m_wpResource(		m_wpPtrGroup->GetResource() )
	,m_wpEffects(		m_wpPtrGroup->GetEffects() )
	,m_wpSound(			m_wpPtrGroup->GetSound() )
	,m_wpCamera(		m_wpPtrGroup->GetCamera() )
	,m_wpRoboStatus(	m_wpPtrGroup->GetRoboStatus() )
	,m_wpBlackScreen(	m_wpPtrGroup->GetBlackScreen() )
#if _DEBUG
	,m_upText( nullptr )
#endif//#if _DEBUG
	,m_enNextScene( enSCENE::NOTHING )
{
}

clsSCENE_BASE::~clsSCENE_BASE()
{
	//�����~�߂�.
	m_wpSound->StopAllSound();

#if _DEBUG
	SAFE_DELETE( m_upText );
#endif//#if _DEBUG

	m_enNextScene = enSCENE::NOTHING;

	m_wpBlackScreen = nullptr;
	m_wpRoboStatus = nullptr;
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


//�V�[���쐬����ɁuSceneManager.cpp�v�́uSwitchScene�v�֐����Ŏg�p����Ă���.
void clsSCENE_BASE::Create()
{

	D3DXMatrixIdentity( &m_mView );
	D3DXMatrixIdentity( &m_mProj );	
	m_vLight = vLIGHT_DIR;

	m_wpBlackScreen->GetBright();

	m_enNextScene = enSCENE::NOTHING;

#if _DEBUG
	//�f�o�b�O�e�L�X�g�̏�����.
	m_upText = new clsDebugText;
	D3DXVECTOR4 vColor( 1.0f, 1.0f, 1.0f, 1.0f );
	if( FAILED( m_upText->Init(
		m_wpContext,
		WND_W, WND_H, 50.0f,
		vColor ) ) )
	{
		MessageBox( NULL, "�f�o�b�O�e�L�X�g�쐬���s", "clsMain::Loop", MB_OK );
	}
#endif//#if _DEBUG

	//�e�V�[����Create.
	CreateProduct();
}

//���[�v���̏���( �������֐����ŕύX����ƍ��̃V�[�����j������A.
//				  �w�肵���V�[������������� ).
void clsSCENE_BASE::Update( enSCENE &enNextScene )
{
	//enNextScene�͊e�V�[���ɒ��ڂ͓��������Ȃ�.
	enSCENE tmpScene = enNextScene;

	//�Ó]�X�V.
	m_wpBlackScreen->Update();

	//�e�V�[����Update.
	UpdateProduct( tmpScene );

//	//UpdateProduct���ł̎󂯎��( �X�V )�Y��h�~.
	m_wpBlackScreen->isBrightEnd();

	//�V�[���ύX�����߂��ꂽ��.
	if( tmpScene != enSCENE::NOTHING ){
		//�Ó]�J�n.
		m_wpBlackScreen->GetDark();
		m_enNextScene = tmpScene;//���ɕύX����V�[�����o��������.
	}

	//�Ó]���������炲�����̃V�[���ɐ؂�ւ���.
	if( m_wpBlackScreen->isDarkEnd() ){
		enNextScene = m_enNextScene;//�o���Ă����V�[����������.
		m_enNextScene = enSCENE::NOTHING;//������.//�o���Ă����Y���.
	}

	//�f�o�b�O�p�V�[���؂�ւ�.
	DebugChangeScene( enNextScene );
}

//�V�[�����̃I�u�W�F�N�g�̕`��֐��̂܂Ƃ�.
void clsSCENE_BASE::Render()
{
	//�J�����֐�.
	Camera();
	//�v���W�F�N�V�����֐�.
	Proj();	

	//�e�V�[���̕`��.
	RenderProduct( m_wpCamera->GetPos() );

	//�Ó]�`��.
	m_wpBlackScreen->Render();

#if _DEBUG
	SetDepth( false );	//Z�e�X�g:OFF.
	//�f�o�b�O�e�L�X�g.
	RenderDebugText();
	SetDepth( true );	//Z�e�X�g:ON.
#endif//#if _DEBUG

}


//�[�x�e�X�g(Z�e�X�g)ON/OFF�ؑ�.
void clsSCENE_BASE::SetDepth( const bool isOn )
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc,
		sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	depthStencilDesc.DepthEnable = isOn;

	m_wpDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_wpDepthStencilState );
	m_wpContext->OMSetDepthStencilState(
		m_wpDepthStencilState, 1 );
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


//3D���W���X�N���[��( 2D )���W�ւƕϊ�����.dimensions(����) conversion(�ϊ�)
D3DXVECTOR3 clsSCENE_BASE::ConvDimPos( const D3DXVECTOR3 &v3DPos )
{
	D3DXVECTOR3 v2DPos;
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	D3DXVec3Project( &v2DPos, &v3DPos, m_wpViewPort, &m_mProj, &m_mView, &mWorld );
	return v2DPos;
}



#if _DEBUG
void clsSCENE_BASE::RenderDebugText()
{
	//NULL�`�F�b�N.
	ASSERT_IF_NULL( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//��s���ɂ������ɂ��炷��.

	sprintf_s( strDbgTxt, 
		"CameraPos : x[%f], y[%f], z[%f]",
		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamLokPos : x[%f], y[%f], z[%f]",
		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );


	//dbgtxty += 10;
	//if( m_pBgm[0]->IsStopped() ){
	//	sprintf_s( strDbgTxt, "Stopped" );
	//	m_pText->Render( strDbgTxt, 0, dbgtxty );
	//}
	//if( m_pBgm[0]->IsPlaying() ){
	//	sprintf_s( strDbgTxt, "Playingn" );
	//	m_pText->Render( strDbgTxt, 0, dbgtxty );
	//}

}
#endif //#if _DEBUG






//�J�����֐�.
void clsSCENE_BASE::Camera()
{
	//�r���[(�J����)�ϊ�.
	D3DXVECTOR3 vUpVec	( 0.0f, 1.0f, 0.0f );	//����ʒu.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)�r���[�v�Z����.
		&GetCameraPos(), &GetCameraLookPos(), &vUpVec );

}
//�v���W�F�N�V�����֐�.
void clsSCENE_BASE::Proj()
{
	//�v���W�F�N�V����(�ˉe�s��)�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,			//(out)�v���W�F�N�V�����v�Z����.
		fZOOM,	//y�����̎���(���W�A���w��)������傫�������王�삪�����Ȃ�.
		static_cast<FLOAT>( WND_W ) / static_cast<FLOAT>( WND_H ),//�A�X�y�N�g��(��/����).
		0.1f,				//�߂��r���[���ʂ�z�l.
		fRENDER_LIMIT );	//�����r���[���ʂ�z�l.
}


//�f�o�b�O�p�V�[���؂�ւ�.
void clsSCENE_BASE::DebugChangeScene( enSCENE &enNextScene ) const
{
	if( GetAsyncKeyState( 'Y' ) & 0x1
		|| m_wpXInput->isLTriggerEnter()){
		enNextScene = enSCENE::TITLE;
	}
	else if( GetAsyncKeyState( 'U' ) & 0x1|| m_wpXInput->isRTriggerExit() ){
		enNextScene = enSCENE::ASSEMBLE;
	}
	else if( GetAsyncKeyState( 'I' ) & 0x1 ){
		enNextScene = enSCENE::MISSION;
	}
	else if( GetAsyncKeyState( 'O' ) & 0x1 ){
		enNextScene = enSCENE::ENDING;
	}
	else if( GetAsyncKeyState( 'P' ) & 0x1 ){
		enNextScene = enSCENE::GAMEOVER;
	}
}
