//#pragma once

#ifndef CHARACTOR
#define CHARACTOR

#include"Global.h"
#include"DX9Mesh.h"
#include"Object.h"

class clsCharactor : public clsObject
{

public:
	//���ʃX�e�[�^�X.
	int HP;//�̗�.
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

	virtual void Action()override{};

	bool m_bRotation;

	float m_fRotSpeed;
	float m_fRotSpeedMax;

	int m_iTopRotSpeedFrame;
	float m_fRotAccele;

	int m_iRotStopFrame;
	float m_fRotDecele;

	float m_fRotDir;

	float m_fJumpPower;

	void Shot();

	//�ړ��֌W.
	void Move(const float fAngle, const float fPush);
	
	bool IsMoveing();
	bool IsMoveControl();

	void SetMoveDir(const float fAngle);
	void MoveControl();

	void MoveAccele(const float fPower);
	void MoveDecele();

	void SetMoveAcceleSpeed(const float fMoveSpeedMax, const int iTopSpeedFrame);//����.
	void SetMoveDeceleSpeed(const int iMoveStopFrame);//����.
	

	//��].
	void Rotate(const float fAngle, const float fPush);
	bool IsRotate();
	bool IsRotControl();
	void RotAccele(const float fPower);
	void RotDecele();
	void SetRotAcceleSpeed(const float fRotSpeedMax, const int iTopRotSpdFrame);
	void SetRotDeceleSpeed(const int iRotStopFrame);
	void SetRotDir(float Angle);
	
	void SetRotationSpeed(const float fSpd);

	void Spin(
		float& fNowYaw,
		const float fTargetYaw,
		const float fTurnSpd,
		const float fTurnStop);

	//�󒆊֌W.
	

	void SetJumpPower(const float fPower)
	{
		m_fJumpPower = fPower;
	}

	void Jump();

	//�����蔻��֌W.

	//�X�t�B�A.

	

	bool PointIntersect(
		const D3DXVECTOR3 StartPos,	//��̈ʒu.
		const D3DXVECTOR3 EndPos,		//�W�I�̈ʒu.
		const clsDX9Mesh* pTarget		//��Q���̕���.
		);
	
	//ڲ��˯Ĕ���̒���.
	float RaySpece;

	clsCharactor();
	virtual ~clsCharactor();

	bool RecoLange(
		const D3DXVECTOR3 CenterPos, 
		const D3DXVECTOR3 TargetPos, 
		const float Range);//�~�͈͔̔���.

private:
	LPD3DXMESH m_pMeshForRay;//ڲ�̂��߂�ү���p.
};

#endif