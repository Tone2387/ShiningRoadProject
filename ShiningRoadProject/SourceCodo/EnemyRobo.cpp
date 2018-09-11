#include"EnemyRobo.h"

void clsEnemyRobo::Init(
	LPSTR strEnemyFolderName,
	clsRobo* pRobo)
{
	m_pBody = pRobo;
	m_pChara = m_pBody;

	m_UpdateState.iHorDirResult = 0;
	m_UpdateState.vHorMovePos = { 0.0f, 0.0f, 0.0f };

	ShotState SSTmp;
	SSTmp.iShotDisMax = 50.0f;
	SSTmp.iShotDisMin = 1.0f;

	MoveState MSTmp;
	MSTmp.iHorDisRandMax = 500;
	MSTmp.iHorDistance = 100;
	MSTmp.iMoveDir = 0;
	MSTmp.iMoveDirRandMax = 0;
	MSTmp.iVerDistance = 5000;
	MSTmp.iVerDistRandMax = 500;
	MSTmp.iVerMoveENLimitParcent = 50;

	m_LShotData.iCategory = 1;
	m_LShotData.v_ShotState.push_back(SSTmp);

	m_RShotData.iCategory = 1;
	m_RShotData.v_ShotState.push_back(SSTmp);

	m_MoveData.iCategory = 1;
	m_MoveData.v_MoveState.push_back(MSTmp);
}

bool clsEnemyRobo::IsBoostOn()
{
	if (m_pTarget)
	{
		float fVerDis = m_pBody->GetPosition().y - m_pTarget->GetPosition().y;

		const int iHulf = 2;

		float fVerDestDis = m_UpdateState.fVerDis + (m_UpdateState.fVerDis / iHulf);

		if (fVerDis > 0.0f)//�G����ɂ���.
		{
			return true;
		}

		if (m_pBody->m_iEnelgy > (m_pBody->m_iEnelgyMax / iHulf))//EN�c�ʂ��\���ɂ���.
		{
			return true;
		}
	}

	return false;
}

bool clsEnemyRobo::IsBoostOff()
{
	if (m_pTarget)
	{
		float fVerDis = m_pBody->GetPosition().y - m_pTarget->GetPosition().y;

		const int iHulf = 2;

		float fVerDestDis = m_UpdateState.fVerDis + (m_UpdateState.fVerDis / iHulf);

		if (fVerDis < 0.0f)//�G�����ɂ���.
		{
			if (abs(fVerDis) > abs(fVerDestDis))//�G�Ƃ̍����̍����傫������.
			{
				return true;
			}
		}

		if (m_pBody->m_iEnelgy < (m_pBody->m_iEnelgyMax / iHulf))//EN�c�ʂ����Ȃ�.
		{
			return true;
		}
	}

	return false;
}

bool clsEnemyRobo::IsQuickTurn(float& fPush, float& fAngle)//�^�[�Q�b�g�ʒu�̕��������ʂ�����ȏ㗣��Ă��ꍇ�A�N�C�b�N�^�[�����g�p.
{
	/*D3DXVECTOR3 vForward = GetVec3Dir(m_pBody->GetRotation().y,g_vDirForward);

	D3DXVECTOR3 vDirTarget = m_pTarget->GetPosition() - m_pBody->GetPosition();

	D3DXVec3Normalize(&vDirTarget, &vDirTarget);

	float fDir = D3DXVec3Dot(&vForward, &vDirTarget);*/

	fPush = 0.0f;
	fAngle = 0.0f;

	if (m_pTarget)
	{
		const float fVecX = m_pTarget->GetPosition().x - m_pBody->GetPosition().x;
		const float fVecZ = m_pTarget->GetPosition().z - m_pBody->GetPosition().z;

		float fRot = atan2f(fVecX, fVecZ) - m_pBody->GetRotation().y;

		int iDirHulf = 180;
		int iDirHFull = 360;

		if (fRot > static_cast<float>D3DXToRadian(iDirHulf))
		{
			fRot -= static_cast<float>D3DXToRadian(iDirHFull);
		}
		else if (fRot < -static_cast<float>D3DXToRadian(iDirHulf))
		{
			fRot += static_cast<float>D3DXToRadian(iDirHFull);
		}

		ObjRollOverGuard(&fRot);

		if (abs(fRot) > D3DXToRadian(150))
		{
			const float fPushFull = 1.0f;

			if (fRot > 0.0f)
			{
				fPush = fPushFull;
			}

			else
			{
				fPush = -fPushFull;
			}

			fAngle = fRot;

			return true;
		}
	}

	return false;
}

bool clsEnemyRobo::IsQuickBoostApproach(float& fPush, float& fAngle)//�N�C�b�N�u�[�X�g���g�p���A�^�[�Q�b�g�Ƃ̋������l�߂�.
{
	if (m_pTarget)
	{
		float fDis = D3DXVec3Length(&(m_pTarget->GetPosition() - m_pBody->GetPosition()));

		if (fDis > 100.0f)
		{
			return true;
		}
	}

	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoid(float& fPush, float& fAngle)//�N�C�b�N�u�[�X�g�ɂ����.
{
	fPush = 0.0f;
	fAngle = 0.0f;

	for (unsigned int i = 0; i < m_v_QuickAvoidState.size(); i++)
	{
		switch (m_v_QuickAvoidState[i].iAvoidNum)
		{
		case enAvoidLockTime:
			if (IsQuickBoostAvoidtoLockTime(m_v_QuickAvoidState[i], fPush, fAngle))
			{
				return true;
			}
			break;

		case enAvoidDamage:
			if (IsQuickBoostAvoidtoDamage(m_v_QuickAvoidState[i], fPush,fAngle))
			{
				return true;
			}
			break;

		default:
			break;
		}
	}

	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoidtoLockTime(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle)
{
	if (m_iLockTime > 0)
	{
		m_iLockTime--;
	}

	else
	{
		m_iLockTime = AvoidState.iLockTimeorDamage * static_cast<int>(g_fFPS);
		fPush = 1.0f;
		fAngle = static_cast<float>(D3DXToRadian(AvoidState.iAvoidDir));
		return true;
	}

	if (!m_pBody->m_bRadarWarning)
	{
		m_iLockTime = AvoidState.iLockTimeorDamage * static_cast<int>(g_fFPS);
	}
	
	

	return false;
}

bool clsEnemyRobo::IsQuickBoostAvoidtoDamage(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle)
{
	if (m_iAvoidDamageUpdateTime > 0)
	{
		m_iDamage += m_pBody->m_iDamage;
		if (m_iDamage > AvoidState.iLockTimeorDamage)
		{
			m_iDamage = 0;
			fPush = 1.0f;
			fAngle = static_cast<float>(D3DXToRadian(AvoidState.iAvoidDir));
			return true;
		}

		m_iAvoidDamageUpdateTime--;
	}

	else
	{

		m_iAvoidDamageUpdateTime = AvoidState.iUpdateTime;
		m_iDamage = 0;
	}

	return false;
}

bool clsEnemyRobo::IsShotR()
{
	m_ShotData = m_RShotData;

	if (IsShot())
	{
		return true;
	}

	return false;
}

bool clsEnemyRobo::IsShotL()
{
	m_ShotData = m_LShotData;

	if (IsShot())
	{
		return true;
	}

	return false;
}

clsRoboCommand* clsEnemyRobo::MoveOperation(float& fPower, float& fAngle)
{
	if (SetMoveDir(fPower, fAngle))
	{
		return m_pComMove;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::MoveSwitchOperation()
{
	if (m_pBody->m_bBoost)
	{
		if (IsBoostOff())
		{
			return m_pComMoveSwitch;
		}
	}

	else
	{
		if (IsBoostOn())
		{
			return m_pComMoveSwitch;
		}
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::RotateOperation(float& fPower, float& fAngle)
{
	if (SetRotate(fPower,fAngle))
	{
		return m_pComRotate;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::LookOperation(float& fPower, float& fAngle)
{
	if (SetLook(fPower, fAngle))
	{
		return m_pComLook;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::QuickTurnOperation(float& fPower, float& fAngle)
{
	if (IsQuickTurn(fPower, fAngle))
	{
		return m_pComQuickTrun;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::QuickBoostOperation(float& fPower, float& fAngle)
{


	return nullptr;
}

clsRoboCommand* clsEnemyRobo::BoostOperation()
{
	if (IsJump())
	{
		return m_pComBoost;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::LShotOperation()
{
	if (IsShotL())
	{
		return m_pComLShot;
	}

	return nullptr;
}

clsRoboCommand* clsEnemyRobo::RShotOperation()
{
	if (IsShotR())
	{
		return m_pComRShot;
	}

	return nullptr;
}

clsEnemyRobo::clsEnemyRobo()
{
	m_pComMove = new clsCommandMove;
	m_pComMoveSwitch = new clsCommandMoveSwitch;
	m_pComRotate = new clsCommandRotate;
	m_pComLook = new clsCommandLookVerMove;

	m_pComQuickBoost = new clsCommandQuickBoost;
	m_pComQuickTrun = new clsCommandQuickTurn;
	m_pComBoost = new clsCommandBoostRising;

	m_pComLShot = new clsCommandLShot;
	m_pComRShot = new clsCommandRShot;
}

clsEnemyRobo::~clsEnemyRobo()
{
	delete m_pComMove;
	delete m_pComMoveSwitch;
	delete m_pComRotate;
	delete m_pComLook;
	
	delete m_pComQuickBoost;
	delete m_pComQuickTrun;
	delete m_pComBoost;
	
	delete m_pComLShot;
	delete m_pComRShot;
}