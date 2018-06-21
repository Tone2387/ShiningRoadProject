#pragma once


#include "Sound.h"


#include <vector>
#include <memory>


////音指定用列挙体.
//enum enBGM : UCHAR
//{
////	enBGM_START_UP = 0,
//	enBGM_TITLE = 0,
//	enBGM_ASSEMBLE,
//	enBGM_MISSION,
//	enBGM_ENDING,
//	enBGM_OVER,
//
//	enBGM_MAX,
//};
//enum enSE : UCHAR
//{
//	enSE_ENTER = 0,
//	enSE_EXIT,
//
//	enSE_MAX,
//};


class clsSOUND_MANAGER
{
public:
	clsSOUND_MANAGER( const HWND hWnd );
	~clsSOUND_MANAGER();

	//すべて停止.
	void StopAllSound();

	//----- BGM -----//
	//再生関数.
	bool PlayBGM( const int bgmNo, const bool bNotify = true ) const;
	//停止関数.
	bool StopBGM( const int bgmNo ) const;
	//音の停止を確認する関数.
	bool IsStoppedBGM( const int bgmNo ) const;
	//音の再生中を確認する関数.
	bool IsPlayingBGM( const int bgmNo ) const;
	//巻き戻し関数(再生位置初期化).
	bool SeekToStartBGM( const int bgmNo ) const;


	//----- SE -----//
	//再生関数.
	bool PlaySE( const int seNo, const bool bNotify = false ) const;
	//停止関数.
	bool StopSE( const int seNo ) const;
	//音の停止を確認する関数.
	bool IsStoppedSE( const int seNo ) const;
	//音の再生中を確認する関数.
	bool IsPlayingSE( const int seNo ) const;
	//巻き戻し関数(再生位置初期化).
	bool SeekToStartSE( const int seNo ) const;

private:

	//サウンドクラス作成.
	void CreateSound();


	//サウンドクラス.
//	clsSound*	m_pBgm[enBGM_MAX];
//
//	clsSound*	m_pSe[enSE_MAX];

	std::vector< std::unique_ptr< clsSound > > m_vupBgm;
	std::vector< std::unique_ptr< clsSound > > m_vupSe;

	HWND m_hWnd;
};

