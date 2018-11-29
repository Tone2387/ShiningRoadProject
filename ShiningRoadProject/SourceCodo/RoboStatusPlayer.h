#ifndef PLAYER_ROBO_STATUS_H_
#define PLAYER_ROBO_STATUS_H_

#include "RoboStatusBase.h"

class clsROBO_STATUS_PLAYER : public clsROBO_STATUS
{
public:
	clsROBO_STATUS_PLAYER();
	~clsROBO_STATUS_PLAYER();


	//�N���A��ʂŎg�� : �^�C�g���p�̏������p�̃f�[�^��p�ӂ��� : �N���A�������{���o���Ă���.
	void SaveHeroData();

	//�^�C�g����ʂł̏�������AssembleModel��new�̑O�Ɏg��.
	void LodeHeroData();

private:

	//�O������f�[�^��ǂݍ���.
	void LoadFileData( const char* sFilePath ) override;



	//�N���A�f�[�^( �^�C�g���ŕK�v ).
	ROBO_STATE_DATA m_RoboStateDataHero;

};




#endif//#ifndef PLAYER_ROBO_STATUS_H_