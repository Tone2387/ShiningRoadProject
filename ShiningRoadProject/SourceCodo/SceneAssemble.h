#pragma once

#include "SceneClass.h"
#include "CameraAssemble.h"




//================================//
//========== ‘g‚ÝŠ·‚¦ƒNƒ‰ƒX ==========//
//================================//
class clsASSEMBLE : public clsSCENE_BASE
{
public:
	clsASSEMBLE( clsPOINTER_GROUP* const ptrGroup );
	~clsASSEMBLE();


private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &nextScene ) final;
	void RenderProduct() final;


	clsSPRITE2D_CENTER* m_pSprite;

	clsCharaStatic* m_pTestChara;
	clsCharaStatic* m_pParts;


};



