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
		ModelUpdate(m_Trans);
	}

	void Render(
		D3DXMATRIX& mView, 
		D3DXMATRIX& mProj, 
		D3DXVECTOR3& vLight, 
		D3DXVECTOR3& vEye,
		const D3DXVECTOR4 &vColorBase,
		const D3DXVECTOR4 &vColorAlpha,
		const bool isAlpha )
	{
		Update();
		ModelRender(mView, mProj, vLight, vEye, vColorBase, vColorAlpha, isAlpha );
	}

private:

};

#endif