#pragma once

#include "Camera.h"


class clsCAMERA_MISSION : public clsCAMERA_BASE
{
public:
	clsCAMERA_MISSION();
	~clsCAMERA_MISSION();

	//最初( シーンのはじめ )の初期化.
	void Create() final;

private:

};

