//#pragma once

#ifndef SKINMESH
#define SKINMESH

#include "CD3DXSKINMESH.h"

static const double g_dAnimSpeedReference = static_cast<double>(1 / g_fFPS);

class clsSkinMesh
{
public:
	clsSkinMesh();
	virtual ~clsSkinMesh();

	//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
	void AttachModel(clsD3DXSKINMESH* const pModel);
	//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け解除.
	void DetatchModel();

	//座標や回転の更新.
	void ModelUpdate(const TRANSFORM Transform);

	//ﾚﾝﾀﾞﾘﾝｸﾞ関数.
	void ModelRender(
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj,
		const D3DXVECTOR3& vLight,
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4& vColorBase = { 1.0f, 1.0f, 1.0f, 1.0f },
		const D3DXVECTOR4& vColorArmor = { 1.0f, 1.0f, 1.0f, 1.0f },
		const bool isAlpha = false);

	//ｱﾆﾒｰｼｮﾝ最大数を取得.
	const int GetAnimSetMax();
	//指定したボーン位置を渡す.
	virtual D3DXVECTOR3 GetBonePos(const char* sBoneName, const bool isLocalPos = false);
	//指定したボーン位置からvDiviation分移動した位置を渡す.
	const D3DXVECTOR3 GetBoneDiviaPos(char* sBoneName, const  D3DXVECTOR3 vDiviation);
	//ｱﾆﾒｰｼｮﾝ切り替え関数.//変更できるならtrue, 変更できないならfalseが返る.
	const bool SetAnimChange(const int iAnimNo, const double dStartPos = 0.0f);
	//現在のアニメーション番号を渡す.
	const int GetAnimNo();
	void SetAnimSpeed(const double dSpeed);
	const double GetAnimSpeed();
	void SetAnimTime(const double dTime);
	const double GetAnimTime();
	const double GetAnimEndTime(const int iAnimNo);
	const bool IsAnimTimeEnd();
	const bool IsAnimTimeAfter(const double dDesignationTime);
	const bool IsAnimTimePoint(const double dDesignationTime);
	const bool IsExistsBone(const char* sBoneName);

	void AnimPause()
	{
		m_bAnimPause = true;
	}

	void AnimPlay()
	{
		m_bAnimPause = false;
	}

	//アニメーションを逆再生させる.
	void AnimReverce(const bool bAnimTimeInit = false);
	//アニメーションを通常再生させる.
	void AnimNormal(const bool bAnimTimeInit = false);
	//アニメーション再生の状態を渡す true:逆再生.
	const bool IsAnimReverce();

	clsD3DXSKINMESH* m_pMesh;

private:
	double m_dAnimTime;
	//ﾓｰｼｮﾝ関係.
	int m_iAnimNo;//ﾓｰｼｮﾝ番号.
	double m_dAnimSpeed;
	bool m_bAnimReverce;
	bool m_bAnimPause;
	
	LPD3DXANIMATIONCONTROLLER m_pAnimCtrl;

	//アニメーションの更新.
	void AnimUpdate();
};


#endif