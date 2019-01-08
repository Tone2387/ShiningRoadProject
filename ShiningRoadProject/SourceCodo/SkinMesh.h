//#pragma once

#ifndef SKINMESH
#define SKINMESH

#include "CD3DXSKINMESH.h"

static const double g_dAnimSpeedReference = static_cast<double>(1 / g_fFPS);

class clsSkinMesh
{
public:
	clsSkinMesh();
	virtual ~clsSkinMesh();

	//�����ް��̊֘A�t��.
	void AttachModel(clsD3DXSKINMESH* const pModel);
	//�����ް��֘A�t������.
	void DetatchModel();

	//���W���]�̍X�V.
	void ModelUpdate(const TRANSFORM Transform);

	//�����ݸފ֐�.
	void ModelRender(
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj,
		const D3DXVECTOR3& vLight,
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4& vColorBase = { 1.0f, 1.0f, 1.0f, 1.0f },
		const D3DXVECTOR4& vColorArmor = { 1.0f, 1.0f, 1.0f, 1.0f },
		const bool isAlpha = false);

	//��Ұ��ݍő吔���擾.
	const int GetAnimSetMax();
	//�w�肵���{�[���ʒu��n��.
	virtual D3DXVECTOR3 GetBonePos(const char* sBoneName, const bool isLocalPos = false);
	//�w�肵���{�[���ʒu����vDiviation���ړ������ʒu��n��.
	const D3DXVECTOR3 GetBoneDiviaPos(char* sBoneName, const  D3DXVECTOR3 vDiviation);
	//��Ұ��ݐ؂�ւ��֐�.//�ύX�ł���Ȃ�true, �ύX�ł��Ȃ��Ȃ�false���Ԃ�.
	const bool SetAnimChange(const int iAnimNo, const double dStartPos = 0.0f);
	//���݂̃A�j���[�V�����ԍ���n��.
	const int GetAnimNo();
	void SetAnimSpeed(const double dSpeed);
	const double GetAnimSpeed();
	void SetAnimTime(const double dTime);
	const double GetAnimTime();
	const double GetAnimEndTime(const int iAnimNo);
	const bool IsAnimTimeEnd();
	const bool IsAnimTimeAfter(const double dDesignationTime);
	const bool IsAnimTimePoint(const double dDesignationTime);
	const bool IsExistsBone(const char* sBoneName);

	void AnimPause()
	{
		m_bAnimPause = true;
	}

	void AnimPlay()
	{
		m_bAnimPause = false;
	}

	//�A�j���[�V�������t�Đ�������.
	void AnimReverce(const bool bAnimTimeInit = false);
	//�A�j���[�V������ʏ�Đ�������.
	void AnimNormal(const bool bAnimTimeInit = false);
	//�A�j���[�V�����Đ��̏�Ԃ�n�� true:�t�Đ�.
	const bool IsAnimReverce();

	clsD3DXSKINMESH* m_pMesh;

private:
	double m_dAnimTime;
	//Ӱ��݊֌W.
	int m_iAnimNo;//Ӱ��ݔԍ�.
	double m_dAnimSpeed;
	bool m_bAnimReverce;
	bool m_bAnimPause;
	
	LPD3DXANIMATIONCONTROLLER m_pAnimCtrl;

	//�A�j���[�V�����̍X�V.
	void AnimUpdate();
};


#endif