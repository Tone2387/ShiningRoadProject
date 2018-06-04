#pragma once


#include "Sound.h"


//音指定用列挙体.
enum enBGM : UCHAR
{
//	enBGM_START_UP = 0,
	enBGM_TITLE = 0,
	enBGM_ASSEMBLE,
	enBGM_MISSION,
	enBGM_ENDING,
	enBGM_OVER,

	enBGM_MAX,
};
enum enSE : UCHAR
{
	enSE_ENTER = 0,
	enSE_EXIT,

	enSE_MAX,
};


class clsSOUND_MANAGER
{
public:
	clsSOUND_MANAGER( const HWND hWnd );
	~clsSOUND_MANAGER();


	//----- BGM -----//
	//再生関数.
	bool PlayBGM( const enBGM bgmNo, const bool bNotify = true ) const;
	//停止関数.
	bool StopBGM( const enBGM bgmNo ) const;
	//音の停止を確認する関数.
	bool IsStoppedBGM( const enBGM bgmNo ) const;
	//音の再生中を確認する関数.
	bool IsPlayingBGM( const enBGM bgmNo ) const;
	//巻き戻し関数(再生位置初期化).
	bool SeekToStartBGM( const enBGM bgmNo ) const;


	//----- SE -----//
	//再生関数.
	bool PlaySE( const enSE seNo, const bool bNotify = false ) const;
	//停止関数.
	bool StopSE( const enSE seNo ) const;
	//音の停止を確認する関数.
	bool IsStoppedSE( const enSE seNo ) const;
	//音の再生中を確認する関数.
	bool IsPlayingSE( const enSE seNo ) const;
	//巻き戻し関数(再生位置初期化).
	bool SeekToStartSE( const enSE seNo ) const;

private:

	//サウンドクラス作成.
	void CreateSound();

	//サウンドクラス.
	clsSound*	m_pBgm[enBGM_MAX];

	clsSound*	m_pSe[enSE_MAX];


	HWND m_hWnd;
};

