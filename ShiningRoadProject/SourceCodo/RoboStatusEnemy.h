#ifndef ENEMY_ROBO_STATE_H_
#define ENEMY_ROBO_STATE_H_

#include "RoboStatusBase.h"


class clsROBO_STATUS_ENEMY : public clsROBO_STATUS
{
public:
	clsROBO_STATUS_ENEMY();
	~clsROBO_STATUS_ENEMY();

private:
	//外部からデータを読み込む.
	void LoadFileData( const char* sFilePath ) override;

};

#endif//#ifndef ENEMY_ROBO_STATE_H_