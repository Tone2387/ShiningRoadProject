#ifndef UI_TEXT_H_
#define UI_TEXT_H_

//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )	

//============================================================
//	�C���N���[�h.
//============================================================
#include "Common.h"

#include "TextSpriteStruct.h"


//UI�Ƃ��ĕ���������.
class clsUiText : public clsCommon
{
public:
	clsUiText();
	~clsUiText();

	//�e�L�X�g�̍��悹�A�E��.
	enum class enPOS : UINT
	{
		LEFT,
		RIGHT,
		MIDDLE,
	};

	HRESULT Create( 
		ID3D11DeviceContext* const pContext,
		const DWORD &dwWidth, const DWORD &dwHeight,
		const float fScale );


	//�����_�����O�֐�.
	//�f�t�H���g������RIGHT�ɂ���ƉE�[���w����W�ɗ���.
	void Render( const enPOS enPos = enPOS::LEFT );

	D3DXVECTOR3 GetPos() const				{ return D3DXVECTOR3( m_vPos.x, m_vPos.y, 0.0f ); };
	void SetPos( const D3DXVECTOR2 &vPos )	{ m_vPos = vPos; };
	void AddPos( const D3DXVECTOR2 &vPos )	{ m_vPos += vPos; };

	void SetScale( const float fScale )		{ m_fScale = fScale; };

	void SetText( const char* sText )		{ m_sText = sText; }

	void SetColor( const D3DXVECTOR4 &vColor = { 1.0f, 1.0f, 1.0f, 1.0f } )	{ m_vColor = vColor; }
	void SetAlpha( const float fAlpha )										{ m_fAlpha = fAlpha; };

private:


	//�t�H���g�����_�����O�֐�.
	void RenderFont( const int FontIndex, const float x, const float y, const float z ) const;


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

	float		m_fKerning[100];	//�J�[�����O(100��).
	D3DXVECTOR2 m_vPos;
	float		m_fScale;			//�g�k�l.
	float		m_fAlpha;			//���ߒl.
	D3DXVECTOR4	m_vColor;		//�F.

	std::string m_sText;//�`�敶��.

	D3DXMATRIX m_mView;	//�r���[�s��.
	D3DXMATRIX m_mProj;	//�v���W�F�N�V�����s��.
	


};
#endif//#ifndef UI_TEXT_H_