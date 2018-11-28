#ifndef ENEMY_ROBO_STATE_H_
#define ENEMY_ROBO_STATE_H_


class clsFILE;
#include "RoboStatusBase.h"


class clsROBO_STATUS_ENEMY : public clsROBO_STATUS
{
public:
	//�����͉��s�ڂ�ǂݍ��ނ���\��.
	clsROBO_STATUS_ENEMY( const int iRow = 0 );
	~clsROBO_STATUS_ENEMY();

private:
	//�O������f�[�^��ǂݍ���.
	void LoadFileData( const char* sFilePath ) override;


private:
	//csv�f�[�^�̉��s�ڂ�ǂݍ��ނ�.
	const int m_iFILE_ROW;

};

#endif//#ifndef ENEMY_ROBO_STATE_H_