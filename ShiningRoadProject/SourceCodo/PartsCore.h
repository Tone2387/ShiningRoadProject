#pragma once


#include "PartsBase.h"



class clsPARTS_CORE : public clsPARTS_BASE
{
public:
	clsPARTS_CORE();
	~clsPARTS_CORE();

	void SetHp( const int iHp );
	int GetHp() const;
	void AddHp( const int iHp );

	void SetActTime( const int iTime );
	int GetActTime() const;
	void AddActTime( const int iTime );

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
	int m_iActTime;	//活動時間(ActivityTime).

};

