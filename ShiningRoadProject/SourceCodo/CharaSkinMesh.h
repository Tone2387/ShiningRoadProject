//#pragma once

#ifndef CHARASKINMESH
#define CHARASKINMESH

#include"Charactor.h"
#include"SkinMesh.h"

class clsCharaSkinMesh : public clsCharactor, public clsSkinMesh
{
public:
	clsCharaSkinMesh()
	{
		ZeroMemory(this, sizeof(clsCharaSkinMesh));
	};
	virtual ~clsCharaSkinMesh(){};

	void Update()
	{
		DXSKIN_TRANSFORM tmpTrans;

		tmpTrans.fPitch = m_Trans.fPitch;
		tmpTrans.fYaw = m_Trans.fYaw;
		tmpTrans.fRoll = m_Trans.fRoll;
		tmpTrans.vPos = m_Trans.vPos;
		tmpTrans.vScale = m_Trans.vScale;

		ModelUpdate(tmpTrans);
	}

	void Render(
		D3DXMATRIX& mView, 
		D3DXMATRIX& mProj, 
		D3DXVECTOR3& vLight, 
		D3DXVECTOR3& vEye,
		const D3DXVECTOR4 &vColor,
		const bool alphaFlg )
	{
		Update();
		ModelRender(mView, mProj, vLight, vEye, vColor, alphaFlg );
	}

private:

};

#endif