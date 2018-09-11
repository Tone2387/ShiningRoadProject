//#pragma once

#ifndef CHARACTOR
#define CHARACTOR

#include"Global.h"
#include"Object.h"
#include"Weapon.h"

struct HitState//�e�������������ɑ��葤�ɑ�����̂܂Ƃ�.
{
	bool bHit;
	int iDamage;
	float fInpuct;
	D3DXVECTOR3 vInpuctDir;

	void Clear();
};

class clsCharactor : public clsObject
{
public:
	//���ʃX�e�[�^�X.
	int m_HP;//�̗�.
	int m_MaxHP;//�ő�̗�.

	bool m_bDeadFlg;//���S�t���O.

	//���͊֌W.
	float fPushMin;

	//�ړ��֌W.
	bool m_bMoving;
	
	float m_fMoveSpeedMax;//�X�s�[�h�̍ő�l.

	int m_iTopMoveSpeedFrame;//���ɒB����܂ł̃t���[���l.
	float m_fMoveAccele;// = m_fMoveSpeedMax / m_fTopSpeedFrame;
	
	int m_iMoveStopFrame;//���S�ɒ�~����܂ł̃t���[���l.
	float m_fMoveDecele;// = m_fMoveSpeed / m_iStopFrame;

	D3DXVECTOR3 m_vAcceleDir;

	int m_iMoveReverseDirInertia;

	bool m_bRotation;

	float m_fRotSpeed;
	float m_fRotSpeedMax;

	int m_iTopRotSpeedFrame;
	float m_fRotAccele;

	int m_iRotStopFrame;
	float m_fRotDecele;

	float m_fRotDir;

	float m_fLookUpDir;

	float m_fJumpPower;

	bool m_bLockStartingPosXSwitch;
	float m_bLockStartingPosX;

	D3DXVECTOR3 m_vLockPos;//���̃L�����̒����_.
	D3DXVECTOR3 m_vLockRangeDir;//���̃L�����̎�������.
	float m_fLockRange;//���b�N�I������.
	float m_fLockCircleRadius;//���b�N�I������̔��a.

	D3D10_VIEWPORT* m_pViewPort;//�O��������񎟌��ւ̕ϊ��p.
	D3DXMATRIX m_mProj;//�O��������񎟌��ւ̕ϊ��p.
	D3DXMATRIX m_mThisCharaView;//���̃L�����̃J�����̃r���[�s��.
	D3DXVECTOR3 m_vLockRangePos;//���b�N�I������̊J�n���W(�J�������W).
	D3DXVECTOR3 m_vLockCenterPos;
	D3DXVECTOR3 m_vTargetScrPos;
	bool m_bCamPosXSwitch;
	bool IsTargetDirBack();//�G���L���������ɂ��邩.

	

	D3DXVECTOR3 GetCamPos();

	std::vector<clsWeapon*> m_v_pWeapons;
	std::vector<D3DXVECTOR3> m_v_vMuzzlePos;
	std::vector<D3DXVECTOR3> m_v_vShotDir;
	std::vector<clsCharactor*> m_v_pEnemys;

	void SetEnemys(std::vector<clsCharactor*> v_pEnemys);//�G�̔F��.

	int m_iWeaponNum;
	int m_iWeaponNumMax;

	bool Shot();
	bool Reload();

	HitState BulletHit(std::vector<clsObject::SPHERE> v_TargetSphere);
	bool Damage(HitState);//�_���[�W�ƏՌ���.

	void WeaponInit(clsPOINTER_GROUP* pPrt, WeaponState* pWeapon,const int iWeaponMax);//pWeapon�ɂ͔z��̃|�C���^�[�����Ă�������.
	
	void WeaponUpdate();

	void LockChara();
	bool IsInLockRange();
	void SetLockRangeDir();
	bool IsInLockRange(D3DXVECTOR3 vTargetPos);

	void Lock();
	void LockOut();

	clsCharactor* m_pTargetChara;
	int m_iTargetNo;

	bool m_bRadarWarning;//���b�N����Ă�.
	int m_iDamage;//�_���[�W(���t���[��������)

	void CharaInit(clsPOINTER_GROUP* pPrt);

	void CharactorUpdate();

	//�ړ��֌W.
	void Move();

	void AddMoveAccele(const float fAngle, const float fPush);//�ړ������x���Z.
	
	bool IsMoveing();
	bool IsMoveControl();

	void SetMoveDir(const float fAngle);
	void MoveControl();

	void MoveAccele(const float fPower);
	void MoveDecele();

	void SetMoveAcceleSpeed(const float fMoveSpeedMax, const int iTopSpeedFrame);//����.
	void SetMoveDeceleSpeed(const int iMoveStopFrame);//����.

	//��].
	void Rotate();

	void AddRotAccele(const float fAngle, const float fPush);//��]���x���Z.

	bool IsRotate();
	bool IsRotControl();

	void RotAccele(const float fPower);
	void RotDecele();

	void SetRotDir(float Angle);

	void SetRotAcceleSpeed(const float fRotSpeedMax, const int iTopRotSpdFrame);
	void SetRotDeceleSpeed(const int iRotStopFrame);

	void Spin(//�p�x�ɉ�]���x�̉��Z���s��.
		float& fNowYaw,
		const float fTargetYaw,
		const float fTurnSpd);

	void LookUp(const float fAngle, const float fPush);//�㉺���_.

	//�W�����v.
	
	void SetJumpPower(const float fPower)
	{
		m_fJumpPower = fPower;
	}

	void Jump();

	bool PointIntersect(
		const D3DXVECTOR3 StartPos,	//��̈ʒu.
		const D3DXVECTOR3 EndPos,		//�W�I�̈ʒu.
		const clsDX9Mesh* pTarget		//��Q���̕���.
		);


	clsCharactor();
	virtual ~clsCharactor();

	bool IsCurcleLange(
		const D3DXVECTOR3 CenterPos, 
		const D3DXVECTOR3 TargetPos, 
		const float Range);//�~�͈͔̔���.

protected:
	void ShotSwich(const int iWeaponNum);//�������镐�킩��g�p���镐������߂�.

private:
	bool m_bMoveAcceleOlder;//�ړ����x���Z���s�������ǂ���.
	bool m_bRotAcceleOlder;//��]���x���Z���s�������ǂ���.

};

#endif