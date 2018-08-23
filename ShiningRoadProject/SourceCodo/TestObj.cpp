#include"TestObject.h"

clsTestObj::clsTestObj()
{
	//ZeroMemory(this, sizeof(clsTestObj));
}

clsTestObj::~clsTestObj()
{

}

void clsTestObj::Init(clsPOINTER_GROUP* const pPtrGroup )
{
	RoboInit(pPtrGroup, pPtrGroup->GetRoboStatus());

	clsObject::SPHERE Tmp;
	Tmp.vCenter = &m_vCenterPos;
	Tmp.fRadius = 0.1f;

	m_v_Spheres.push_back(Tmp);
	m_v_Spheres.shrink_to_fit();

	//m_pMesh->SetAnimSpeed(0.01);
}

void clsTestObj::ActionProduct()
{
	
}

void clsTestObj::InhUpdate()
{
	//m_pMesh->SetAnimSpeed(0.01);
}