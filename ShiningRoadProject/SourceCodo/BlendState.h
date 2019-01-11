#ifndef MY_BLEND_STATE_H_
#define MY_BLEND_STATE_H_

#include "Global.h"


//ID3D11BlendState���ȒP�ɑ��삷�邽�߂̃N���X.
//�V���O���g���Ŏg��.
class clsBLEND_STATE
{
public:

	clsBLEND_STATE();
//	clsBLEND_STATE(
//		ID3D11Device* const pDevice,
//		ID3D11DeviceContext* const pContext );
	~clsBLEND_STATE();

	void Create(
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext );

	//��������true�Ȃ�������������ł��u�����h�X�e�[�g����蒼��.
	void SetBlend( const bool isAlpha, const bool isCreateAbsolute = true );


private:

	//�u�����h�X�e�[�g�쐬.
	HRESULT CreateBlendState( ID3D11BlendState** ppBlendState );
	
	//�u�����h�f�X�N�쐬.
	D3D11_BLEND_DESC CreateBlendDesc( D3D11_BLEND_DESC* const pBlendDesc );

private:

	ID3D11Device*		 m_wpDevice;
	ID3D11DeviceContext* m_wpContext;

	bool m_isUse = false;

	bool m_isAlpha;
};


#endif//#ifndef MY_BLEND_STATE_H_