#pragma once

#include "Global.h"
#include "SkinMesh.h"

#include "Object.h"

//========== パーツの基底クラス ==========//.
class clsPARTS_BASE : public clsSkinMesh , public clsObject
{
public:
	clsPARTS_BASE();
	virtual ~clsPARTS_BASE();
	
	//作成時.
	void Create();
	//毎フレーム.
	void Update();
	//描画.
	void Render(
		const D3DXMATRIX& const mView, 
		const D3DXMATRIX& const mProj, 
		const D3DXVECTOR3& const vLight, 
		const D3DXVECTOR3& const vEye,
		const D3DXVECTOR4 &vColor,
		const bool isAlpha );

protected:
	//----- 各シーンごとの関数 -----//.
	virtual void CreateProduct() = 0;//各シーンのCreate.
	virtual void UpdateProduct() = 0;//各シーンのUpdate.
	virtual void RenderProduct(
		const D3DXMATRIX& const mView, 
		const D3DXMATRIX& const mProj, 
		const D3DXVECTOR3& const vLight, 
		const D3DXVECTOR3& const vEye,
		const D3DXVECTOR4 &vColor,
		const bool isAlpha ) = 0;//各シーンのRender.
	//----- 各シーンごとの関数 -----//.

private:


};

