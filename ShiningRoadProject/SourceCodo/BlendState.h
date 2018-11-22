#ifndef MY_BLEND_STATE_H_
#define MY_BLEND_STATE_H_

#include "Global.h"


//ID3D11BlendState���ȒP�ɑ��삷�邽�߂̃N���X.
//�V���O���g��.
class clsBLEND_STATE
{
public:

	clsBLEND_STATE(
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext );
	~clsBLEND_STATE();

	enum class encBLEND_FLAG : UCHAR
	{
		ALPHA = 0,
		NOT_ALPHA,		
	};

	void SetBlend( const encBLEND_FLAG encBlendFlag );

private:

	//�u�����h�X�e�[�g�쐬.
	HRESULT CreateBlendState( ID3D11BlendState** ppBlendState );
	
	//�u�����h�f�X�N�쐬.
	D3D11_BLEND_DESC CreateBlendDesc( D3D11_BLEND_DESC* const pBlendDesc );

private:

	ID3D11Device*		 m_wpDevice;
	ID3D11DeviceContext* m_wpContext;

	encBLEND_FLAG m_encBrendFlg;
};


#endif//#ifndef MY_BLEND_STATE_H_