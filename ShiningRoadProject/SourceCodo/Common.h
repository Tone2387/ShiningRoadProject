#ifndef COMMON_H_
#define COMMON_H_

#define ALIGN16	_declspec( align( 16 ) )

//インクルード.
#include "Global.h"



//幅高さ構造体.
struct WHSIZE_FLOAT
{
	float w;
	float h;
};

//共通クラス(親クラス).
class clsCommon
{
public:
	clsCommon();
	virtual ~clsCommon();





protected:

	//頂点の構造体.
	struct SpriteVertex
	{
		D3DXVECTOR3 vPos;	//位置.
		D3DXVECTOR2 vTex;	//テクスチャ座標.
	};

	//ブレンドステート作成.
	HRESULT CreateBlendState();

	//透過(アルファブレンド)設定の切り替え.
	void SetBlend( const bool flg );

protected:

	//↓アプリに一つ.
	ID3D11Device*			m_pDevice11;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext11;	//デバイスコンテキスト.

	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//ブレンドステート.

};

#endif//#define COMMON_H_
