#include"SkinMesh.h"

clsSkinMesh::clsSkinMesh() :
	m_pMesh( NULL ),
	m_dAnimTime( 0.0 ),
	m_iAnimNo( 0 ),
	m_pAnimCtrl( NULL ),
	m_dAnimSpeed( 0.1 ),
	m_bAnimReverce( false ),
	m_bAnimPause(false)
{

}
clsSkinMesh::~clsSkinMesh()
{
	DetatchModel();
}

void clsSkinMesh::ModelRender(
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3& vLight, 
	const D3DXVECTOR3& vEye,
	const D3DXVECTOR4& vColorBase,
	const D3DXVECTOR4& vColorArmor,
	const bool isAlpha )
{
	if (m_pMesh == NULL || m_pAnimCtrl == NULL)return;

	AnimUpdate();
	m_pMesh->Render(mView, mProj, vLight, vEye, vColorBase, vColorArmor, isAlpha, m_pAnimCtrl);
}

void clsSkinMesh::AnimUpdate()
{
	if (m_bAnimPause)
	{
		if (m_pAnimCtrl != NULL){
			m_pAnimCtrl->AdvanceTime(0, NULL);
		}

		return;
	}

	if (!m_bAnimReverce)
	{
		m_dAnimTime += abs(m_dAnimSpeed);

		if (m_pAnimCtrl != NULL){
			m_pAnimCtrl->AdvanceTime(m_dAnimSpeed, NULL);
		}
	}

	else
	{
		if (m_dAnimTime - m_dAnimSpeed > 0)
		{
			m_dAnimTime -= abs(m_dAnimSpeed);
		}

		SetAnimChange(m_iAnimNo, m_dAnimTime);

		// ��Ґi�s.
		if (m_pAnimCtrl != NULL){
			m_pAnimCtrl->AdvanceTime(0, NULL);
		}
	}
}

void clsSkinMesh::DetatchModel()
{
	if (m_pMesh != NULL)
	{
		m_pMesh = NULL;

		if (m_pAnimCtrl != NULL)
		{
			m_pAnimCtrl->Release();
			m_pAnimCtrl = NULL;
		}
	}
}

//�����ް��̊֘A�t��.
void clsSkinMesh::AttachModel(clsD3DXSKINMESH* const pModel)
{
	if (pModel == NULL)
	{
		return;
	}

	//���ِݒ�.
	m_pMesh = pModel;
	//��Ұ��ݑ��x.
	m_dAnimSpeed = m_pMesh->GetAnimSpeed();

	//��Ұ��ݺ��۰̸ׂ۰݂��쐬.
	LPD3DXANIMATIONCONTROLLER pAC = m_pMesh->GetAnimController();

	pAC->CloneAnimationController(
		pAC->GetMaxNumAnimationOutputs(),
		pAC->GetMaxNumAnimationSets(),
		pAC->GetMaxNumTracks(),
		pAC->GetMaxNumEvents(),
		&m_pAnimCtrl);//(out)��Ұ��ݺ��۰�.
}

//��Ұ��ݍő吔���擾.
const int clsSkinMesh::GetAnimSetMax()
{
	if (m_pAnimCtrl != NULL)
	{
		return (int)m_pAnimCtrl->GetMaxNumAnimationSets();
	}
	return -1;
}

//��Ұ��ݐ؂�ւ��֐�.//�ύX�ł���Ȃ�true, �ύX�ł��Ȃ��Ȃ�false���Ԃ�.
const bool clsSkinMesh::SetAnimChange(const int iAnimNo, const double dStartPos)
{
	//��Ұ��݂͈͓̔�������.
	if (iAnimNo < 0 || iAnimNo >= GetAnimSetMax()){
		//�͈͊O�Ȃ珉���A�j���[�V�������Đ���,�{��.
		m_pMesh->ChangeAnimSet_StartPos(0, dStartPos, m_pAnimCtrl);
//		assert( !"�͈͊O�̃A�j���[�V�������w�肳��܂���" );
		return false;
	}
	m_pMesh->ChangeAnimSet_StartPos(iAnimNo, dStartPos, m_pAnimCtrl);
	m_iAnimNo = iAnimNo;
	m_dAnimTime = dStartPos;
	return true;
}

void clsSkinMesh::ModelUpdate(const TRANSFORM Transform)
{
	m_pMesh->m_Trans = Transform;
}

D3DXVECTOR3 clsSkinMesh::GetBonePos(const char* sBoneName, const bool isLocalPos)
{
	D3DXVECTOR3 vBonePos;

//	ModelUpdate( m_Trans );

	//�{�[���̍��W�����.
	if( !m_pMesh->GetPosFromBone( sBoneName, &vBonePos, isLocalPos ) ){
		//�{�[����������Ȃ����.
//		ERR_MSG( sBoneName, "���̖��O�̃{�[���͑��݂��܂���" );
	}

	return vBonePos;
}

//�w�肵���{�[���ʒu����vDiviation���ړ������ʒu���擾����.
const D3DXVECTOR3 clsSkinMesh::GetBoneDiviaPos(char* sBoneName, const  D3DXVECTOR3 vDiviation)
{
	D3DXVECTOR3 vBonePos;

	m_pMesh->GetDeviaPosFromBone(sBoneName, &vBonePos, vDiviation);
	return vBonePos;
}

void clsSkinMesh::SetAnimSpeed(const double& dSpeed)
{
	m_dAnimSpeed = dSpeed;
	m_pMesh->SetAnimSpeed(m_dAnimSpeed);
}

const double clsSkinMesh::GetAnimSpeed()
{
	return m_dAnimSpeed;
}

const double clsSkinMesh::GetAnimEndTime(const int AnimIndex)
{
	return m_pMesh->GetAnimPeriod(AnimIndex); 
}

void clsSkinMesh::SetAnimTime(const double& dTime)
{
	m_dAnimTime = dTime;
	m_pMesh->SetAnimTime(dTime);
}

const double clsSkinMesh::GetAnimTime()
{
	return m_dAnimTime; 
}

const bool clsSkinMesh::IsAnimTimeEnd()
{
	if (!m_bAnimReverce)
	{
		if (GetAnimEndTime(m_iAnimNo) - m_dAnimSpeed < m_dAnimTime)
		{
			m_dAnimTime = GetAnimEndTime(m_iAnimNo);
			return true;
		}
	}

	else
	{
		if (m_dAnimTime < m_dAnimSpeed)
		{
			m_dAnimTime = 0.0f;
			return true;
		}
	}

	return false;
}

const bool clsSkinMesh::IsAnimTimeAfter(const double& dDesignationTime)
{
	if (!m_bAnimReverce)
	{
		if (dDesignationTime - m_dAnimSpeed < m_dAnimTime)
		{
			m_dAnimTime = dDesignationTime;
			return true;
		}
	}

	else
	{
		if (m_dAnimTime < (dDesignationTime + m_dAnimSpeed))
		{
			m_dAnimTime = dDesignationTime;
			return true;
		}
	}


	return false;
}

const bool clsSkinMesh::IsAnimTimePoint(const double& dDesignationTime)
{
	if (dDesignationTime - m_dAnimSpeed < m_dAnimTime &&
		dDesignationTime + m_dAnimSpeed > m_dAnimTime)
	{
		return true;
	}

	return false;
}

const int clsSkinMesh::GetAnimNo()
{
	return m_iAnimNo;
}

const bool clsSkinMesh::IsExistsBone(const char* sBoneName)
{
	return m_pMesh->ExistsBone(sBoneName);
}

void clsSkinMesh::AnimReverce(const bool bAnimTimeInit)
{
	if (bAnimTimeInit)
	{
		m_dAnimTime = GetAnimEndTime(m_iAnimNo);
	}
	m_bAnimReverce = true;
}

void clsSkinMesh::AnimNormal(const bool bAnimTimeInit)
{
	if (bAnimTimeInit)
	{
		m_dAnimTime = 0.0;
	}
	m_bAnimReverce = false;
}

const bool clsSkinMesh::IsAnimReverce()
{
	return m_bAnimReverce;
}