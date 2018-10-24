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

}

clsWINDOW_BOX::~clsWINDOW_BOX()
{
}


void clsWINDOW_BOX::Update()
{
	clsLINE_BOX::Update();

	m_upBack->SetPos( m_vPos );
	m_upBack->SetScale( m_vSize );

}

void clsWINDOW_BOX::Render()
{

	m_upBack->Render();
	clsLINE_BOX::Render();


}


void clsWINDOW_BOX::SetAlpha( const float fAlpha )
{
	clsLINE_BOX::SetAlpha( fAlpha );
	SetAlphaBack( fAlpha );

}

void clsWINDOW_BOX::SetAlphaBack( const float fAlpha )
{
	m_fAlpha = fAlpha;
	m_upBack->SetAlpha( m_fAlpha );
}
