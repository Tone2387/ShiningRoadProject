#define _CRT_SECURE_NO_WARNINGS
//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )

//���ꂪ���Ă���ƋN�����Ƀt���X�N���[���ɂȂ�\�肾��������������߂܂���.
//#define STARTUP_FULLSCREEN_


class clsGAME;

//============================================================
//	�C���N���[�h.
//============================================================
#include "Global.h"

#ifdef _DEBUG
class clsDX9Mesh;
//#include "DebugText.h"
#include "Ray.h"		//���C�\���N���X.

#endif//#ifdef _DEBUG



//���������[�N���o�p.
#include <crtdbg.h>




////============================================================
////	���C�u����.
////============================================================
//#pragma comment( lib, "winmm.lib" )
//
//#pragma comment( lib, "d3dx11.lib" )
//#pragma comment( lib, "d3d11.lib" )
//
//#pragma comment( lib, "d3dx10.lib" )//�uD3DX�`�v�̒�`�g�p���ɕK�v.







//============================================================
//	���C���N���X.
//============================================================
class clsMain
{
public:
	clsMain();	//�R���X�g���N�^.
	~clsMain();	//�f�X�g���N�^.

	//�E�B���h�E�������֐�.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y, INT width, INT height,
		LPSTR WindowName);

	//�E�B���h�E�֐�(���b�Z�[�W���̏���).
	LRESULT MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam );

	//DirectX������.
	HRESULT InitD3D();
	//Direct3D�I������.
	void DestroyD3D();

	//���b�Z�[�W���[�v.
	void Loop();

private:

	//�N�����̕`��.

	//�A�v���P�[�V�������C������.
	bool AppMain();
	//�`��(�����_�����O).
	void Render();


	//���b�V���ǂݍ��݊֐�(�܂Ƃ߂�).
	HRESULT ReadMesh();

#ifdef _DEBUG
	//�X�t�B�A�쐬.
	HRESULT InitSphere( clsDX9Mesh* pMesh, float fScale = 0.7f );
	//�X�t�B�A�Փ˔���֐�.
	bool Collision( clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget );
	//�o�E���f�B���O�{�b�N�X�쐬.
	HRESULT InitBBox( clsDX9Mesh* pMesh );
	//�{�b�N�X�Փ˔���֐�.
	bool BBoxCollision( clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget );

	//���C�\���N���X.
	clsRay*			m_pRayV;	//����.
	clsRay*			m_pRayFB;	//�O��.
	clsRay*			m_pRayH;	//���E.


#endif //#ifdef _DEBUG


	//�Q�[��.
	std::unique_ptr< clsGAME >	m_upGame;



	HWND	m_hWnd;	//�E�B���h�E�n���h��.

	//���A�v���Ɉ��.
	ID3D11Device*			m_pDevice;			//�f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pDeviceContext;	//�f�o�C�X�R���e�L�X�g.
	IDXGISwapChain*			m_pSwapChain;		//�X���b�v�`�F�[��.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;//�����_�[�^�[�Q�b�g�r���[.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;//�o�b�N�o�b�t�@.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;//�f�v�X�X�e���V���r���[.


	//�V�[���N���X�ɂĎg�p����֐��ɕK�v.
	//ConvDimPos�̎��O����.
	void SetViewPort10( D3D11_VIEWPORT* const Vp );
	//2DSp�p.
	D3D10_VIEWPORT* m_spViewPort10;
	
	
	//�����p.
	D3D11_VIEWPORT* m_spViewPort11;


#ifdef STARTUP_FULLSCREEN_
	HRESULT ChangeWindowMode();
	void SetWindowMode();
//	//�\�����[�h�ؑ֍œK��.
//	HRESULT ChangeWindowModeOptimization( const UINT Width, const UINT Height );
#endif//#ifdef STARTUP_FULLSCREEN_

};