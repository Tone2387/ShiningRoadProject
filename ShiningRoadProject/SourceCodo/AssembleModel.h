#pragma once

#include "PartsBase.h"
#include "FactoryParts.h"

//アセンブルシーンのモデルさん.
class clsASSEMBLE_MODEL
{
public:
	clsASSEMBLE_MODEL();
	~clsASSEMBLE_MODEL();

	//アセンブルシーンの各関数内で使います.
	void Create();
	void UpDate();
	void Render(
		const D3DXMATRIX& const mView, 
		const D3DXMATRIX& const mProj, 
		const D3DXVECTOR3& const vLight, 
		const D3DXVECTOR3& const vEye,
		const D3DXVECTOR4 &vColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		const bool isAlpha = false );

	//モデルつけ変え.
	void AttachModel( const enPARTS enParts, clsD3DXSKINMESH* const pModel );


private:

	//モデルの初期セット.
	void Init();

	clsFACTORY_PARTS*	m_pPartsFactory;
	//パーツの数分のポインタ.
	clsPARTS_BASE**	m_wppParts;
	UCHAR			m_ucPartsMax;//パーツの最大数.

};

