#pragma once

#include	"MyMacro.h"

#include	<Windows.h>

#include	<D3DX11.h>
#include	<D3D11.h>

#include	<D3DX10.h>//「D3DX～」の定義使用時に必要.
#include	<D3D10.h>



//============================================================
//	ライブラリ.
//============================================================
#pragma comment( lib, "winmm.lib" )

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )

#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.

//構造体.
struct  TEXT_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;
	D3DXVECTOR4	vColor;
	D3DXVECTOR4	fAlpha;
};

//テキストの位置情報.
struct TextVertex
{
	D3DXVECTOR3	Pos;	//位置.
	D3DXVECTOR2	Tex;	//テクスチャ座標.
};

