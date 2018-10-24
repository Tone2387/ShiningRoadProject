//---------------------------------------------------------------------------
//ゲーム内に存在する物体全てが持つ[位置][回転][拡縮][当たり判定]を持つクラス.
//---------------------------------------------------------------------------
//#pragma once

#ifndef OBJECT
#define OBJECT

#include"Global.h"
#include"DX9Mesh.h"

#include"Stage.h"

const float g_fPercentage = 0.01f;//百分率に変換用.
const float g_fDistanceReference = 0.01f;//距離の規定値.
const float g_fDirectionReference = 0.001f;//回転値の規定値.
const float g_fGravity = 0.01f;//重力.
const float g_fGroundSpece = 0.01f;
const float g_fRaySpace = 0.01f;

const D3DXVECTOR3 g_vDirForward	= D3DXVECTOR3(  0.0f,  0.0f,  1.0f);
const D3DXVECTOR3 g_vDirBack	= D3DXVECTOR3(  0.0f,  0.0f, -1.0f);
const D3DXVECTOR3 g_vDirRight	= D3DXVECTOR3(  1.0f,  0.0f,  0.0f);
const D3DXVECTOR3 g_vDirLeft	= D3DXVECTOR3( -1.0f,  0.0f,  0.0f);
const D3DXVECTOR3 g_vDirUp		= D3DXVECTOR3(  0.0f,  1.0f,  0.0f);
const D3DXVECTOR3 g_vDirDown	= D3DXVECTOR3(  0.0f, -1.0f,  0.0f);

enum enVec3Direction
{
	enForward,
	enBack,
	enRight,
	enLeft,
	enUp,
	enDown,

	enDirectionSize
};

const D3DXVECTOR3 g_vDirs[enDirectionSize]
{
	{ 0.0f,  0.0f,  1.0f},
	{ 0.0f,  0.0f, -1.0f},
	{ 1.0f,  0.0f,  0.0f},
	{-1.0f,  0.0f,  0.0f},
	{ 0.0f,  1.0f,  0.0f},
	{ 0.0f, -1.0f,  0.0f},
};

//回転値調整.
void ObjRollOverGuard(float* fRot);
//現在の方向
D3DXVECTOR3 GetVec3Dir(const float fAngle, const D3DXVECTOR3 vAxis);

class clsObject
{
public:
	clsObject();
	virtual ~clsObject();

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
	D3DXVECTOR3 m_vCenterPos;//オブジェクトの中心位置(モデルに合わせた中心座標).

	float m_fRaySpece;
	float m_fFollPower;

	float m_fMoveSpeed;//最終的に加算されるスピード.
	D3DXVECTOR3 m_vMoveDir;

	bool m_bGround;
	bool m_NoFollObj;

	std::vector<SPHERE> m_v_Spheres;

	void Action(clsStage* pStage);

	virtual void ActionProduct();

	virtual void Render(
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj,
		const D3DXVECTOR3& vLight,
		const D3DXVECTOR3& vEye)
	{

	}

	//位置関係関数.
	void SetPosition(const D3DXVECTOR3& vPos){ 
		m_Trans.vPos = vPos; 
	}
	D3DXVECTOR3 GetPosition(){ return m_Trans.vPos; }
	const D3DXVECTOR3 GetCenterPos()const;//中心座標を渡す.
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

	//スフィア衝突判定関数.
	bool Collision(SPHERE pAttacker, SPHERE pTarget);//Sphere対Sphereの当たり判定.
	bool ObjectCollision(std::vector<SPHERE> pTarget);

	D3DXVECTOR3 GetRotation(){ return D3DXVECTOR3(m_Trans.fPitch, m_Trans.fYaw, m_Trans.fRoll); }
	void SetScale(const float fScale){ m_Trans.vScale = D3DXVECTOR3(fScale, fScale, fScale); }
	void SetScale(const D3DXVECTOR3 vScale){ m_Trans.vScale = vScale; }

	bool WallJudge(clsStage* const pStage);

	bool WallSetAxis(const clsDX9Mesh* pWall, float* fResultDis, const D3DXVECTOR3 vRayDir);
	bool WallForward(const clsDX9Mesh* pWall, const bool bSlip = true);
	bool WallBack(const clsDX9Mesh* pWall, const bool bSlip = true);
	bool WallLeft(const clsDX9Mesh* pWall, const bool bSlip = true);
	bool WallRight(const clsDX9Mesh* pWall, const bool bSlip = true);
	bool WallUp(const clsDX9Mesh* pWall);
	bool WallUnder(const clsDX9Mesh* pWall);

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

	void FreeFoll();
};

#endif