#include "SceneEnding.h"

//================================//
//========== エンディングクラス ==========//
//================================//
clsENDING::clsENDING( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
{
}

clsENDING::~clsENDING()
{
}

void clsENDING::CreateProduct()
{

}


void clsENDING::UpdateProduct( enSCENE &nextScene )
{


	DebugChangeScene( nextScene );
}

void clsENDING::RenderProduct()

{
	//Render関数の引数を書きやすくするための変数.
	D3DXVECTOR3 vCamPos = m_wpCamera->GetPos();

}


