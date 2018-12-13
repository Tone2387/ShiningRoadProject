#include "Sound.h"
#include "MyMacro.h"
#include <stdio.h>
#include <assert.h>


//コンストラクタ.
clsSound::clsSound()
{
//	ZeroMemory( this, sizeof( clsSound ) );
	m_hWnd = nullptr;
	m_iMaxVol = 1000;
	m_bOpend = false;
	ZeroMemory( m_sAlias, sizeof( m_sAlias ) );
}

//デストラクタ.
clsSound::~clsSound()
{
	if( m_bOpend ){
		Stop();
		Close();
		m_bOpend = false;
	}

	m_hWnd = nullptr;
}


//音声ファイルを開く.
bool clsSound::Open( const LPCTSTR sFName, const char* const sAlias, const HWND hWnd )
{
	if( m_bOpend ){
		assert( !"もう開いています" );
		return true;
	}

	m_bOpend = true;

	//初期設定.
	SetInitParam( sAlias, hWnd );

	//コマンド.
	char command[STR_BUFF_MAX] = "open Data\\Sound\\";//面倒なパスを引数から省略.

	strcat_s( command, sizeof( command ), sFName );		//ファイル名を結合.
	strcat_s( command, sizeof( command ), " type mpegvideo alias " );//「 type mpegvideo 」: こいつを入れることで、wavをmp3と同じように開く(扱える)ので、音量の変化を受け付けるようになる.
	strcat_s( command, sizeof( command ), m_sAlias );	//エイリアスを結合 : エイリアス(ニックネームのようなもの).

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}

	ERR_MSG( m_sAlias, sFName );//エイリアス名がかぶったり.
	m_bOpend = false;
	return false;
}

//音声ファイルを閉じる.
bool clsSound::Close()
{
	if( !m_bOpend ) return false;

	//コマンド.
	char command[STR_BUFF_MAX] = "close ";
	strcat_s( command, sizeof( command ), m_sAlias );	//エイリアスを結合.
														//エイリアス(ニックネームのようなもの).
	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
//		ERR_MSG( m_sAlias, "閉じるの成功" );
		m_bOpend = false;
		m_hWnd = nullptr;
		return true;
	}
	ERR_MSG( m_sAlias, "閉じるの失敗" );

	return false;
}

//再生関数.
bool clsSound::Play( const bool bNotify ) const
{
	if( !m_bOpend ) return false;

	//コマンド.
	char command[STR_BUFF_MAX] = "play ";
	strcat_s( command, sizeof( command ), m_sAlias );	//エイリアスを結合.

	//	ループする場合メッセージを送る.
	if( bNotify ){
		strcat_s( command, sizeof( command ), " notify" );
		//notify: 処理が終了したときにhWndにMM_MCINOTIFYメッセージをポストする.
	}

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//停止関数.
bool clsSound::Stop() const
{
	if( !m_bOpend ) return false;

	//コマンド.
	char command[STR_BUFF_MAX] = "stop ";
	strcat_s( command, sizeof( command ), m_sAlias );	//エイリアスを結合.
													//エイリアス(ニックネームのようなもの).
	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		SeekToStart();
		return true;
	}

//	assert( !"止めれませんでした" );
	ERR_MSG( m_sAlias, "停止の失敗" );
	return false;
}

////一時停止.
//bool clsSound::Pause()
//{
//	//コマンド.
//	char command[STR_BUFF_MAX] = "pause ";
//	strcat_s( command, sizeof( command ), m_sAlias );	//エイリアスを結合.
//													//エイリアス(ニックネームのようなもの).
//	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
//		SeekToStart();
//		return true;
//	}
//	return false;
//}
//
////一時停止の解除.
//bool clsSound::Resume()
//{
//	//コマンド.
//	char command[STR_BUFF_MAX] = "resume ";
//	strcat_s( command, sizeof( command ), m_sAlias );	//エイリアスを結合.
//													//エイリアス(ニックネームのようなもの).
//	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
//		SeekToStart();
//		return true;
//	}
//	return false;
//}



//巻き戻し関数(再生位置初期化).
bool clsSound::SeekToStart() const
{
	//コマンド.
	char command[STR_BUFF_MAX] = "seek ";

	strcat_s( command, sizeof( command ), m_sAlias );	//エイリアスを結合.
	strcat_s( command, sizeof( command ), " to start" );

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//状態取得関数.
//	sStatus の配列は256以下にすること.
//※ただし、状態を取得する場合は、再生時に「notify」を設定し、.
//	ウィンドウハンドルのメッセージを送っておく必要がある.
bool clsSound::GetStatus( char* const sStatus ) const
{
	//コマンド.
	char command[STR_BUFF_MAX] = "status ";
	strcat_s( command, sizeof( command ), m_sAlias );	//エイリアスを結合.
	strcat_s( command, sizeof( command ), " mode" );

	if( mciSendString( command, sStatus, ( STR_BUFF_MAX - 1 ), m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//音の停止を確認する関数.
bool clsSound::IsStopped() const
{
	char sStatus[STR_BUFF_MAX] = "";

	//状態取得.
	if( GetStatus( sStatus ) ){
		//文字列比較.
		if( strcmp( sStatus, "stopped" ) == 0 ){
			return true;
		}
	}
	return false;
}

//音の再生中を確認する関数.
bool clsSound::IsPlaying() const
{
	char sStatus[STR_BUFF_MAX] = "";

	//状態取得.
	if( GetStatus( sStatus ) ){
		//文字列比較.
		if( strcmp( sStatus, "playing" ) == 0 ){
			return true;
		}
	}
	return false;
}

//============================================================
//	音量を設定する.
//============================================================
bool clsSound::SetVolume( int ivolume ) const
{
	//数値の丸め.
	if( ivolume > 1000 )	{ ivolume = 1000; }
	else if( ivolume < 0 )	{ ivolume = 0	; }
	
	//音量を文字列に変換.
	char sVolume[STR_BUFF_MAX] = "";
	sprintf_s( sVolume, sizeof( sVolume ), "%d", ivolume );

	//コマンド.
	char command[STR_BUFF_MAX] = "setaudio ";

	strcat_s( command, sizeof( command ), m_sAlias );	//エイリアスを結合.
	strcat_s( command, sizeof( command ), " volume to " );
	strcat_s( command, sizeof( command ), sVolume );//音量を結合.

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}



//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


//============================================================
//初期設定.
//============================================================
HRESULT clsSound::SetInitParam( const char *sAlias, const HWND hWnd )
{
	if( m_hWnd != nullptr ){
		return E_FAIL;
	}

	//ウィンドウハンドルウィンドウハンドルを渡す.
	m_hWnd = hWnd;

	//エイリアス.
	strcpy_s( m_sAlias, sizeof( m_sAlias ), sAlias );
	

	return S_OK;
}
