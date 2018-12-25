#ifndef CAMERA_MOVIE_BASE_
#define CAMERA_MOVIE_BASE_

#include "Camera.h"


//�J�����̓�������ꂽ���N���X.
class clsCAMERA_MOVIE_BASE : public clsCAMERA_BASE
{
public:
	clsCAMERA_MOVIE_BASE();
	virtual ~clsCAMERA_MOVIE_BASE();

	//�ŏ�( �V�[���̂͂��� )�̏�����.
	virtual void Create() override = 0;
	//���t���[���̏���.
	virtual void Update() override = 0;


	void SetPos( const D3DXVECTOR3& vPos, bool isWithLook = true ) override {
		clsCAMERA_BASE::SetPos( vPos, isWithLook );
		UpdateDistance();
		UpdateRot();
	}
	void AddPos( const D3DXVECTOR3& vVec, bool isWithLook = true ) override {
		clsCAMERA_BASE::AddPos( vVec, isWithLook );
		UpdateDistance();
		UpdateRot();
	}
	void SetLookPos( const D3DXVECTOR3& vPos ) override { 
		clsCAMERA_BASE::SetLookPos( vPos );
		UpdateDistance();
		UpdateRot();
	}
	void AddLookPos( const D3DXVECTOR3& vVec ) override {
		clsCAMERA_BASE::AddLookPos( vVec );
		UpdateDistance();
		UpdateRot();
	}



	//�Ď��Ώۂ𒆐S�ɐ��񂷂�.
	void Spn( const float fSpn ){
		SpnProduct( fSpn, &m_vPos, &m_vLook, &m_vRot );
	}

	//�J�����ʒu�𒆐S�ɂ��Č���.
	void Turn( const float fTurn ){
		TurnProduct( fTurn, &m_vPos, &m_vLook, &m_vRot );
	}

	//���̃J���������ɑO�i.advancing
	void Advancing( const float fMove ){
		AdvancingProduct( fMove, &m_vPos, &m_vLook, &m_vRot );
	}
	//�����s�ړ�( +�ŉE���A-�ō����Ɉړ� ).
	void CrabWalk( const float fMove ){
		CrabWalkProduct( fMove, &m_vPos, &m_vLook, &m_vRot );
	}


	//���_�Ƃ̋�����ύX.
	void SetDistance( const float fDistance, const bool isCamMove );
	void AddDistance( const float fDistance, const bool isCamMove );

protected:

	void UpdateRot();

	void UpdateDistance();

	//�Ď��Ώۂ𒆐S�ɐ��񂷂�.
	void SpnProduct( 
		const float fSpn, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;

	//�J�����ʒu�𒆐S�ɂ��Č���.
	void TurnProduct( const float fTurn, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;

	//���̃J���������ɑO�i.advancing
	void AdvancingProduct( 
		const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;
	//�����s�ړ�.
	void CrabWalkProduct( 
		const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;


protected:

	float m_fDistance;//�J�����Ǝ��_�̋���( x&z�������̓񎟌���ł� ).


private:


};


#endif//#ifndef CAMERA_MOVIE_BASE_