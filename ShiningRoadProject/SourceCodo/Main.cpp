#include "Main.h"
#include "RenderAtStartUp.h"
#include "Game.h"
#include "Singleton.h"
#include "BlendState.h"
#include <stdio.h>
#include <thread>

using namespace std;

//============================================================
//	�萔.
//============================================================
#define WND_TITLE	"Gigant Weapons"
#define APR_NAME	"Gigant Weapons"


//��������������������������������������������
//�O���[�o���ϐ�.
//��������������������������������������������
unique_ptr< clsMain >	g_upMain;


//============================================================
//	���C���֐�.
//============================================================
INT WINAPI WinMain(
	HINSTANCE hInstance,		//�C���X�^���ԍ�(�E�B���h�E�̔ԍ�).
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	INT nCmdShow )
{
	// ���������[�N���o
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

//	g_pClsMain = new clsMain;	//������&�N���X�̐錾.
	g_upMain = make_unique< clsMain >();

	//�N���X�����݂��Ă��邩�`�F�b�N.
	if( g_upMain != nullptr ){
		//�E�B���h�E�쐬����.
		if( SUCCEEDED( g_upMain->InitWindow(
			hInstance,
			64, 64,
			WND_W, WND_H,
			WND_TITLE ) ) )
		{
			//Dx11�p�̏�����
			if( SUCCEEDED( g_upMain->InitD3D() ) ){
				g_upMain->CreateResource();
				//���b�Z�[�W���[�v.
				g_upMain->Loop();
			}
		}
		//�I��.
		g_upMain->DestroyD3D();//Direct3D�̉��.

		g_upMain.reset( nullptr );
	}

	return 0;
}


//============================================================
//	�E�B���h�E�v���V�[�W��.
//============================================================
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam )
{
	//�v���V�[�W��.
	return g_upMain->MsgProc( hWnd, uMsg, wParam, lParam );
}


//============================================================
//	���C���N���X.
//============================================================

//============================================================
//	�R���X�g���N�^.
//============================================================
clsMain::clsMain()
	:m_hWnd( nullptr )
	,m_pDevice( nullptr )
	,m_pDeviceContext( nullptr )
	,m_pSwapChain( nullptr )
	,m_pBackBuffer_TexRTV( nullptr )
	,m_pBackBuffer_DSTex( nullptr )
	,m_pBackBuffer_DSTexDSV( nullptr )
	,m_spViewPort10( nullptr )
	,m_spViewPort11( nullptr )
{
}

//============================================================
//	�f�X�g���N�^.
//============================================================
clsMain::~clsMain()
{
	DestroyD3D();
}

//============================================================
//�E�B���h�E�������֐�.
//============================================================
HRESULT clsMain::InitWindow(
	HINSTANCE hInstance,	//�C���X�^���X.
	INT x, INT y,			//�E�B���h�E��x,y���W.
	INT width, INT height,	//�E�B���h�E�̕�,����.
	LPSTR WindowName )		//�E�B���h�E��.
{
	//�E�B���h�E�̒�`.
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) );//������.

	wc.cbSize			= sizeof( wc );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	wc.lpszClassName	= APR_NAME;
	wc.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );

	//�E�B���h�E�N���X��Windows�ɓo�^.
	if( !RegisterClassEx( &wc ) ){
		MessageBox( NULL, "�E�B���h�E�N���X�m�o�^�Ƀ~�X", "clsMainInitWindow", MB_OK );
		return E_FAIL;
	}

	//�E�B���h�E�̍쐬.
	m_hWnd = CreateWindow(
		APR_NAME,			//�A�v����.
		WindowName,			//�E�B���h�E�^�C�g��.
		WS_OVERLAPPEDWINDOW,//�E�B���h�E���.
		x, y,				//�\�����W.
		width, height,		//�E�B���h�E�̕�,����.
		NULL,				//�e�E�B���h�E�n���h��.
		NULL,				//���j���[�ݒ�.
		hInstance,			//�C���X�^���X�ԍ�.
		NULL );				//�E�B���h�E�쐬���ɔ�������C�x���g�ɓn���f�[�^.

	if( !m_hWnd ){
		MessageBox( NULL, "�E�B���h�E�쐬�Ƀ~�X", "clsMain::InitWindow", MB_OK );
		return E_FAIL;
	}
#ifdef Inoue
#ifdef _DEBUG
	DragAcceptFiles(
		m_hWnd,    // �o�^����E�B���h�E
		true // �A�N�Z�v�g�I�v�V����
	   );
#endif	//#ifdef _DEBUG
#endif//#ifdef Inoue

	//�E�B���h�E�̕\��.
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	//�}�E�X�J�[�\���̔�\��.
#ifndef _DEBUG
	ShowCursor( false );
#endif//#ifndef _DEBUG

	return S_OK;
}

//============================================================
//	�E�B���h�E�֐�(���b�Z�[�W���̏���).
//============================================================
LRESULT clsMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_DESTROY:	//�E�B���h�E�j����.
		//�A�v���P�[�V�����̏I����Windows�ɒʒm����.
		PostQuitMessage( 0 );
		break;
	case WM_DROPFILES:
//		{
//		HDROP hDrop;
//		UINT uiFileNo;
//		g_bFileActive = true;
//		hDrop = (HDROP)wParam;
//		uiFileNo = DragQueryFile( (HDROP)wParam, 0xFFFFFFFF, NULL, 0 );
//		char Path[MAX_PATH];
//		for (UINT i = 0; i < uiFileNo; i++){
//			DragQueryFile( hDrop, i, Path, sizeof( Path ) );
//			g_vsFilePath.push_back( Path );
//		}
//		DragFinish( hDrop );
//		}
		break;
	case WM_KEYDOWN:	//�L�[�{�[�h�������ꂽ�Ƃ�.
		//�L�[�ʂ̏���.
		switch( (char)wParam )
		{
		case VK_ESCAPE:	//ESC�L�[.
			if( MessageBox( NULL,
				"�Q�[�����I�����܂���?", "Message",
				MB_YESNO ) == IDYES )
			{
				//�E�B���h�E��j������.
				DestroyWindow( hWnd );
			}
			break;
		}
		break;
	}

	//���C���ɕԂ����.
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


void clsMain::CreateResource()
{
	//�N�����̕`��.
	unique_ptr< clsRENDER_AT_START_UP > upRenderAtStartUp =
		make_unique< clsRENDER_AT_START_UP >( 
			m_pDevice,
			m_pDeviceContext,
			m_pSwapChain,
			m_pBackBuffer_TexRTV,
			m_pBackBuffer_DSTexDSV );
	//�ʃX���b�h�ŕ`��.
	thread thStartUpRender( &clsRENDER_AT_START_UP::Loop, upRenderAtStartUp.get() );


	//���b�V���ǂݍ��݊֐����܂Ƃ߂�����.
	ReadMesh();

	//�K�v�Ȃ��Ȃ����̂ŕ���.
	upRenderAtStartUp->FinishLoad();
	thStartUpRender.join();
	upRenderAtStartUp.reset();

	//���[�h��ʏI�����Create���Ȃ��ƌ��ʉ��̃^�C�~���O�����������Ȃ�.
	m_upGame->Create();

}


//============================================================
//	���b�Z�[�W���[�v�ƃA�v���P�[�V���������̓����.
//============================================================
void clsMain::Loop()
{
	//----------------------------------------------------------
	//	�t���[�����[�g.
	//----------------------------------------------------------
	float fRate		= 0.0f;	//���[�g.
	DWORD sync_old	= timeGetTime();	//�ߋ�����.
	DWORD sync_now;
	//���ԏ����ׁ̈A�ŏ��P�ʂ�1�~���b�ɕύX.
	timeBeginPeriod( 1 );

	//���b�Z�[�W���[�v.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof( msg ) );

	//�I���t���O.
	bool isExitApp = true;

	while( msg.message != WM_QUIT && isExitApp )
	{
		Sleep( 1 );
		sync_now = timeGetTime();	//���ݎ��Ԃ��擾.

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		fRate = 1000.0f / g_fFPS;	//���z���Ԃ��Z�o.

		if( sync_now - sync_old >= fRate ){
			sync_old = sync_now;	//���ݎ��Ԃɒu������.

			//�A�v���P�[�V���������͂���������.
			isExitApp = AppMain();
		}
#ifdef STARTUP_FULLSCREEN_
		if( GetAsyncKeyState( VK_F4 ) & 0x1 )ChangeWindowMode();
#endif//#ifdef STARTUP_FULLSCREEN_

	}
	//�A�v���P�[�V�����̏I��.
	timeEndPeriod( 1 );	//����.
}

//============================================================
//	�A�v���P�[�V�������C������.
//============================================================
bool clsMain::AppMain()
{
	bool isExitApp;

	//�Q�[�����[�v.
	assert( m_upGame );
	isExitApp = m_upGame->Update();

	//�����_�����O.
	Render();

	return isExitApp;
}

//============================================================
//	�`��(�����_�����O).
//	�V�[��(���)����ʂɃ����_�����O(�`��).
//	(��DX9MESH����Main����2���݂���̂Œ���).
//============================================================
void clsMain::Render()
{
	//����Render�֐��̑O��AppMain�֐��Ń`�F�b�N���Ă���̂ŃA�T�[�g�͏Ȃ�.
	m_upGame->Render( m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV );
	
	//�����_�����O���ꂽ�C���[�W��\��.
	m_pSwapChain->Present( 0, 0 );

}



//============================================================
//	DirectX������.
//============================================================
HRESULT clsMain::InitD3D()
{
	//---------------------------------------------------
	//	�f�o�C�X�ƃX���b�v�`�F�[���֌W.
	//---------------------------------------------------

	//�X���b�v�`�F�[���\����.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount		= 1;		//�o�b�N�o�b�t�@�̐�.
	sd.BufferDesc.Width	= WND_W;
	sd.BufferDesc.Height= WND_H;
	sd.BufferDesc.Format= DXGI_FORMAT_R8G8B8A8_UNORM;
									//�t�H�[�}�b�g(32�r�b�g�J���[).
	sd.BufferDesc.RefreshRate.Numerator = 60;
									//���t���b�V�����[�g(����) ��FPS:60.
	sd.BufferDesc.RefreshRate.Denominator = 1;
									//���t���b�V�����[�g(���q).
	sd.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
									//�g����(�\����).
	sd.OutputWindow		= m_hWnd;	//�E�B���h�E�n���h��.
	sd.SampleDesc.Count	= 1;		//�}���`�T���v���̐�.
	sd.SampleDesc.Quality=0;		//�}���`�T���v���̃N�I���e�B.
	sd.Windowed			= TRUE;		//�E�B���h�E���[�h(�t���X�N����FALSE).


	//�쐬�����݂�@�\���x���̗D����w��.
	//	(GPU���T�|�[�g����@�\�Z�b�g�̒�`).
	//	D3D_FEATURE_LEVEL�񋓑̂̔z��.
	//	D3D_FEATURE_LEVEL_10_1:Direct3D 10.1��GPU���x��.
	D3D_FEATURE_LEVEL	pFeatureLevels	= D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL*	pFeatureLevel	= nullptr;	//�z��̗v�f��.


	//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬.
	//	�n�[�h�E�F�A(GPU)�f�o�C�X�ō쐬.
	if( FAILED( D3D11CreateDeviceAndSwapChain(
				NULL,					//�r�f�I�A�_�v�^�ւ̃|�C���^.
				D3D_DRIVER_TYPE_HARDWARE,//�쐬����f�o�C�X�̎��.
				NULL,					//�\�t�g�E�F�A���X�^���C�U�[����������DLL�̃n���h��.
				0,						//�L���ɂ��郉���^�C�����C���[.
				&pFeatureLevels,		//�쐬�����݂�@�\���x���̏������w�肷��z��.
				1,						//���̗v�f��.
				D3D11_SDK_VERSION,		//SDK�̃o�[�W����.
				&sd,					//�X���b�v�`�F�[���̏������p�����[�^�̃|�C���^.
				&m_pSwapChain,			//(out)�����_�����O�Ɏg�p���ꂽ�X���b�v�`�F�[��.
				&m_pDevice,				//(out)�쐬���ꂽ�f�o�C�X.
				pFeatureLevel,			//�@�\���x���̔z��ɂ���ŏ��̗v�f��\���|�C���^.
				&m_pDeviceContext ) ) )	//(out)�f�o�C�X�R���e�L�X�g.
	{
		//WARP�f�o�C�X�̍쐬.
		// D3D_FEATURE_LEVEL_9_1 �` D3D_FEATURE_LEVEL_10_1.
		if( FAILED( D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_WARP,
				NULL, 0, &pFeatureLevels, 1,
				D3D11_SDK_VERSION, &sd,
				&m_pSwapChain, &m_pDevice,
				pFeatureLevel, &m_pDeviceContext ) ) )
		{
			//���t�@�����X�f�o�C�X�̍쐬.
			//	DirectX SDK���C���X�g�[������Ă��Ȃ��Ǝg���Ȃ�.
			if( FAILED( D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_REFERENCE,
				NULL, 0, &pFeatureLevels, 1,
				D3D11_SDK_VERSION, &sd,
				&m_pSwapChain, &m_pDevice,
				pFeatureLevel, &m_pDeviceContext ) ) )
			{
				MessageBox( NULL, "�f�o�C�X�ƃX���b�v�`�F�[���̍쐬�Ƀ~�X", "error(main.cpp)", MB_OK );
				return E_FAIL;
			}
		}
	}


	//�e��e�N�X�`���[�ƁA����ɕt�т���e��r���[(���)���쐬.

	//---------------------------------------------------
	//	�o�b�N�o�b�t�@����:�J���[�o�b�t�@�ݒ�.
	//---------------------------------------------------

	//�o�b�N�o�b�t�@�e�N�X�`���[���擾(���ɂ���̂ō쐬�͂Ȃ�).
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),	//__uuidof:���Ɋ֘A�t����GUID���擾.
									//		   Texture2D�̗B��̕��Ƃ��Ĉ���.
		(LPVOID*)&pBackBuffer_Tex );//(out)�o�b�N�o�b�t�@�e�N�X�`��.

	//���̃e�N�X�`���[�ɑ΂������_�[�^�[�Q�b�g�r���[(RTV)���쐬.
	m_pDevice->CreateRenderTargetView(
		pBackBuffer_Tex,
		NULL,
		&m_pBackBuffer_TexRTV );//(out)RTV.
	//�o�b�N�o�b�t�@�e�N�X�`���[�����.
	SAFE_RELEASE( pBackBuffer_Tex );//�}�N�����ɃZ�~�R����������̂ł���Ȃ��������ڂ̓���ł��Ă���.

	//---------------------------------------------------
	//	�o�b�N�o�b�t�@����:�f�v�X(�[�x)�X�e���V���֌W.
	//---------------------------------------------------

	//�f�v�X(�[��or�[�x)�X�e���V���r���[�p�̃e�N�X�`���[���쐬.
	D3D11_TEXTURE2D_DESC descDepth;

	descDepth.Width				= WND_W;					//��.
	descDepth.Height			= WND_H;					//����.
	descDepth.MipLevels			= 1;						//�~�b�v�}�b�v���x��:1.
	descDepth.ArraySize			= 1;						//�z��:1.
	descDepth.Format			= DXGI_FORMAT_D32_FLOAT;	//32�r�b�g�t�H�[�}�b�g.
	descDepth.SampleDesc.Count	= 1;						//�}���`�T���v���̐�.
	descDepth.SampleDesc.Quality= 0;						//�}���`�T���v���̃N�I���e�B.
	descDepth.Usage				= D3D11_USAGE_DEFAULT;		//�g�p���@:�f�t�H���g.
	descDepth.BindFlags			= D3D11_BIND_DEPTH_STENCIL;	//�[�x(�X�e���V���Ƃ��Ďg�p).
	descDepth.CPUAccessFlags	= 0;						//CPU����A�N�Z�X���Ȃ�.
	descDepth.MiscFlags			= 0;						//���̑��ݒ�Ȃ�.

	m_pDevice->CreateTexture2D(
		&descDepth,
		NULL,
		&m_pBackBuffer_DSTex );//(out)�f�v�X�X�e���V���p�e�N�X�`��.

	//���̃e�N�X�`���ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬.
	m_pDevice->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		NULL,
		&m_pBackBuffer_DSTexDSV );//(out)DSV.

	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���p�C�v���C���ɃZ�b�g.
	m_pDeviceContext->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV );


//	//�[�x�e�X�g(Z�e�X�g)��L���ɂ���.
//	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
//	ZeroMemory( &depthStencilDesc,
//		sizeof( D3D11_DEPTH_STENCIL_DESC ) );
//	depthStencilDesc.DepthEnable = true;
//
//	if( SUCCEEDED( m_pDevice->CreateDepthStencilState(
//		&depthStencilDesc, &m_spDepthStencilState ) ) )
//	{
//		m_pDeviceContext->OMSetDepthStencilState(
//			m_spDepthStencilState, 1 );
//	}

	//�r���[�|�[�g�̐ݒ�.
	if( !m_spViewPort11 ){
		m_spViewPort11 = new D3D11_VIEWPORT;
	}
	m_spViewPort11->Width		= WND_W;//��.
	m_spViewPort11->Height		= WND_H;//����.
	m_spViewPort11->MinDepth	= 0.0f;	//�ŏ��[�x(��O).
	m_spViewPort11->MaxDepth	= 1.0f;	//�ő�[�x(��).
	m_spViewPort11->TopLeftX	= 0.0f;	//����ʒux.
	m_spViewPort11->TopLeftY	= 0.0f;	//����ʒuy.
	m_pDeviceContext->RSSetViewports( 1, m_spViewPort11 );

	//Sp2D�p.
	SetViewPort10( m_spViewPort11 );

	//���X�^���C�Y(�ʂ̓h��Ԃ���)�ݒ�.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory( &rdc, sizeof( rdc ) );
	rdc.FillMode = D3D11_FILL_SOLID;	//�h��Ԃ�(�\���b�h).
	rdc.CullMode = D3D11_CULL_BACK;
					//D3D11_CULL_NONE :�J�����O��؂�(���w�ʂ�`�悷��).
					//D3D11_CULL_BACK :�w�ʂ�`�悵�Ȃ�.
					//D3D11_CULL_FRONT:���ʂ�`�悵�Ȃ�.
	rdc.FrontCounterClockwise	= FALSE;
								//�|���S���̕\�������肷��t���O.
								//TRUE :�����Ȃ�O����,�E���Ȃ������.
								//FALSE:���̋t�ɂȂ�.
	rdc.DepthClipEnable	= FALSE;	//�����ɂ��ẴN���b�s���O�L��.

	ID3D11RasterizerState* pIr	= nullptr;
	m_pDevice->CreateRasterizerState( &rdc, &pIr );
	m_pDeviceContext->RSSetState( pIr );
	SAFE_RELEASE( pIr );

	//===== �V���O���g�� =====//.
	clsBLEND_STATE* pBlend = &clsSINGLETON<clsBLEND_STATE>::GetInstance();
	pBlend->Create( m_pDevice, m_pDeviceContext );

	return S_OK;
}

//============================================================
//	Direct3D�I������.
//============================================================
void clsMain::DestroyD3D()
{

#ifdef _DEBUG
	if( m_pRayH != nullptr ){
		delete m_pRayH;
		m_pRayH = nullptr;
	}
	if( m_pRayFB != nullptr ){
		delete m_pRayFB;
		m_pRayFB = nullptr;
	}
	if( m_pRayV != nullptr ){
		delete m_pRayV;
		m_pRayV = nullptr;
	}
#endif //#ifdef _DEBUG


#ifdef Tahara
	m_upGame.reset( nullptr );
#endif //#ifdef Tahara


#ifdef STARTUP_FULLSCREEN_
	//�t���X�N���[���Ȃ�WindowMode��.
	SetWindowMode();
#endif//#ifdef STARTUP_FULLSCREEN_

	clsSINGLETON_FINALIZER::Finalize();//�V���O���g���̏���.

	SAFE_DELETE( m_spViewPort10 );
	SAFE_RELEASE( m_pBackBuffer_DSTexDSV );
	SAFE_RELEASE( m_pBackBuffer_DSTex );
	SAFE_RELEASE( m_pBackBuffer_TexRTV );
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pDeviceContext );
	SAFE_RELEASE( m_pDevice );
}

////============================================================
//	���b�V���ǂݍ��݊֐�(�܂Ƃ߂�).
////============================================================
HRESULT clsMain::ReadMesh()
{
	m_upGame = make_unique< clsGAME >( 
		m_hWnd, 
		m_pDevice, 
		m_pDeviceContext, 
		m_spViewPort10, 
		m_spViewPort11 );
//	m_upGame->Create();//�N�����Ɍ��ʉ��^�C�~���O����΍�Ń��[�h��ʏI���u�ԂɈړ�����.



#ifdef _DEBUG


	//���C�\���̏�����(����).
	m_pRayV = new clsRay;
	m_pRayV->m_Ray.vPoint[0] = D3DXVECTOR3( 0.0f,-5.0f, 0.0f);
	m_pRayV->m_Ray.vPoint[1] = D3DXVECTOR3( 0.0f, 5.0f, 0.0f);
	m_pRayV->Init( m_pDevice, m_pDeviceContext );
	//���C�\���̏�����(�O��).
	m_pRayFB = new clsRay;
	m_pRayFB->m_Ray.vPoint[0] = D3DXVECTOR3( 0.0f, 0.0f,-5.0f);
	m_pRayFB->m_Ray.vPoint[1] = D3DXVECTOR3( 0.0f, 0.0f, 5.0f);
	m_pRayFB->Init( m_pDevice, m_pDeviceContext );
	//���C�\���̏�����(���E).
	if( m_pRayH == nullptr ){
		m_pRayH = new clsRay;
		m_pRayH->m_Ray.vPoint[0] = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
		m_pRayH->m_Ray.vPoint[1] = D3DXVECTOR3( 5.0f, 0.0f, 0.0f);
		m_pRayH->Init( m_pDevice, m_pDeviceContext );
	}

#endif //#ifdef _DEBUG

	return S_OK;
}


//ConvDimPos�̎��O����.
void clsMain::SetViewPort10( D3D11_VIEWPORT* const Vp )
{
	if( m_spViewPort10 == nullptr ){
		m_spViewPort10 = new D3D10_VIEWPORT;
	}

	m_spViewPort10->TopLeftX	= static_cast<INT>( Vp->TopLeftX );
	m_spViewPort10->TopLeftY	= static_cast<INT>( Vp->TopLeftY );
	m_spViewPort10->MaxDepth	= Vp->MaxDepth;
	m_spViewPort10->MinDepth	= Vp->MinDepth;
	m_spViewPort10->Width		= static_cast<UINT>( Vp->Width );
	m_spViewPort10->Height	= static_cast<UINT>( Vp->Height );
};


#ifdef STARTUP_FULLSCREEN_
HRESULT clsMain::ChangeWindowMode()
{
	if( !m_pSwapChain ) return E_FAIL;

	HRESULT hr = E_FAIL;

	DXGI_SWAP_CHAIN_DESC desc;
	hr = m_pSwapChain->GetDesc( &desc );
	if( FAILED( hr ) ) return E_FAIL;

	BOOL isFullScreen;

	//�t���X�N���[�����擾.
	hr = m_pSwapChain->GetFullscreenState( &isFullScreen, NULL );
	if( FAILED( hr ) ) return E_FAIL;

	//�t���X�N���[����on, off���t�]����.
	hr = m_pSwapChain->SetFullscreenState( !isFullScreen, NULL );
	if( FAILED( hr ) ) return E_FAIL;

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

//	ChangeWindowModeOptimization( 0, 0 );

	return S_OK;
}

void clsMain::SetWindowMode()
{
	if( !m_pSwapChain ) return;

	HRESULT hr = E_FAIL;

	DXGI_SWAP_CHAIN_DESC desc;
	hr = m_pSwapChain->GetDesc( &desc );
	if( FAILED( hr ) ) return;

	BOOL isFullScreen;

	//�t���X�N���[�����擾.
	hr = m_pSwapChain->GetFullscreenState( &isFullScreen, NULL );
	if( FAILED( hr ) ) return;

	//Window���[�h�Ȃ�.
	if( !isFullScreen ){
		return;
	}

	//�t���X�N���[����on, off���t�]����.
	hr = m_pSwapChain->SetFullscreenState( !isFullScreen, NULL );
	if( FAILED( hr ) ) return;

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

}


//HRESULT clsMain::ChangeWindowModeOptimization( const UINT Width, const UINT Height )
//{
//	HRESULT hr = E_FAIL;
//
//	if( !m_pSwapChain ) return E_FAIL;
//
//	DXGI_SWAP_CHAIN_DESC desc;
//	hr = m_pSwapChain->GetDesc( &desc );
//	if( FAILED( hr ) ) return E_FAIL;
//
//	//�^�[�Q�b�g�r���[����.
//	m_pDeviceContext->OMSetRenderTargets( 0, NULL, NULL );
//	SAFE_RELEASE( m_pBackBuffer_DSTexDSV );
//	SAFE_RELEASE( m_pBackBuffer_TexRTV );
//
//	//�X���b�v�`�F�[���̃o�b�N�o�b�t�@�T�C�Y�A�t�H�[�}�b�g�A����уo�b�t�@�[����ύX����.
//	//�A�v���P�[�V�����E�B���h�E�̃T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo���K�v������.
//	hr = m_pSwapChain->ResizeBuffers( desc.BufferCount, Width, Height, desc.BufferDesc.Format, desc.Flags );
//	if( FAILED( hr ) ) return E_FAIL;
//
//	hr = CreateRenderTargetView( &desc );
//	if( FAILED( hr ) ) return E_FAIL;
//
//
//
//
//	return S_OK;
//}
#endif//#ifdef STARTUP_FULLSCREEN_
