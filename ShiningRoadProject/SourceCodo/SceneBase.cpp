#include "SceneBase.h"
#include "ScreenTexture.h"


using namespace std;

#define XINPUT_ENTER ( XINPUT_START | XINPUT_B )
#define XINPUT_EXIT  ( XINPUT_A )

namespace{

	//���C�g����.
	const D3DXVECTOR3 vLIGHT_DIR = { 0.005f, 0.01f, -0.01f };
	//�J�����̂��.
	const float fZOOM = static_cast<float>( D3DX_PI / 4.0 );
	//�`����E����.
	const float fRENDER_LIMIT = 760.0f;//640.0f;.//150.0f.



	//�{�^���̃z�[���h�t���[��.
	const int iHOLD_FREAM = 30;
	const int iHOLD_FREAM_FIRST = 6;
	const int iHOLD_FREAM_FIRST_STEP = 1;

	const int iNOISE_FRAME_RANGE_RATE = 4;
	const int iNOISE_DOWN_RATE = 2;
	const float fNOISE_BLOCK_RATE = 512.0f;
	const float fNOISE_PULSE_RATE = 0.25f;

	//����ݏ悷�邩 & ������.
	const float fNOISE_ORIGINAL = 2.0f;
	

#ifdef _DEBUG
	const D3DXVECTOR4 vDEBUG_TEXT_COLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	const float fDEBUG_TEXT_SIZE = 50.0f;
#endif//#ifdef _DEBUG
}


//================================//
//========== ���N���X ==========//
//================================//
clsSCENE_BASE::clsSCENE_BASE( clsPOINTER_GROUP* const ptrGroup )
	:m_wpPtrGroup(				ptrGroup )
	,m_wpDevice(				m_wpPtrGroup->GetDevice() )
	,m_wpContext(				m_wpPtrGroup->GetContext() )
	,m_wpViewPort10(			m_wpPtrGroup->GetViewPort10() )
	,m_wpViewPort11(			m_wpPtrGroup->GetViewPort11() )
	,m_wpDxInput(				m_wpPtrGroup->GetDxInput() )
	,m_wpXInput(				m_wpPtrGroup->GetXInput() )
	,m_wpResource(				m_wpPtrGroup->GetResource() )
	,m_wpEffects(				m_wpPtrGroup->GetEffects() )
	,m_wpSound(					m_wpPtrGroup->GetSound() )
	,m_wpCamera(				m_wpPtrGroup->GetCamera() )
	,m_wpRoboStatus(			m_wpPtrGroup->GetRoboStatus() )
	,m_wpBlackScreen(			m_wpPtrGroup->GetBlackScreen() )
	,m_wpFont(					m_wpPtrGroup->GetFont() )
	,m_enNextScene(				enSCENE::NOTHING )
	,m_encNoise(				encNOISE::NOTHING )
	,m_wpViewPortUsing(			m_wpViewPort11 )
	,m_pDepthStencilStateOn(	nullptr )
	,m_pDepthStencilStateOff(	nullptr )
	,m_iNoiseFrame(				0 )
	,m_fBlock(					0.0f )
	,m_fPulse(					0.0f )
	,m_bStopNoiseSe(			false )
	,m_fRenderLimit(			fRENDER_LIMIT )
	,m_fZoom(					fZOOM )
{
}

clsSCENE_BASE::~clsSCENE_BASE()
{
	if( m_wpFont ) m_wpFont->Release();

	//���̃V�[���ɗ]�v�ȃG�t�F�N�g���������܂Ȃ�.
	if( m_wpEffects ) m_wpEffects->StopAll();

	SAFE_RELEASE( m_pDepthStencilStateOff );
	SAFE_RELEASE( m_pDepthStencilStateOn );


	m_enNextScene = enSCENE::NOTHING;

	m_wpFont = nullptr;
	m_wpBlackScreen = nullptr;
	m_wpRoboStatus = nullptr;
	m_wpCamera = nullptr;
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
	m_wpDxInput = nullptr;
	m_wpPtrGroup = nullptr;
	m_wpViewPortUsing = nullptr;
	m_wpViewPort11 = nullptr;
	m_wpViewPort10 = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
}


//�V�[���쐬����ɁuSceneManager.cpp�v�́uSwitchScene�v�֐����Ŏg�p����Ă���.
void clsSCENE_BASE::Create( const HWND hWnd )
{
	if( FAILED( CreateDepthStencilState() ) ){
		assert( !"�f�v�X�X�e���V���쐬���s" );
		return;
	}

	D3DXMatrixIdentity( &m_mView );
	D3DXMatrixIdentity( &m_mProj );	
	m_vLight = vLIGHT_DIR;

	m_wpBlackScreen->SetChangeSpd( fBLACK_SCREEN_DEFAULT_SPD );
	m_wpBlackScreen->GetBright();

#ifdef RENDER_SCREEN_TEXTURE_
	assert( !m_upScreenTexture );
	m_upScreenTexture = make_unique<clsSCREEN_TEXTURE>( hWnd, m_wpContext );
#endif//#ifdef RENDER_SCREEN_TEXTURE_


#ifdef _DEBUG
	//�f�o�b�O�e�L�X�g�̏�����.
	m_upText = make_unique< clsDebugText >();
	if( FAILED( m_upText->Init(
		m_wpContext,
		WND_W, WND_H, fDEBUG_TEXT_SIZE,
		vDEBUG_TEXT_COLOR ) ) )
	{
		assert( !"�f�o�b�O�e�L�X�g�쐬���s" );
	}
#endif//#ifdef _DEBUG

	//�e�V�[����Create.
	CreateProduct();

}

//���[�v���̏���( �������֐����ŕύX����ƍ��̃V�[�����j������A.
//				  �w�肵���V�[������������� ).
void clsSCENE_BASE::Update( enSCENE &enNextScene )
{
//	m_upKey->Update();

	//�T�E���h���[�v.
	m_wpSound->UpdateLoop();

	//enNextScene�͊e�V�[���ɒ��ڂ͓��������Ȃ�.
	enSCENE tmpScene = enNextScene;

	//�Ó]�X�V.
	m_wpBlackScreen->Update();

#ifdef _DEBUG
	//BGM�̃`�F�b�N.
	DebugBgm();
#endif//#ifdef _DEBUG

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

#ifdef RENDER_SCREEN_TEXTURE_	
	if( m_upScreenTexture->isNoiseFlag() ){
		//�m�C�Y���N�����Ȃ�X�V( ���� )����.
		m_upScreenTexture->NoiseUpdate();
	}
	else{
		if( m_bStopNoiseSe ){
			m_bStopNoiseSe = false;
			m_upScreenTexture->StopSe();
		}
	}
#endif//#ifdef RENDER_SCREEN_TEXTURE_
	
	//�f�o�b�O�p�V�[���؂�ւ�.
	DebugChangeScene( enNextScene );
}

//�V�[�����̃I�u�W�F�N�g�̕`��֐��̂܂Ƃ�.
void clsSCENE_BASE::Render( 
	ID3D11RenderTargetView* const pBackBuffer_TexRTV,
	ID3D11DepthStencilView* const pDepthStencilView )
{
	//�J�����֐�.
	Camera();
	//�v���W�F�N�V�����֐�.
	Proj();	


#ifdef RENDER_SCREEN_TEXTURE_	
	if( m_upScreenTexture->isUse() ){
		//Rendertarget���e�N�X�`���ɂ���.
		m_upScreenTexture->SetRenderTargetTexture( pDepthStencilView );
	}
#endif//#ifdef RENDER_SCREEN_TEXTURE_


	//�e�V�[���̕`��.
	SetDepth( true );	//Z�e�X�g:ON.
	RenderProduct( m_wpCamera->GetPos() );
	
	//�G�t�F�N�g�`��.
	m_wpEffects->Render( m_mView, m_mProj, m_wpCamera->GetPos() );

	//���ʂ�̃r���[�|�[�g.
	SetViewPort( m_wpViewPort11, m_wpCamera->GetPos(), m_wpCamera->GetLookPos(), WND_W, WND_H );

	//�e�V�[����UI�̕`��.
	SetDepth( false );
	RenderUi();

	//���ʂ�̃r���[�|�[�g.
	if( m_wpViewPortUsing != m_wpViewPort11 ){
		m_wpViewPortUsing = m_wpViewPort11;
		m_wpContext->RSSetViewports( 1, m_wpViewPort11 );
	}

	//�Ó]�`��.
	SetDepth( true );	//Z�e�X�g:ON.
	m_wpBlackScreen->Render();


#ifdef RENDER_SCREEN_TEXTURE_	
	if( m_upScreenTexture->isUse() ){
		//�e�N�X�`���̓��e����ʂɕ`��.
		m_upScreenTexture->RenderWindowFromTexture( pBackBuffer_TexRTV, pDepthStencilView );

		this->UpdateNoise();
	}

	if( GetAsyncKeyState( 'Z' ) & 0x1 ){
		NoiseStrong( 60 );
	}
	if( GetAsyncKeyState( 'X' ) & 0x8000 ){
		NoiseWeak( 10 );
	}
//	if( GetAsyncKeyState( 'C' ) & 0x1 ){
//		static bool nega = false;
//		m_upScreenTexture->SetNega( nega );
//		nega = !nega;
//	}
//	if( GetAsyncKeyState( 'V' ) & 0x8000 ){
//		m_upScreenTexture->SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } );
//	}
//	if( GetAsyncKeyState( 'B' ) & 0x8000 ){
//		m_upScreenTexture->SetColor( { 0.5f, 0.5f, 1.0f, 1.0f } );
//	}

#endif//#ifdef RENDER_SCREEN_TEXTURE_


#ifdef _DEBUG
	SetDepth( false );
	RenderDebugText();
	SetDepth( true );	//Z�e�X�g:ON.
#endif//#ifdef _DEBUG


}

//���j���[����Ɏg���Ă�.
bool clsSCENE_BASE::isPressRight() const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_RIGHT ) ){
			return true;
		}
		else if( m_wpXInput->isSlopeEnter( XINPUT_RIGHT ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsLSRightEnter() ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_RIGHT ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressLeft() const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_LEFT ) ){
			return true;
		}
		else if( m_wpXInput->isSlopeEnter( XINPUT_LEFT ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsLSLeftEnter() ){
			return true;
		}
	}
	
	if( GetAsyncKeyState( VK_LEFT ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressUp()const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_UP ) ){
			return true;
		}
		else if( m_wpXInput->isSlopeEnter( XINPUT_UP ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsLSUpEnter() ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_UP ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressDown()const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_DOWN ) ){
			return true;
		}
		else if( m_wpXInput->isSlopeEnter( XINPUT_DOWN ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsLSDownEnter() ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_DOWN ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressEnter()const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_ENTER ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKeyEnter( DINPUT_ENTER ) ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_RETURN ) & 0x1 ){
		return true;
	}

	return false;
}
bool clsSCENE_BASE::isPressExit()const
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_EXIT ) ){
			return true;
		}
	}
	else{
		if( m_wpDxInput->IsPressKeyEnter( DINPUT_EXIT ) ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_BACK ) & 0x1 ){
		return true;
	}

	return false;
}

//���j���[����Ɏg���Ă�.
bool clsSCENE_BASE::isPressHoldRight( bool isWithStick )
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_RIGHT ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_RIGHT ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDxInput->IsLSRightStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldRight.iHoldFream ++;
		//�ŏ�.
		if(!m_HoldRight.iFirstPush ){
			m_HoldRight.iFirstPush ++;
			m_HoldRight.iHoldFream = 0;
			return true;
		}
		//��Ԗ�.
		else if(m_HoldRight.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldRight.iHoldFream >= iHOLD_FREAM ){
				m_HoldRight.iFirstPush ++;
				m_HoldRight.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldRight.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldRight.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldRight.iHoldFream = iHOLD_FREAM;
		m_HoldRight.iFirstPush = 0;
	}

	return false;
}
bool clsSCENE_BASE::isPressHoldLeft( bool isWithStick )
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_LEFT ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_LEFT ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDxInput->IsLSLeftStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_LEFT ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldLeft.iHoldFream ++;
		//�ŏ�.
		if(!m_HoldLeft.iFirstPush ){
			m_HoldLeft.iFirstPush ++;
			m_HoldLeft.iHoldFream = 0;
			return true;
		}
		//��Ԗ�.
		else if(m_HoldLeft.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldLeft.iHoldFream >= iHOLD_FREAM ){
				m_HoldLeft.iFirstPush ++;
				m_HoldLeft.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldLeft.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldLeft.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldLeft.iHoldFream = iHOLD_FREAM;
		m_HoldLeft.iFirstPush = 0;
	}

	return false;
}
bool clsSCENE_BASE::isPressHoldUp( bool isWithStick )
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_UP ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_UP ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDxInput->IsLSUpStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_UP ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldUp.iHoldFream ++;
		//�ŏ�.
		if(!m_HoldUp.iFirstPush ){
			m_HoldUp.iFirstPush ++;
			m_HoldUp.iHoldFream = 0;
			return true;
		}
		//��Ԗ�.
		else if(m_HoldUp.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldUp.iHoldFream >= iHOLD_FREAM ){
				m_HoldUp.iFirstPush ++;
				m_HoldUp.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldUp.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldUp.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldUp.iHoldFream = iHOLD_FREAM;
		m_HoldUp.iFirstPush = 0;
	}

	return false;
}
bool clsSCENE_BASE::isPressHoldDown( bool isWithStick )
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_DOWN ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_DOWN ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDxInput->IsLSDownStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldDown.iHoldFream ++;
		//�ŏ�.
		if( !m_HoldDown.iFirstPush ){
			m_HoldDown.iFirstPush ++;
			m_HoldDown.iHoldFream = 0;
			return true;
		}
		//��Ԗ�.
		else if( m_HoldDown.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldDown.iHoldFream >= iHOLD_FREAM ){
				m_HoldDown.iFirstPush ++;
				m_HoldDown.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldDown.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldDown.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldDown.iHoldFream = iHOLD_FREAM;
		m_HoldDown.iFirstPush = 0;
	}

	return false;
}

//3D���W���X�N���[��( 2D )���W�ւƕϊ����� conversion(�ϊ�) dimensions(����).
D3DXVECTOR3 clsSCENE_BASE::ConvDimPos( const D3DXVECTOR3 &v3DPos )const
{
	D3DXVECTOR3 v2DPos;
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	D3DXVec3Project( &v2DPos, &v3DPos, m_wpViewPort10, &m_mProj, &m_mView, &mWorld );
	return v2DPos;
}

//�[�x�e�X�g(Z�e�X�g)ON/OFF�ؑ�.
void clsSCENE_BASE::SetDepth( const bool isOn )const
{
	if( isOn ){
		m_wpContext->OMSetDepthStencilState(
			m_pDepthStencilStateOn, 1 );
	}
	else{
		m_wpContext->OMSetDepthStencilState(
			m_pDepthStencilStateOff, 1 );
	}
}





#ifdef _DEBUG

void clsSCENE_BASE::RenderDebugText()
{
	//NULL�`�F�b�N.
	assert( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//��s���ɂ������ɂ��炷��.

//	sprintf_s( strDbgTxt, 
//		"CameraPos : x[%f], y[%f], z[%f]",
//		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
//
//	sprintf_s( strDbgTxt, 
//		"CamLokPos : x[%f], y[%f], z[%f]",
//		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
//	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );


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


//BGM�̃`�F�b�N.
void clsSCENE_BASE::DebugBgm()
{
	if( !m_wpSound ) return;
	static int iBGM_DEBUG_NUMBER = 0;

	if( GetAsyncKeyState( 'N' ) & 0x1 ){
		if( iBGM_DEBUG_NUMBER > 0 )	iBGM_DEBUG_NUMBER --;
		m_wpSound->StopAllSound();
		m_wpSound->PlayBGM( iBGM_DEBUG_NUMBER );
	}
	if( GetAsyncKeyState( 'M' ) & 0x1 ){
		iBGM_DEBUG_NUMBER ++;
		m_wpSound->StopAllSound();
		m_wpSound->PlayBGM( iBGM_DEBUG_NUMBER );
	}

}


#endif //#ifdef _DEBUG



HRESULT clsSCENE_BASE::CreateDepthStencilState()
{
	assert( !m_pDepthStencilStateOn );
	assert( !m_pDepthStencilStateOff );

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc,
		sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	//ON.
	depthStencilDesc.DepthEnable = true;
	m_wpDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilStateOn );

	//OFF.
	depthStencilDesc.DepthEnable = false;
	m_wpDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilStateOff );

	return S_OK;
}



//�J�����֐�.
void clsSCENE_BASE::Camera()
{
	assert( m_wpCamera );
	//�r���[(�J����)�ϊ�.
	D3DXVECTOR3 vUpVec	( 0.0f, 1.0f, 0.0f );	//����ʒu.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)�r���[�v�Z����.
		&m_wpCamera->GetPos(), &m_wpCamera->GetLookPos(), &vUpVec );

}
//�v���W�F�N�V�����֐�.
void clsSCENE_BASE::Proj()
{
	//�v���W�F�N�V����(�ˉe�s��)�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,			//(out)�v���W�F�N�V�����v�Z����.
		m_fZoom,	//y�����̎���(���W�A���w��)������傫�������王�삪�L���Ȃ邭�Ȃ�.
		static_cast<FLOAT>( WND_W ) / static_cast<FLOAT>( WND_H ),//�A�X�y�N�g��(��/����).
		0.1f,				//�߂��r���[���ʂ�z�l.
		m_fRenderLimit );	//�����r���[���ʂ�z�l.
}


//�f�o�b�O�p�V�[���؂�ւ�.
void clsSCENE_BASE::DebugChangeScene( enSCENE &enNextScene ) const
{
	if( GetAsyncKeyState( 'Y' ) & 0x1 ){
		enNextScene = enSCENE::TITLE;
	}
	else if( GetAsyncKeyState( 'U' ) & 0x1 ){
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

void clsSCENE_BASE::SetViewPort( 
	D3D11_VIEWPORT* const pVp, const 
	D3DXVECTOR3 &vCamPos, const D3DXVECTOR3 &vCamLookPos,
	const float fWndW, const float fWndH,
	const float fRenderLimit )
{
	if( !pVp ) return;
	if( m_wpViewPortUsing == pVp ) return;

	m_wpViewPortUsing = pVp;

	//�r���[(�J����)�ϊ�.
	D3DXVECTOR3 vUpVec	( 0.0f, 1.0f, 0.0f );	//����ʒu.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)�r���[�v�Z����.
		&vCamPos, &vCamLookPos, &vUpVec );

	//�v���W�F�N�V����(�ˉe�s��)�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,			//(out)�v���W�F�N�V�����v�Z����.
		fZOOM,	//y�����̎���(���W�A���w��)������傫�������王�삪�����Ȃ�.
		fWndW / fWndH,//�A�X�y�N�g��(��/����).
		0.1f,				//�߂��r���[���ʂ�z�l.
		fRenderLimit );	//�����r���[���ʂ�z�l.

	assert( m_wpContext );
	m_wpContext->RSSetViewports( 1, m_wpViewPortUsing );

//	//��ʂ̃N���A.
//	m_wpContext->ClearRenderTargetView(
//		pBackBuffer_TexRTV, g_fClearColor );

}




#ifdef RENDER_SCREEN_TEXTURE_	
//�m�C�Y���N����.
void clsSCENE_BASE::NoiseStrong( const int iPower )
{
	assert( m_upScreenTexture );

//	int iBlockFrame = iFrame;
//	const int iFRAME_MAX = 30;
//	if( iBlockFrame > iFRAME_MAX ){
//		iBlockFrame = iFRAME_MAX;
//	}

	m_iNoiseFrame = iPower;

//	//�w��.
//	const float fEXPONENT_ADD = 1.0f;
//	m_fBlock = std::powf( fNOISE_ORIGINAL, fEXPONENT_ADD + iBlockFrame ) / 6.0f;
//	m_fPulse = std::powf( fNOISE_ORIGINAL, fEXPONENT_ADD + iFrame ) * ( fNOISE_PULSE_RATE / fNOISE_BLOCK_RATE );

	m_fBlock = std::sqrtf( static_cast<float>( m_iNoiseFrame ) ) / ( iNOISE_FRAME_RANGE_RATE * iNOISE_DOWN_RATE ) * fNOISE_BLOCK_RATE;
	m_fPulse = std::sqrtf( static_cast<float>( m_iNoiseFrame ) ) / ( iNOISE_FRAME_RANGE_RATE * iNOISE_DOWN_RATE ) * fNOISE_PULSE_RATE;

	m_upScreenTexture->SetNoiseFlag( true );

	m_upScreenTexture->SetBlock( static_cast<int>( m_fBlock ) );
	m_upScreenTexture->SetPulse( m_fPulse );

	m_upScreenTexture->PlaySeStrong();

	m_bStopNoiseSe = true;

	m_encNoise = encNOISE::BLOCK_AND_PULSE;
}
void clsSCENE_BASE::NoiseWeak( const int iFrame )
{
	assert( m_upScreenTexture );

	m_upScreenTexture->SetNoiseFlag( true );
	m_iNoiseFrame = iFrame;

	const int iNOISE_SMALL_BLOCK = 512;
	m_upScreenTexture->SetBlock( iNOISE_SMALL_BLOCK );
	m_upScreenTexture->SetPulse( 0.0f );

	m_upScreenTexture->PlaySeWeak();

	m_bStopNoiseSe = true;

	m_encNoise = encNOISE::MINUTE_BLOCK;
}

void clsSCENE_BASE::UpdateNoise()
{
	if( m_iNoiseFrame > 0 ){
		m_iNoiseFrame --;
	}

	switch( m_encNoise )
	{
	case encNOISE::BLOCK_AND_PULSE:
		//�I��.
		if( m_fBlock <= fNOISE_ORIGINAL ){
			m_upScreenTexture->SetNoiseFlag( false );
			m_encNoise = encNOISE::NOTHING;
		}

		//�m�C�Y����.
		if( m_iNoiseFrame % iNOISE_FRAME_RANGE_RATE == 0 ){
			m_fBlock /= fNOISE_ORIGINAL;
			m_fPulse /= fNOISE_ORIGINAL;
			m_upScreenTexture->SetBlock( static_cast<int>( m_fBlock ) );
			m_upScreenTexture->SetPulse( m_fPulse );
		}
		break;

	case encNOISE::MINUTE_BLOCK:
		if( m_iNoiseFrame <= 0 ){
			m_upScreenTexture->SetNoiseFlag( false );
			m_encNoise = encNOISE::NOTHING;
		}
		break;
	}
}
#endif//#ifdef RENDER_SCREEN_TEXTURE_
