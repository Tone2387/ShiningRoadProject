#pragma once

#include "SceneClass.h"
#include "CameraAssemble.h"




//================================//
//========== ‘g‚ÝŠ·‚¦ƒNƒ‰ƒX ==========//
//================================//
class clsSCENE_ASSEMBLE : public clsSCENE_BASE
{
public:
	clsSCENE_ASSEMBLE( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_ASSEMBLE();


private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &nextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;


	clsSPRITE2D_CENTER* m_pSprite;

	clsCharaStatic* m_pTestChara;
	clsCharaStatic* m_pParts;


};



