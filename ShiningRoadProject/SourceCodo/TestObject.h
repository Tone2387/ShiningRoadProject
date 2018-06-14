#pragma once


#ifdef Tahara
//2018/05/16( �� ).
//�֐��uInit�v�ɁuclsPOINTER_GROUP�v�^�̈�����ǉ����A.
//�ǉ����������o�ϐ�( m_wpResource�Am_wpEffects�Am_wpSound ).
//�ɃA�h���X�����蓖�Ă鏈����ǉ����܂���.
#endif//#ifdef Tahara


//#ifndef TESTOBJECT
//#define TESTOBJECT

#include"CharaSkinMesh.h"
#include"Robo.h"
#include"InputRobo.h"

//#ifdef Tahara
//#include "PtrGroup.h"
//#endif//#ifdef Tahara

class clsTestObj final : public clsRobo
{
public:
	clsTestObj();
	~clsTestObj();

	void Init(HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11, 
		clsDxInput* pControll,
		clsPOINTER_GROUP* const pPtrGroup );

	void Act(const clsDX9Mesh* pWall);

	void Action()final{ Act(NULL); };

	void InhUpdate();//Inheritance

private:
	clsInputRobo* m_pInput;
};

//#endif