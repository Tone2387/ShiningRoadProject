#ifndef GLOBAL_H_	//インクルードガード.
#define GLOBAL_H_


//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

//井上KyoStar.
#define Inoue

//太原.
#define Tahara


//============================================================
//	インクルード.
//============================================================
#include "MyMacro.h"
	    
#include <Windows.h>
	    
#include <D3DX11.h>
#include <D3D11.h>
	    
#include <D3DX10.h>//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include <memory>

//============================================================
//	ライブラリ.
//============================================================
#pragma comment( lib, "winmm.lib" )

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )

#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.



//============================================================
//	定数.
//============================================================
#define WND_W 1280	//ウィンドウ幅.
#define WND_H 720	//ウィンドウ高さ.



//進行方向列挙体.
enum class enDIRECTION : UCHAR
{
	STOP	= 0,//停止.
	FOWARD,		//前進.
	BACK,	//後退.
//	enDirection_LeftTurn,	//左回転.
//	enDirection_RightTurn	//右回転.
};


//このゲームの各シーンの列挙体( フラグ用 ).
enum class enSCENE : UCHAR
{
	TITLE,		//タイトル画面.
	ASSEMBLE,	//パーツ組み換え画面.
	MISSION,	//アクションシーン( ゲームのメイン ).
	ENDING,		//クリア後.
	GAMEOVER,		//ゲームオーバー.			

	NOTHING		//「SwitchScene」関数のフラグ用.
				//なんのシーンも指定していない.
};



#endif//#ifndef GLOBAL_H_