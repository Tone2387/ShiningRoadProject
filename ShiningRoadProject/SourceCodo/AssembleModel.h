#pragma once

#include "PartsBase.h"

//アセンブルシーンのモデルさん.
class clsASSEMBLE_MODEL
{
public:
	clsASSEMBLE_MODEL();
	~clsASSEMBLE_MODEL();

	void Create();

private:

	//パーツの数分のポインタ.
	clsPARTS_BASE**	m_wppParts;
	UCHAR			m_ucPartsMax;

};

