#ifndef COMMON_H_
#define COMMON_H_

#define ALIGN16	_declspec( align( 16 ) )

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

	//�u�����h�X�e�[�g�쐬.
	HRESULT CreateBlendState();

	//����(�A���t�@�u�����h)�ݒ�̐؂�ւ�.
	void SetBlend( const bool flg );

protected:

	//���A�v���Ɉ��.
	ID3D11Device*			m_pDevice11;		//�f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pDeviceContext11;	//�f�o�C�X�R���e�L�X�g.

	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//�u�����h�X�e�[�g.

};

#endif//#define COMMON_H_
