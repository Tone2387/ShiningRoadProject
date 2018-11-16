#pragma once


#ifdef Tahara
//2018/05/16( �� ).
//�֐��uInit�v�ɁuclsPOINTER_GROUP�v�^�̈�����ǉ����A.
//�ǉ����������o�ϐ�( m_wpResource�Am_wpEffects�Am_wpSound ).
//�ɃA�h���X�����蓖�Ă鏈����ǉ����܂���.
#endif//#ifdef Tahara


//#ifndef TESTOBJECT
//#define TESTOBJECT

#include"Robo.h"
#include"AIRoboCommand.h"

//#ifdef Tahara
//#include "PtrGroup.h"
//#endif//#ifdef Tahara

class clsTestObj final : public clsRobo
{
public:
	clsTestObj();
	~clsTestObj();

	void Init(clsPOINTER_GROUP* const pPtrGroup);
	
	void Init(clsPOINTER_GROUP* const pPtrGroup,
		std::string strEnemyFolderName);

	void Action() final;

	void InhUpdate();//Inheritance

	void SwitchMove();

private:
	clsEnemyRobo* m_pAI;
	bool m_bAct;
};

//#endif