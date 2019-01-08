#ifndef DEBUG_TEXT_H_
#define DEBUG_TEXT_H_

//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )	

//============================================================
//	�C���N���[�h.
//============================================================
#include "Common.h"
#include "TextSpriteStruct.h"



//============================================================
//	�萔.
//============================================================



//�e�L�X�g�N���X.
class clsDebugText : public clsCommon
{
public:
	clsDebugText();	//�R���X�g���N�^.
	~clsDebugText();//�f�X�g���N�^.

	HRESULT Init( ID3D11DeviceContext* pContext,
		DWORD dwWidth, DWORD dwHeight,
		float fSize, D3DXVECTOR4 vColor );


	//�����_�����O�֐�.
	void Render( char* text, int x, int y );

	//�t�H���g�����_�����O�֐�.
	void RenderFont( int FontIndex, int x, int y );

private:

	//�����f���̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;	//���_�V�F�[�_.
	ID3D11InputLayout*		m_pVertexLayout;	//���_���C�A�E�g.
	ID3D11PixelShader*		m_pPixelShader;		//�s�N�Z���V�F�[�_.
	ID3D11Buffer*			m_pConstantBuffer;	//�R���X�^���g�o�b�t�@.

	//�����f�����Ƃɗp��.
	ID3D11Buffer*			m_pVertexBuffer[100];	//���_�o�b�t�@(100��).

	ID3D11ShaderResourceView*	m_pAsciiTexture;//�A�X�L�[�e�N�X�`��.
	ID3D11SamplerState*			m_pSampleLinear;//�e�N�X�`���̃T���v���[:/�e�N�X�`���Ɋe��t�B���^��������.



	DWORD	m_dwWindowWidth;	//�E�B���h�E��.
	DWORD	m_dwWindowHeight;	//�E�B���h�E����.

	float	m_fKerning[100];	//�J�[�����O(100��).
	float	m_fScale;			//�g�k�n(25pixel��� 25pixel=1.0f).
	float	m_fAlpha;			//���ߒl.
	D3DXVECTOR4	m_vColor;		//�F.

	D3DXMATRIX m_mView;	//�r���[�s��.
	D3DXMATRIX m_mProj;	//�v���W�F�N�V�����s��.
	

};


#endif//#define DEBUG_TEXT_H_