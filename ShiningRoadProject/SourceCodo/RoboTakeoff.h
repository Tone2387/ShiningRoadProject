#ifndef ROBO_TAKEOFF_H_
#define ROBO_TAKEOFF_H_

#include "Robo.h"

class clsROBO_TAKEOFF : public clsRobo
{
public:
	clsROBO_TAKEOFF();
	~clsROBO_TAKEOFF();

	void IgnitionCoreBoost( const bool isIgnition );

private:

};


#endif//#ifndef ROBO_TAKEOFF_H_