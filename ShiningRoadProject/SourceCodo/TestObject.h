#pragma once


#ifdef Tahara
//2018/05/16( 水 ).
//関数「Init」に「clsPOINTER_GROUP」型の引数を追加し、.
//追加したメンバ変数( m_wpResource、m_wpEffects、m_wpSound ).
//にアドレスを割り当てる処理を追加しました.
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