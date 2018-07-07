#include "LineBox.h"


clsLINE_BOX::clsLINE_BOX()
{
}

clsLINE_BOX::~clsLINE_BOX()
{
}


void clsLINE_BOX::SetPos( const D3DXVECTOR3 &vPos )
{

}

void clsLINE_BOX::SetScale( const D3DXVECTOR3 &vScale )
{

}

void clsLINE_BOX::SetScale( const float &fScale )
{

}

D3DXVECTOR3 clsLINE_BOX::GetPos() const
{
	return m_vPos;
}
D3DXVECTOR3 clsLINE_BOX::GetScale() const
{
	return m_vPScale;
}

void clsLINE_BOX::SetAlpha( const float fAlpha )
{
	m_fAlpha = fAlpha;
}

//”ÍˆÍ‚ðover‚·‚é‚Æfalse‚ª•Ô‚Á‚Ä‚­‚é.
bool clsLINE_BOX::AddAlpha( const float fAlpha )
{
	float tmp = m_fAlpha + fAlpha;

	if( tmp > 1.0f ){
		m_fAlpha = 1.0f;
		return false;
	}
	else if( tmp < 0.0f ){
		m_fAlpha = 0.0f;
		return false;
	}

	m_fAlpha = tmp;
	return false;
}

