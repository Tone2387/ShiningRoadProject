#pragma once

#include "PartsBase.h"
#include "FactoryParts.h"

//アセンブルシーンのモデルさん.
class clsASSEMBLE_MODEL
{
public:
	clsASSEMBLE_MODEL();
	~clsASSEMBLE_MODEL();

	void Create();

private:

	clsFACTORY_PARTS*	m_pPartsFactory;
	//パーツの数分のポインタ.
	clsPARTS_BASE**	m_wppParts;
	UCHAR			m_ucPartsMax;

};

