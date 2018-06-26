#pragma once


#include "Sound.h"

#include <vector>
#include <deque>
#include <memory>



//サウンドクラスの基底クラス.
class clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_BASE( const HWND hWnd );
	virtual ~clsSOUND_MANAGER_BASE();

	//new直後に一度だけ使用.
	void Create();

	//毎フレーム一回使う.
	void UpdateLoop();

	//すべて停止.
	void StopAllSound();

	//----- BGM -----//
	//再生関数.
	bool PlayBGM( const int bgmNo, const bool bNotify = true );
	//停止関数.
	bool StopBGM( const int bgmNo );
	//音の停止を確認する関数.
	bool IsStoppedBGM( const int bgmNo ) const;
	//音の再生中を確認する関数.
	bool IsPlayingBGM( const int bgmNo ) const;
	//巻き戻し関数(再生位置初期化).
	bool SeekToStartBGM( const int bgmNo ) const;


	//----- SE -----//
	//再生関数.
	bool PlaySE( const int seNo, const bool bNotify = false );
	//停止関数.
	bool StopSE( const int seNo );
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

	//長い型は嫌いなので.
	using SOUND_SET = std::vector< std::unique_ptr< clsSound > >;

	//サウンドクラス作成.
	void CreateSound( 
		SOUND_SET &vpSound,
		std::deque<bool> &dqbLoop,
		const unsigned int uiRESERVE_SIZE,
		const std::string sFilePath );

	//サウンドデータ作成.
	void CreateSoundData(
		std::vector< clsSound::SOUND_DATA > &vData,
		const std::string sFilePath );

	//サウンドクラス.
	SOUND_SET m_vupBgm;
	SOUND_SET m_vupSe;
	//ループ再生するかのフラグ.
	std::deque<bool> m_dqbLoopBgm;
	std::deque<bool> m_dqbLoopSe;

	//リザーブのサイズ.
	const unsigned int m_uiRESERVE_SIZE_BGM;
	const unsigned int m_uiRESERVE_SIZE_SE;

	HWND m_hWnd;

};

