#ifndef SCREEN_TEXTURE_H_
#define SCREEN_TEXTURE_H_

class clsSOUND_MANAGER_NOISE;
#include "Global.h"

//レンダリングテクスチャ用.
//いきなりバックバッファではなくいったんここに描画する.
class clsSCREEN_TEXTURE
{
public:
	clsSCREEN_TEXTURE(
		const HWND hWnd,
		ID3D11DeviceContext* const pContext );
	~clsSCREEN_TEXTURE();


	//Rendertargetをテクスチャにする.
	void SetRenderTargetTexture( ID3D11DepthStencilView* const pDepthStencilView ) const;
	
	//ノイズの更新.
	void NoiseUpdate();
	
	//テクスチャの内容を画面に描画.
	void RenderWindowFromTexture( 
		ID3D11RenderTargetView* const pBackBuffer_TexRTV,
		ID3D11DepthStencilView* const pDepthStencilView ) const;

	//効果音再生.
	//再生出来たらtrueを返す.
	bool PlaySeStrong();
	bool PlaySeWeak();
	//効果音停止.
	void StopSe();

	//trueでノイズ.
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



	//----- ノイズ -----//.
	//フラグ.
	bool	m_isNoise;
	//ブロック.
	int		m_iBlock;//分割数.
	int		m_iSeed;
	//パルス.
	float	m_fPulse;
	float	m_fPulseOffset;
	float	m_fPulseOffsetAdd;

	//音声.
	std::unique_ptr< clsSOUND_MANAGER_NOISE >	m_upSound;
	struct SE_FLAG
	{
		bool	isCanPlay;	//再生可能.
		bool	isContinue;	//同じ効果音を再生する.
		int		iContinueNo;//同じ効果音を再生するSE番号.
		SE_FLAG()
			:isCanPlay( false )
			,isContinue( false )
			,iContinueNo( 0 )
		{}
	};
	SE_FLAG m_SeFlagStrong;
	SE_FLAG m_SeFlagWeak;
	//----- ノイズ -----//.

	//----- 画面色 -----//.
	bool m_isNega;
	D3DXVECTOR4 m_vColor;
	//----- 画面色 -----//.



	ID3D11Device*				m_wpDevice;
	ID3D11DeviceContext*		m_wpContext;


	ID3D11Texture2D*			m_pTexture;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11ShaderResourceView*	m_pShaderResourceView;
	ID3D11SamplerState*			m_pSamplerState;
	ID3D11VertexShader*			m_pVertexShader;	
	ID3D11PixelShader*			m_pDefaultPS;	//通常時.	
	ID3D11PixelShader*			m_pNoisePS;		//ノイズ.
	ID3D11Buffer*				m_pConstantBuffer;

};

#endif//#ifndef SCREEN_TEXTURE_H_