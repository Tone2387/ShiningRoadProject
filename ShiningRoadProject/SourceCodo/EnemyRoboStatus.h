#ifndef ENEMY_ROBO_STATE_H_
#define ENEMY_ROBO_STATE_H_

#include "RoboStatusBase.h"


class clsENEMY_ROBO_STATUS : public clsROBO_STATUS
{
public:
	clsENEMY_ROBO_STATUS();
	~clsENEMY_ROBO_STATUS();

private:
	//外部からデータを読み込む.
	void LoadFileData( const char* sFilePath ) override;

};

#endif//#ifndef ENEMY_ROBO_STATE_H_