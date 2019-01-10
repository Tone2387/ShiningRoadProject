#ifndef RENDER_AT_START_UP_H_
#define RENDER_AT_START_UP_H_

class clsBLEND_STATE;

//#include "Global.h"
#include "Sprite2DCenter.h"
#include "LineBox.h"
#include "UiText.h"
#include "BlackScreen.h"
#include <vector>
#include <string>

class clsRENDER_AT_START_UP
{
public:
	clsRENDER_AT_START_UP(
		ID3D11Device*			const pDevice,
		ID3D11DeviceContext*	const pContext,
		IDXGISwapChain*			const pSwapChain,
		ID3D11RenderTargetView*	const pBackBuffer_TexRTV,
		ID3D11DepthStencilView*	const pBackBuffer_DSTexDSV );
	~clsRENDER_AT_START_UP();

	//���[�v����.
	void Loop();

	//���[�h���I��������Ɏg��.
	void FinishLoad();

private:
	HRESULT CreateDepthStencilState();

	void Update();

	//�`��.
	void Render( bool isLoop = true ) const;



	//�[�x�e�X�g(Z�e�X�g)ON/OFF�ؑ�.
	void SetDepth( bool isOn ) const;
	

	enum class enMODE
	{
		LINE_V,		//�O�g���c�ɑ傫���Ȃ�.
		LINE_H,		//�O�g�����ɑ傫���Ȃ�.

		GAGE_H,		//�Q�[�W�̘g�����ɑ傫���Ȃ�.
		GAGE_V,		//�Q�[�W�̘g���c�ɑ傫���Ȃ�.

		GAGE_MOVE,	//�Q�[�W������.

		COMPLETE,	//���[�h����.
	}	m_enMode;

	//switch���̒��g.
	void BiggerLineBoxV();//�O�g���c�ɑ傫���Ȃ�.
	void BiggerLineBoxH();//�O�g�����ɑ傫���Ȃ�.
	void BiggerGageBoxH();//�Q�[�W�̘g�����ɑ傫���Ȃ�.
	void BiggerGageBoxV();//�Q�[�W�̘g���c�ɑ傫���Ȃ�.
	void UpdateLoadMsg();//�Q�[�W������.
	void Complete();	//���[�h������.


	//�I�������邽�߂̏���.
	void End(){
		m_bEnd = true;//���̃t���O��true�ɂȂ�΂��̃N���X�̃��[�v�͏I���.
	};

private:

	int m_iTimer;//���[�h�́A�ҋ@����.


	std::unique_ptr< clsLINE_BOX > m_upLineBox;
	std::unique_ptr< clsLINE_BOX > m_upGageBox;

	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupGage;//�Q�[�W�{��.

	std::unique_ptr< clsBLACK_SCREEN > m_upBlack;

	//���S.
	std::unique_ptr< clsSPRITE2D_CENTER > m_upLogo;

	//���[�h�����b�Z�[�W.
	std::unique_ptr< clsUiText > m_upText;
	std::string m_sLodeMsg;

	bool						m_bEnd;//true�ɂȂ�����I��.

	ID3D11Device*				m_wpDevice;
	ID3D11DeviceContext*		m_wpContext;			//�f�o�C�X�R���e�L�X�g.
	IDXGISwapChain*				m_wpSwapChain;			//�X���b�v�`�F�[��.
	ID3D11RenderTargetView*		m_wpBackBuffer_TexRTV;	//�����_�[�^�[�Q�b�g�r���[.
	ID3D11DepthStencilView*		m_wpBackBuffer_DSTexDSV;//�f�v�X�X�e���V���r���[.
	ID3D11DepthStencilState*	m_pDepthStencilStateOn;	//�[�x(Z)�e�X�g�ݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;	//�[�x(Z)�e�X�g�ݒ�.


#ifdef _DEBUG
	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupDebugImage;
#endif//#ifdef _DEBUG

};


#endif//#ifndef RENDER_AT_START_UP_H_