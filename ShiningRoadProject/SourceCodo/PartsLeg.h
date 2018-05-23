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

	float m_fWalkSpd;
	float m_fBoostSpd;

};

