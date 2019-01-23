#ifndef MY_BLEND_STATE_H_
#define MY_BLEND_STATE_H_

#include "Global.h"


//ID3D11BlendStateを簡単に操作するためのクラス.
//シングルトンで使う.
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

	//第二引数がtrueなら第一引数が同じでもブレンドステートを作り直す.
	void SetBlend( const bool isAlpha, const bool isCreateAbsolute = true );


private:

	//ブレンドステート作成.
	HRESULT CreateBlendState( ID3D11BlendState** ppBlendState );
	
	//ブレンドデスク作成.
	D3D11_BLEND_DESC CreateBlendDesc( D3D11_BLEND_DESC* const pBlendDesc );

private:

	ID3D11Device*		 m_wpDevice;
	ID3D11DeviceContext* m_wpContext;

	bool m_isUse = false;

	bool m_isAlpha;
};


#endif//#ifndef MY_BLEND_STATE_H_