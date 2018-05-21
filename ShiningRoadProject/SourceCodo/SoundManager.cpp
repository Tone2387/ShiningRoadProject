#include "SoundManager.h"


//BGM.
//タイトル.
#define ALIAS_NAME_BGM_TITLE "Title"
#define  FILE_PATH_BGM_TITLE "BGM\\100Title\\100Title.mp3"
const int	  iVOL_BGM_TITLE = 350;

//メイン.
#define ALIAS_NAME_BGM_MAIN "Main"
#define  FILE_PATH_BGM_MAIN "BGM\\200Main\\010Stage.mp3"
const int	  iVOL_BGM_MAIN = 250;

//リザルト.
#define ALIAS_NAME_BGM_RESULT "Result"
#define  FILE_PATH_BGM_RESULT "BGM\\300Result\\200ResultJingle.mp3"
const int	  iVOL_BGM_RESULT = 750;

//クリア.
#define ALIAS_NAME_BGM_CLEAR "Clear"
#define  FILE_PATH_BGM_CLEAR "BGM\\400Clear\\100EndRoll.mp3"
const int	  iVOL_BGM_CLEAR = 200;

//オーバー.
#define ALIAS_NAME_BGM_OVER "Over"
#define  FILE_PATH_BGM_OVER "BGM\\500Over\\100Bye.mp3"
const int	  iVOL_BGM_OVER = 300;


//SE.
//明るいやつ.
#define ALIAS_NAME_SE_ENTER "Enter"
#define  FILE_PATH_SE_ENTER "SE\\700Button\\100Enter.wav"
const int	  iVOL_SE_ENTER = 300;

//暗いやつ.
#define ALIAS_NAME_SE_EXIT "Exit"
#define  FILE_PATH_SE_EXIT "SE\\700Button\\200Exit.wav"
const int	  iVOL_SE_EXIT = 300;






clsSOUND_MANAGER::clsSOUND_MANAGER( const HWND hWnd ) : 
	m_hWnd( hWnd )
{
	for( UCHAR i=0; i<enBGM_MAX; i++){
		m_pBgm[i] = nullptr;
	}
	for( UCHAR i=0; i<enSE_MAX; i++){
		m_pSe[i] = nullptr;
	}

	//BGM作成.
	CreateSound();

}

clsSOUND_MANAGER::~clsSOUND_MANAGER()
{
	for( UCHAR i=0; i<enSE_MAX; i++){
		if( m_pSe[i] != nullptr ){
			m_pSe[i]->Stop();
			m_pSe[i]->Close();
			delete m_pSe[i];
			m_pSe[i] = nullptr;
		}
	}
	for( UCHAR i=0; i<enBGM_MAX; i++){
		if( m_pBgm[i] != nullptr ){
			m_pBgm[i]->Stop();
			m_pBgm[i]->Close();
			delete m_pBgm[i];
			m_pBgm[i] = nullptr;
		}
	}

	m_hWnd = nullptr;
}





void clsSOUND_MANAGER::CreateSound()
{
	//BGM.
	{
		//サウンド構造体.
		clsSound::SOUND_DATA tmpSData[enBGM_MAX] =
		{
//			{ ALIAS_NAME_BGM_START_UP,	FILE_PATH_BGM_START_UP, iVOL_BGM_START_UP	},
			{ ALIAS_NAME_BGM_TITLE,		FILE_PATH_BGM_TITLE,	iVOL_BGM_TITLE		},
			{ ALIAS_NAME_BGM_MAIN,		FILE_PATH_BGM_MAIN,		iVOL_BGM_MAIN		},
			{ ALIAS_NAME_BGM_RESULT,	FILE_PATH_BGM_RESULT,	iVOL_BGM_RESULT		},
			{ ALIAS_NAME_BGM_CLEAR,		FILE_PATH_BGM_CLEAR,	iVOL_BGM_CLEAR		},
			{ ALIAS_NAME_BGM_OVER,		FILE_PATH_BGM_OVER,		iVOL_BGM_OVER		},
		};
		for( int i=0; i<enBGM_MAX; i++ ){
			m_pBgm[i] = new clsSound;
			//名前.
			char cAliasName[STR_BUFF_MAX] = "";
			strcat_s( cAliasName, sizeof( cAliasName ), tmpSData[i].sAlias );
			//作成.
			m_pBgm[i]->Open( tmpSData[i].sPath, cAliasName, m_hWnd );
	//		//最大音量設定.
	//		m_pBgm[i]->SetMaxVolume( tmpSData[i].iMaxVolume );
			//現音量初期化.
			m_pBgm[i]->SetVolume( tmpSData[i].iMaxVolume );
		}
	}


	//SE.
	{
		clsSound::SOUND_DATA tmpSData[enSE_MAX] =
		{
			{ ALIAS_NAME_SE_ENTER,	FILE_PATH_SE_ENTER, iVOL_SE_ENTER	},
			{ ALIAS_NAME_SE_EXIT,	FILE_PATH_SE_EXIT,	iVOL_SE_EXIT	},
		};
		for( int i=0; i<enSE_MAX; i++ ){
			m_pSe[i] = new clsSound;
			//名前.
			char cAliasName[STR_BUFF_MAX] = "";
			strcat_s( cAliasName, sizeof( cAliasName ), tmpSData[i].sAlias );
			//作成.
			m_pSe[i]->Open( tmpSData[i].sPath, cAliasName, m_hWnd );
	//		//最大音量設定.
	//		m_pBgm[i]->SetMaxVolume( tmpSData[i].iMaxVolume );
			//現音量初期化.
			m_pSe[i]->SetVolume( tmpSData[i].iMaxVolume );
		}
	}
}







//----- BGM -----//
//再生関数.
bool clsSOUND_MANAGER::PlayBGM( const enBgm bgmNo, const bool bNotify ) const
{
	return m_pBgm[bgmNo]->Play( bNotify );
}
//停止関数.
bool clsSOUND_MANAGER::StopBGM( const enBgm bgmNo ) const
{
	return m_pBgm[bgmNo]->Stop();
}
//音の停止を確認する関数.
bool clsSOUND_MANAGER::IsStoppedBGM( const enBgm bgmNo ) const
{
	return m_pBgm[bgmNo]->IsStopped();
}
//音の再生中を確認する関数.
bool clsSOUND_MANAGER::IsPlayingBGM( const enBgm bgmNo ) const
{
	return m_pBgm[bgmNo]->IsPlaying();
}
//巻き戻し関数(再生位置初期化).
bool clsSOUND_MANAGER::SeekToStartBGM( const enBgm bgmNo ) const
{
	return m_pBgm[bgmNo]->SeekToStart();
}


//----- SE -----//
//再生関数.
bool clsSOUND_MANAGER::PlaySE( const enSE seNo, const bool bNotify ) const
{
	return m_pSe[seNo]->Play( bNotify );
}
//停止関数.
bool clsSOUND_MANAGER::StopSE( const enSE seNo ) const
{
	return m_pSe[seNo]->Stop();
}
//音の停止を確認する関数.
bool clsSOUND_MANAGER::IsStoppedSE( const enSE seNo ) const
{
	return m_pSe[seNo]->Stop();
}
//音の再生中を確認する関数.
bool clsSOUND_MANAGER::IsPlayingSE( const enSE seNo ) const
{
	return m_pSe[seNo]->Stop();
}
//巻き戻し関数(再生位置初期化).
bool clsSOUND_MANAGER::SeekToStartSE( const enSE seNo ) const
{
	return m_pSe[seNo]->Stop();
}




