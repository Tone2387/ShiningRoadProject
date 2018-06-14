//---------------------------------------------------------------------------
//�Q�[�����ɑ��݂��镨�̑S�Ă�����[�ʒu][��]][�g�k][�����蔻��]�����N���X.
//---------------------------------------------------------------------------
//#pragma once

#ifndef OBJECT
#define OBJECT

#include"Global.h"
#include"DX9Mesh.h"

const float g_fPercentage = 0.01f;

const float g_fGravity = 0.01f;

const D3DXVECTOR3 g_vDirForward	= D3DXVECTOR3(  0.0f,  0.0f,  1.0f);
const D3DXVECTOR3 g_vDirBack	= D3DXVECTOR3(  0.0f,  0.0f, -1.0f);
const D3DXVECTOR3 g_vDirRight	= D3DXVECTOR3(  1.0f,  0.0f,  0.0f);
const D3DXVECTOR3 g_vDirLeft	= D3DXVECTOR3( -1.0f,  0.0f,  0.0f);
const D3DXVECTOR3 g_vDirUp		= D3DXVECTOR3(  0.0f,  1.0f,  0.0f);
const D3DXVECTOR3 g_vDirDown	= D3DXVECTOR3(  0.0f, -1.0f,  0.0f);

const float g_fGroundSpece = 0.01f;

//��]�l����.
void ObjRollOverGuard(float* fRot);

D3DXVECTOR3 GetVec3Dir(const float Angle, const D3DXVECTOR3 vAxis);

class clsObject
{
public:
	clsObject():
		m_fMoveSpeed(0.0f),
		m_vMoveDir({ 0.0f, 0.0f, 0.0f })
		{ ZeroMemory(this, sizeof(clsObject)); };
	virtual ~clsObject(){};

	struct RAYSTATE
	{
		D3DXVECTOR3 vRayStart;
		D3DXVECTOR3 vAxis;
	};

	//�X�t�B�A�\����.
	struct SPHERE
	{
		D3DXVECTOR3* vCenter;//���S.
		float fRadius;	//���a.
	};

	TRANSFORM m_Trans;
	D3DXVECTOR3 m_vOldPos;

	float m_fRaySpece;
	float m_fFollPower;

	float m_fMoveSpeed;//�ŏI�I�ɉ��Z�����X�s�[�h.
	D3DXVECTOR3 m_vMoveDir;

	void Updata(const clsDX9Mesh* pGround)
	{
		m_vOldPos = m_Trans.vPos;

		Action();

		WallJudge(pGround);
	}

	virtual void Action(){};

	bool m_bGround;

	SPHERE** m_ppColSpheres;
	int m_iColSpheresMax;

	//�ʒu�֌W�֐�.
	void SetPosition(const D3DXVECTOR3& vPos){ 
		m_Trans.vPos = vPos; 
	}
	D3DXVECTOR3 GetPosition(){ return m_Trans.vPos; }
	//��]�֌W�֐�.
	void SetRotation(const D3DXVECTOR3& vRot)
	{
		m_Trans.fYaw = vRot.y;
		m_Trans.fPitch = vRot.x;
		m_Trans.fRoll = vRot.z;

		ObjRollOverGuard(&m_Trans.fYaw);
		ObjRollOverGuard(&m_Trans.fPitch);
		ObjRollOverGuard(&m_Trans.fRoll);
	}
	D3DXVECTOR3 GetRotation(){ return D3DXVECTOR3(m_Trans.fPitch, m_Trans.fYaw, m_Trans.fRoll); }
	void SetScale(float fScale){ m_Trans.vScale = D3DXVECTOR3(fScale, fScale, fScale); }
	
	void WallJudge(const clsDX9Mesh* pWall, const bool bFoll = true);

	bool WallSetAxis(const clsDX9Mesh* pWall, float* fResultDis, const D3DXVECTOR3 vRayDir);
	bool WallForward(const clsDX9Mesh* pWall, const bool bSlip = true);
	bool WallBack(const clsDX9Mesh* pWall, const bool bSlip = true);
	bool WallLeft(const clsDX9Mesh* pWall, const bool bSlip = true);
	bool WallRight(const clsDX9Mesh* pWall, const bool bSlip = true);
	bool WallUp(const clsDX9Mesh* pWall);
	bool WallUnder(const clsDX9Mesh* pWall, const bool bFoll);

	bool Intersect(
		const RAYSTATE RayState,
		const clsDX9Mesh* pTarget,//�Ώۂ̕���.
		float* pfDistance, //(out)����.
		D3DXVECTOR3* pvIntersect);//(out)�������W.

private:
	//�����ʒu����غ�݂̒��_��������.
	HRESULT FindVecticesOnPoly(
		const LPD3DXMESH pTarget,
		const DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices);

	void FreeFoll()
	{
		m_fFollPower -= g_fGravity;
	}
};

#endif