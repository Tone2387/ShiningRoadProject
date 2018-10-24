#ifndef CAMERA_GAME_OVER_H_
#define CAMERA_GAME_OVER_H_

#include "Camera.h"


class clsCAMERA_GAMEOVER : public clsCAMERA_BASE
{
public:
	clsCAMERA_GAMEOVER();
	~clsCAMERA_GAMEOVER();


	void Create() final;
	void Update() final;

private:

};

#endif//#ifndef CAMERA_GAME_OVER_H_