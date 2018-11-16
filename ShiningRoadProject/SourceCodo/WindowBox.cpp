#include "WindowBox.h"


using namespace std;

namespace{


	const char LINE_MAX = 2;
	const char JOINT_MAX = 4;
}



clsWINDOW_BOX::clsWINDOW_BOX(		
	ID3D11Device* const pDevice11,
	ID3D11DeviceContext* const pContext11 ) : clsLINE_BOX( pDevice11, pContext11 )
{
	SPRITE_STATE ss;
	ss.Disp = { 1.0f, 1.0f };

	const char* sFILE_NAME = "Data\\Image\\AssembleUi\\BoxBack.png";
	m_upBack = make_unique< clsSPRITE2D_CENTER >();
	m_upBack->Create( pDevice11, pContext11, sFILE_NAME, ss );

}

