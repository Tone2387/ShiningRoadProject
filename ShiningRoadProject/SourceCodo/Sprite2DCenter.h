#ifndef SPRITE_2D_CENTER_H_
#define SPRITE_2D_CENTER_H_

#include "Sprite2D.h"

//��_�����S�ɂ���̂ŉ�]���ł���.
class clsSPRITE2D_CENTER : public clsSprite2D
{
public:
	clsSPRITE2D_CENTER();
	~clsSPRITE2D_CENTER();

		//�`��(�����_�����O)
	void Render() override;

	D3DXVECTOR3 GetRot() const { return m_vRot; };
	void SetRot( const D3DXVECTOR3 &vRot ){
		m_vRot = vRot;
		GuardRotOver( m_vRot );
	};
	void AddRot( const D3DXVECTOR3 &vRot ){
		m_vRot += vRot;
		GuardRotOver( m_vRot );
	};


protected:

	//���f���쐬.
	HRESULT InitModel( const SPRITE_STATE ss ) override;

	//��]�̐��l�̌��E�����яo�Ȃ�.
	D3DXVECTOR3 GuardRotOver( D3DXVECTOR3 &vRot );
	float GuardRotOver( float &fRot );

protected:

	D3DXVECTOR3 m_vRot;//��].
};

#endif//#ifndef SPRITE_2D_CENTER_H_