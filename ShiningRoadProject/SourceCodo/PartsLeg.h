#pragma once


#include "PartsBase.h"


class clsPARTS_LEG : public clsPARTS_BASE
{
public:
	clsPARTS_LEG();
	~clsPARTS_LEG();

	void SetWalkSpd( const float fSpd );
	float GetWalkSpd() const;

	void SetBoostSpd( const float fSpd );
	float GetBoostSpd() const;

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

	float m_fWalkSpd;
	float m_fBoostSpd;

};

