#include "LineBox.h"

using namespace std;


const char LINE_MAX = 2;
const char JOINT_MAX = 4;

const char* sLINE_PATH = "Data\\Image\\UiLineBody.png";
const char* sJOINT_PATH = "Data\\Image\\UiLineJoint.png";

const WHSIZE_FLOAT SS_DISP = { 1.0f, 1.0f };
const WHSIZE_FLOAT SS_ANIM = { 1.0f, 1.0f };

clsLINE_BOX::clsLINE_BOX()
{
}

clsLINE_BOX::~clsLINE_BOX()
{
	for( char i=0; i<JOINT_MAX; i++ ){
		if( !m_upLineJoint[i] ) continue;
		m_upLineJoint[i].reset( nullptr );
	}

	for( char i=0; i<LINE_MAX; i++ ){
		if( !m_upLineV[i] ) continue;
		m_upLineV[i].reset( nullptr );
	}

	for( char i=0; i<LINE_MAX; i++ ){
		if( !m_upLineH[i] ) continue;
		m_upLineH[i].reset( nullptr );
	}
}

void clsLINE_BOX::Create(		
	ID3D11Device* const pDevice11,
	ID3D11DeviceContext* const pContext11 )
{
	SPRITE_STATE ss;
	ss.Disp = SS_DISP;
	ss.Anim = SS_ANIM;

	D3DXVECTOR3 tmp = { 600.0f, 600.0f, 1.0f };

	for( char i=0; i<LINE_MAX; i++ ){
		if( m_upLineH[i] ) continue;
		m_upLineH[i] = make_unique< clsSPRITE2D_CENTER >();
		m_upLineH[i]->Create( pDevice11, pContext11, sLINE_PATH, ss );
		m_upLineH[i]->SetScale( tmp );
	}

	for( char i=0; i<LINE_MAX; i++ ){
		if( m_upLineV[i] ) continue;
		m_upLineV[i] = make_unique< clsSPRITE2D_CENTER >();
		m_upLineV[i]->Create( pDevice11, pContext11, sLINE_PATH, ss );
		m_upLineV[i]->SetScale( tmp );
	}


	for( char i=0; i<JOINT_MAX; i++ ){
		if( m_upLineJoint[i] ) continue;
		m_upLineJoint[i] = make_unique< clsSPRITE2D_CENTER >();
		m_upLineJoint[i]->Create( pDevice11, pContext11, sJOINT_PATH, ss );
		m_upLineJoint[i]->SetScale( tmp );
	}

}


void clsLINE_BOX::Render()
{
	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineH[i]->Render();
	}

	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineV[i]->Render();
	}


	for( char i=0; i<JOINT_MAX; i++ ){
		m_upLineJoint[i]->Render();
	}

}





void clsLINE_BOX::SetPos( const D3DXVECTOR3 &vPos )
{
	m_vPos = vPos;

}

void clsLINE_BOX::SetSize( const D3DXVECTOR3 &vScale )
{
	m_vSize = vScale;


}

void clsLINE_BOX::SetSize( const float &fScale )
{
	m_vSize = { fScale, fScale, 1.0f };


}

D3DXVECTOR3 clsLINE_BOX::GetPos() const
{
	return m_vPos;
}
D3DXVECTOR3 clsLINE_BOX::GetSize() const
{
	return m_vSize;
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

