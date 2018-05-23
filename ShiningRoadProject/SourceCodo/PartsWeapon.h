#pragma once

#include "PartsBase.h"



class clsPARTS_WEAPON : public clsPARTS_BASE
{
public:
	clsPARTS_WEAPON();
	~clsPARTS_WEAPON();

	void SetAtk( const int iAtk );
	int GetAtk() const;

	void SetLoadTime( const int iLoad );
	int GetLoadTime() const;

private:

	int m_iAtk;		//UŒ‚—Í.
	int m_iLoadTime;//‘•“UŠÔ.

};

