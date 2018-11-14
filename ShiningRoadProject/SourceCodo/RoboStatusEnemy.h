#ifndef ENEMY_ROBO_STATE_H_
#define ENEMY_ROBO_STATE_H_

#include "RoboStatusBase.h"


class clsROBO_STATUS_ENEMY : public clsROBO_STATUS
{
public:
	//引数は何行目を読み込むかを表す.
	clsROBO_STATUS_ENEMY( const int iRow = 0 );
	~clsROBO_STATUS_ENEMY();

private:
	//外部からデータを読み込む.
	void LoadFileData( const char* sFilePath ) override;

private:
	//csvデータの何行目を読み込むか.
	const int m_iFILE_ROW;

};

#endif//#ifndef ENEMY_ROBO_STATE_H_