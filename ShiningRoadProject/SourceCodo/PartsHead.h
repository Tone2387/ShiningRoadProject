#pragma once

#include "PartsBase.h"



class clsPARTS_HEAD : public clsPARTS_BASE
{
public:
	clsPARTS_HEAD();
	~clsPARTS_HEAD();

	void SetHp( const int iHp );
	int GetHp() const;
	void AddHp( const int iHp );


private:
	//----- 各シーンごとの関数 -----//.
	void CreateProduct() final;//各シーンのCreate.
	void UpdateProduct() final;//各シーンのUpdate.
	void RenderProduct(
		const D3DXMATRIX& const mView, 
		const D3DXMATRIX& const mProj, 
		const D3DXVECTOR3& const vLight, 
		const D3DXVECTOR3& const vEye,
		const D3DXVECTOR4 &vColor,
		const bool isAlpha ) final;//各シーンのRender.
	//----- 各シーンごとの関数 -----//.

	int m_iHp;

};

