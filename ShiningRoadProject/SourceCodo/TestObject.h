#pragma once


#ifdef Tahara
//2018/05/16( 水 ).
//関数「Init」に「clsPOINTER_GROUP」型の引数を追加し、.
//追加したメンバ変数( m_wpResource、m_wpEffects、m_wpSound ).
//にアドレスを割り当てる処理を追加しました.
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

	void Action(const clsDX9Mesh* pWall);

	void tenshi()final{ Action(NULL); };

	void InhUpdate();//Inheritance

private:
	clsInputRobo* m_pInput;
};

//#endif