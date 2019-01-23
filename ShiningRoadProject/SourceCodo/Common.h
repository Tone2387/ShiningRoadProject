#ifndef COMMON_H_
#define COMMON_H_

#define ALIGN16	_declspec( align( 16 ) )

class clsBLEND_STATE;

//�C���N���[�h.
#include "Global.h"

//�������\����.
struct WHSIZE_FLOAT
{
	float w;
	float h;
};


//���ʃN���X(�e�N���X).
class clsCommon
{
public:
	clsCommon();
	virtual ~clsCommon();


protected:

	//���_�̍\����.
	struct SpriteVertex
	{
		D3DXVECTOR3 vPos;	//�ʒu.
		D3DXVECTOR2 vTex;	//�e�N�X�`�����W.
	};


protected:

	//���A�v���Ɉ��.
	ID3D11Device*			m_wpDevice;		//�f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_wpContext;	//�f�o�C�X�R���e�L�X�g.

	clsBLEND_STATE*	m_psinBlend;

};

#endif//#define COMMON_H_
