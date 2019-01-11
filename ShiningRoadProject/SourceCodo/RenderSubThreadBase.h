#ifndef RENDER_SUB_THREAD_BASE_H_
#define RENDER_SUB_THREAD_BASE_H_


#include "Global.h"

class clsRENDER_SUB_THREAD_BASE
{
public:
	clsRENDER_SUB_THREAD_BASE(
		ID3D11Device*			const pDevice,
		ID3D11DeviceContext*	const pContext,
		IDXGISwapChain*			const pSwapChain,
		ID3D11RenderTargetView*	const pBackBuffer_TexRTV,
		ID3D11DepthStencilView*	const pBackBuffer_DSTexDSV );
	~clsRENDER_SUB_THREAD_BASE();

	//���[�v����.
	virtual void Loop() = 0;

	//���[�h���I��������Ɏg��.
	//�p����̂��̒��̂ǂ�����End()���g��.
	virtual void FinishLoad() = 0;

protected:

	//�p����̂��̒��̂ǂ�����End()���g��.
	void Update();

	//�p����̓��e.
	virtual void UpdateProduct() = 0;
	virtual void RenderProduct() const = 0;


	//�I�������邽�߂̏���.
	//�p�����Update()��FinishLoad()���̂ǂ����Ŏg��.
	void End(){
		m_bEnd = true;//���̃t���O��true�ɂȂ�΂��̃N���X�̃��[�v�͏I���.
	};

	bool isEnd(){
		return m_bEnd;
	}

private:


	//�`��.
	void Render( bool isLoop = true ) const;


	HRESULT CreateDepthStencilState();

	//�[�x�e�X�g(Z�e�X�g)ON/OFF�ؑ�.
	void SetDepth( bool isOn ) const;
	

private:


	bool						m_bEnd;//true�ɂȂ�����I��.

	ID3D11Device*				m_wpDevice;
	ID3D11DeviceContext*		m_wpContext;			//�f�o�C�X�R���e�L�X�g.
	IDXGISwapChain*				m_wpSwapChain;			//�X���b�v�`�F�[��.
	ID3D11RenderTargetView*		m_wpBackBuffer_TexRTV;	//�����_�[�^�[�Q�b�g�r���[.
	ID3D11DepthStencilView*		m_wpBackBuffer_DSTexDSV;//�f�v�X�X�e���V���r���[.
	ID3D11DepthStencilState*	m_pDepthStencilStateOn;	//�[�x(Z)�e�X�g�ݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;	//�[�x(Z)�e�X�g�ݒ�.

};


#endif//RENDER_SUB_THREAD_BASE_H_