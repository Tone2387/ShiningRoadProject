//#pragma once

#ifndef SKINMESH
#define SKINMESH

#include "CD3DXSKINMESH.h"

class clsSkinMesh
{
public:
	double m_AnimTime;
	//ﾓｰｼｮﾝ関係.
	int m_AnimNo;//ﾓｰｼｮﾝ番号.
	double m_dAnimSpeed;
	bool m_bAnimReverce;

	clsD3DXSKINMESH* m_pMesh;
	LPD3DXANIMATIONCONTROLLER m_pAnimCtrl;
	
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

	void AnimUpdate();
	//ｱﾆﾒｰｼｮﾝ最大数を取得.
	int GetAnimSetMax();
	//ｱﾆﾒｰｼｮﾝ切り替え関数.
	void SetAnimChange(int index, double dStartPos = 0.0f);

	//座標や回転の更新.
	void ModelUpdate(TRANSFORM Transform);

	D3DXVECTOR3 GetBonePos(char* sBoneName);

	//指定したボーン位置からvDiviation分移動した位置を取得する.
	D3DXVECTOR3 GetBoneDiviaPos(char* sBoneName, D3DXVECTOR3 vDiviation);

	void SetAnimSpeed(double dSpeed);
	double GetAnimSpeed();

	double GetAnimTime();
	void SetAnimTime(double dTime);
	double GetAnimEndTime(int AnimIndex);
	
	bool IsAnimTimeEnd(int AnimIndex);
	bool IsAnimTimeAfter(int AnimIndex, double DesignationTime);
	bool IsAnimTimePoint(int AnimIndex, double DesignationTime);

	clsSkinMesh();
	virtual ~clsSkinMesh();

private:

};


#endif