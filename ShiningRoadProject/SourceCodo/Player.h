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

	void ActionProduct() override;

	float GetLockCircleScale();
	D3DXVECTOR3 GetLockRangeTmp();
	D3DXVECTOR3 GetLockCenterPos();

	bool GetTargetPos(D3DXVECTOR3& vTmpPos);

	D3DXVECTOR3 GetCamTargetPos();
	D3DXVECTOR3 GetLookTargetPos();

private:
	clsInputRobo* m_pInput;

	D3DXVECTOR3 m_vLockCenterPos;
	bool m_bCamPosXSwitch;//false:‰E.

	void UpdateCamTargetPos();

	D3DXVECTOR3 m_vCamTargetPos;
	D3DXVECTOR3 m_vLookTargetPos;
	D3DXVECTOR3 m_vLockRange;
};