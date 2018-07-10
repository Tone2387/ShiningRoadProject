#ifndef SOUND_H_
#define SOUND_H_

#include <Windows.h>
#include <string>
//ライブラリ.
#pragma comment( lib, "winmm.lib" )



//文字列バッファ.
const int STR_BUFF_MAX = 256;




//============================================================
//	サウンドクラス.
//		midi, mp3, wav形式ファイルの再生・停止等を行う.
//============================================================
class clsSound
{
public:
	clsSound();
	~clsSound();

	//サウンド構造体.
	struct SOUND_DATA
	{
		std::string sAlias;	//エイリアス名.
		std::string sPath;	//ファイルパス (「BGM」or「SE」からでよい ).
		int  iMaxVolume;			//最大音量.
	};


	//音声ファイルを開く.
	bool Open( const LPCTSTR sFName, const char* const sAlias, const HWND hWnd );

	//音声ファイルを閉じる.
	bool Close();

	//再生関数.
	bool Play( const bool bNotify = false ) const;

	//停止関数.
	bool Stop() const;

//	//一時停止.
//	bool Pause();
//	//一時停止の解除.
//	bool Resume();

	//音の停止を確認する関数.
	bool IsStopped() const;

	//音の再生中を確認する関数.
	bool IsPlaying() const;


	//巻き戻し関数(再生位置初期化).
	bool SeekToStart() const;

	//音量を設定する.
	bool SetVolume( int iVolume ) const;
	//最大音量を設定する.
	void SetMaxVolume( const int iVolume = 1000 ){
		m_iMaxVol = iVolume;
	}


private:
	//初期設定.
	HRESULT	SetInitParam( const char *sAlias, const HWND hWnd );

	//状態取得関数.
	bool GetStatus( char* const sStatus ) const;


	HWND	m_hWnd;					//ウィンドウハンドル.
	char	m_sAlias[STR_BUFF_MAX];	//エイリアス.


	int		m_iMaxVol;//最大音量.

	bool	m_bOpend;//true = Open済.

};

#endif//#define SOUND_H_