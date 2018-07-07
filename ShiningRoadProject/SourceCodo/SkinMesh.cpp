#include"SkinMesh.h"

clsSkinMesh::clsSkinMesh() :
	m_pMesh( NULL ),
	m_AnimTime( 0.0 ),
	m_AnimNo( 0 ),
	m_pAnimCtrl( NULL ),
	m_dAnimSpeed( 0.1 ),
	m_bAnimReverce( false )
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
	const D3DXVECTOR4& vColor,
	const bool alphaFlg )
{
	if (m_pMesh == NULL || m_pAnimCtrl == NULL)return;

	AnimUpdate();
	m_pMesh->Render(mView, mProj, vLight, vEye, vColor, alphaFlg, m_pAnimCtrl);
}

void clsSkinMesh::AnimUpdate()
{
	if (!m_bAnimReverce)
	{
		m_AnimTime += abs(m_dAnimSpeed);

		if (m_pAnimCtrl != NULL){
			m_pAnimCtrl->AdvanceTime(m_dAnimSpeed, NULL);
		}
	}

	else
	{
		if (m_AnimTime - m_dAnimSpeed > 0)
		{
			m_AnimTime -= abs(m_dAnimSpeed);
		}

		SetAnimChange(m_AnimNo, m_AnimTime);

		// ｱﾆﾒ進行.
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

//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
void clsSkinMesh::AttachModel(clsD3DXSKINMESH* const pModel)
{
	if (pModel == NULL)
	{
		return;
	}

	//ﾓﾃﾞﾙ設定.
	m_pMesh = pModel;
	//ｱﾆﾒｰｼｮﾝ速度.
	m_dAnimSpeed = m_pMesh->GetAnimSpeed();

	//ｱﾆﾒｰｼｮﾝｺﾝﾄﾛｰﾗのｸﾛｰﾝを作成.
	LPD3DXANIMATIONCONTROLLER pAC = m_pMesh->GetAnimController();

	pAC->CloneAnimationController(
		pAC->GetMaxNumAnimationOutputs(),
		pAC->GetMaxNumAnimationSets(),
		pAC->GetMaxNumTracks(),
		pAC->GetMaxNumEvents(),
		&m_pAnimCtrl);//(out)ｱﾆﾒｰｼｮﾝｺﾝﾄﾛｰﾗ.
}

//ｱﾆﾒｰｼｮﾝ最大数を取得.
int clsSkinMesh::GetAnimSetMax()
{
	if (m_pAnimCtrl != NULL)
	{
		return (int)m_pAnimCtrl->GetMaxNumAnimationSets();
	}
	return -1;
}

//ｱﾆﾒｰｼｮﾝ切り替え関数.//変更できるならtrue, 変更できないならfalseが返る.
bool clsSkinMesh::SetAnimChange(int index, double dStartPos)
{
	//ｱﾆﾒｰｼｮﾝの範囲内かﾁｪｯｸ.
	if (index < 0 || index >= GetAnimSetMax()){
		//範囲外なら初期アニメーションを再生し,怒る.
		m_pMesh->ChangeAnimSet_StartPos(0, dStartPos, m_pAnimCtrl);
//		assert( !"範囲外のアニメーションが指定されました" );
		return false;
	}
	m_pMesh->ChangeAnimSet_StartPos(index, dStartPos, m_pAnimCtrl);
	return true;
}

void clsSkinMesh::ModelUpdate(TRANSFORM Transform)
{
	m_pMesh->m_Trans = Transform;
}

D3DXVECTOR3 clsSkinMesh::GetBonePos( const char* sBoneName, const bool isLocalPos )
{
	D3DXVECTOR3 vBonePos;

	//ボーンの座標を取る.
	if( !m_pMesh->GetPosFromBone( sBoneName, &vBonePos, isLocalPos ) ){
		//ボーンが見つからなければ.
//		ERR_MSG( sBoneName, "その名前のボーンは存在しません");
	}

	return vBonePos;
}

//指定したボーン位置からvDiviation分移動した位置を取得する.
D3DXVECTOR3 clsSkinMesh::GetBoneDiviaPos(char* sBoneName, D3DXVECTOR3 vDiviation)
{
	D3DXVECTOR3 vBonePos;

	m_pMesh->GetDeviaPosFromBone(sBoneName, &vBonePos, vDiviation);
	return vBonePos;
}

void clsSkinMesh::SetAnimSpeed(double dSpeed)
{
	m_dAnimSpeed = dSpeed;
	m_pMesh->SetAnimSpeed(m_dAnimSpeed);
}

double clsSkinMesh::GetAnimSpeed()
{
	return m_dAnimSpeed;
}

double clsSkinMesh::GetAnimEndTime(int AnimIndex)
{
	return m_pMesh->GetAnimPeriod(AnimIndex); 
}

void clsSkinMesh::SetAnimTime(double dTime)
{
	m_AnimTime = dTime;
	m_pMesh->SetAnimTime(dTime);
}

double clsSkinMesh::GetAnimTime()
{
	return m_AnimTime; 
}

bool clsSkinMesh::IsAnimTimeEnd(int AnimIndex)
{
	if (!m_bAnimReverce)
	{
		if (GetAnimEndTime(AnimIndex) - m_dAnimSpeed < m_AnimTime)
		{
			m_AnimTime = GetAnimEndTime(AnimIndex);
			return true;
		}
	}

	else
	{
		if (m_AnimTime < m_dAnimSpeed)
		{
			m_AnimTime = 0.0f;
			return true;
		}
	}

	return false;
}

bool clsSkinMesh::IsAnimTimeAfter(int AnimIndex, double DesignationTime)
{
	if (!m_bAnimReverce)
	{
		if (DesignationTime - m_dAnimSpeed < m_AnimTime)
		{
			m_AnimTime = DesignationTime;
			return true;
		}
	}

	else
	{
		if (m_AnimTime < (DesignationTime + m_dAnimSpeed))
		{
			m_AnimTime = DesignationTime;
			return true;
		}
	}


	return false;
}

bool clsSkinMesh::IsAnimTimePoint(int AnimIndex, double DesignationTime)
{
	if (DesignationTime - m_dAnimSpeed < m_AnimTime &&
		DesignationTime + m_dAnimSpeed > m_AnimTime)
	{
		return true;
	}

	return false;
}