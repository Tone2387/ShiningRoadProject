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

//push_back用.
const clsSound::SOUND_DATA INIT_SOUND_DATA = { "", "", 1000 };
const char cALIAS_NUM = 0;
const char cPATH_NUM = 1;
const char cVOLUME_NUM = 2;

//ファイルクラスのパス用.
const string sDATA_PASS = "Data\\Sound\\Data\\";
const string sBGM_PASS = "BGM.csv";
const string sSE_PASS = "SE.csv";


clsSOUND_MANAGER_BASE::clsSOUND_MANAGER_BASE( const HWND hWnd )
	:m_hWnd( hWnd )
	,m_vupBgm()
	,m_vupSe()
	,m_uiRESERVE_SIZE_BGM( uiRESERVE_SIZE_BGM )
	,m_uiRESERVE_SIZE_SE( uiRESERVE_SIZE_SE )	
{
}

clsSOUND_MANAGER_BASE::~clsSOUND_MANAGER_BASE()
{
	StopAllSound();

	//サウンドクラス.
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
void clsSOUND_MANAGER_BASE::StopAllSound()
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




void clsSOUND_MANAGER_BASE::Create()
{
	//継承クラスで動く関数( m_sSceneNameを入れる ).
	CreateSceneName();

	string tmpPass = sDATA_PASS + m_sSceneName;

	//BGM.
	CreateSound( m_vupBgm, tmpPass + sBGM_PASS );
	//SE.
	CreateSound( m_vupSe, tmpPass + sSE_PASS );
}



//サウンドクラス作成.
void clsSOUND_MANAGER_BASE::CreateSound( 
	vector< unique_ptr< clsSound > > &vpSound,
	const std::string sFilePath )
{
	//サウンドデータ.
	std::vector< clsSound::SOUND_DATA > vData;
	CreateSoundData( vData, sFilePath );

	//音の数.
	int iSoundNum = vData.size();

	//容量を多めにとる.
	vpSound.reserve( uiRESERVE_SIZE_BGM );

	for( int i=0; i<iSoundNum; i++ ){
		//配列を増やしてnew( make_unique )する.
		vpSound.push_back( nullptr );
		vpSound[i] = make_unique<clsSound>();

		//名前.
		char cAliasName[STR_BUFF_MAX] = "";
		strcat_s( cAliasName, sizeof( cAliasName ), vData[i].sAlias.c_str() );
		//作成.
		vpSound[i]->Open( vData[i].sPath.c_str(), cAliasName, m_hWnd );
		//最大音量設定.
		vpSound[i]->SetMaxVolume( vData[i].iMaxVolume );
		//現音量初期化.
		vpSound[i]->SetVolume( vData[i].iMaxVolume );
	}
	//余分な分を消す.
	vpSound.shrink_to_fit();
	vData.clear();
	vData.shrink_to_fit();
}

//サウンドデータ作成.
void clsSOUND_MANAGER_BASE::CreateSoundData(
	std::vector< clsSound::SOUND_DATA > &vData,
	const std::string sFilePath )
{
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	//開けなかったらスルー.
	if( upFile->Open( sFilePath ) ){
		//とりあえず消す.
		vData.clear();
		//ファイルがとった行数分増やす.
		vData.reserve( upFile->GetSizeRow() );
		for( unsigned int i=0; i<upFile->GetSizeRow(); i++ ){
			vData.push_back( INIT_SOUND_DATA );

			vData[i].sAlias = upFile->GetDataString( i, cALIAS_NUM );
			vData[i].sPath = upFile->GetDataString( i, cPATH_NUM );
			vData[i].iMaxVolume = upFile->GetDataInt( i, cVOLUME_NUM );
		}
		vData.shrink_to_fit();

	}
	upFile->Close();
	upFile.reset( nullptr );
}




//----- BGM -----//
//再生関数.
bool clsSOUND_MANAGER_BASE::PlayBGM( const int bgmNo, const bool bNotify ) const
{
	return m_vupBgm[bgmNo]->Play( bNotify );
}
//停止関数.
bool clsSOUND_MANAGER_BASE::StopBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->Stop();
}
//音の停止を確認する関数.
bool clsSOUND_MANAGER_BASE::IsStoppedBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->IsStopped();
}
//音の再生中を確認する関数.
bool clsSOUND_MANAGER_BASE::IsPlayingBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->IsPlaying();
}
//巻き戻し関数(再生位置初期化).
bool clsSOUND_MANAGER_BASE::SeekToStartBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->SeekToStart();
}


//----- SE -----//
//再生関数.
bool clsSOUND_MANAGER_BASE::PlaySE( const int seNo, const bool bNotify ) const
{
	return m_vupSe[seNo]->Play( bNotify );
}
//停止関数.
bool clsSOUND_MANAGER_BASE::StopSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}
//音の停止を確認する関数.
bool clsSOUND_MANAGER_BASE::IsStoppedSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}
//音の再生中を確認する関数.
bool clsSOUND_MANAGER_BASE::IsPlayingSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}
//巻き戻し関数(再生位置初期化).
bool clsSOUND_MANAGER_BASE::SeekToStartSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}




