#ifndef GAME_H_
#define GAME_H_

class clsSCENE_BASE;
class clsCAMERA_BASE;
class clsFont;
class clsPOINTER_GROUP;
class clsDxInput;
class clsXInput;
class clsResource;
class clsEffects;
class clsSOUND_MANAGER_BASE;
class clsROBO_STATUS_PLAYER;
class clsBLACK_SCREEN;

#include "Global.h"


//�Q�[���̓����͂��̃N���X�̒��ōs��.
class clsGAME
{
public:
	//�����̓V�[���̏������ɕK�v�Ȃ���.
	clsGAME( 
		const HWND hWnd, 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext,
		D3D10_VIEWPORT* const pViewPort10,
		D3D11_VIEWPORT* const pViewPort11, 
		IDXGISwapChain* const pSwapChain,
		ID3D11RenderTargetView* const pBackBuffer_TexRTV,
		ID3D11DepthStencilView* const pBackBuffer_DSTexDSV );
	~clsGAME();

	//new����Ɏg��.
	void Create();
	//���t���[���g��( �߂�l�́A����Ȃ�true, �I���Ȃ�false ).
	bool Update();
	//���t���[���g��.
	void Render() const;




private:

	//�����̃V�[���ɐ؂�ւ�.
	void SwitchScene( const enSCENE enNextScene );

private:

	//�V�[���N���X.
	std::unique_ptr< clsSCENE_BASE > m_upScene;

	//�J����.
	clsCAMERA_BASE*	m_spCamera;
	
	//�t�H���g.
	clsFont*			m_spFont;

	//�V�[���������ɕK�v�ȃ|�C���^�̂܂Ƃ�.
	clsPOINTER_GROUP* m_spPtrGroup; 


	HWND					m_hWnd;
	ID3D11Device*			m_wpDevice;		//�f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_wpContext;	//�f�o�C�X�R���e�L�X�g.
	//2DSp�p.
	D3D10_VIEWPORT* m_wpViewPort10;
	//�����p.
	D3D11_VIEWPORT* m_wpViewPort11;


	IDXGISwapChain*			m_wpSwapChain;		//�X���b�v�`�F�[��.
	ID3D11RenderTargetView*	m_wpBackBuffer_TexRTV;//�����_�[�^�[�Q�b�g�r���[.
	ID3D11DepthStencilView*	m_wpBackBuffer_DSTexDSV;//�f�v�X�X�e���V���r���[.


	//�W���C�X�e�B�b�N.
	clsDxInput*		m_spDxInput;
	clsXInput*		m_spXInput;

	//���\�[�X�N���X.
	clsResource*	m_spResource;
	//�G�t�F�N�g.
	clsEffects*		m_spEffect;
	//�T�E���h.
	clsSOUND_MANAGER_BASE* m_spSound;

	//( �V�[�����܂��� )���{�b�g�̃X�e�[�^�X.
	clsROBO_STATUS_PLAYER* m_spRoboStatus;

	//�Ö�����낤.
	clsBLACK_SCREEN* m_spBlackScreen;

};


#endif//#ifndef GAME_H_