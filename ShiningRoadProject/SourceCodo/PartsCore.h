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

	int m_iHp;
	int m_iActTime;	//Šˆ“®ŽžŠÔ(ActivityTime).

};

