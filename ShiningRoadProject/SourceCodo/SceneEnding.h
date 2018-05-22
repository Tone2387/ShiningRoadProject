#pragma once

#include "SceneClass.h"
#include "CameraEnding.h"


//================================//
//========== エンディングクラス ==========//
//================================//
class clsENDING : public clsSCENE_BASE
{
public:
	clsENDING( clsPOINTER_GROUP* const ptrGroup );
	~clsENDING();


private:
	void CreateProduct() final;
	void UpdateProduct( enSCENE &nextScene ) final;
	void RenderProduct() final;


};
