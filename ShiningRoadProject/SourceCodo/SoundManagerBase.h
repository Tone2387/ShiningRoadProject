#ifndef SOUND_MANAGER_BASE_H_
#define SOUND_MANAGER_BASE_H_


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
	bool PlayBGM( const int bgmNo, const bool bNotify = true )	{ return Play( m_BgmSet, m_dqisLoopBgm, m_veciBgmNum, bgmNo, bNotify ); };
	//停止関数.
	bool StopBGM( const int bgmNo )								{ return Stop( m_BgmSet, m_dqisLoopBgm, bgmNo ); };
	//音の停止を確認する関数.
	bool IsStoppedBGM( const int bgmNo ) const					{ return IsStopped( m_BgmSet, bgmNo ); };
	//音の再生中を確認する関数.
	bool IsPlayingBGM( const int bgmNo ) const					{ return IsPlaying( m_BgmSet, bgmNo ); };
	//巻き戻し関数(再生位置初期化).
	bool SeekToStartBGM( const int bgmNo ) const				{ return SeekToStart( m_BgmSet, bgmNo ); };


	//----- SE -----//
	//再生関数.
	bool PlaySE( const int seNo, const bool bNotify = false )	{ return Play( m_SeSet, m_dqisLoopSe, m_veciSeNum, seNo, bNotify ); };
	//停止関数.
	bool StopSE( const int seNo )								{ return Stop( m_SeSet, m_dqisLoopSe, seNo ); };
	//音の停止を確認する関数.
	bool IsStoppedSE( const int seNo ) const					{ return IsStopped( m_SeSet, seNo ); };
	//音の再生中を確認する関数.
	bool IsPlayingSE( const int seNo ) const					{ return IsPlaying( m_SeSet, seNo ); };
	//巻き戻し関数(再生位置初期化).
	bool SeekToStartSE( const int seNo ) const					{ return SeekToStart( m_SeSet, seNo ); };

protected:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	virtual void CreateSceneName() = 0;

protected:

	//継承クラスで中身を入れる.
	std::string m_sSceneName;

private:

	//長い型は嫌いなので.
	using SOUND_SET = 
		std::vector< std::vector< std::unique_ptr< clsSound > > >;

	//サウンドクラス作成.
	void CreateSound( 
		SOUND_SET &vpSound,
		std::deque<bool> &dqbLoop,			//ループフラグの数を準備する.
		const std::string &sFilePath,		
		const std::string &sSubPath,
		std::vector<int> &viMaxNum );

	//サウンドデータ作成.
	void CreateSoundData(
		std::vector< clsSound::SOUND_DATA > &vData,
		const std::string &sFilePath,
		const std::string &sSubPath,
		std::vector<int> &viMaxNum );


	//----- 各関数の中身 -----//.
	//再生関数.
	bool Play( 
		const SOUND_SET &vpSound, 
		std::deque<bool> &dqbLoop,
		std::vector<int> &viNum,
		const int No, const bool bNotify ) const;
	//停止関数.
	bool Stop( 
		const SOUND_SET &vpSound, std::deque<bool> &dqbLoop, const int No ) const;
	//音の停止を確認する関数.
	bool IsStopped( const SOUND_SET &vpSound, const int No ) const;
	//音の再生中を確認する関数.
	bool IsPlaying( const SOUND_SET &vpSound, const int No ) const;
	//巻き戻し関数(再生位置初期化).
	bool SeekToStart( const SOUND_SET &vpSound, const int No ) const;


private:

	//サウンドクラス.
	SOUND_SET m_BgmSet;
	SOUND_SET m_SeSet;
	//ループ再生するかのフラグ.
	std::deque<bool> m_dqisLoopBgm;
	std::deque<bool> m_dqisLoopSe;
	//音を同時再生用に何個作る?のフラグ( 起動時のみ ).
	//兼、次に再生するのは何番目かを指すフラグ.
	std::vector<int> m_veciBgmNum;
	std::vector<int> m_veciSeNum;



	HWND m_hWnd;

};

#endif//#ifndef SOUND_MANAGER_BASE_H_