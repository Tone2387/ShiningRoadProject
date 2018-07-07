#include "LineBox.h"

using namespace std;


const char LINE_MAX = 2;
const char JOINT_MAX = 4;

const char* sLINE_PATH = "Data\\Image\\UiLineBody.png";
const char* sJOINT_PATH = "Data\\Image\\UiLineJoint.png";

const WHSIZE_FLOAT SS_DISP = { 1.0f, 1.0f };
const WHSIZE_FLOAT SS_ANIM = { 1.0f, 1.0f };

//äpÇÃâÒì]äp.
const float fROT_ROLL = static_cast< float >( 3.14 * 0.5 );

const D3DXVECTOR3 vSCALE = { 16.0f, 16.0f, 1.0f };

clsLINE_BOX::clsLINE_BOX()
	:m_vPos()
{
	m_vPos = { WND_W/2, WND_H/2, 0.0f };
	m_vSize = { WND_W/2, WND_H/2, 0.0f };
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

	for( char i=0; i<LINE_MAX; i++ ){
		if( m_upLineH[i] ) continue;
		m_upLineH[i] = make_unique< clsSPRITE2D_CENTER >();
		m_upLineH[i]->Create( pDevice11, pContext11, sLINE_PATH, ss );
		m_upLineH[i]->SetScale( vSCALE );
	}

	for( char i=0; i<LINE_MAX; i++ ){
		if( m_upLineV[i] ) continue;
		m_upLineV[i] = make_unique< clsSPRITE2D_CENTER >();
		m_upLineV[i]->Create( pDevice11, pContext11, sLINE_PATH, ss );
		m_upLineV[i]->SetScale( vSCALE );
		m_upLineV[i]->SetRot( { 0.0f, 0.0f, fROT_ROLL } );
	}


	for( char i=0; i<JOINT_MAX; i++ ){
		if( m_upLineJoint[i] ) continue;
		m_upLineJoint[i] = make_unique< clsSPRITE2D_CENTER >();
		m_upLineJoint[i]->Create( pDevice11, pContext11, sJOINT_PATH, ss );
		m_upLineJoint[i]->SetScale( vSCALE );
		m_upLineJoint[i]->SetRot( { 0.0f, 0.0f, ( static_cast<float>( i ) * fROT_ROLL ) } );
	}

}

//î†Çå`çÏÇÈ.
void clsLINE_BOX::SetUp()
{
	D3DXVECTOR3 vHarfSize = m_vSize * 0.5f;

	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineH[i]->SetPos( m_vPos );
		m_upLineV[i]->SetPos( m_vPos );
	}
	for( char i=0; i<JOINT_MAX; i++ ){
		m_upLineJoint[i]->SetPos( m_vPos );
	}

	m_upLineH[0]->AddPos( { 0.0f, -vHarfSize.y, 0.0f } );
	m_upLineH[1]->AddPos( { 0.0f,  vHarfSize.y, 0.0f } );
	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineH[i]->SetScale( { m_vSize.x, vSCALE.y, 1.0f } );
	}

	m_upLineV[0]->AddPos( { -vHarfSize.x, 0.0f, 0.0f } );
	m_upLineV[1]->AddPos( {  vHarfSize.x, 0.0f, 0.0f } );
	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineV[i]->SetScale( { m_vSize.y, vSCALE.x, 1.0f } );
	}

	m_upLineJoint[0]->SetPos( { m_upLineV[0]->GetPos().x, m_upLineH[0]->GetPos().y, 1.0f } );
	m_upLineJoint[1]->SetPos( { m_upLineV[1]->GetPos().x, m_upLineH[0]->GetPos().y, 1.0f } );
	m_upLineJoint[2]->SetPos( { m_upLineV[1]->GetPos().x, m_upLineH[1]->GetPos().y, 1.0f } );
	m_upLineJoint[3]->SetPos( { m_upLineV[0]->GetPos().x, m_upLineH[1]->GetPos().y, 1.0f } );

}

void clsLINE_BOX::Update()
{
	SetUp();
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

//îÕàÕÇoverÇ∑ÇÈÇ∆falseÇ™ï‘Ç¡ÇƒÇ≠ÇÈ.
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

