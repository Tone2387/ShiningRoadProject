#ifndef SPRITE_H_
#define SPRITE_H_

//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )	

//============================================================
//	�C���N���[�h.
//============================================================
#include "Common.h"//���ʃN���X.



////���_�̍\����.
//struct SpriteVertex
//{
//	D3DXVECTOR3 vPos;	//���_���W(x,y,z).
//	D3DXVECTOR2 vTex;	//�e�N�X�`�����W.
//};




//============================================================
//	�X�v���C�g�N���X.
//============================================================
class clsSprite : public clsCommon
{
public:
	clsSprite();	//�R���X�g���N�^.
	virtual ~clsSprite();	//�f�X�g���N�^.

	//������.
	HRESULT Create( ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11,
		const char* sTexName );


	//�`��(�����_�����O)(��DX9MESH����Main����2���݂���̂Œ���).
	virtual void Render( 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj,
		const D3DXVECTOR3 &vEye,
		const D3DXVECTOR4& vColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		bool isBillBoard = false );


	D3DXVECTOR3 GetPos()					{ return m_vPos; };
	void SetPos( const D3DXVECTOR3& vPos )	{ m_vPos = vPos; };
	void AddPos( const D3DXVECTOR3& vPos )	{ m_vPos += vPos; };

	D3DXVECTOR3 GetRot()					{ return m_vRot; };
	void SetRot( const D3DXVECTOR3& vRot )	{ m_vRot = vRot; };
	void AddRot( const D3DXVECTOR3& vRot )	{ m_vRot += vRot; };

	D3DXVECTOR3 GetScale()						{ return m_vScale; };
	void SetScale( const D3DXVECTOR3& vScale )	{ m_vScale = vScale; };
	void AddScale( const D3DXVECTOR3& vScale )	{ m_vScale += vScale; };

	void SetSplit( const D3DXVECTOR2& vSplit )	{ m_vSplit = vSplit; }


protected:

	//�V�F�[�_�쐬.
	HRESULT InitShader( const char* sErrFileName );
	////���f���쐬.
	virtual HRESULT InitModel( const char* sTexName );
	//�}�X�N�쐬.
	HRESULT CreateMask( const char* sTexName );

protected:

	D3DXVECTOR3		m_vPos;	//�ʒu.
	D3DXVECTOR3		m_vRot;
	D3DXVECTOR3		m_vScale;

	D3DXVECTOR2		m_vSplit;


	//�����f���̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;	//���_�V�F�[�_.
	ID3D11InputLayout*		m_pVertexLayout;	//���_���C�A�E�g.
	ID3D11PixelShader*		m_pPixelShader;		//�s�N�Z���V�F�[�_.
	ID3D11Buffer*			m_pConstantBuffer;	//�R���X�^���g�o�b�t�@.

	//�����f�����Ƃɗp��.
	ID3D11Buffer*			m_pVertexBuffer;	//���_�o�b�t�@.

	ID3D11ShaderResourceView*	m_pTexture;		//�e�N�X�`��.
	ID3D11SamplerState*			m_pSampleLinear;//�e�N�X�`���̃T���v���[:/�e�N�X�`���Ɋe��t�B���^��������.

	std::vector< ID3D11ShaderResourceView* >	m_vecpMask;		//�e�N�X�`���̃}�X�N.



	int		m_AnimCount;//UV�X�N���[��.


};


#endif	//#ifndef SPRITE_H_
