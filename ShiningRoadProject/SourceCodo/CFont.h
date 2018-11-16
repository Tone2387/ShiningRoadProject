#ifndef _C_FONT_H_
#define _C_FONT_H_

#define _CRT_SECURE_NO_WARNINGS

#include "Global.h"


//�A���C�������g�ݒ�(�����I��16�o�C�g�ɐݒ肷��).
#define ALIGN16 _declspec( align ( 16 ) )


//���{���UI�p.
//����ɓǂݍ��܂��镶����f�[�^�͑S�đS�p�ɂ��邱��.
class clsFont
{
public:

	clsFont( 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext );
	~clsFont();

	enum class encPOS : UCHAR
	{
		LEFT = 0,
		CENTER,
		RIGHT,
	};


	void Create( const char *sTextFileName );//�V�[���J�n���Ɏg��.
	void Release();							//�V�[���I�����Ɏg��.

	void Render( const int iTextRow, const int iCharNum = -1 );



	D3DXVECTOR3 GetPos()	const			{ return m_vPos; }
	D3DXVECTOR3 GetPosLast()const			{ return m_vPosLast; }
	void SetPos( const D3DXVECTOR3 &vPos )	{ m_vPos = vPos; };
	void AddPos( const D3DXVECTOR3 &vPos )	{ SetPos( m_vPos + vPos ); }

	float GetScale() const				{ return m_fScale; }
	void SetScale( const float fScale )	{ m_fScale = fScale; };

	void SetColor( const D3DXVECTOR4 &vColor );
	void SetAlpha( const float fAlpha )	{ m_fAlpha = fAlpha; }

	//�܂�Ԃ��ʒu.
	void SetIndentPos( const float fPosX ){ m_fIndentionPosint = fPosX; }
	
	//�ǂݍ��񂾃e�L�X�g�̐�( Create���Ă��Ȃ���-1���Ԃ� ).
	int GetTextRow() const { return m_iTextRow; }
	//�e�L�X�g�̓��e.
	std::string GetText( const int iRow ) const;

private:

	//�u�����h�X�e�[�g�쐬.
	HRESULT CreateBlendState();
	//�V�F�[�_�쐬.
	HRESULT CreateShader();
	//�o�[�e�b�N�X�V�F�[�_�쐬.
	HRESULT CreateVertexBuffer();
	//�萔�o�b�t�@�쐬.
	HRESULT CreateConstantBuffer();
	HRESULT LoadTextFile( const char *FileName );//3�s, ������.
	HRESULT	CreateTexture();

	void SetBlend( const bool isAlpha ) const;

	//�������ׂ�����{����Ԃ�( ���Ȃ��Ȃ�1.0f ).
	//��������( �S�p�� )�������ڂ�.
	//�Ō�̈����͕������m�̊Ԋu�ɉ����|���邩.
	float GetFineCharactorRate(
		const int iTextRow, 
		const int iCharNum, 
		float* outfAddLeft ) const;

	//�߂�l�p�񋓑�.
	enum class encCHARACTOR_TYPE : UCHAR
	{
		ALPHABET = 0,//���p.
		JAPANESE_HEAD,//���{��擪�o�C�g.
		JAPANESE_FOOT,//���{�ꖖ���o�C�g.
	};
	encCHARACTOR_TYPE GetCharactorType( 
		const int iTextRow, const int iCharNum ) const;


private:

	//�\����.
	struct FONTSHADER_CONSTANT_BUFFER
	{
		ALIGN16 D3DXMATRIX mW;			//�ʒu�ƃJ�����ʒu�ƕ\����ʐݒ�.
		ALIGN16 float ViewPortWidth;	//�J���[(RGBA�̌^�ɍ��킹��).
		ALIGN16 float ViewPortHeight;	//UV���W.
		ALIGN16 float Alpha;	//���ߒl.
		ALIGN16 D3DXVECTOR2 Uv;	//UV���W.
		ALIGN16 D3DXVECTOR4 Color;	//UV���W.
	};

	//���_�̍\����.
	struct FONT_VERTEX
	{
		D3DXVECTOR3 Pos;		//�ʒu.
		D3DXVECTOR2 Tex;		//�e�N�X�`��.
	};

	D3DXVECTOR3		m_vPos;			//�ʒu.
	float			m_fScale;		//�g�k.
	D3DXVECTOR4		m_vColor;		//�F.
	float			m_fAlpha;
		
	//�e�N�X�`���T�C�Y( �𑜓x ).
	int		m_iFontSize;
	//�����ƕ����̌���.
	float	m_fFontMarginX;
	float	m_fFontMarginY;
	//���s����x���W.
	float	m_fIndentionPosint;
	//�Ō�̕����̍��W.
	D3DXVECTOR3	m_vPosLast;

	std::vector< std::string > 		m_vecsTextData;//[ TEXT_H ][ TEXT_W ]	//����.

	//�ǂݍ��񂾃e�L�X�g�̐�( Create���Ă��Ȃ���-1���Ԃ� ).
	int m_iTextRow;



	DESIGNVECTOR		m_Design;

	//�e�N�X�`���֘A.
	std::vector< ID3D11Texture2D* >							m_vecpTex2D;//[ TEXT_H ];//2�c�e�N�X�`��.
	std::vector< std::vector< ID3D11ShaderResourceView* > > m_vecvecpAsciiTexture;//[ TEXT_H ][ TEXT_W ]; //�e�N�X�`��.


	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;

	//�|���S���p.
	ID3D11VertexShader* m_pVertexShader;	//���_�V�F�[�_�[.
	ID3D11InputLayout*	m_pVertexLayout;	//���_���C�A�E�g.
	ID3D11PixelShader*  m_pPixelShader;		//�s�N�Z���V�F�[�_�[.
	ID3D11Buffer*		m_pConstantBuffer;	//�R���X�^���g�o�b�t�@.
	ID3D11Buffer*		m_pVertexBuffer;	//���_�o�b�t�@	.

	ID3D11SamplerState* m_pSampleLinear;	//�e�N�X�`���̃T���v���[.

	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//�u�����h�X�e�[�g.

};

#endif _C_FONT_H_