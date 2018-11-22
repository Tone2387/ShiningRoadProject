#ifndef MY_BLEND_STATE_H_
#define MY_BLEND_STATE_H_

#include "Global.h"


//ID3D11BlendStateを簡単に操作するためのクラス.
//シングルトン.
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

	//ブレンドステート作成.
	HRESULT CreateBlendState( ID3D11BlendState** ppBlendState );
	
	//ブレンドデスク作成.
	D3D11_BLEND_DESC CreateBlendDesc( D3D11_BLEND_DESC* const pBlendDesc );

private:

	ID3D11Device*		 m_wpDevice;
	ID3D11DeviceContext* m_wpContext;

	encBLEND_FLAG m_encBrendFlg;
};


#endif//#ifndef MY_BLEND_STATE_H_