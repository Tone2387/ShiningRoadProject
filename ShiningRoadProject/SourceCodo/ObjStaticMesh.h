#pragma once

#include"Object.h"
#include"StaticMesh.h"

class clsObjStaticMesh : public clsObject, public clsStaticMesh
{
public:
	void Update()
	{
		if (m_pMesh == NULL)return;

		m_pMesh->m_Trans.vPos = m_Trans.vPos;
		m_pMesh->m_Trans.fYaw = m_Trans.fYaw;
		m_pMesh->m_Trans.fPitch = m_Trans.fPitch;
		m_pMesh->m_Trans.fRoll = m_Trans.fRoll;
		m_pMesh->m_Trans.fScale = m_Trans.vScale.x;
	}

	clsObjStaticMesh();
	~clsObjStaticMesh();

private:

};

clsObjStaticMesh::clsObjStaticMesh()
{
}

clsObjStaticMesh::~clsObjStaticMesh()
{
}