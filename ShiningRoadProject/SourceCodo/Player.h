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

	//void Action(clsStage* const pWall);

	void ActionProduct() override;

	void InhUpdate();//Inheritance

	D3DXVECTOR3 GetCamTargetPos();
	D3DXVECTOR3 GetLookTargetPos();

private:
	bool m_bCamPosXSwitch;//false:‰E.

	clsInputRobo* m_pInput;

	void UpdateCamTargetPos();

	D3DXVECTOR3 m_vCamTargetPos;
	D3DXVECTOR3 m_vLookTargetPos;
};