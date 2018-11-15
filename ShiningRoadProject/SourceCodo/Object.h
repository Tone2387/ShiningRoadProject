//---------------------------------------------------------------------------
//�Q�[�����ɑ��݂��镨�̑S�Ă�����[�ʒu][��]][�g�k][�����蔻��]�����N���X.
//---------------------------------------------------------------------------
//#pragma once

#ifndef OBJECT
#define OBJECT

#include"Global.h"
#include"DX9Mesh.h"

#include"Stage.h"

const float g_fPercentage = 0.01f;//�S�����ɕϊ��p.
const float g_fDistanceReference = 0.01f;//�����̋K��l.
const float g_fDirectionReference = 0.001f;//��]�l�̋K��l.
const float g_fGravity = 0.01f;//�d��.
const float g_fGroundSpece = 0.001f;
const float g_fRaySpace = 0.5f;

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

//��]�l����.
void ObjRollOverGuard(float* fRot);
//���݂̕���
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

	//�X�t�B�A�\����.
	struct SPHERE
	{
		D3DXVECTOR3* vCenter;//���S.
		float fRadius;	//���a.
	};

	TRANSFORM m_Trans;
	D3DXVECTOR3 m_vOldPos;
	D3DXVECTOR3 m_vCenterPos;//�I�u�W�F�N�g�̒��S�ʒu(���f���ɍ��킹�����S���W).

	float m_fRaySpece;
	float m_fFollPower;

	float m_fMoveSpeed;//�ŏI�I�ɉ��Z�����X�s�[�h.
	D3DXVECTOR3 m_vMoveDir;
	D3DXVECTOR3 m_vWallHit;

	bool m_bGround;
	bool m_NoFollObj;

	std::vector<SPHERE> m_v_Spheres;

	void Update(clsStage* pStage);

	virtual void UpdateProduct(clsStage* pStage);

	virtual void Action();

	virtual void Render(
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj,
		const D3DXVECTOR3& vLight,
		const D3DXVECTOR3& vEye)
	{

	}

	//�ʒu�֌W�֐�.
	void SetPosition(const D3DXVECTOR3& vPos){ 
		m_Trans.vPos = vPos; 
	}
	D3DXVECTOR3 GetPosition(){ return m_Trans.vPos; }
	const D3DXVECTOR3 GetCenterPos()const;//���S���W��n��.
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

	//�X�t�B�A�Փ˔���֐�.
	bool Collision(SPHERE pAttacker, SPHERE pTarget);//Sphere��Sphere�̓����蔻��.
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
		const clsDX9Mesh* pTarget,//�Ώۂ̕���.
		float* pfDistance, //(out)����.
		D3DXVECTOR3* pvIntersect);//(out)�������W.

private:
	//�����ʒu����غ�݂̒��_��������.
	HRESULT FindVecticesOnPoly(
		const LPD3DXMESH pTarget,
		const DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices);

	void FreeFoll();
};

#endif