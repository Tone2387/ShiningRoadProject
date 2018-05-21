#pragma once

#ifndef SKINMESH
#define SKINMESH

#include "CD3DXSKINMESH.h"

class clsSkinMesh
{
public:
	clsD3DXSKINMESH* m_pMesh;
	//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
	void AttachModel(clsD3DXSKINMESH* pModel);
	//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け解除.
	void DetatchModel();

	//ﾚﾝﾀﾞﾘﾝｸﾞ関数.
	void ModelRender( 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4 &vColor,
		const bool alphaFlg );

	//ｱﾆﾒｰｼｮﾝ最大数を取得.
	int GetAnimSetMax();
	//ｱﾆﾒｰｼｮﾝ切り替え関数.
	void ChangeAnimSet(int index, double dStartPos = 0.0f);

	double m_AnimTime;
	//ﾓｰｼｮﾝ関係.
	int m_AnimNo;//ﾓｰｼｮﾝ番号.

	//座標や回転の更新.
	//void Update();

	void ModelUpdate(DXSKIN_TRANSFORM Transform)
	{
		m_pMesh->m_Trans.fPitch = Transform.fPitch;
		m_pMesh->m_Trans.fYaw = Transform.fYaw;
		m_pMesh->m_Trans.fRoll = Transform.fRoll;
		m_pMesh->m_Trans.vPos = Transform.vPos;
		m_pMesh->m_Trans.vScale = Transform.vScale;
	}

	D3DXVECTOR3 GetBonePos(char* sBoneName)
	{
		D3DXVECTOR3 vBonePos;

		m_pMesh->GetPosFromBone(sBoneName, &vBonePos);

		return vBonePos;
	}

	D3DXVECTOR3 GetBoneDiviaPos(char* sBoneName,D3DXVECTOR3 vDiviation)
	{
		D3DXVECTOR3 vBonePos;

		m_pMesh->GetDeviaPosFromBone(sBoneName, &vBonePos, vDiviation);

		return vBonePos;
	}

	LPD3DXANIMATIONCONTROLLER m_pAnimCtrl;
	double m_dAnimSpeed;
	bool m_bAnimReverce;

	//D3DXMATRIX GetMatWorld(){ return m_pMesh->m_mWorld; }

	void SetAnimSpeed(double dSpeed)
	{
		m_dAnimSpeed = dSpeed;
		m_pMesh->SetAnimSpeed(m_dAnimSpeed);
	}
	double GetAnimSpeed(){ return m_dAnimSpeed; }
	double GetAnimEndTime(int AnimIndex){ return m_pMesh->GetAnimPeriod(AnimIndex); }
	void SetAnimTime(double dTime)
	{
		m_AnimTime = dTime;
		m_pMesh->SetAnimTime(dTime);
	}

	double GetAnimTime(){ return m_AnimTime; }

	bool IsAnimTimeAfter(int AnimIndex, double DesignationTime = 1.00f)
	{
		if (!m_bAnimReverce)
		{
			if (GetAnimEndTime(AnimIndex) / DesignationTime - m_dAnimSpeed < m_AnimTime)
			{
				return true;
			}
		}

		else
		{
			if (m_AnimTime < (DesignationTime * m_dAnimSpeed))
			{
				return true;
			}
		}


		return false;
	}

	bool IsAnimTimePoint(int AnimIndex, double DesignationTime = 1.00f)
	{
		if (m_pMesh->GetAnimPeriod(AnimIndex) / DesignationTime - m_dAnimSpeed - 0.036f < m_AnimTime &&
			m_pMesh->GetAnimPeriod(AnimIndex) / DesignationTime + m_dAnimSpeed + 0.036f > m_AnimTime)
		{
			return true;
		}

		return false;
	}


	clsSkinMesh();
	virtual ~clsSkinMesh();

private:

};


#endif