#include "WindowBox.h"

using namespace std;

const char* sFILE_NAME = "Data\\Image\\AssembleUi\\BoxBack.png";

const char LINE_MAX = 2;
const char JOINT_MAX = 4;




clsWINDOW_BOX::clsWINDOW_BOX(		
	ID3D11Device* const pDevice11,
	ID3D11DeviceContext* const pContext11 ) : clsLINE_BOX( pDevice11, pContext11 )
{
	SPRITE_STATE ss;
	ss.Disp = { 1.0f, 1.0f };

	m_upBack = make_unique< clsSPRITE2D_CENTER >();
	m_upBack->Create( pDevice11, pContext11, sFILE_NAME, ss );

	m_upText = make_unique< clsUiText >();
	m_upText->Create( pContext11, WND_W, WND_H, 1.0f );
}

clsWINDOW_BOX::~clsWINDOW_BOX()
{
}


void clsWINDOW_BOX::Update()
{
	SetUpBox();
	SetUpJointSize();

	m_upBack->SetPos( m_vPos );
	m_upBack->SetScale( m_vSize );
	m_upBack->SetAlpha( m_fAlpha );

	m_upText->SetPos( { m_vTextOffset.x + m_vPos.x, m_vTextOffset.y + m_vPos.y } );
}

void clsWINDOW_BOX::Render()
{
	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineH[i]->Render();
		m_upLineV[i]->Render();
	}


	for( char i=0; i<JOINT_MAX; i++ ){
		m_upLineJoint[i]->Render();
	}

	m_upBack->Render();

	m_upText->Render();
}


//----- 文字用 -----//.
//( ウィンドウの左上からの )座標.
void clsWINDOW_BOX::SetTextPos( const D3DXVECTOR2 &vPos )
{
	m_vTextOffset = vPos;
	m_upText->SetPos( { m_vTextOffset.x + m_vPos.x, m_vTextOffset.y + m_vPos.y } );
}

void clsWINDOW_BOX::SetTextScale( const float fScale )
{
	m_upText->SetScale( fScale );
}

void clsWINDOW_BOX::SetText( const char* sText )
{
	m_upText->SetText( sText );
}

void clsWINDOW_BOX::SetTextColor( const D3DXVECTOR4 &vColor )
{
	m_upText->SetColor( vColor );
}
