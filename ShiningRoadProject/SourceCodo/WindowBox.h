#pragma once

#include "LineBox.h"
#include "UiText.h"


class clsWINDOW_BOX : public clsLINE_BOX
{
public:
	clsWINDOW_BOX(		
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11 );

	~clsWINDOW_BOX();

	//----- 文字用 -----//.
	//( ウィンドウの左上からの )座標.
	void SetTextPos( const D3DXVECTOR2 &vPos );
	void SetTextScale( const float fScale );
	void SetText( const char* sText );
	void SetTextColor( const D3DXVECTOR4 &vColor = { 1.0f, 1.0f, 1.0f, 1.0f } );

private:

	void UpdateProduct() override;

	void RenderProduct() override;

	std::unique_ptr< clsSPRITE2D_CENTER >	m_upBack;//背景画像.

	std::unique_ptr< clsUiText >			m_upText;//文字表示用.
	D3DXVECTOR2								m_vTextOffset;//テキストの窓からのずれ.

};

