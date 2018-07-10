#pragma once

//ステージのオブジェクト等に使う.

#include"Object.h"

class clsObjStaticMesh : public clsObject
{
public:
	
	clsDX9Mesh* GetStaticMesh() const;

	void ModelTransUpdate()
	{
		if (m_pMesh == NULL)return;

		m_pMesh->m_Trans.vPos = m_Trans.vPos;
		m_pMesh->m_Trans.fYaw = m_Trans.fYaw;
		m_pMesh->m_Trans.fPitch = m_Trans.fPitch;
		m_pMesh->m_Trans.fRoll = m_Trans.fRoll;
		m_pMesh->m_Trans.vScale = m_Trans.vScale;
	}

	virtual void Render(D3DXMATRIX& const mView, D3DXMATRIX& const mProj, D3DXVECTOR3& const vLight, D3DXVECTOR3& const vEye)
	{
		ModelTransUpdate();

		m_pMesh->Render(mView, mProj, vLight, vEye);
	}

	clsObjStaticMesh();
	~clsObjStaticMesh();

private:
	clsDX9Mesh* m_pMesh;
};

clsObjStaticMesh::clsObjStaticMesh()
{
}

clsObjStaticMesh::~clsObjStaticMesh()
{
}