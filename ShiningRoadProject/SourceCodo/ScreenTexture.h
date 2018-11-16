#ifndef SCREEN_TEXTURE_H_
#define SCREEN_TEXTURE_H_

class clsSOUND_MANAGER_NOISE;
#include "Global.h"

//�����_�����O�e�N�X�`���p.
//�����Ȃ�o�b�N�o�b�t�@�ł͂Ȃ��������񂱂��ɕ`�悷��.
class clsSCREEN_TEXTURE
{
public:
	clsSCREEN_TEXTURE(
		const HWND hWnd,
		ID3D11DeviceContext* const pContext );
	~clsSCREEN_TEXTURE();


	//Rendertarget���e�N�X�`���ɂ���.
	void SetRenderTargetTexture( ID3D11DepthStencilView* const pDepthStencilView ) const;
	
	//�m�C�Y�̍X�V.
	void NoiseUpdate();
	
	//�e�N�X�`���̓��e����ʂɕ`��.
	void RenderWindowFromTexture( 
		ID3D11RenderTargetView* const pBackBuffer_TexRTV,
		ID3D11DepthStencilView* const pDepthStencilView ) const;

	//���ʉ��Đ�.
	//�Đ��o������true��Ԃ�.
	bool PlaySeStrong();
	bool PlaySeWeak();
	//���ʉ���~.
	void StopSe();

	//true�Ńm�C�Y.
	void SetNoiseFlag( const bool isNoise )	{ m_isNoise = isNoise; };

	void SetBlock( const int iBlock ){ m_iBlock = iBlock; };

	void SetPulse( const float fPulse ){ m_fPulse = fPulse; };
	void SetPulseOffset( const float fPulseOffset ){ m_fPulseOffset = fPulseOffset; };
	void SetPulseOffsetAdd( const float fPulseOffsetAdd ){ m_fPulseOffsetAdd = fPulseOffsetAdd; };

	void SetNega( const bool isNega ){ m_isNega = isNega; };
	void SetColor( const D3DXVECTOR4& vColor ){ m_vColor = vColor; };


	bool isNoiseFlag()const { return m_isNoise; };
	bool isNegaFlag() const	{ return m_isNega; };
	bool isDifferentColor() const { 
		if( m_vColor == D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ) ){
			return false;
		}
		return true; 
	};
	bool isUse() const {
		if( isNoiseFlag()	||
			isNegaFlag()	||
			isDifferentColor() )
		{
			return true;
		}
		return false;
	}

private:

	HRESULT CreateTexture();
	HRESULT CreateShader();
	HRESULT CreateConstantBuffer();


	bool PlaySeProduct( const int iMin, const int iSize, int* const outSeNo );

private:



	//----- �m�C�Y -----//.
	//�t���O.
	bool	m_isNoise;
	//�u���b�N.
	int		m_iBlock;//������.
	int		m_iSeed;
	//�p���X.
	float	m_fPulse;
	float	m_fPulseOffset;
	float	m_fPulseOffsetAdd;

	//����.
	std::unique_ptr< clsSOUND_MANAGER_NOISE >	m_upSound;
	struct SE_FLAG
	{
		bool	isCanPlay;	//�Đ��\.
		bool	isContinue;	//�������ʉ����Đ�����.
		int		iContinueNo;//�������ʉ����Đ�����SE�ԍ�.
		SE_FLAG()
			:isCanPlay( false )
			,isContinue( false )
			,iContinueNo( 0 )
		{}
	};
	SE_FLAG m_SeFlagStrong;
	SE_FLAG m_SeFlagWeak;
	//----- �m�C�Y -----//.

	//----- ��ʐF -----//.
	bool m_isNega;
	D3DXVECTOR4 m_vColor;
	//----- ��ʐF -----//.



	ID3D11Device*				m_wpDevice;
	ID3D11DeviceContext*		m_wpContext;


	ID3D11Texture2D*			m_pTexture;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11ShaderResourceView*	m_pShaderResourceView;
	ID3D11SamplerState*			m_pSamplerState;
	ID3D11VertexShader*			m_pVertexShader;	
	ID3D11PixelShader*			m_pDefaultPS;	//�ʏ펞.	
	ID3D11PixelShader*			m_pNoisePS;		//�m�C�Y.
	ID3D11Buffer*				m_pConstantBuffer;

};

#endif//#ifndef SCREEN_TEXTURE_H_