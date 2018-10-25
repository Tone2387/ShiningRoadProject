#pragma once
#include"Object.h"
#include"SkinMesh.h"


class clsObjSkinMesh : public clsObject, public clsSkinMesh 
{
public:

	void Update()
	{
		if (m_pMesh == NULL)return;

		m_pMesh->m_Trans.vPos = m_Trans.vPos;
		m_pMesh->m_Trans.fYaw = m_Trans.fYaw;
		m_pMesh->m_Trans.fPitch = m_Trans.fPitch;
		m_pMesh->m_Trans.fRoll = m_Trans.fRoll;
		m_pMesh->m_Trans.vScale = m_Trans.vScale;
	}
	
	clsObjSkinMesh();
	virtual ~clsObjSkinMesh(){};

private:

};

clsObjSkinMesh::clsObjSkinMesh()
{
	ZeroMemory(this, sizeof(clsObjSkinMesh));
}
