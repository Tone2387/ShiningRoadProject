#ifndef COMMON_H_
#define COMMON_H_


//インクルード.
#include "Global.h"

//頂点の構造体.
struct SpriteVertex
{
	D3DXVECTOR3 vPos;	//位置.
	D3DXVECTOR2 vTex;	//テクスチャ座標.
};

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


	//透過(アルファブレンド)設定の切り替え.
	void SetBlend( const bool flg );




protected:

	//ブレンドステート作成.
	HRESULT CreateBlendState();

	//↓アプリに一つ.
	ID3D11Device*			m_pDevice11;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext11;	//デバイスコンテキスト.

	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//ブレンドステート.

};

#endif//#define COMMON_H_
