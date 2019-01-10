#include "RenderAtStartUp.h"
#include "BlendState.h"
#include "Singleton.h"

using namespace std;


namespace{
	//==============================================================.
	#ifdef _DEBUG
	const WHSIZE_FLOAT INIT_DISP = { 256, 256 };
	const WHSIZE_FLOAT INIT_ANIM = { 3, 2 };

	const D3DXVECTOR3 vINIT_POS = { WND_W * 0.5f, WND_H * 0.5f, 0.0f };
	const D3DXVECTOR3 vUPDATE_ROT = { 0.0f, 0.0f, 0.0625f };

	const char* cIMAGE_PATH = "Data\\Image\\StartUp\\Gear.png";
	//���Ԑ�.
	const char cSPRITE_MAX = 16;
	#endif//#ifdef _DEBUG
	//==============================================================.



	//==============================================================.
	//�����ʒu.
	const D3DXVECTOR3 vINIT_LINE_BOX_POS = { WND_W * 0.5f, WND_H * 0.5f, 0.0f };
	const D3DXVECTOR3 vINIT_GAGE_BOX_POS = { WND_W * 0.5f, WND_H * 0.75f, 0.0f };


	//�ҋ@����.
	const int iSTOP_TIME_SHORT = 6;
	const int iSTOP_TIME_LONG = 60;

	//�����傫���Ȃ�x��.
	const float fLINE_BOX_ADD_SIZE = 128.0f;
	const float fGAGE_BOX_ADD_SIZE = 64.0f;

	//�ŏI�I�Ȕ��̑傫��.
	const D3DXVECTOR3 vLINE_BOX_SIZE = { WND_W - 32.0f, WND_H - 32.0f, 0.0f };
	const D3DXVECTOR3 vGAGE_BOX_SIZE = { 720.0f, 64.0f, 0.0f };


	//�Q�[�W�{�̂̃p�X.
	const char* cGAGE_PATH = "Data\\Image\\StartUp\\LodeGage.png";
	//�Q�[�W�{�̂̐�.
	const char cGAGE_MAX = 16;
	//�Q�[�W�{�̂̓��ߗ�.
	const float fGAGE_ALPHA_OFFSET = 1.0f / static_cast<float>( cGAGE_MAX );
	//�Q�[�W�T�C�Y.
	const WHSIZE_FLOAT INIT_DISP_GAGE = { 1.0f, 1.0f };
	//�Q�[�W�͘g�̉�����?.
	const float fGAGE_SIZE_RATE = 64.0f;
	//���̑���( �Q�[�W�������ׂ����� ).
	const float fLINE_WIDTH = 1.0f;

	//==============================================================.

	//���[�h������.
	const float fTEXT_SIZE = 8.0f;
	const D3DXVECTOR2 vTEXT_POS = { 270.0f, WND_H * 0.5f + 30.0f };
	const string sTEXT_MESSAGE = "Now Loading...";
	const int iFLASH_RATE = 15;//�_�ŊԊu.


	const string sCOMPLETE_MESSAGE = "Complete!";
	const float fTEXT_POS_X_COMPLETE = 386.0f;

	//==============================================================.

	const char* sBLACK_PATH = "Data\\Image\\BlackScreen.png";

	//==============================================================.

	//���S.
	const WHSIZE_FLOAT LOGO_SIZE = { 960.0f, 640.0f };
	const float fLOGO_SCALE_RATE = 0.5f;
	const D3DXVECTOR3 vLOGO_SCALE = { LOGO_SIZE.w * fLOGO_SCALE_RATE, LOGO_SIZE.h * fLOGO_SCALE_RATE, 0.0f };
	const char* sLOGO_PATH = "Data\\Image\\TitleUi\\TitleLogo.png";



}

clsRENDER_AT_START_UP::clsRENDER_AT_START_UP(	
	ID3D11Device*			const pDevice,
	ID3D11DeviceContext*	const pContext,
	IDXGISwapChain*			const pSwapChain,
	ID3D11RenderTargetView*	const pBackBuffer_TexRTV,
	ID3D11DepthStencilView*	const pBackBuffer_DSTexDSV )
	:m_wpDevice(pDevice)
	,m_wpContext( pContext )
	,m_wpSwapChain( pSwapChain )
	,m_wpBackBuffer_TexRTV( pBackBuffer_TexRTV )
	,m_wpBackBuffer_DSTexDSV( pBackBuffer_DSTexDSV )
	,m_pDepthStencilStateOn( nullptr )
	,m_pDepthStencilStateOff( nullptr )
	,m_bEnd( false )
	,m_enMode( enMODE::LINE_V )
	,m_iTimer( 0 )
{
	//���̃N���X�̏��������ɊD�F��ʂ��o��̂�h��.
	Render( false );

//	clsBLEND_STATE* pBlend = &clsSINGLETON<clsBLEND_STATE>::GetInstance();
//	pBlend->Create( m_wpDevice, m_wpContext );

	CreateDepthStencilState();

	SPRITE_STATE ss;
	ss.Disp = INIT_DISP_GAGE;

	m_vecupGage.resize( cGAGE_MAX );
	for( char i=0; i<cGAGE_MAX; i++ ){
		m_vecupGage[i] = make_unique< clsSPRITE2D_CENTER >();
		m_vecupGage[i]->Create( m_wpDevice, m_wpContext, cGAGE_PATH, ss );

		m_vecupGage[i]->SetScale( 0.0f );

		float tmp = 1.0f - ( fGAGE_ALPHA_OFFSET * static_cast<float>( i ) );
		m_vecupGage[i]->SetAlpha( tmp );
	}



	if( !m_upLineBox ){
		m_upLineBox = make_unique< clsLINE_BOX >( pDevice, pContext );
		m_upLineBox->SetPos( vINIT_LINE_BOX_POS );
		m_upLineBox->SetSize( { 0.0f, 0.0f, 0.0f } );
	}

	if( !m_upGageBox ){
		m_upGageBox = make_unique< clsLINE_BOX >( pDevice, pContext );
		m_upGageBox->SetPos( vINIT_GAGE_BOX_POS );
		m_upGageBox->SetSize( { 0.0f, 0.0f, 0.0f } );
	}

	if( !m_upBlack ){
		m_upBlack = make_unique< clsBLACK_SCREEN >();
		ss.Disp = { WND_W, WND_H };
		m_upBlack->Create( pDevice, pContext, sBLACK_PATH, ss );
		m_upBlack->SetPos( { 0.0f, 0.0f, 0.0f } );
		m_upBlack->SetAlpha( 0.0f );
	}

	if( !m_upText ){
		m_upText = make_unique< clsUiText >();
		m_upText->Create( pContext, WND_W, WND_H, fTEXT_SIZE );
		m_upText->SetPos( vTEXT_POS );
	}

	if( !m_upLogo ){
		m_upLogo = make_unique< clsSPRITE2D_CENTER >();
		ss.Disp = LOGO_SIZE;
		m_upLogo->Create( pDevice, pContext, sLOGO_PATH, ss );
		const D3DXVECTOR3 vLOGO_POS = { static_cast<float>( WND_W ) * 0.5f, 250.0f, 0.0f };
		m_upLogo->SetPos( vLOGO_POS );
		m_upLogo->SetScale( { 0.0f, LOGO_SIZE.h, 0.0f } );
	}


#ifdef _DEBUG
	ss.Disp = INIT_DISP;
	ss.Anim = INIT_ANIM;
	m_vecupDebugImage.reserve( cSPRITE_MAX );
	for( char i=0; i<cSPRITE_MAX; i++ ){
		m_vecupDebugImage.push_back( nullptr );
		m_vecupDebugImage[i] = make_unique< clsSPRITE2D_CENTER >();
		m_vecupDebugImage[i]->Create( m_wpDevice, m_wpContext, cIMAGE_PATH, ss );

	//	m_upRogo->SetPos( vINIT_POS );
		float tmpY;
		if( i%2 )	tmpY = -128.0f;
		else		tmpY = WND_H * 0.5f;
		m_vecupDebugImage[i]->SetPos( { -128.0f, tmpY, 0.0f } );//256size.
		m_vecupDebugImage[i]->AddPos( { 96.0f*i, 32.0f*i, 0.0f } );
		m_vecupDebugImage[i]->SetAnim( { static_cast<float>( i % 3 ), static_cast<float>( i % 2 ) } );
		float tmpAlpha;
		if( i%2 )	tmpAlpha = 0.75f;
		else		tmpAlpha = 0.5f;
		m_vecupDebugImage[i]->SetAlpha( tmpAlpha );
	}
#endif//#ifdef _DEBUG

}

clsRENDER_AT_START_UP::~clsRENDER_AT_START_UP()
{
	End();

	if( m_upBlack ){
		m_upBlack.reset( nullptr );
	}

	if( m_upText ){
		m_upText.reset( nullptr );
	}

	for( unsigned int i=0; i<m_vecupGage.size(); i++ ){
		if( m_vecupGage[i] ){
			m_vecupGage[i].reset( nullptr );
		}
	}
	m_vecupGage.clear();
	m_vecupGage.shrink_to_fit();

#ifdef _DEBUG
	for( unsigned int i=0; i<m_vecupDebugImage.size(); i++ ){
		if( m_vecupDebugImage[i] ){
			m_vecupDebugImage[i].reset( nullptr );
		}
	}
	m_vecupDebugImage.clear();
	m_vecupDebugImage.shrink_to_fit();
#endif//#ifdef _DEBUG

	if( m_upLineBox ){
		m_upLineBox.reset( nullptr );
	}

	if( m_upGageBox ){
		m_upGageBox.reset( nullptr );
	}

	m_bEnd = false;

	SAFE_RELEASE( m_pDepthStencilStateOff );
	SAFE_RELEASE( m_pDepthStencilStateOn );
	m_wpBackBuffer_DSTexDSV = nullptr;
	m_wpBackBuffer_TexRTV = nullptr;	
	m_wpSwapChain = nullptr;			
	m_wpContext = nullptr;		
	m_wpDevice = nullptr;
}


//���[�v����.
void clsRENDER_AT_START_UP::Loop()
{
	//----------------------------------------------------------
	//	�t���[�����[�g.
	//----------------------------------------------------------
	float fRate		= 0.0f;	//���[�g.
	float fFPS		= 60.0f;//FPS�l.
	DWORD sync_old	= timeGetTime();	//�ߋ�����.
	DWORD sync_now;
	//���ԏ����ׁ̈A�ŏ��P�ʂ�1�~���b�ɕύX.
	timeBeginPeriod( 1 );

	while( !m_bEnd )
	{
		Sleep( 1 );
		sync_now = timeGetTime();	//���ݎ��Ԃ��擾.

		fRate = 1000.0f / fFPS;	//���z���Ԃ��Z�o.

		if( sync_now - sync_old >= fRate ){
			sync_old = sync_now;	//���ݎ��Ԃɒu������.

			Update();
		}
	}
	timeEndPeriod( 1 );	//����.
}


void clsRENDER_AT_START_UP::Update()
{
#ifdef _DEBUG
	for( unsigned int i=0; i<m_vecupDebugImage.size(); i++ ){
		m_vecupDebugImage[i]->AddRot( vUPDATE_ROT*( (i+1) * 0.5f) );
	}
#endif//#ifdef _DEBUG


	switch( m_enMode )
	{
	case enMODE::LINE_V://�O�g���c�ɑ傫���Ȃ�.
		BiggerLineBoxV();
		break;
	
	case enMODE::LINE_H://�O�g�����ɑ傫���Ȃ�.
		BiggerLineBoxH();
		break;
	
	case enMODE::GAGE_H://�Q�[�W�̘g�����ɑ傫���Ȃ�.
		BiggerGageBoxH();
		break;
	
	case enMODE::GAGE_V://�Q�[�W�̘g���c�ɑ傫���Ȃ�.
		BiggerGageBoxV();
		break;
	
	case enMODE::GAGE_MOVE://�Q�[�W������.
		UpdateLoadMsg();
		break;
	
	case enMODE::COMPLETE://�Q�[�W������.
		Complete();
		break;
	}
	m_iTimer ++;

	m_upLineBox->Update();
	m_upGageBox->Update();

	Render();

}

//�N�����̕`��.
void clsRENDER_AT_START_UP::Render( bool isLoop ) const
{
	//��ʂ̃N���A.
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//�N���A�F(RGBA��)(0.0f~1.0f).
	//�J���[�o�b�N�o�b�t�@.
	m_wpContext->ClearRenderTargetView(
		m_wpBackBuffer_TexRTV, ClearColor );
	//�f�v�X�X�e���V���r���[�o�b�N�o�b�t�@.
	m_wpContext->ClearDepthStencilView(
		m_wpBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );

	SetDepth( false );	//Z�e�X�g:OFF.

	if( isLoop ){
#ifdef _DEBUG
		for( unsigned int i=0; i<m_vecupDebugImage.size(); i++ ){
			m_vecupDebugImage[i]->Render();
		}

#endif//#ifdef _DEBUG

		m_upLineBox->Render();

		for( unsigned int i=0; i<m_vecupGage.size(); i++ ){
			//�g�̒������`�悷��.
			if( m_vecupGage[i]->GetPos().x < m_upGageBox->GetPos().x - ( m_upGageBox->GetSize().x * 0.5f ) ||
				m_vecupGage[i]->GetPos().x > m_upGageBox->GetPos().x + ( m_upGageBox->GetSize().x * 0.5f ) )
			{
				continue;
			}
			m_vecupGage[i]->Render();
		}
		m_upGageBox->Render();

		m_upLogo->Render();

		m_upText->Render();

		m_upBlack->Render();

	}
	SetDepth( true );	//Z�e�X�g:ON.

	//�����_�����O���ꂽ�C���[�W��\��.
	m_wpSwapChain->Present( 0, 0 );
}



//�d���̏I���.
void clsRENDER_AT_START_UP::FinishLoad()
{
	m_enMode = enMODE::COMPLETE;
	m_iTimer = 0;
	//�Ó]�J�n.
	m_upBlack->GetDark();
	//�e�L�X�g�X�V.
	m_sLodeMsg = sCOMPLETE_MESSAGE;
	m_upText->SetText( m_sLodeMsg.c_str() );
	m_upText->SetPos( { fTEXT_POS_X_COMPLETE, vTEXT_POS.y } );

	//�ז��ȓz�͏���.
	for( unsigned int i=0; i<m_vecupGage.size(); i++ ){
		m_vecupGage[i]->SetAlpha( 0.0f );
	}
	//�_�ł�����͈̂�ŏ\��.
	m_vecupGage[0]->SetPos( { m_upGageBox->GetPos().x, m_upGageBox->GetPos().y, 0.0f } );
	m_vecupGage[0]->SetScale( { m_upGageBox->GetSize().x, m_upGageBox->GetSize().y, 0.0f } );

}


//switch���̒��g.
//�O�g���c�ɑ傫���Ȃ�.
void clsRENDER_AT_START_UP::BiggerLineBoxV()
{
	if( m_iTimer < iSTOP_TIME_LONG ) return;

	m_upLineBox->AddSize( { 0.0f, fLINE_BOX_ADD_SIZE, 0.0f } );

	if( m_upLineBox->GetSize().y >= vLINE_BOX_SIZE.y ){
		m_upLineBox->SetSize( { 0.0f, vLINE_BOX_SIZE.y, 1.0f } );
		m_enMode = enMODE::LINE_H;
		m_iTimer = 0;
	}
}

//�O�g�����ɑ傫���Ȃ�.
void clsRENDER_AT_START_UP::BiggerLineBoxH()
{
	if( m_iTimer < iSTOP_TIME_SHORT ) return;

	m_upLineBox->AddSize( { fLINE_BOX_ADD_SIZE, 0.0f, 0.0f } );

	m_upLogo->SetScale( { 
		vLOGO_SCALE.x * ( m_upLineBox->GetSize().x / vLINE_BOX_SIZE.x ) / LOGO_SIZE.w, 
		vLOGO_SCALE.y / LOGO_SIZE.h, 
		0.0f } );

	if( m_upLineBox->GetSize().x >= vLINE_BOX_SIZE.x ){
		m_upLineBox->SetSize( { vLINE_BOX_SIZE.x, vLINE_BOX_SIZE.y, 1.0f } );
		m_upLogo->SetScale( { vLOGO_SCALE.x / LOGO_SIZE.w, vLOGO_SCALE.y / LOGO_SIZE.h, 0.0f } );
		m_enMode = enMODE::GAGE_H;
		m_iTimer = 0;
	}
}

//�Q�[�W�̘g�����ɑ傫���Ȃ�.
void clsRENDER_AT_START_UP::BiggerGageBoxH()
{
	if( m_iTimer < iSTOP_TIME_LONG ) return;

	m_upGageBox->AddSize( { fGAGE_BOX_ADD_SIZE, 0.0f, 0.0f } );

	if( m_upGageBox->GetSize().x >= vGAGE_BOX_SIZE.x ){
		m_upGageBox->SetSize( { vGAGE_BOX_SIZE.x, 0.0f, 1.0f } );
		m_enMode = enMODE::GAGE_V;
		m_iTimer = 0;
	}
}

//�Q�[�W�̘g���c�ɑ傫���Ȃ�.
void clsRENDER_AT_START_UP::BiggerGageBoxV()
{
	if( m_iTimer < iSTOP_TIME_SHORT ) return;

	m_upGageBox->AddSize( { 0.0f, fGAGE_BOX_ADD_SIZE, 0.0f } );

	if( m_upGageBox->GetSize().y >= vGAGE_BOX_SIZE.y ){
		m_upGageBox->SetSize( { vGAGE_BOX_SIZE.x, vGAGE_BOX_SIZE.y, 1.0f } );
		m_enMode = enMODE::GAGE_MOVE;
		m_iTimer = 0;

		//�Q�[�W�̏�����.
		for( unsigned int i=0; i<m_vecupGage.size(); i++ ){
			m_vecupGage[i]->SetScale( {
				m_upGageBox->GetSize().x / fGAGE_SIZE_RATE - fLINE_WIDTH, m_upGageBox->GetSize().y, 0.0f } );
			m_vecupGage[i]->SetPos( {
				WND_W - ( m_vecupGage[i]->GetScale().x * static_cast<float>( i ) ), 
				m_upGageBox->GetPos().y, 
				0.0f } );
		}
	}
}

//�Q�[�W������.
void clsRENDER_AT_START_UP::UpdateLoadMsg()
{
//	if( m_iTimer >= iSTOP_TIME_SHORT ){ 
		//�Q�[�W�̓���.
		for( unsigned int i=0; i<m_vecupGage.size(); i++ ){
			m_vecupGage[i]->AddPos( { m_vecupGage[i]->GetScale().x, 0.0f, 0.0f } );
			if( m_vecupGage[i]->GetPos().x > WND_W ){
				m_vecupGage[i]->SetPos( { 
					m_upGageBox->GetPos().x - ( m_upGageBox->GetSize().x * 0.5f ) + fLINE_WIDTH, 
					m_upGageBox->GetPos().y,
					0.0f } );
			}
		}
//	}

	//���b�Z�[�W�ύX.
	switch( m_iTimer )
	{
	case iSTOP_TIME_SHORT:
		m_sLodeMsg = "";
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 2://N.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 1 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 3://No.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 2 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 4://Now.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 3 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 5://Now L.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 5 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 6://Now Lo.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 6 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 7://Now Loa.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 7 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 8://Now Load.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 8 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 9://Now Loadi.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 9 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 10://Now Loadin.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 10 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 11://Now Loading.
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 11 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 12://Now Loading..
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 12 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 13://Now Loading...
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 13 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 14://Now Loading....
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 14 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 15://.
		m_sLodeMsg = "";
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 16://Now Loading....
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 14 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 17://.
		m_sLodeMsg = "";
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 18://Now Loading....
		m_sLodeMsg = "";
		m_sLodeMsg = sTEXT_MESSAGE.substr( 0, 14 );
		break;

	case iSTOP_TIME_SHORT + iFLASH_RATE * 19:
		m_sLodeMsg = "";
		m_iTimer = iSTOP_TIME_SHORT;
		break;
	}
	m_upText->SetText( m_sLodeMsg.c_str() );

}

//���[�h������.
void clsRENDER_AT_START_UP::Complete()
{
	//�Ó].
	m_upBlack->Update();

	//�Q�[�W�_��.
	if( m_vecupGage[0]->GetAlpha() )m_vecupGage[0]->SetAlpha( 0.0f );
	else							m_vecupGage[0]->SetAlpha( 1.0f );
	
	//�Ó]���I���ΏI���.
	if( m_upBlack->isDarkEnd() ){
		End();
	}

}


HRESULT clsRENDER_AT_START_UP::CreateDepthStencilState()
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


//�[�x�e�X�g(Z�e�X�g)ON/OFF�ؑ�.
void clsRENDER_AT_START_UP::SetDepth( bool isOn ) const
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

