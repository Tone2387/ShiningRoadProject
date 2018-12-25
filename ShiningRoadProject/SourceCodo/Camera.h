#ifndef CAMERA_BASE_H_
#define CAMERA_BASE_H_

#include "Global.h"



//�J�����̊��N���X.
class clsCAMERA_BASE
{
public:
	clsCAMERA_BASE();
	virtual ~clsCAMERA_BASE();

	//�ŏ�( �V�[���̂͂��� )�̏�����.
	virtual void Create() = 0;
	//���t���[���̏���.
	virtual void Update() = 0;

	//��������true->m_vLook�����Ă���.
	//��������false->m_vLook�͂��̂܂܂̈ʒu.
	virtual void SetPos( const D3DXVECTOR3& vPos, bool isWithLook = true ){
		if( isWithLook ){
			D3DXVECTOR3 tmpVec;//�J�����̈ړ���.
			tmpVec = vPos - m_vPos;
			m_vLook = vPos + tmpVec;
		}
		m_vPos = vPos;
	};
	virtual void AddPos( const D3DXVECTOR3& vVec, bool isWithLook = true ){
		if( isWithLook ){ m_vLook += vVec; }
		m_vPos += vVec;
	};
	D3DXVECTOR3 GetPos() const { return m_vPos; };


	virtual void SetLookPos( const D3DXVECTOR3& vPos )	{ m_vLook = vPos; };
	virtual void AddLookPos( const D3DXVECTOR3& vVec )	{ m_vLook += vVec; };
	D3DXVECTOR3 GetLookPos() const				{ return m_vLook; };

	D3DXVECTOR3 GetRot() const				{ return m_vRot; };
	void SetRot( const D3DXVECTOR3 &vRot )	{ m_vRot = vRot; };

protected:

	D3DXVECTOR3 m_vPos;	//�J�����ʒu.
	D3DXVECTOR3 m_vLook;//�����ʒu.
	D3DXVECTOR3 m_vRot;	//��]�l.

	D3DXMATRIX	m_mRot;	//��]�s��.

};

#endif//#ifndef CAMERA_BASE_H_