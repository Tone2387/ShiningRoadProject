#ifndef CAMERA_ENDING_H_
#define CAMERA_ENDING_H_


#include "Camera.h"


class clsCAMERA_ENDING : public clsCAMERA_BASE
{
public:
	clsCAMERA_ENDING();
	~clsCAMERA_ENDING();


	void Create() final;
	void Update() final;

private:

};

#endif//#ifndef CAMERA_ENDING_H_