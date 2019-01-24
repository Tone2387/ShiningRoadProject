#ifndef C_FONT_H_
#define C_FONT_H_

#define _CRT_SECURE_NO_WARNINGS

#include "Common.h"



//���{���UI�p.
//����ɓǂݍ��܂��镶����f�[�^�͑S�đS�p�ɂ��邱��.
class clsFont : public clsCommon
{
public:

	clsFont( 
		const HWND hWnd,
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext );
	~clsFont();

	enum class encPOS : UCHAR
	{
		LEFT = 0,
		CENTER,
		RIGHT,
	};

	enum enTEXT_SCENE : unsigned int
	{
		enTEXT_SCENE_TITLE = 0,
		enTEXT_SCENE_ASSEMBLE,
		enTEXT_SCENE_MISSION,
		enTEXT_SCENE_ENDING,
		enTEXT_SCENE_GAMEOVER,

		enTEXT_SCENE_size
	};

//	void Create( const char *sTextFileName );//�V�[���J�n���Ɏg��.
//	void Release();							//�V�[���I�����Ɏg��.
//	void Release();							//�V�[���I�����Ɏg��.
	void SetScene( const enTEXT_SCENE enTextScene ){
		if( enTextScene >= enTEXT_SCENE_size ){ m_uiTextRowIndex = 0; }
		else { m_uiTextRowIndex = enTextScene; }
	}

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
	int GetTextRow() const { 
		if( m_vecupText.size() <= m_uiTextRowIndex ){ return -1; }
		if( !m_vecupText[ m_uiTextRowIndex ] ){ return -1; }
		return m_vecupText[ m_uiTextRowIndex ]->iTextRow; 
	}
	//�e�L�X�g�̓��e.
	std::string GetText( const int iRow ) const;

private:

	//�V�F�[�_�쐬.
	HRESULT CreateShader();
	//�o�[�e�b�N�X�V�F�[�_�쐬.
	HRESULT CreateVertexBuffer();
	//�萔�o�b�t�@�쐬.
	HRESULT CreateConstantBuffer();


	void CreateTextSprite();//�t�H���g�X�v���C�g�쐬.
	HRESULT LoadTextFile( const unsigned int uiINDEX, const char *FileName );//3�s, ������.
	HRESULT	CreateTexture( const unsigned int uiINDEX, const char* sErrFilePath );


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
		ALPHABET = 0, //���p.
		JAPANESE_HEAD,//���{��擪�o�C�g.
		JAPANESE_FOOT,//���{�ꖖ���o�C�g.
	};
	encCHARACTOR_TYPE GetCharactorType( 
		const int iTextRow, const int iCharNum ) const;


private:

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





	DESIGNVECTOR		m_Design;



	//�|���S���p.
	ID3D11VertexShader* m_pVertexShader;	//���_�V�F�[�_�[.
	ID3D11InputLayout*	m_pVertexLayout;	//���_���C�A�E�g.
	ID3D11PixelShader*  m_pPixelShader;		//�s�N�Z���V�F�[�_�[.
	ID3D11Buffer*		m_pConstantBuffer;	//�R���X�^���g�o�b�t�@.
	ID3D11Buffer*		m_pVertexBuffer;	//���_�o�b�t�@	.

	ID3D11SamplerState* m_pSampleLinear;	//�e�N�X�`���̃T���v���[.

	HWND m_hWnd;

	//�`��Ɏg��.
	struct TEXT_SPRITE
	{
		//�ǂݍ��񂾃e�L�X�g�̍s��( Create���Ă��Ȃ���-1���Ԃ� ).
		int iTextRow;
		std::vector< std::string > 		vecsTextData;//[ TEXT_H ][ TEXT_W ]	//����.
		//�e�N�X�`���֘A.
		std::vector< ID3D11Texture2D* >							vecpTex2D;//2�c�e�N�X�`���̃��\�[�X.
		std::vector< std::vector< ID3D11ShaderResourceView* > > vecvecpAsciiTexture;//������e�N�X�`��.
	
		
		TEXT_SPRITE() : iTextRow( -1 ){}
		~TEXT_SPRITE()
		{
			vecsTextData.clear();
			vecsTextData.shrink_to_fit();

			for( unsigned int iTex=0; iTex<vecvecpAsciiTexture.size(); iTex++ )
			{
				for( unsigned int i=0; i<vecvecpAsciiTexture[ iTex ].size(); i++ )
				{
					if( !vecvecpAsciiTexture[ iTex ][i] ) continue;
					vecvecpAsciiTexture[ iTex ][i]->Release();
					vecvecpAsciiTexture[ iTex ][i] = nullptr;
				}
				vecvecpAsciiTexture[ iTex ].clear();
				vecvecpAsciiTexture[ iTex ].shrink_to_fit();
			}
			vecvecpAsciiTexture.clear();
			vecvecpAsciiTexture.shrink_to_fit();

			DeleteTex2D();

			iTextRow = -1;
		}
		void DeleteTex2D()
		{
			for( unsigned int iTex=0; iTex<vecpTex2D.size(); iTex++ ){
				if( !vecpTex2D[ iTex ] ) continue;
				vecpTex2D[ iTex ]->Release();
				vecpTex2D[ iTex ] = nullptr;
			}
			vecpTex2D.clear();
			vecpTex2D.shrink_to_fit();
		}
	};
	//�V�[���̐������쐬����.
	std::vector< std::unique_ptr< TEXT_SPRITE > >	m_vecupText;
	//��L��vector�̓Y����.
	unsigned int m_uiTextRowIndex;

};

#endif C_FONT_H_