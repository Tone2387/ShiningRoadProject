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