#pragma once

#include "Sprite2DCenter.h"


class clsLINE_BOX
{
public:
	clsLINE_BOX();
	~clsLINE_BOX();


	void SetPos( const D3DXVECTOR3 &vPos );
	void SetScale( const D3DXVECTOR3 &vScale );
	void SetScale( const float &fScale );

	D3DXVECTOR3 GetPos() const;
	D3DXVECTOR3 GetScale() const;

	void SetAlpha( const float fAlpha );
	//範囲をoverするとfalseが返ってくる.
	bool AddAlpha( const float fAlpha );

private:


	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vPScale;

	float		m_fAlpha;	//アルファ値.

};
