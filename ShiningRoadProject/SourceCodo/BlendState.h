#ifndef MY_BLEND_STATE_H_
#define MY_BLEND_STATE_H_

#include "Global.h"


//ID3D11BlendState���ȒP�ɑ��삷�邽�߂̃N���X.
class clsBLEND_STATE
{
public:
	clsBLEND_STATE(
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext );
	~clsBLEND_STATE();

	void SetBlend( const bool isAlpha ) const;

private:

	HRESULT CreateBlendState( ID3D11Device* const pDevice );

	bool m_isAlpha;

	ID3D11DeviceContext*	m_pContext;

	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//�u�����h�X�e�[�g.

};
#endif//#ifndef MY_BLEND_STATE_H_