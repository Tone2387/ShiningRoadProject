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

	void Action() final;

	float GetLockCircleScale();
	D3DXVECTOR3 GetLockRangeTmp();
	D3DXVECTOR3 GetLockCenterPos();

	bool GetTargetPos(D3DXVECTOR3& vTmpPos);

	D3DXVECTOR3 GetCamTargetPos();
	D3DXVECTOR3 GetLookTargetPos();

private:
	clsInputRobo* m_pInput;

	D3DXVECTOR3 m_vLockCenterPos;

	D3DXVECTOR3 m_vCamTargetPos;
	D3DXVECTOR3 m_vLookTargetPos;
	D3DXVECTOR3 m_vLockRange;
	D3DXVECTOR3 m_vCamPosDivia;//自機の位置からカメラ位置までの差.
};