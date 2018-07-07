#pragma once

#include "Sprite2DCenter.h"


class clsLINE_BOX
{
public:
	clsLINE_BOX();
	~clsLINE_BOX();

	void Create(	
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11 );


	void Render();

	void SetPos( const D3DXVECTOR3 &vPos );
	void SetSize( const D3DXVECTOR3 &vScale );
	void SetSize( const float &fScale );

	D3DXVECTOR3 GetPos() const;
	D3DXVECTOR3 GetSize() const;

	void SetAlpha( const float fAlpha );
	//範囲をoverするとfalseが返ってくる.
	bool AddAlpha( const float fAlpha );

private:

	std::unique_ptr< clsSPRITE2D_CENTER > m_upLineH[2];//水平( 横 )( 0が上 ).
	std::unique_ptr< clsSPRITE2D_CENTER > m_upLineV[2];//垂直( 縦 )( 0が左 ).
	std::unique_ptr< clsSPRITE2D_CENTER > m_upLineJoint[4];//角( 左上から時計回り ).

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vSize;

	float		m_fAlpha;	//アルファ値.

};
