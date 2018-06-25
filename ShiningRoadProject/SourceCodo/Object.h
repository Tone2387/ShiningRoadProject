//---------------------------------------------------------------------------
//ゲーム内に存在する物体全てが持つ[位置][回転][拡縮][当たり判定]を持つクラス.
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
const float g_fRaySpace = 1.0f;

//回転値調整.
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

	//スフィア構造体.
	struct SPHERE
	{
		D3DXVECTOR3* vCenter;//中心.
		float fRadius;	//半径.
	};

	TRANSFORM m_Trans;
	D3DXVECTOR3 m_vOldPos;
	D3DXVECTOR3 m_vCenterPos;//オブジェクトの中心.

	float m_fRaySpece;
	float m_fFollPower;

	float m_fMoveSpeed;//最終的に加算されるスピード.
	D3DXVECTOR3 m_vMoveDir;

	void Updata(const clsDX9Mesh* pGround)
	{
		m_vOldPos = m_Trans.vPos;

		ActionProduct();

		WallJudge(pGround);
	}

	virtual void tenshi(){};

	virtual void ActionProduct(){};

	bool m_bGround;

	SPHERE** m_ppColSpheres;
	int m_iColSpheresMax;

	//位置関係関数.
	void SetPosition(const D3DXVECTOR3& vPos){ 
		m_Trans.vPos = vPos; 
	}
	D3DXVECTOR3 GetPosition(){ return m_Trans.vPos; }
	//回転関係関数.
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
		const clsDX9Mesh* pTarget,//対象の物体.
		float* pfDistance, //(out)距離.
		D3DXVECTOR3* pvIntersect);//(out)交差座標.

private:
	//交差位置のﾎﾟﾘｺﾞﾝの頂点を見つける.
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