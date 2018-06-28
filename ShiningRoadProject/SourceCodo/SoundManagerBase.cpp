#include "SoundManagerBase.h"
#include "File.h"

#include "OperationString.h"

#include <assert.h>

using namespace std;

const unsigned int uiRESERVE_SIZE_BGM = 16;
const unsigned int uiRESERVE_SIZE_SE = 256;
const unsigned int uiRESURVE_SIZE_MAX = 8;	//同時再生数.

//push_back用.
const clsSound::SOUND_DATA INIT_SOUND_DATA = { "", "", 1000 };
const char cALIAS_NUM = 0;
const char cPATH_NUM = 1;
const char cVOLUME_NUM = 2;
const char cMAX_NUM = 3;

//ファイルクラスのパス用.
const string sDATA_PASS = "Data\\Sound\\Data\\";
const string sBGM_PASS = "BGM.csv";
const string sSE_PASS = "SE.csv";

//サウンドクラスへ指示を出す際にvectorの範囲を超えていたら、だめですとおしかりをくれるマクロ.
#define SOUND_NUMBER_OVER_SHECK(no,vp) \
if( (no) >= static_cast<int>( (vp).size() ) ){\
	assert( !"指定された番号のBGMは存在しません" );\
	return false;\
}
	

//ループフラグの初期化.
const bool bLOOP_INIT = false;


clsSOUND_MANAGER_BASE::clsSOUND_MANAGER_BASE( const HWND hWnd )
	:m_hWnd( hWnd )
	,m_vvupBgm()
	,m_vvupSe()
	,m_dqbLoopBgm()
	,m_dqbLoopSe()
	,m_viBgmNum()
	,m_viSeNum()
	,m_uiRESERVE_SIZE_BGM( uiRESERVE_SIZE_BGM )
	,m_uiRESERVE_SIZE_SE( uiRESERVE_SIZE_SE )	
{
}

clsSOUND_MANAGER_BASE::~clsSOUND_MANAGER_BASE()
{
	StopAllSound();

	//サウンドクラス.
	for( unsigned int i=0; i<m_vvupSe.size(); i++ ){
		for( unsigned int j=0; j<m_vvupSe[i].size(); j++ ){
			if( m_vvupSe[i][j] ){
				m_vvupSe[i][j]->Close();
				m_vvupSe[i][j].reset();
				m_vvupSe[i][j] = nullptr;
			}
		}
		m_vvupSe[i].clear();
		m_vvupSe[i].shrink_to_fit();
	}
	m_vvupSe.clear();
	m_vvupSe.shrink_to_fit();

	for( unsigned int i=0; i<m_vvupBgm.size(); i++ ){
		for( unsigned int j=0; j<m_vvupBgm[i].size(); j++ ){
			if( m_vvupBgm[i][j] ){
				m_vvupBgm[i][j]->Close();
				m_vvupBgm[i][j].reset();
				m_vvupBgm[i][j] = nullptr;
			}
		}
		m_vvupBgm[i].clear();
		m_vvupBgm[i].shrink_to_fit();
	}
	m_vvupBgm.clear();
	m_vvupBgm.shrink_to_fit();

	m_dqbLoopBgm.clear();
	m_dqbLoopBgm.shrink_to_fit();
	m_dqbLoopSe.clear();
	m_dqbLoopSe.shrink_to_fit();

	m_viBgmNum.clear();
	m_viBgmNum.shrink_to_fit();
	m_viSeNum.clear();
	m_viSeNum.shrink_to_fit();



	m_hWnd = nullptr;

}





void clsSOUND_MANAGER_BASE::Create()
{
	//継承クラスで動く関数( m_sSceneNameを入れる ).
	CreateSceneName();

	string tmpPass = sDATA_PASS + m_sSceneName;

	//BGM.
	CreateSound( m_vvupBgm, m_dqbLoopBgm, uiRESERVE_SIZE_BGM, tmpPass + sBGM_PASS, m_viBgmNum );
	//SE.
	CreateSound( m_vvupSe,  m_dqbLoopSe,  uiRESERVE_SIZE_SE,  tmpPass + sSE_PASS,  m_viSeNum );
}

//毎フレーム一回使う.
void clsSOUND_MANAGER_BASE::UpdateLoop()
{
	//ループ再生.
	for( unsigned int i=0; i<m_vvupBgm.size(); i++ ){
		//ループする必要ある?.
		if( !m_dqbLoopBgm[i] ){
			continue;
		}
		//存在する?.
		if( !m_vvupBgm[i][0] ){
			continue;
		}
		//止まっているなら.
		if( IsStoppedBGM( i ) ){
			//もう一度.
			PlayBGM( i, true );//ループする音は次もループできるようにする.
		}		
	}

	for( unsigned int i=0; i<m_vvupSe.size(); i++ ){
		if( !m_dqbLoopSe[i] ){
			continue;
		}
		if( !m_vvupSe[i][0] ){
			continue;
		}
		if( IsStoppedSE( i ) ){
			PlaySE( i, true );
		}		
	}
}

//すべて停止.
void clsSOUND_MANAGER_BASE::StopAllSound()
{
	for( unsigned int i=0; i<m_vvupBgm.size(); i++ ){
		StopBGM( i );
	}

	for( unsigned int i=0; i<m_vvupSe.size(); i++ ){
		StopSE( i );
	}
}


//サウンドクラス作成.
void clsSOUND_MANAGER_BASE::CreateSound( 
	SOUND_SET &vpSound,
	std::deque<bool> &dqbLoop,
	const unsigned int uiRESERVE_SIZE,
	const string sFilePath,
	vector<int> &viMaxNum )
{
	//サウンドデータ.
	vector< clsSound::SOUND_DATA > vData;
	CreateSoundData( vData, sFilePath, viMaxNum );

	//音の数.
	int iSoundNum = vData.size();

	//エイリアス名用.
	clsOPERATION_STRING OprtStr;

	//容量を多めにとる.
	vpSound.reserve( uiRESERVE_SIZE );

	//何行目に書いた音?.
	vpSound.resize( iSoundNum );
	for( int i=0; i<iSoundNum; i++ ){
//		vpSound.push_back( vINIT_VEC_UP_SOUND );
		vpSound[i].reserve( uiRESURVE_SIZE_MAX );
		//何個まで同時再生させる?.
		for( int j=0; j<viMaxNum[i]; j++ ){
			//配列を増やしてnew( make_unique )する.
			vpSound[i].push_back( nullptr );
			vpSound[i][j]= make_unique<clsSound>();

			//エイリアス名に数字をつなげる.
			string tmpAlias = OprtStr.ConsolidatedNumber( vData[i].sAlias.c_str(), j );

			//作成.
			vpSound[i][j]->Open( vData[i].sPath.c_str(), tmpAlias.c_str(), m_hWnd );
			//最大音量設定.
			vpSound[i][j]->SetMaxVolume( vData[i].iMaxVolume );
			//現音量初期化.
			vpSound[i][j]->SetVolume( vData[i].iMaxVolume );
		}
		vpSound[i].shrink_to_fit();
	}
	//余分な分を消す.
	vpSound.shrink_to_fit();
	vData.clear();
	vData.shrink_to_fit();
	//ループフラグ作成.
	dqbLoop.resize( vpSound.size(), bLOOP_INIT );
	dqbLoop.shrink_to_fit();
}

//サウンドデータ作成.
void clsSOUND_MANAGER_BASE::CreateSoundData(
	vector< clsSound::SOUND_DATA > &vData,
	const string &sFilePath,
	std::vector<int> &viMaxNum )
{
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	//開けなかったらスルー.
	if( upFile->Open( sFilePath ) ){
		//とりあえず消す.
		vData.clear();
		viMaxNum.clear();
		//ファイルがとった行数分増やす.
		vData.reserve( upFile->GetSizeRow() );
		viMaxNum.reserve( vData.size() );
		//データの中身を入れる.
		for( unsigned int i=0; i<upFile->GetSizeRow(); i++ ){
			vData.push_back( INIT_SOUND_DATA );

			vData[i].sAlias = upFile->GetDataString( i, cALIAS_NUM );
			vData[i].sPath = upFile->GetDataString( i, cPATH_NUM );
			vData[i].iMaxVolume = upFile->GetDataInt( i, cVOLUME_NUM );
			
			viMaxNum.push_back( upFile->GetDataInt( i, cMAX_NUM ) );
		}
		vData.shrink_to_fit();
		viMaxNum.shrink_to_fit();

	}
	upFile->Close();
	upFile.reset( nullptr );
}




//----- BGM -----//
//再生関数.
bool clsSOUND_MANAGER_BASE::PlayBGM( const int bgmNo, const bool bNotify )
{
	return Play( m_vvupBgm, m_dqbLoopBgm, m_viBgmNum, bgmNo, bNotify );
}
//停止関数.
bool clsSOUND_MANAGER_BASE::StopBGM( const int bgmNo )
{
	return Stop( m_vvupBgm, m_dqbLoopBgm, bgmNo );
}
//音の停止を確認する関数.
bool clsSOUND_MANAGER_BASE::IsStoppedBGM( const int bgmNo ) const
{
	return IsStopped( m_vvupBgm, bgmNo );
}
//音の再生中を確認する関数.
bool clsSOUND_MANAGER_BASE::IsPlayingBGM( const int bgmNo ) const
{
	return IsPlaying( m_vvupBgm, bgmNo );
}
//巻き戻し関数(再生位置初期化).
bool clsSOUND_MANAGER_BASE::SeekToStartBGM( const int bgmNo ) const
{
	return SeekToStart( m_vvupBgm, bgmNo );
}


//----- SE -----//
//再生関数.
bool clsSOUND_MANAGER_BASE::PlaySE( const int seNo, const bool bNotify )
{
	return Play( m_vvupSe, m_dqbLoopSe, m_viSeNum, seNo, bNotify );
}
//停止関数.
bool clsSOUND_MANAGER_BASE::StopSE( const int seNo )
{
	return Stop( m_vvupSe, m_dqbLoopSe, seNo );
}
//音の停止を確認する関数.
bool clsSOUND_MANAGER_BASE::IsStoppedSE( const int seNo ) const
{
	return IsStopped( m_vvupSe, seNo );
}
//音の再生中を確認する関数.
bool clsSOUND_MANAGER_BASE::IsPlayingSE( const int seNo ) const
{
	return IsPlaying( m_vvupSe, seNo );
}
//巻き戻し関数(再生位置初期化).
bool clsSOUND_MANAGER_BASE::SeekToStartSE( const int seNo ) const
{
	return SeekToStart( m_vvupSe, seNo );
}


//----- 各関数の中身 -----//.
//再生関数.
bool clsSOUND_MANAGER_BASE::Play( 
	const SOUND_SET &vpSound, 
	std::deque<bool> &dqbLoop,
	std::vector<int> &viNum,
	const int No, const bool bNotify )
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	dqbLoop[No] = bNotify;
#if 0
	vpSound[No]->SeekToStart();
	return vpSound[No]->Play( bNotify );
#else//#if 0

	//そのNoの音の何番目を鳴らすか?( ここでm_viBgmNum、m_viSeNumの使い方が変わる ).
	viNum[No] ++;
	if( static_cast<unsigned int>( viNum[No] ) >= vpSound[No].size() ){
		viNum[No] = 0;
	}

	vpSound[No][ viNum[No] ]->SeekToStart();

	return vpSound[No][ viNum[No] ]->Play( bNotify );
#endif//#if 0
}

//停止関数.
bool clsSOUND_MANAGER_BASE::Stop( 
	const SOUND_SET &vpSound, std::deque<bool> &dqbLoop, const int No )
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	dqbLoop[No] = bLOOP_INIT;

#if 0
	vpSound[No]->SeekToStart();

	return vpSound[No]->Stop();
#else//#if 0

	for( unsigned int i=0; i<vpSound[No].size(); i++ ){
		//既に止まっているなら何もしない.
		if( vpSound[No][i]->IsStopped() ){
			continue;
		}
		if( !vpSound[No][i]->Stop() ){
			assert( !"止めれませんでした" );
			return false;
		}
	}
	return true;

#endif//#if 0
}

//音の停止を確認する関数.
bool clsSOUND_MANAGER_BASE::IsStopped( const SOUND_SET &vpSound, const int No ) const
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	for( unsigned int i=0; i<vpSound[No].size(); i++ ){
		if( !vpSound[No][i]->IsStopped() ){
			return false;
		}
	}
	return true;
}

//音の再生中を確認する関数.
bool clsSOUND_MANAGER_BASE::IsPlaying( const SOUND_SET &vpSound, const int No ) const
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	for( unsigned int i=0; i<vpSound[No].size(); i++ ){
		if( !vpSound[No][i]->IsPlaying() ){
			return false;
		}
	}
	return true;
}

//巻き戻し関数(再生位置初期化).
bool clsSOUND_MANAGER_BASE::SeekToStart( const SOUND_SET &vpSound, const int No ) const
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	for( unsigned int i=0; i<vpSound[No].size(); i++ ){
		if( !vpSound[No][i]->SeekToStart() ){
			assert( !"戻せなかった" );
			return false;
		}
	}
	return true;
}


