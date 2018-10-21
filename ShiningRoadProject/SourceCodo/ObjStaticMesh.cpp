#include"ObjStaticMesh.h"

void clsObjStaticMesh::AttachModel(clsDX9Mesh* const pMesh)
{
	//ƒ‚ƒfƒ‹‚ÌŽæ“¾.
	m_pModel = pMesh;
}

void clsObjStaticMesh::DttachModel()
{
	if (m_pModel)
	{
		m_pModel = nullptr;
	}
}

clsDX9Mesh* clsObjStaticMesh::GetStaticMesh() const
{
	return m_pModel;
}

void clsObjStaticMesh::ModelTransUpdate()
{
	if (!m_pModel)return;

	m_pModel->m_Trans.vPos = m_Trans.vPos;
	m_pModel->m_Trans.fYaw = m_Trans.fYaw;
	m_pModel->m_Trans.fPitch = m_Trans.fPitch;
	m_pModel->m_Trans.fRoll = m_Trans.fRoll;
	m_pModel->m_Trans.vScale = m_Trans.vScale;
}

void clsObjStaticMesh::Render(
	D3DXMATRIX& const mView, 
	D3DXMATRIX& const mProj, 
	D3DXVECTOR3& const vLight, 
	D3DXVECTOR3& const vEye)
{
	if (!m_pModel)return;
	ModelTransUpdate();

	m_pModel->Render(mView, mProj, vLight, vEye);
}

clsObjStaticMesh::clsObjStaticMesh()
	: m_pModel(nullptr)
{
}

clsObjStaticMesh::~clsObjStaticMesh()
{
	DttachModel();
}