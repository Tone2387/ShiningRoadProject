#pragma once

#include"AI.h"
#include"RoboCommand.h"

#define sShot

class clsEnemyRobo : public clsEnemyBase
{
public:
	clsEnemyRobo();
	virtual ~clsEnemyRobo();

	void Init(
		LPSTR strEnemyFolderName,
		clsRobo* pRobo);

	clsRoboCommand* MoveOperation(float& fPower, float& fAngle);
	clsRoboCommand* MoveSwitchOperation();
	clsRoboCommand* RotateOperation(float& fPower, float& fAngle);
	clsRoboCommand* LookOperation(float& fPower, float& fAngle);

	clsRoboCommand* QuickTurnOperation(float& fPower, float& fAngle);
	clsRoboCommand* QuickBoostOperation(float& fPower, float& fAngle);
	clsRoboCommand* BoostOperation();

	clsRoboCommand* LShotOperation();
	clsRoboCommand* RShotOperation();
	
private:
	struct Boost
	{
		int iRisingENParcent;//�㏸�J�n���Ă���EN�c�ʗ�.
		int iENLimitParcent;//�㏸����߂�EN�c�ʗ�.
	};

	enum enBoostFileOrder
	{
		enBoostRisingENParcent = 0,
		enBoostENLimitParcent,

		enBoostSize
	};

	struct QuickBoostApproach
	{
		int iUpdateTime;
		int iDis;
		int iENLimit;

		//�ȉ��A���t���[���X�V�̂��߂̕ϐ�.
		int iUpdateCnt;
	};

	enum enQuickBoostApproachFileOrder
	{
		enQuickBoostAppUpdateTime = 0,
		enQuickBoostAppDis,
		enQuickBoostAppENLimit,

		enQuickBoostAppSize
	};

	struct QuickTrun
	{
		int iUpdateTime;
		int iDir;
		int iENLimit;

		//�ȉ��A���t���[���X�V�̂��߂̕ϐ�.
		int iUpdateCnt;
	};

	enum enQuickTrunFileOrder
	{
		enQuickTrunUpdateTime = 0,
		enQuickTrunDir,
		enQuickTrunENLimit,

		enQuickTrunSize
	};

	enum enQuickBoostAvoidType
	{
		enQuickBoostAvoidTypeLookTime = 0,//�탍�b�N�I�����Ԃɂ����s.
		enQuickBoostAvoidTypeLookTimePriorityUpdateTime,//���s�Ԋu��D��.
		enQuickBoostAvoidTypeDamage,//��_���[�W�ɂ����s.
		enQuickBoostAvoidTypeDamagePriorityUpdateTime,//���s�Ԋu��D��.

		enQuickBoostAvoidTypeSize
	};

	struct QuickBoostAvoid
	{
		int iUpdateTime;//�X�V�Ԋuor���s�Ԋu.
		int iAvoidType;//��������J�e�S���i���o�[.
		int iConditions;//��������̋��e�l.
		int iAvoidDir;//����������.
		int iENLimit;//�����EN����

		//�ȉ��A���t���[���X�V�̂��߂̕ϐ�.
		int iUpdateCnt;//�X�V�Ԋuor���s�Ԋu�J�E���g.
		int iConditionsCnt;//�����̒~��.
	};

	enum enQuickBoostAvoidFileOrder
	{
		enQuickBoostAvoidUpdateTime = 0,
		enQuickBoostAvoidType,
		enQuickBoostAvoidLockTimeorDamage,
		enQuickBoostAvoidDir,
		enQuickBoostAvoidENLimit,

		enQuickBoostAvoidSize
	};

	clsRobo* m_pBody;

	clsRoboCommand* m_pComMove;
	clsRoboCommand* m_pComMoveSwitch;
	clsRoboCommand* m_pComRotate;
	clsRoboCommand* m_pComLook;

	clsRoboCommand* m_pComQuickBoost;
	clsRoboCommand* m_pComQuickTrun;
	clsRoboCommand* m_pComBoost;

	clsRoboCommand* m_pComLShot;
	clsRoboCommand* m_pComRShot;

	void IsENSaving();//�u�[�X�g�㏸���s���Ă����ق�EN���񕜂������𔻒�.
	bool IsBoostRising();

	bool IsBoostOn();
	bool IsBoostOff();

	bool IsQuickTurn(float& fPush, float& fAngle);//�^�[�Q�b�g�ʒu�̕��������ʂ�����ȏ㗣��Ă��ꍇ�A�N�C�b�N�^�[�����g�p.
	bool IsQuickBoostApproach(float& fPush, float& fAngle);//�N�C�b�N�u�[�X�g���g�p���A�^�[�Q�b�g�Ƃ̋������l�߂�.

	bool IsQuickBoostAvoid(float& fPush, float& fAngle);//�N�C�b�N�u�[�X�g�ɂ����.

	//�����������֐��Q.
	//�탍�b�N�I������(��������D��)
	bool IsQuickBoostAvoidLockTime(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	//�탍�b�N�I������(���s�Ԋu�D��)
	bool IsQuickBoostAvoidLockTimePriorityUpdateTime(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	//��_���[�W��(��������D��)
	bool IsQuickBoostAvoidDamage(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	//��_���[�W��(���s�Ԋu�D��)
	bool IsQuickBoostAvoidDamagePriorityUpdateTime(QuickBoostAvoid& AvoidState, float& fPush, float& fAngle);
	//void ResetAvoidtoDamage();

	bool IsShotR();
	bool IsShotL();

	virtual void SetDataProduct();
	virtual void SetShotData();

	void SetBoostData();

	void SetQuickBoostAppData();
	void SetQuickTrunData();
	void SetQuickBoostAvoidData();

	std::vector<ShotState> m_v_LShotState;
	std::vector<ShotState> m_v_RShotState;

	Boost m_BoostState;

	std::vector<QuickBoostApproach> m_v_QuickBoostAppState;
	std::vector<QuickTrun> m_v_QuickTrunState;
	std::vector<QuickBoostAvoid> m_v_QuickBoostAvoidState;

	bool m_bENSaving;
};