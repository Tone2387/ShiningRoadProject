#pragma once


#include "Sound.h"

#include <vector>
#include <memory>


//サウンドクラスの基底クラス.
class clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_BASE( const HWND hWnd );
	virtual ~clsSOUND_MANAGER_BASE();

	//new直後に一度だけ使用.
	void Create();

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

protected:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	virtual void CreateSceneName() = 0;

	//継承クラスで中身を入れる.
	std::string m_sSceneName;

private:

	//サウンドデータ作成.
	void CreateSoundData(
		std::vector< clsSound::SOUND_DATA > &vData,
		const std::string sFilePath );

	//サウンドクラス作成.
	void CreateSound( 
		std::vector< std::unique_ptr< clsSound > > &vpSound,
		const std::string sFilePath );

	//サウンドクラス.
	std::vector< std::unique_ptr< clsSound > > m_vupBgm;
	std::vector< std::unique_ptr< clsSound > > m_vupSe;

	//リザーブのサイズ.
	const unsigned int m_uiRESERVE_SIZE_BGM;
	const unsigned int m_uiRESERVE_SIZE_SE;

	HWND m_hWnd;

};

