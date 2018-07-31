#pragma once

#include"CharaSkinMesh.h"
#include"Robo.h"
#include"InputRobo.h"

//#ifdef Tahara
//#include "PtrGroup.h"
//#endif//#ifdef Tahara

class clsPlayer final : public clsRobo
{
public:
	clsPlayer();
	~clsPlayer();

	void Init(clsPOINTER_GROUP* const pPtrGroup);

	void Action(const clsDX9Mesh* pWall);

	void InhUpdate();//Inheritance

private:
	clsInputRobo* m_pInput;

	D3DXVECTOR3 m_vCamPos;
};