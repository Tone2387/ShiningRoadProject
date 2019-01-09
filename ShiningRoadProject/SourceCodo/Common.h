#ifndef COMMON_H_
#define COMMON_H_

#define ALIGN16	_declspec( align( 16 ) )

class clsBLEND_STATE;

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


protected:

	//↓アプリに一つ.
	ID3D11Device*			m_wpDevice;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_wpContext;	//デバイスコンテキスト.

	clsBLEND_STATE*	m_psinBlend;

};

#endif//#define COMMON_H_
