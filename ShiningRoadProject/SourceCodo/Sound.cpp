#include "Sound.h"
#include "MyMacro.h"
#include <stdio.h>
#include <assert.h>


//�R���X�g���N�^.
clsSound::clsSound()
{
//	ZeroMemory( this, sizeof( clsSound ) );
	m_hWnd = nullptr;
	m_iMaxVol = 1000;
	m_bOpend = false;
	ZeroMemory( m_sAlias, sizeof( m_sAlias ) );
}

//�f�X�g���N�^.
clsSound::~clsSound()
{
	if( m_bOpend ){
		Stop();
		Close();
		m_bOpend = false;
	}

	m_hWnd = nullptr;
}


//�����t�@�C�����J��.
bool clsSound::Open( const LPCTSTR sFName, const char* const sAlias, const HWND hWnd )
{
	if( m_bOpend ){
		assert( !"�����J���Ă��܂�" );
		return true;
	}

	m_bOpend = true;

	//�����ݒ�.
	SetInitParam( sAlias, hWnd );

	//�R�}���h.
	char command[STR_BUFF_MAX] = "open Data\\Sound\\";//�ʓ|�ȃp�X����������ȗ�.

	strcat_s( command, sizeof( command ), sFName );		//�t�@�C����������.
	strcat_s( command, sizeof( command ), " type mpegvideo alias " );//�u type mpegvideo �v: ���������邱�ƂŁAwav��mp3�Ɠ����悤�ɊJ��(������)�̂ŁA���ʂ̕ω����󂯕t����悤�ɂȂ�.
	strcat_s( command, sizeof( command ), m_sAlias );	//�G�C���A�X������ : �G�C���A�X(�j�b�N�l�[���̂悤�Ȃ���).

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}

	ERR_MSG( m_sAlias, sFName );//�G�C���A�X�������Ԃ�����.
	m_bOpend = false;
	return false;
}

//�����t�@�C�������.
bool clsSound::Close()
{
	if( !m_bOpend ) return false;

	//�R�}���h.
	char command[STR_BUFF_MAX] = "close ";
	strcat_s( command, sizeof( command ), m_sAlias );	//�G�C���A�X������.
														//�G�C���A�X(�j�b�N�l�[���̂悤�Ȃ���).
	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
//		ERR_MSG( m_sAlias, "����̐���" );
		m_bOpend = false;
		m_hWnd = nullptr;
		return true;
	}
	ERR_MSG( m_sAlias, "����̎��s" );

	return false;
}

//�Đ��֐�.
bool clsSound::Play( const bool bNotify ) const
{
	if( !m_bOpend ) return false;

	//�R�}���h.
	char command[STR_BUFF_MAX] = "play ";
	strcat_s( command, sizeof( command ), m_sAlias );	//�G�C���A�X������.

	//	���[�v����ꍇ���b�Z�[�W�𑗂�.
	if( bNotify ){
		strcat_s( command, sizeof( command ), " notify" );
		//notify: �������I�������Ƃ���hWnd��MM_MCINOTIFY���b�Z�[�W���|�X�g����.
	}

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//��~�֐�.
bool clsSound::Stop() const
{
	if( !m_bOpend ) return false;

	//�R�}���h.
	char command[STR_BUFF_MAX] = "stop ";
	strcat_s( command, sizeof( command ), m_sAlias );	//�G�C���A�X������.
													//�G�C���A�X(�j�b�N�l�[���̂悤�Ȃ���).
	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		SeekToStart();
		return true;
	}

//	assert( !"�~�߂�܂���ł���" );
	ERR_MSG( m_sAlias, "��~�̎��s" );
	return false;
}

////�ꎞ��~.
//bool clsSound::Pause()
//{
//	//�R�}���h.
//	char command[STR_BUFF_MAX] = "pause ";
//	strcat_s( command, sizeof( command ), m_sAlias );	//�G�C���A�X������.
//													//�G�C���A�X(�j�b�N�l�[���̂悤�Ȃ���).
//	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
//		SeekToStart();
//		return true;
//	}
//	return false;
//}
//
////�ꎞ��~�̉���.
//bool clsSound::Resume()
//{
//	//�R�}���h.
//	char command[STR_BUFF_MAX] = "resume ";
//	strcat_s( command, sizeof( command ), m_sAlias );	//�G�C���A�X������.
//													//�G�C���A�X(�j�b�N�l�[���̂悤�Ȃ���).
//	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
//		SeekToStart();
//		return true;
//	}
//	return false;
//}



//�����߂��֐�(�Đ��ʒu������).
bool clsSound::SeekToStart() const
{
	//�R�}���h.
	char command[STR_BUFF_MAX] = "seek ";

	strcat_s( command, sizeof( command ), m_sAlias );	//�G�C���A�X������.
	strcat_s( command, sizeof( command ), " to start" );

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//��Ԏ擾�֐�.
//	sStatus �̔z���256�ȉ��ɂ��邱��.
//���������A��Ԃ��擾����ꍇ�́A�Đ����Ɂunotify�v��ݒ肵�A.
//	�E�B���h�E�n���h���̃��b�Z�[�W�𑗂��Ă����K�v������.
bool clsSound::GetStatus( char* const sStatus ) const
{
	//�R�}���h.
	char command[STR_BUFF_MAX] = "status ";
	strcat_s( command, sizeof( command ), m_sAlias );	//�G�C���A�X������.
	strcat_s( command, sizeof( command ), " mode" );

	if( mciSendString( command, sStatus, ( STR_BUFF_MAX - 1 ), m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//���̒�~���m�F����֐�.
bool clsSound::IsStopped() const
{
	char sStatus[STR_BUFF_MAX] = "";

	//��Ԏ擾.
	if( GetStatus( sStatus ) ){
		//�������r.
		if( strcmp( sStatus, "stopped" ) == 0 ){
			return true;
		}
	}
	return false;
}

//���̍Đ������m�F����֐�.
bool clsSound::IsPlaying() const
{
	char sStatus[STR_BUFF_MAX] = "";

	//��Ԏ擾.
	if( GetStatus( sStatus ) ){
		//�������r.
		if( strcmp( sStatus, "playing" ) == 0 ){
			return true;
		}
	}
	return false;
}

//============================================================
//	���ʂ�ݒ肷��.
//============================================================
bool clsSound::SetVolume( int ivolume ) const
{
	//���l�̊ۂ�.
	if( ivolume > 1000 )	{ ivolume = 1000; }
	else if( ivolume < 0 )	{ ivolume = 0	; }
	
	//���ʂ𕶎���ɕϊ�.
	char sVolume[STR_BUFF_MAX] = "";
	sprintf_s( sVolume, sizeof( sVolume ), "%d", ivolume );

	//�R�}���h.
	char command[STR_BUFF_MAX] = "setaudio ";

	strcat_s( command, sizeof( command ), m_sAlias );	//�G�C���A�X������.
	strcat_s( command, sizeof( command ), " volume to " );
	strcat_s( command, sizeof( command ), sVolume );//���ʂ�����.

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
//�����ݒ�.
//============================================================
HRESULT clsSound::SetInitParam( const char *sAlias, const HWND hWnd )
{
	if( m_hWnd != nullptr ){
		return E_FAIL;
	}

	//�E�B���h�E�n���h���E�B���h�E�n���h����n��.
	m_hWnd = hWnd;

	//�G�C���A�X.
	strcpy_s( m_sAlias, sizeof( m_sAlias ), sAlias );
	

	return S_OK;
}
