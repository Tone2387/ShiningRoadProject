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

	void Create() final;
	void Update( enSCENE &nextScene ) final;
	void Render(
		const D3DXMATRIX &mView, 
		const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight ) const final;

private:


};
