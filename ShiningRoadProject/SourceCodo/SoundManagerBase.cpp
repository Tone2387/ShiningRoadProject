#include "SoundManagerBase.h"
#include "File.h"

#include "OperationString.h"

#include <assert.h>

using namespace std;

//サウンドクラスへ指示を出す際にvectorの範囲を超えていたら、だめですとおしかりをくれるマクロ.
#define SOUND_NUMBER_OVER_SHECK(no,vp) \
if( (no) >= static_cast<int>( (vp).size() ) ){\
	assert( !"指定された番号のサウンドクラスは存在しません" );\
	return false;\
}

namespace{

	//添え字.
	const char cALIAS_NUM = 0;	//エイリアス名.
	const char cPATH_NUM = 1;	//音声データのファイルパス.
	const char cVOLUME_NUM = 2;	//最大音量.
	const char cMAX_NUM = 3;	//同時再生可能数.

	//ファイルクラスのパス用.
	const string sDATA_PASS = "Data\\Sound\\Data\\";
	const string sBGM_PASS = "BGM.csv";
	const string sSE_PASS = "SE.csv";
	const string sSUB_PASS_BGM = "BGM\\";
	const string sSUB_PASS_SE = "SE\\";

	//ループフラグの初期化.
	const bool bLOOP_INIT = false;

	//エイリアスと番号の間,番号の後に挿入する.
	const char sCONSOLIDATE[] = "cons";
}


clsSOUND_MANAGER_BASE::clsSOUND_MANAGER_BASE( const HWND hWnd )
	:m_hWnd( hWnd )
	,m_BgmSet()
	,m_SeSet()
	,m_dqisLoopBgm()
	,m_dqisLoopSe()
	,m_veciBgmNum()
	,m_veciSeNum()
{
}

clsSOUND_MANAGER_BASE::~clsSOUND_MANAGER_BASE()
{
	StopAllSound();

	//サウンドクラス.
	for( unsigned int i=0; i<m_SeSet.size(); i++ ){
		for( unsigned int j=0; j<m_SeSet[i].size(); j++ ){
			if( m_SeSet[i][j] ){
//				m_SeSet[i][j]->Close();
				m_SeSet[i][j].reset();
				m_SeSet[i][j] = nullptr;
			}
		}
		m_SeSet[i].clear();
		m_SeSet[i].shrink_to_fit();
	}
	m_SeSet.clear();
	m_SeSet.shrink_to_fit();

	for( unsigned int i=0; i<m_BgmSet.size(); i++ ){
		for( unsigned int j=0; j<m_BgmSet[i].size(); j++ ){
			if( m_BgmSet[i][j] ){
//				m_BgmSet[i][j]->Close();
				m_BgmSet[i][j].reset();
				m_BgmSet[i][j] = nullptr;
			}
		}
		m_BgmSet[i].clear();
		m_BgmSet[i].shrink_to_fit();
	}
	m_BgmSet.clear();
	m_BgmSet.shrink_to_fit();

	m_dqisLoopBgm.clear();
	m_dqisLoopBgm.shrink_to_fit();
	m_dqisLoopSe.clear();
	m_dqisLoopSe.shrink_to_fit();

	m_veciBgmNum.clear();
	m_veciBgmNum.shrink_to_fit();
	m_veciSeNum.clear();
	m_veciSeNum.shrink_to_fit();



	m_hWnd = nullptr;

}





void clsSOUND_MANAGER_BASE::Create()
{
	//継承クラスで動く関数( m_sSceneNameを入れる ).
	CreateSceneName();

	//CSVデータのパス.
	string tmpDataPass = sDATA_PASS + m_sSceneName;
	string BgmDataPath = tmpDataPass + sBGM_PASS;
	string SeDataPath  = tmpDataPass + sSE_PASS;

	//BGM.
	CreateSound( m_BgmSet, m_dqisLoopBgm, BgmDataPath, sSUB_PASS_BGM, m_veciBgmNum );
	//SE.
	CreateSound( m_SeSet,  m_dqisLoopSe,  SeDataPath,  sSUB_PASS_SE,  m_veciSeNum );
}

//毎フレーム一回使う.
void clsSOUND_MANAGER_BASE::UpdateLoop()
{
	//ループ再生.
	for( unsigned int i=0; i<m_BgmSet.size(); i++ ){
		//ループする必要ある?.
		if( !m_dqisLoopBgm[i] ){
			continue;
		}
		//存在する?.
		if( !m_BgmSet[i][0] ){
			continue;
		}
		//止まっているなら.
		if( IsStoppedBGM( i ) ){
			//もう一度.
			PlayBGM( i, true );//ループする音は次もループできるようにする.
		}		
	}

	for( unsigned int i=0; i<m_SeSet.size(); i++ ){
		if( !m_dqisLoopSe[i] ){
			continue;
		}
		if( !m_SeSet[i][0] ){
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
	for( unsigned int i=0; i<m_BgmSet.size(); i++ ){
		StopBGM( i );
	}

	for( unsigned int i=0; i<m_SeSet.size(); i++ ){
		StopSE( i );
	}
}


//サウンドクラス作成.
void clsSOUND_MANAGER_BASE::CreateSound( 
	SOUND_SET &vpSound,
	std::deque<bool> &dqbLoop,
	const string &sFilePath,
	const string &sSubPath,
	vector<int> &viMaxNum )
{
	//サウンドデータ.
	vector< clsSound::SOUND_DATA > vData;
	CreateSoundData( vData, sFilePath, sSubPath, viMaxNum );

	//音の数.
	int iSoundNum = vData.size();

	//エイリアス名用.
	clsOPERATION_STRING OprtStr;

	//何行目に書いた音?.
	vpSound.resize( iSoundNum );
	for( unsigned int i=0; i<vpSound.size(); i++ )
	{
		vpSound[i].resize( viMaxNum[i] );
		//何個まで同時再生させる?.
		for( unsigned int j=0; j<vpSound[i].size(); j++ ){
			vpSound[i][j]= make_unique<clsSound>();
			
			//エイリアスと番号の間に挿入する.
			vData[i].sAlias += sCONSOLIDATE;
			//エイリアス名に数字をつなげる.
			string tmpAlias = OprtStr.ConsolidatedNumber( vData[i].sAlias.c_str(), j );
			//番号の後に挿入する.
			tmpAlias += sCONSOLIDATE;

			//作成.
			assert( vpSound[i][j]->Open( vData[i].sPath.c_str(), tmpAlias.c_str(), m_hWnd ) );
//			vpSound[i][j]->Open( vData[i].sPath.c_str(), tmpAlias.c_str(), m_hWnd );
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
	const string &sSubPath,
	std::vector<int> &viMaxNum )
{
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	//開けなかったらスルー.
	if( upFile->Open( sFilePath ) ){
		//とりあえず消す.
		vData.clear();
		viMaxNum.clear();
		//ファイルがとった行数分増やす.
		vData.resize( upFile->GetSizeRow() );
		viMaxNum.resize( vData.size() );
		//データの中身を入れる.
		for( unsigned int i=0; i<upFile->GetSizeRow(); i++ ){
			vData[i].sAlias = upFile->GetDataString( i, cALIAS_NUM );
			vData[i].sPath = sSubPath + upFile->GetDataString( i, cPATH_NUM );
			vData[i].iMaxVolume = upFile->GetDataInt( i, cVOLUME_NUM );
			
			viMaxNum[i] = upFile->GetDataInt( i, cMAX_NUM );
		}
	}
	upFile->Close();
	upFile.reset( nullptr );
}




////----- BGM -----//
////再生関数.
//bool clsSOUND_MANAGER_BASE::PlayBGM( const int bgmNo, const bool bNotify )
//{
//	return Play( m_BgmSet, m_dqisLoopBgm, m_veciBgmNum, bgmNo, bNotify );
//}
////停止関数.
//bool clsSOUND_MANAGER_BASE::StopBGM( const int bgmNo )
//{
//	return Stop( m_BgmSet, m_dqisLoopBgm, bgmNo );
//}
////音の停止を確認する関数.
//bool clsSOUND_MANAGER_BASE::IsStoppedBGM( const int bgmNo ) const
//{
//	return IsStopped( m_BgmSet, bgmNo );
//}
////音の再生中を確認する関数.
//bool clsSOUND_MANAGER_BASE::IsPlayingBGM( const int bgmNo ) const
//{
//	return IsPlaying( m_BgmSet, bgmNo );
//}
////巻き戻し関数(再生位置初期化).
//bool clsSOUND_MANAGER_BASE::SeekToStartBGM( const int bgmNo ) const
//{
//	return SeekToStart( m_BgmSet, bgmNo );
//}


////----- SE -----//
////再生関数.
//bool clsSOUND_MANAGER_BASE::PlaySE( const int seNo, const bool bNotify )
//{
//	return Play( m_SeSet, m_dqisLoopSe, m_veciSeNum, seNo, bNotify );
//}
////停止関数.
//bool clsSOUND_MANAGER_BASE::StopSE( const int seNo )
//{
//	return Stop( m_SeSet, m_dqisLoopSe, seNo );
//}
////音の停止を確認する関数.
//bool clsSOUND_MANAGER_BASE::IsStoppedSE( const int seNo ) const
//{
//	return IsStopped( m_SeSet, seNo );
//}
////音の再生中を確認する関数.
//bool clsSOUND_MANAGER_BASE::IsPlayingSE( const int seNo ) const
//{
//	return IsPlaying( m_SeSet, seNo );
//}
////巻き戻し関数(再生位置初期化).
//bool clsSOUND_MANAGER_BASE::SeekToStartSE( const int seNo ) const
//{
//	return SeekToStart( m_SeSet, seNo );
//}


//----- 各関数の中身 -----//.
//再生関数.
bool clsSOUND_MANAGER_BASE::Play( 
	const SOUND_SET &vpSound, 
	std::deque<bool> &dqbLoop,
	std::vector<int> &viNum,
	const int No, 
	const bool bNotify ) const
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	dqbLoop[No] = bNotify;
#if 0
	vpSound[No]->SeekToStart();
	return vpSound[No]->Play( bNotify );
#else//#if 0

	//存在しない番が来たら末尾の音を鳴らす.
	unsigned int uiNo = static_cast<int>( No );
	if( uiNo >= vpSound.size() ){
		uiNo =  vpSound.size() - 1;
//		assert( !"ない音を選ばないで" );
	}


	//そのNoの音の何番目を鳴らすか?( ここでm_veciBgmNum、m_veciSeNumの使い方が変わる ).
	viNum[ uiNo ] ++;
	if( static_cast<unsigned int>( viNum[ uiNo ] ) >= vpSound[ uiNo ].size() ){
		viNum[ uiNo ] = 0;
	}

	vpSound[ uiNo ][ viNum[ uiNo ] ]->SeekToStart();

	return vpSound[uiNo][viNum[uiNo]]->Play(bNotify);
#endif//#if 0
}

//停止関数.
bool clsSOUND_MANAGER_BASE::Stop( 
	const SOUND_SET &vpSound, std::deque<bool> &dqbLoop, const int No ) const
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
//			assert( !"止めれませんでした" );
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


