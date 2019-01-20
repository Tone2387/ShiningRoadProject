#define _CRT_SECURE_NO_WARNINGS
#include"Shot.h"

const int g_iColNum = 1;

clsShot::clsShot(clsPOINTER_GROUP* pPtrGroup)
	: m_fRangeMax(0.0f)
{
	m_wpEffect = pPtrGroup->GetEffects();
	m_wpSound = pPtrGroup->GetSound();

	m_bNoFollObj = true;
}

clsShot::~clsShot()
{
}

HRESULT clsShot::Init(BulletState BState)
{
	//strWeaponFileName��������󂯎��.
	//���̑傫��.
	//����.
	//�G�t�F�N�g.
	m_ShotState = BState;
	m_fMoveSpeed = m_ShotState.fSpeed;

	ActStart();

	//�����蔻��̑傫�������߂�.

	//m_v_Spheres.resize(g_iColNum);

	clsObject::SPHERE Tmp;
	Tmp.vCenter = &m_Trans.vPos;
	Tmp.fRadius = m_ShotState.fScale;

	m_v_Spheres.push_back(Tmp);
	m_v_Spheres.shrink_to_fit();
	
	//�m�F�p�̨̽��������ݸނ���.
	//�����蔻��Ƃ��ẮA�����ȍ~�͕s�v.

#ifdef _DEBUG
	//m_pBodySphere = clsResource::GetInstance()->GetStaticModels(clsResource::enStaticModel_Shpere);
#endif

	return S_OK;
}

bool clsShot::Hit(std::vector<clsObject::SPHERE> v_TargetSphere)
{
	if (m_bShotExistFlg)
	{
		for (unsigned int i = 0; i < g_iColNum; i++)
		{
			for (unsigned int j = 0; j < v_TargetSphere.size(); j++)
			{
				float fLenght = D3DXVec3Length(&(*m_v_Spheres[i].vCenter - *v_TargetSphere[j].vCenter));
				
				//�V���b�g�̑��x���������ē�����Ȃ���������.
				D3DXVECTOR3 vTmpPos = *m_v_Spheres[i].vCenter;
				if ((fLenght + m_v_Spheres[i].fRadius) < m_ShotState.fSpeed)
				{
					*m_v_Spheres[i].vCenter += m_vMoveDir * fLenght;
				}

				if (Collision(m_v_Spheres[i], v_TargetSphere[j]))
				{
					Explosion();
					return true;
				}

				*m_v_Spheres[i].vCenter = vTmpPos;
			}
		}
	}

	return false;
}

bool clsShot::Form(D3DXVECTOR3 vShotPos, D3DXVECTOR3 vDir)
{
	if (m_bExistFlg)
	{
		return false;
	}	

	m_vMoveDir = vDir;

	m_fFollPower = m_vMoveDir.y * m_fMoveSpeed;

	m_Trans.vPos = m_vStartPos = vShotPos;

	m_bShotExistFlg = true;
	m_bExistFlg = true;

	//Excel�̍s�ԍ�	���W.
	m_ShotEfcHandles[enEfcShot] = m_wpEffect->Play(m_ShotState.iShotEfcNum, m_Trans.vPos);
	//m_ShotEfcHandles[enEfcLine] = m_wpEffect->Play(m_ShotState.iLineEfcNum, m_Trans.vPos);
	m_ShotEfcHandles[enEfcMuzzle] = m_wpEffect->Play(m_ShotState.iMuzzeleEfcNum, m_Trans.vPos);
	
	//�x�N�g�������]�l�����߂�.
	D3DXVECTOR3 vDirTmp = m_vMoveDir;
	D3DXVECTOR3 vRot = { 0.0f, 0.0f, 0.0f };

	vRot.x = atan2f(-vDirTmp.y, -vDirTmp.z);//.
	vRot.x = (vDirTmp.y * cosf(vRot.x)) - (vDirTmp.z * sinf(vRot.x));
	vRot.x = asin(vRot.x);
	vRot.y = atan2f(vDirTmp.x, vDirTmp.z);//( ���́A�}�C�i�X���������Ă�����AX,Z������ւ���Ă���̂��Ƃ����΁A0�x�Ń��f�������������������� ).

	ObjRollOverGuard(&vRot.x);
	ObjRollOverGuard(&vRot.y);
	//m_wpEffect->SetRotation(m_ShotEfcHandles[enEfcShot],vRot);
	//m_wpEffect->SetRotation(m_ShotEfcHandles[enEfcMuzzle],vRot);

	m_vMoveDir.y = 0.0f;

	return true;
}

void clsShot::UpdateProduct(clsStage* pStage)
{
	if (!m_bShotExistFlg)return;

	if (WallJudge(pStage))
	{
		Explosion();
	}
}

void clsShot::Action(clsStage* const pStage)
{
	if (!m_bExistFlg)
	{
		return;
	}

	if (!m_bShotExistFlg)
	{
		if (//!m_wpEffect->isPlay(m_ShotEfcHandles[enEfcLine]) && 
			!m_wpEffect->isPlay(m_ShotEfcHandles[enEfcHit]))
		{
			m_bExistFlg = false;
		}
		return;
	}

	if (D3DXVec3Length(&(m_Trans.vPos - m_vStartPos)) > m_ShotState.fRangeMax)
	{
		m_wpEffect->Stop(m_ShotEfcHandles[enEfcShot]);
		//m_wpEffect->Stop(m_ShotEfcHandles[enEfcLine]);
		m_bShotExistFlg = false;
		return;
	}

	m_Trans.vPos += m_vMoveDir * m_fMoveSpeed;

	//���W.
	m_wpEffect->SetPosition(m_ShotEfcHandles[enEfcShot], m_Trans.vPos);
	//m_wpEffect->SetPosition(m_ShotEfcHandles[enEfcLine], m_Trans.vPos);
}

void clsShot::ReStart()
{
	m_bShotExistFlg = false;//�e�̑��݊m�F(false�Ȃ�e�����݂��Ȃ����).
	m_bExistFlg = false;//�e,����,�O�Ղ̑��݊m�F(false�Ȃ炻���S�Ă����݂��Ȃ����)
}

void clsShot::Explosion()
{
	m_ShotEfcHandles[enEfcHit] = m_wpEffect->Play(m_ShotState.iHitEfcNum, m_Trans.vPos);
	m_wpEffect->Stop(m_ShotEfcHandles[enEfcShot]);
	//m_wpEffect->Stop(m_ShotEfcHandles[enEfcLine]);
	m_bShotExistFlg = false;
	//����SE.
	m_wpSound->PlaySE(0);
}