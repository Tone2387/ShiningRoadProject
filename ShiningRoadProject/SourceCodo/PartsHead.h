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

	int m_iHp;

};

