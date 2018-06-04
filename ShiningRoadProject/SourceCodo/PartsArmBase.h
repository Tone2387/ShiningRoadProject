#pragma once


#include "PartsBase.h"



class clsPARTS_ARM_BASE : public clsPARTS_BASE
{
public:
	clsPARTS_ARM_BASE();
	virtual ~clsPARTS_ARM_BASE();

private:
	//----- 各シーンごとの関数 -----//.
	virtual void CreateProduct() override;//各シーンのCreate.
	virtual void UpdateProduct() override;//各シーンのUpdate.
	virtual void RenderProduct(
		const D3DXMATRIX& const mView, 
		const D3DXMATRIX& const mProj, 
		const D3DXVECTOR3& const vLight, 
		const D3DXVECTOR3& const vEye,
		const D3DXVECTOR4 &vColor,
		const bool isAlpha ) override;//各シーンのRender.
	//----- 各シーンごとの関数 -----//.

};

