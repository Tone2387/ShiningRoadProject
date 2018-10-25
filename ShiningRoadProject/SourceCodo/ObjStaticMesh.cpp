#include"ObjStaticMesh.h"

void clsObjStaticMesh::AttachModel(clsDX9Mesh* const pMesh)
{
	//ƒ‚ƒfƒ‹‚ÌŽæ“¾.
	m_pModel = pMesh;
}

void clsObjStaticMesh::DetatchModel()
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
	const D3DXMATRIX&  mView,
	const D3DXMATRIX& mProj,
	const D3DXVECTOR3& vLight,
	const D3DXVECTOR3& vEye)
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
	DetatchModel();
}