#include "SoundManagerBase.h"

using namespace std;

const unsigned int uiRESERVE_SIZE_BGM = 16;
const unsigned int uiRESERVE_SIZE_SE = 256;

//BGM.
const clsSound::SOUND_DATA BGM_DATA[] =
{
	//タイトル.
	{ "Title", "BGM\\100Title\\100Title.mp3", 350 },
	//アセンブル.
	{ "Assemble", "BGM\\200Main\\010Stage.mp3", 250 },
	//ミッション.
	{ "Mission", "BGM\\300Result\\200ResultJingle.mp3", 350 },
	//クリア.
	{ "Clear", "BGM\\400Clear\\100EndRoll.mp3", 200 },
	//ゲームオーバー.
	{ "GameOver", "BGM\\500Over\\100Bye.mp3", 300 },
};

//SE.
const clsSound::SOUND_DATA SE_DATA[] =
{
	//明るいやつ.
	{ "Enter", "SE\\700Button\\100Enter.wav", 300 },
	//暗いやつ.
	{ "Exit", "SE\\700Button\\200Exit.wav", 300 },
};





clsSOUND_MANAGER::clsSOUND_MANAGER( const HWND hWnd )
	:m_hWnd( hWnd )
	,m_vupBgm()
	,m_vupSe()
{

	//BGM作成.
	CreateSound();

}

clsSOUND_MANAGER::~clsSOUND_MANAGER()
{
	StopAllSound();

	for( unsigned int i=0; i<m_vupSe.size(); i++ ){
		if( m_vupSe[i] ){
			m_vupSe[i]->Close();
			m_vupSe[i].reset();
			m_vupSe[i] = nullptr;
		}
	}
	m_vupSe.clear();
	m_vupSe.shrink_to_fit();

	for( unsigned int i=0; i<m_vupBgm.size(); i++ ){
		if( m_vupBgm[i] ){
			m_vupBgm[i]->Close();
			m_vupBgm[i].reset();
			m_vupBgm[i] = nullptr;
		}
	}
	m_vupBgm.clear();
	m_vupBgm.shrink_to_fit();

	m_hWnd = nullptr;
}

//すべて停止.
void clsSOUND_MANAGER::StopAllSound()
{
	for( unsigned int i=0; i<m_vupBgm.size(); i++ ){
		if( m_vupBgm[i] ){
			m_vupBgm[i]->Stop();
		}
	}

	for( unsigned int i=0; i<m_vupSe.size(); i++ ){
		if( m_vupSe[i] ){
			m_vupSe[i]->Stop();
		}
	}
}




void clsSOUND_MANAGER::CreateSound()
{
	//BGM.
	{
		//音の数.
		int iBgmNum = sizeof( BGM_DATA ) / sizeof( BGM_DATA[0] );

		//容量を多めにとる.
		m_vupBgm.reserve( uiRESERVE_SIZE_BGM );

		for( int i=0; i<iBgmNum; i++ ){
			//配列を増やしてnew( make_unique )する.
			m_vupBgm.push_back( nullptr );
			m_vupBgm[i] = make_unique<clsSound>();

			//名前.
			char cAliasName[STR_BUFF_MAX] = "";
			strcat_s( cAliasName, sizeof( cAliasName ), BGM_DATA[i].sAlias );
			//作成.
			m_vupBgm[i]->Open( BGM_DATA[i].sPath, cAliasName, m_hWnd );
			//最大音量設定.
			m_vupBgm[i]->SetMaxVolume( BGM_DATA[i].iMaxVolume );
			//現音量初期化.
			m_vupBgm[i]->SetVolume( BGM_DATA[i].iMaxVolume );
		}
		//余分な分を消す.
		m_vupBgm.shrink_to_fit();
	}


	//SE.
	{
		//音の数.
		int iSeNum = sizeof( SE_DATA ) / sizeof( SE_DATA[0] );

		//容量を多めにとる.
		m_vupSe.reserve( uiRESERVE_SIZE_SE );

		for( int i=0; i<iSeNum; i++ ){
			m_vupSe.push_back( nullptr );
			m_vupSe[i] = make_unique<clsSound>();
			//名前.
			char cAliasName[STR_BUFF_MAX] = "";
			strcat_s( cAliasName, sizeof( cAliasName ), SE_DATA[i].sAlias );
			//作成.
			m_vupSe[i]->Open( SE_DATA[i].sPath, cAliasName, m_hWnd );
			//最大音量設定.
			m_vupSe[i]->SetMaxVolume( SE_DATA[i].iMaxVolume );
			//現音量初期化.
			m_vupSe[i]->SetVolume( SE_DATA[i].iMaxVolume );
		}
		//余分な分を消す.
		m_vupSe.shrink_to_fit();
	}
}







//----- BGM -----//
//再生関数.
bool clsSOUND_MANAGER::PlayBGM( const int bgmNo, const bool bNotify ) const
{
	return m_vupBgm[bgmNo]->Play( bNotify );
}
//停止関数.
bool clsSOUND_MANAGER::StopBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->Stop();
}
//音の停止を確認する関数.
bool clsSOUND_MANAGER::IsStoppedBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->IsStopped();
}
//音の再生中を確認する関数.
bool clsSOUND_MANAGER::IsPlayingBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->IsPlaying();
}
//巻き戻し関数(再生位置初期化).
bool clsSOUND_MANAGER::SeekToStartBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->SeekToStart();
}


//----- SE -----//
//再生関数.
bool clsSOUND_MANAGER::PlaySE( const int seNo, const bool bNotify ) const
{
	return m_vupSe[seNo]->Play( bNotify );
}
//停止関数.
bool clsSOUND_MANAGER::StopSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}
//音の停止を確認する関数.
bool clsSOUND_MANAGER::IsStoppedSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}
//音の再生中を確認する関数.
bool clsSOUND_MANAGER::IsPlayingSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}
//巻き戻し関数(再生位置初期化).
bool clsSOUND_MANAGER::SeekToStartSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}




